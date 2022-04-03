// vdifparse_api.h - provides public API functions to open, parse, manipulate,
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
#ifndef VDIFPARSE_API_H
#define VDIFPARSE_API_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vdifparse_headers.h"
#include "vdifparse_types.h"
#include "vdifparse_utils.h"

#define MAX_BITS_PER_SAMPLE 32
#define MAX_DATA_THREADS 1024
#define MAX_FRAME_LENGTH 134217728 // bytes

// get data
struct InputStream* open_file(char* file_path);

// configure stream
void set_gap_policy(struct InputStream* stream, enum GapPolicy policy);

// validity checks

// configure output

// prepare for output

// process data

#endif // VDIFPARSE_API_H
