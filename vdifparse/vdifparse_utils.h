// vdifparse_utils.h - provides general utility functions such as for printing
// structs, converting between enum and string types, and more.
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
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#include "vdifparse_types.h"

#define vpout stdout // TODO: remove
#define vperr stderr // TODO: remove

void raise_exception(const char *format, ...);

void print_stream_attributes(struct DataStream* ds);
void print_thread_attributes(struct DataThread* dt);
void print_frame_attributes(struct DataFrame* df);

char* string_for_input_mode(enum InputMode mode);
char* string_for_data_format(enum DataFormat format);
char* string_for_data_type(enum DataType type);
char* string_for_gap_policy(enum GapPolicy policy);
char* string_for_edv(enum ExtendedDataVersion version);
char* string_for_frequency(unsigned long frequency);

#endif // VDIFPARSE_UTILS_H
