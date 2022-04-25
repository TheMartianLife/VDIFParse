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

#include <stdlib.h>

#include "vdifparse_types.h"

// MARK: initialise stream object

DataStream open_file(char* file_path);
DataStream open_sink();

// MARK: configure objects

int set_format_designator(DataStream* ds, const char* format_designator);
void set_gap_policy(DataStream* ds, enum GapPolicy policy);

void close(DataStream ds);

#endif // VDIFPARSE_API_H
