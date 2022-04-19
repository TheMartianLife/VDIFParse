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
struct DataStream* open_file(char* file_path, enum DataFormat format);
struct DataStream* open_sink(enum DataFormat format);

// configure stream
void set_thread_attributes(struct DataStream* ds, unsigned short thread_id, float frequency, float bandwidth, char* channel_name);
void set_channel_attributes(struct DataStream* ds, unsigned short thread_id, unsigned long channel_num, unsigned float frequency, float bandwidth, char* channel_name);
void set_gap_policy(struct DataStream* ds, enum GapPolicy policy);
void seek_to(struct DataStream* ds, unsigned int epoch, unsigned long second);

// populate stream
void ingest_data(struct DataStream* ds, unsigned int num_bytes, char** data);

// configure output
void select_thread(struct DataStream* ds, unsigned short thread_id);
void select_all_threads(struct DataStream* ds);
void deselect_all_threads(struct DataStream* ds);

// check stream configuration
struct DataThread* get_thread_attributes(struct DataStream* ds, unsigned short thread_id);
enum GapPolicy get_gap_policy(struct DataStream* ds);
unsigned long* get_cursor(struct DataStream* ds);
unsigned short* get_selected_threads(struct DataStream* ds);
unsigned char is_selected_thread(struct DataStream* ds, unsigned short thread_id);

// validity checks


// prepare for output


// move data around
void read_frames(struct DataStream* ds, unsigned int num_frames, char** out);
struct DataStream** separate_threads(struct DataStream* ds);

// process data
void decode_samples(struct DataStream* ds, unsigned long num_samples, float** out, unsigned long** valid_samples);


// cleanup
void close(struct DataStream* ds);

#endif // VDIFPARSE_API_H
