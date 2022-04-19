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

#include "vdifparse_input.h"

#define DEFAULT_HEADER_LENGTH 32


void open_file_input(struct DataStream* ds, char* file_path) {
    // open file in binary mode
    ds->_input_file_handle = fopen(file_path, "rb");
    if (ds->_input_file_handle == NULL) { // check it actually opened
        return;
    }

    // read first header from file
    unsigned char header_length = ds->frame_header_length;
    if (header_length == 0) { 
        header_length = DEFAULT_HEADER_LENGTH;
    }
    struct DataFrame* df = get_next_frame(ds, header_length);
    ds->_frame_buffer = df;
}

struct DataFrame* get_next_frame(struct DataStream* ds, unsigned char header_length) {
    unsigned char* first_header = malloc(header_length);
    fread(first_header, header_length, 1, ds->_input_file_handle);
    struct DataFrame* df = parse_vdif_header(ds, (unsigned int*)first_header);
    if (ds->frame_header_length != header_length) {
        // in case we consumed 32 bytes but the header was only 16
        fseek(ds->_input_file_handle, ds->frame_header_length, SEEK_SET);
    }
    unsigned long frame_length = ds->threads[df->thread_id]->frame_length;
    frame_length -= ds->frame_header_length;
    unsigned char* frame_data = malloc(frame_length);
    fread(frame_data, frame_length, 1, ds->_input_file_handle);
    df->data_buffer = (unsigned int*)frame_data;
    return df;
}


// void buffer_frames(struct DataStream* ds) {
//     // buffer frame
//     unsigned short thread_id = 0; // TODO
//     if (is_selected_thread(ds, thread_id)) {
//         // append to thread buffer
//     }
// }