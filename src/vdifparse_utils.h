// vdifparse_utils.h - provides general utility functions such as for printing
// structs, converting between enum and char* types, and more.
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
#ifndef VDIFPARSE_UTILS_H
#define VDIFPARSE_UTILS_H

#include <stdlib.h>
#include <time.h>

#include "vdifparse_types.h"

void raise_exception(const char *format, ...);
void raise_warning(const char *format, ...);

char* string_for_datetime(datetime dt);
char* string_for_input_mode(enum InputMode mode);
char* string_for_data_format(enum DataFormat format);
char* string_for_data_type(enum DataType type);
char* string_for_gap_policy(enum GapPolicy policy);
char* string_for_edv(enum VDIFExtendedDataVersion version);
char* string_for_hertz(uint32_t frequency);
char* string_for_ascii(uint64_t sequence);

unsigned int* string_to_numeric(const char* string_value);
int split_string(const char* string_value, const char* separators, char*** out);

void print_stream(DataStream ds);
void print_frame(DataFrame df);

#endif // VDIFPARSE_UTILS_H
