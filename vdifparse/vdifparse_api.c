// vdifparse_api.c - provides public API functions to open, parse, manipulate,
// and split VDIF and CODIF files for use in related software
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

#include "vdifparse_api.h"

struct InputStream* open_file(char* file_path) {
    // open file in binary mode
    FILE *input_file = fopen(file_path, "rb");
    if (input_file == NULL) { // check it actually opened
        fprintf(vp_stderr, "File %s could not be opened.", file_path);
        exit(1); // TODO: better error recovery
    }

    struct InputStream* in;
    in = (struct InputStream *)calloc(1, sizeof(struct InputStream));
    in->mode = FileMode;
    // TODO: remove
    unsigned char* file_contents;
    fseek(input_file, 0, SEEK_END);
    int file_size = ftell(input_file);
    file_contents = malloc(file_size);
    fseek(input_file, 0, SEEK_SET);
    fread(file_contents, file_size, 1, input_file);
    in->raw_data_buffer = file_contents;
    fclose(input_file);
    // TODO: remove

    parse_vdif_header(in);

    return in;
}

void set_gap_policy(struct InputStream* in, enum GapPolicy policy) {
    in->gap_policy = policy;
}
