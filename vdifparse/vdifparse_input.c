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


void open_file_input(struct DataStream* ds, char* file_path) {
    // open file in binary mode
    ds->_input_file_handle = fopen(file_path, "rb");
    if (ds->_input_file_handle == NULL) { // check it actually opened
        return;
    }

    // read first header from file
    unsigned char* first_header = malloc(DEFAULT_HEADER_SIZE);
    fread(first_header, DEFAULT_HEADER_SIZE, 1, ds->_input_file_handle);
    struct DataFrame* df = parse_vdif_header(ds, first_header);
    // now that we can know the frame size, load the first frame into buffer
    unsigned long frame_length = ds->threads[df->thread_id]->frame_length;
    frame_length -= ds->frame_header_length;
    unsigned char* first_frame = malloc(frame_length);
    fread(first_frame, frame_length, 1, ds->_input_file_handle);
    df->data_buffer = first_frame;
    ds->frame_buffer = df;
}


void buffer_frames(struct DataStream* ds) {
    // buffer frame
    unsigned short thread_id = 0; // TODO
    if (is_selected_thread(ds, thread_id)) {
        // append to thread buffer
    }
}