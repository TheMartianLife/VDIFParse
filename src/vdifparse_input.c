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
    } else {
        raise_exception("file format could not be inferred.");
        return VDIF; // to silence the compiler
    }
}

void print_bits(char c){
    for( int i = 7; i >= 0; i-- ) {
        printf( "%d", ( c >> i ) & 1 ? 1 : 0 );
    }
    printf(" ");
}

static DataFrame_VDIF peek_frame_vdif(DataStream ds) {
    DataFrame_VDIF* frame = (DataFrame_VDIF*)init_frame(ds.format);
    fread((uint8_t*)frame->header, sizeof(VDIFHeader), 1, get_file_handle(ds));
    // now extended data, if any
    if (ds.format == VDIF) {
        fread((uint8_t*)frame->extended_data, VDIF_EXTENDED_DATA_BYTES, 1, get_file_handle(ds));
    }
    return *frame;
}

static DataFrame_CODIF peek_frame_codif(DataStream ds) {
    DataFrame_CODIF* frame = (DataFrame_CODIF*)init_frame(ds.format);
    fread((uint8_t*)frame->header, sizeof(CODIFHeader), 1, get_file_handle(ds));
    // now metadata
    fread((uint8_t*)frame->metadata, CODIF_METADATA_BYTES, 1, get_file_handle(ds));
    return *frame;
}

int peek_file(DataStream* ds, const char* file_path) {
    DataStreamInput_File* di = (DataStreamInput_File*)init_input(FileMode);
    // open file in binary mode
    di->file_handle = fopen(file_path, "rb");
    if (di->file_handle == NULL) { // check it actually opened
        return FAILED_TO_OPEN_FILE;
    }

    // get a bit of the file
    uint8_t head[5];
    fread(head, 5, 1, di->file_handle);
    fseek(di->file_handle, 0, SEEK_SET);
    ds->input = di;

    // see which format it is
    ds->format = peek_format(head);
    // TODO nicer error handling of this

    #ifdef __DEBUG__
        fprintf(stdout, "File format inferred to be: %s\n", string_for_data_format(ds->format));
    #endif

    return SUCCESS;
}


int buffer_frames(DataStream ds, unsigned int num_frames) {
    ds.buffered_frames = 0;
    uint32_t frame_length;
    // a bit silly to duplicate code like this but it's still faster like this
    // bc you only check once and don't have to cast ptrs to/from void*
    if (ds.format == CODIF) {
        while (ds.buffered_frames < num_frames && ! feof(get_file_handle(ds))) {
            DataFrame_CODIF frame = peek_frame_codif(ds);
            frame_length = frame.header->frame_length * 8;
            if (should_buffer_frame(ds, &frame)) {
                frame.data = (uint32_t*)malloc(frame_length);
                fread((uint8_t*)frame.data, frame_length, 1, get_file_handle(ds));
                ds.frames[ds.buffered_frames] = &frame;
                ds.buffered_frames++;
            } else {
                // skip over this frame in the file
                fseek(get_file_handle(ds), frame_length, SEEK_CUR);

            }
        }
    } else {
        DataFrame_VDIF frame;
        while (ds.buffered_frames < num_frames && ! feof(get_file_handle(ds))) {
            frame = peek_frame_vdif(ds);
            frame_length = frame.header->frame_length * 8;
            if (should_buffer_frame(ds, &frame)) {
                frame.data = (uint32_t*)malloc(frame_length);
                fread((uint8_t*)frame.data, frame_length, 1, get_file_handle(ds));
                ds.frames[ds.buffered_frames] = &frame;
                ds.buffered_frames++;
            } else {
                // skip over this frame in the file
                fseek(get_file_handle(ds), frame_length, SEEK_CUR);
            }
        }
    }

    return (ds.buffered_frames == num_frames) ? SUCCESS : REACHED_END_OF_FILE;
}