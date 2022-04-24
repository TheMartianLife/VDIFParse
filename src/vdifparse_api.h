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

// MARK: populate stream

void ingest_data(DataStream ds, uint8_t num_bytes, const char** data);

// MARK: configure output

void select_thread(DataStream ds, uint16_t thread_id);
void select_all_threads(DataStream ds);
void deselect_all_threads(DataStream ds);

// MARK: check configuration

static inline enum InputMode get_mode(DataStream ds) { return ds.mode; }
static inline enum DataFormat get_data_format(DataStream ds) { return ds.format; }
static inline unsigned int get_num_threads(DataStream ds) { return ds.num_threads; }
static inline enum GapPolicy get_gap_policy(DataStream ds) { return ds.gap_policy; }

// TODO maybe add functionality for validity checking?

// MARK: prepare for output

// TODO maybe add functionality that tells you size of malloc needed for decode?

void seek_to(DataStream ds, uint8_t epoch, uint32_t second);

// MARK: move data around

void read_frames(DataStream ds, uint8_t num_frames, char** out);
DataStream** separate_threads(DataStream ds);

// MARK: process data

void decode_samples(DataStream ds, uint32_t num_samples, float** out, uint32_t* valid_samples);


// MARK: cleanup

void close(DataStream ds);

#endif // VDIFPARSE_API_H
