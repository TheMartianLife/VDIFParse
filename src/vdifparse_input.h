// vdifparse_input.h - provides functions to open and fill buffers from file
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
#ifndef VDIFPARSE_INPUT_H
#define VDIFPARSE_INPUT_H

#include <stdio.h>

#include "vdifparse_types.h"

static enum DataFormat peek_format(const uint8_t* bytes);
static struct DataFrame_VDIF peek_frame_vdif(struct DataStream ds);
static struct DataFrame_CODIF peek_frame_codif(struct DataStream ds);
int peek_file(struct DataStream* ds, const char* file_path);

int buffer_frames(struct DataStream ds, unsigned int num_frames);

#endif // VDIFPARSE_INPUT_H
