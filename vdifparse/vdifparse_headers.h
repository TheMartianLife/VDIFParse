// vdifparse_headers.h - provides functions for interpreting format header 
// fields into stream attributes.
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
#ifndef VDIFPARSE_HEADERS_H
#define VDIFPARSE_HEADERS_H

#include <math.h>
#include <stdlib.h>

#include "vdifparse_types.h"
#include "vdifparse_utils.h"

#define MAX_BITS_PER_SAMPLE 32
#define MAX_DATA_THREADS 1024
#define MAX_FRAME_LENGTH 134217728 // bytes
#define ASCII_0 0x30

void parse_vdif_header(struct InputStream* in);
void parse_codif_header(struct InputStream* in);
void parse_vdif_extended_data(struct InputStream* in);
void parse_vdif_extended_data_nict(struct InputStream* in);
void parse_vdif_extended_data_alma(struct InputStream* in);
void parse_vdif_extended_data_nrao(struct InputStream* in);
void parse_vdif_extended_data_corner_turned(struct InputStream* in);
void parse_vdif_extended_data_haystack(struct InputStream* in);

#endif // VDIFPARSE_HEADERS_H
