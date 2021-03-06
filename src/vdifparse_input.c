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

int peek_file(DataStream* ds, const char* file_path) {
    // open file in binary mode
    FILE* file_handle = fopen(file_path, "rb");
    file_handle = fopen(file_path, "rb");
    if (file_handle == NULL) { // check it actually opened
        return FAILED_TO_OPEN_FILE;
    }

    // get a bit of the file
    uint8_t head[5];
    fread(head, 5, 1, file_handle);
    fseek(file_handle, 0, SEEK_SET);
    ds->input.file->file_handle = file_handle;

    // see which format it is
    ds->format = peek_format(head);
    // TODO nicer error handling of this

    #ifdef __DEBUG__
        fprintf(stdout, "File format inferred to be: %s\n", string_for_data_format(ds->format));
    #endif

    return SUCCESS;
}

static DataFrame peek_frame(DataStream ds) {
    DataFrame df = init_frame(ds.format);
    if (ds.format == CODIF) {
        fread((uint8_t*)df.codif->header, sizeof(CODIFHeader), 1, get_file_handle(ds.input));
        // now metadata
        fread((uint8_t*)df.codif->metadata->none, CODIF_METADATA_BYTES, 1, get_file_handle(ds.input)); 
        df.codif->metadata->version = df.codif->metadata->none->metadata_version;
    } else {
        fread((uint8_t*)df.vdif->header, sizeof(VDIFHeader), 1, get_file_handle(ds.input));
        // now extended data, if any
        if (ds.format == VDIF) {
            fread((uint8_t*)df.vdif->extended_data->none, VDIF_EXTENDED_DATA_BYTES, 1, get_file_handle(ds.input));
            df.vdif->extended_data->version = df.vdif->extended_data->none->extended_data_version;
        } else {
            df.vdif->extended_data = NULL;
        }
    }
    return df;
}

int buffer_frames(DataStream* ds, unsigned int num_frames) {
    ds->num_buffered_frames = 0;
    ds->num_processed_frames = 0;
    uint32_t frame_length;
    while (ds->num_buffered_frames < num_frames && !feof(get_file_handle(ds->input))) {
        DataFrame df = peek_frame(*ds);
        frame_length = get_data_length(df);
        if (should_buffer_frame(*ds, df)) {
            if (ds->format == CODIF) {
                df.codif->data = malloc(frame_length);
                fread(df.codif->data, frame_length, 1, get_file_handle(ds->input));
            } else {
                df.vdif->data = malloc(frame_length);
                fread(df.vdif->data, frame_length, 1, get_file_handle(ds->input));
            }
            ds->frames[ds->num_buffered_frames] = df;
            ds->num_buffered_frames++;
        } else {
            // skip over this frame in the file
            fseek(get_file_handle(ds->input), frame_length, SEEK_CUR);
        }
    }
    return (ds->num_buffered_frames == num_frames) ? SUCCESS : REACHED_END_OF_FILE;
}

