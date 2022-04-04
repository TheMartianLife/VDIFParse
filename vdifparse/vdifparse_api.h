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

#include "vdifparse_headers.h"
#include "vdifparse_input.h"
#include "vdifparse_types.h"
#include "vdifparse_utils.h"

// initialise stream object
struct InputStream* open_file(char* file_path);
struct InputStream* open_stream();

// configure stream
void set_gap_policy(struct InputStream* in, enum GapPolicy policy);

// validity checks

// configure output

// prepare for output

// process data

// cleanup
void close(struct InputStream* in);

#endif // VDIFPARSE_API_H
