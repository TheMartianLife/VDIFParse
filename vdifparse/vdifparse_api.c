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
    struct InputStream* in;
    in = (struct InputStream *)calloc(1, sizeof(struct InputStream));
    in->mode = FileMode;

    open_file_input(in, file_path);
    if (in->input_file_handle == NULL) { // check it actually opened
        fprintf(vp_stderr, "File %s could not be opened.", file_path);
        exit(1); // TODO: make this recoverable maybe?
    }

    parse_vdif_header(in);
    return in;
}

struct InputStream* open_stream() {
    struct InputStream* in;
    in = (struct InputStream *)calloc(1, sizeof(struct InputStream));
    in->mode = StreamMode;
    // TODO: what else can we know at this point?
    return in;
}

void set_gap_policy(struct InputStream* in, enum GapPolicy policy) {
    in->gap_policy = policy;
}

void close(struct InputStream* in) {
    if (in->input_file_handle != NULL) {
        fclose(in->input_file_handle);
    }

    // TODO: free memory
}
