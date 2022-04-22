// vdifparse_input.c - provides functions to open and fill buffers from file
// and pipe sources.
// Copyright (C) 2022 Mars Buttfield-Addison
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later 
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT 
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more 
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program. If not, see <http://www.gnu.org/licenses/>.

#include <stdio.h>

#include "vdifparse_input.h"
#include "vdifparse_utils.h"

static enum DataFormat peek_format(const uint8_t* bytes) {
    // TODO scrub for synch fields first? or assume good?
    uint8_t legacy_mode = (bytes[0] >> 1) & 0b1;
    uint8_t version = bytes[4] & 0b111;
    if (version == 0 || version == 1) {
        return (legacy_mode) ? VDIF_LEGACY : VDIF;
    } else if (version == 7) {
        return CODIF;
    }
    return (enum DataFormat)NULL;
}

static struct DataFrame_VDIF peek_frame_vdif(struct DataStream ds) {
    struct DataFrame_VDIF* frame = (struct DataFrame_VDIF*)init_frame(ds.format);
    fread((void*)frame->header, 1, sizeof(struct VDIFHeader), get_file_handle(ds));
    // now extended data, if any
    if (ds.format == VDIF) {
        fread((void*)frame->extended_data, 1, VDIF_EXTENDED_DATA_BYTES, get_file_handle(ds));
    }
    return *frame;
}

static struct DataFrame_CODIF peek_frame_codif(struct DataStream ds) {
    struct DataFrame_CODIF* frame = (struct DataFrame_CODIF*)init_frame(ds.format);
    fread((void*)frame->header, 1, sizeof(struct CODIFHeader), get_file_handle(ds));
    // now metadata
    fread((void*)frame->metadata, 1, CODIF_METADATA_BYTES, get_file_handle(ds));
    return *frame;
}

int peek_file(struct DataStream* ds, const char* file_path) {
    struct DataStreamInput_File* di = (struct DataStreamInput_File*)init_input(FileMode);
    // open file in binary mode
    di->file_handle = fopen(file_path, "rb");
    if (di->file_handle == NULL) { // check it actually opened
        return FAILED_TO_OPEN_FILE;
    }

    // get a bit of the file
    uint8_t* head = (uint8_t*)calloc(5, sizeof(uint8_t));
    fread(head, 5, 1, di->file_handle);
    fseek(di->file_handle, 0, SEEK_SET);
    ds->input = di;

    // see which format it is
    ds->format = peek_format(head);
    if (ds->format == NULL) {
        return FILE_HEADER_INVALID;
    }

    #ifdef __DEBUG__
        fprintf(stdout, "File format inferred to be: %s\n", string_for_data_format(ds->format));
    #endif

    return SUCCESS;
}


int buffer_frames(struct DataStream ds, unsigned int num_frames) {
    ds.buffered_frames = 0;
    uint32_t frame_length;

    if (ds.format == CODIF) {
        while (ds.buffered_frames < num_frames && ! feof(get_file_handle(ds))) {
            struct DataFrame_CODIF frame = peek_frame_codif(ds);
            frame_length = frame.header->frame_length * 8;
            if (should_buffer_frame(ds, &frame)) {
                frame.data = (uint32_t*)malloc(frame_length);
                fread((void*)frame.data, 1, frame_length, get_file_handle(ds));
                ds.frames[ds.buffered_frames] = &frame;
                ds.buffered_frames++;
                #ifdef __DEBUG__
                    fprintf(stdout, "Buffered frame: %d bytes\n", frame_length);
                #endif
            } else {
                // skip over this frame in the file
                fseek(get_file_handle(ds), frame_length, SEEK_CUR);
                #ifdef __DEBUG__
                    fprintf(stdout, "Skipped frame: %d bytes\n", frame_length);
                #endif
            }
        }
    } else {
        struct DataFrame_VDIF frame;
        while (ds.buffered_frames < num_frames && ! feof(get_file_handle(ds))) {
            frame = peek_frame_vdif(ds);
            frame_length = frame.header->frame_length * 8;
            if (should_buffer_frame(ds, &frame)) {
                frame.data = (uint32_t*)malloc(frame_length);
                fread(frame.data, 1, frame_length, get_file_handle(ds));
                ds.frames[ds.buffered_frames] = &frame;
                ds.buffered_frames++;
                #ifdef __DEBUG__
                    fprintf(stdout, "Buffered frame: %d bytes\n", frame_length);
                #endif
            } else {
                // skip over this frame in the file
                fseek(get_file_handle(ds), frame_length, SEEK_CUR);
                #ifdef __DEBUG__
                    fprintf(stdout, "Skipped frame: %d bytes\n", frame_length);
                #endif
            }
        }
    }

    return (ds.buffered_frames == num_frames) ? SUCCESS : REACHED_END_OF_FILE;
}