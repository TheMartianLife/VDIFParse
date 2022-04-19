// vdifparse_types.c - defines the DataStream type and associated enum types
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


struct DataStream* _init_stream() {
    struct DataStream* ds;
    ds = (struct DataStream*)calloc(1, sizeof(struct DataStream));
    ds->frame_buffer = (struct DataFrame*)calloc(20, sizeof(struct DataFrame));
    return ds;
}


struct DataThread* _init_thread() {
    struct DataThread* dt;
    dt = (struct DataThread*)calloc(1, sizeof(struct DataThread));
    return dt;
}


struct DataChannel* _init_channel() {
    struct DataChannel* dc;
    dc = (struct DataChannel*)calloc(1, sizeof(struct DataChannel));
    return dc;  
}


struct DataFrame* _init_frame() {
    struct DataThread* df;
    df = (struct DataFrame*)calloc(1, sizeof(struct DataFrame));
    return df;
}