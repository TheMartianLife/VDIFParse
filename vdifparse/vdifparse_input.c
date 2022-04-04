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

void open_file_input(struct InputStream* in, char* file_path) {
    // open file in binary mode
    in->input_file_handle = fopen(file_path, "rb");
    if (in->input_file_handle == NULL) { // check it actually opened
        return;
    }

    // TODO: don't read in whole file, just a certain buffer size or num frames?
    unsigned char* file_contents;
    fseek(in->input_file_handle, 0, SEEK_END);
    int file_size = ftell(in->input_file_handle);
    file_contents = malloc(file_size);
    fseek(in->input_file_handle, 0, SEEK_SET);
    fread(file_contents, file_size, 1, in->input_file_handle);
    in->raw_data_buffer = file_contents;

    return;
}
