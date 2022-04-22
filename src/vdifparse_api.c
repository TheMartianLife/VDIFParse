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
#include "vdifparse_input.h"
#include "vdifparse_utils.h"

struct DataStream open_file(char* file_path) {
    struct DataStream ds = init_stream(FileMode);
    int status = peek_file(&ds, file_path);
    if (status != SUCCESS) {
        raise_exception("file %s could not be opened.", file_path);
    }
    // TODO remove
    ds.frames = malloc(sizeof(struct DataFrame_VDIF*));
    buffer_frames(ds, 1);
    return ds;
}

struct DataStream open_sink() {
    // unfortunately nothing else can be known at this time
    return init_stream(StreamMode);
}