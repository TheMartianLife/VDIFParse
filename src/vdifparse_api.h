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

struct DataStream open_file(char* file_path);
struct DataStream open_sink();

// MARK: configure objects

static inline enum InputMode get_mode(struct DataStream ds) { return ds.mode; }
static inline enum DataFormat get_data_format(struct DataStream ds) { return ds.format; }
static inline unsigned int get_num_threads(struct DataStream ds) { return ds.num_threads; }
static inline enum GapPolicy get_gap_policy(struct DataStream ds) { return ds.gap_policy; }
static inline void set_gap_policy(struct DataStream* ds, enum GapPolicy policy) { ds->gap_policy = policy; }

// MARK: populate stream

void ingest_data(struct DataStream ds, uint8_t num_bytes, const char** data);

// MARK: configure output

void select_thread(struct DataStream ds, uint16_t thread_id);
void select_all_threads(struct DataStream ds);
void deselect_all_threads(struct DataStream ds);

// TODO maybe add functionality for validity checking?

// MARK: prepare for output

// TODO maybe add functionality that tells you size of malloc needed for decode?

void seek_to(struct DataStream ds, uint8_t epoch, uint32_t second);

// MARK: move data around

void read_frames(struct DataStream ds, uint8_t num_frames, char** out);
struct DataStream** separate_threads(struct DataStream ds);

// MARK: process data

void decode_samples(struct DataStream ds, uint32_t num_samples, float** out, uint32_t* valid_samples);


// MARK: cleanup

void close(struct DataStream ds);

#endif // VDIFPARSE_API_H
