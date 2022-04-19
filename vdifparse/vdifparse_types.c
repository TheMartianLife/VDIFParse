// vdifparse_types.c - defines the InputStream type and associated enum types
// used throughout the library.
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

#include "vdifparse_types.h"

#define MAX_THREADS 1024


struct DataStream* _init_stream(enum InputMode mode, enum InputFormat format, unsigned int buffer_size) {
    struct DataStream* in;
    in = (struct DataStream *)calloc(1, sizeof(struct DataStream));
    in->mode = mode;
    // init thread atributes structures
    in->thread_attrs = (struct ThreadAttributes **)calloc(MAX_THREADS, sizeof(struct ThreadAttributes*));
    if (mode == FileMode) {
        // open file, inspect attributes
    }
    return in;
}


struct ThreadAttributes* _init_thread_attributes() {
    struct ThreadAttributes* attrs;
    attrs = (struct ThreadAttributes *)calloc(1, sizeof(struct ThreadAttributes)); 
    return attrs;
}


void print_attributes(struct DataStream* in, unsigned short int thread_num) {
    // TODO
}