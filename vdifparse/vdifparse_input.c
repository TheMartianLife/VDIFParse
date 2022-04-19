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


void open_file_input(struct DataStream* in, char* file_path) {
    // open file in binary mode
    in->_input_file_handle = fopen(file_path, "rb");
    if (in->_input_file_handle == NULL) { // check it actually opened
        return;
    }

    // read first header from file
    unsigned char* file_contents = malloc(DEFAULT_HEADER_SIZE);
    fread(file_contents, DEFAULT_HEADER_SIZE, 1, in->_input_file_handle);
    in->_raw_data_buffer = file_contents;
    parse_vdif_header(in);
    // now that we can know the frame size, load the first frame into buffer
    fseek(in->_input_file_handle, 0, SEEK_SET);
    // TODO variable frame size?
    in->_raw_data_buffer = malloc(BUFFER_SIZE);
    buffer_frames(in);
    return;
}


void buffer_frames(struct DataStream* in) {
    // TODO variable frame size?
    // int bytes = in->frame_data_length * BUFFER_FRAMES;
    // unsigned char* file_contents = malloc(bytes);
    // fread(file_contents, bytes, 1, in->_input_file_handle);
    // in->_raw_data_buffer = file_contents;
    // update_vdif_header_attributes(in);
    // in->_cursor_epoch = in->reference_epoch;
    // in->_cursor_second = in->seconds_from_epoch;
    // printf("Buffered %d frames.\n", BUFFER_FRAMES);
}

void next_frame(struct DataStream* in)