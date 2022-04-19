// vdifparse_api.c - provides public API functions to open, parse, manipulate,
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

#include "vdifparse_api.h"

struct DataStream* open_file(char* file_path, enum DataFormat format) {
    struct DataStream* ds = _init_stream();
    ds->mode = FileMode;
    ds->format = format;
    open_file_input(ds, file_path);
    if (ds->_input_file_handle == NULL) { // check it actually opened
        fprintf(vp_stderr, "File %s could not be opened.", file_path);
        exit(1); // TODO: make this recoverable maybe?
    }
    return ds;
}


struct DataStream* open_sink(enum DataFormat format) {
    struct DataStream* ds = _init_stream();
    ds->mode = StreamMode;
    ds->format = format;
    return ds;
}


void set_thread_attributes(struct DataStream* ds, unsigned short thread_id, float frequency, float bandwidth, char* channel_name) {
    struct DataThread* thread = ds->threads[thread_id];
    if (!thread) {
        thread =  _init_thread_attributes();
    }
    thread->frequency = frequency;
    thread->bandwidth = bandwidth;
    thread->channel_name = channel_name;
    ds->threads[thread_id] = thread;
}


void set_gap_policy(struct DataStream* ds, enum GapPolicy policy) {
    ds->gap_policy = policy;
}


void seek_to(struct DataStream* ds, unsigned int epoch, unsigned long second) {

}


void ingest_data(struct DataStream* ds, unsigned int num_bytes, char** data) {

}


void select_thread(struct DataStream* ds, unsigned short thread_id) {
    
}

void select_all_threads(struct DataStream* ds) {

}


struct DataThread* get_thread_attributes(struct DataStream* ds, unsigned short thread_id) {
    return ds->threads[thread_id];
}


enum GapPolicy get_gap_policy(struct DataStream* ds) {
    return ds->gap_policy;
}

unsigned long* get_cursor(struct DataStream* ds) {

}


unsigned short* get_selected_threads(struct DataStream* ds) {

}

unsigned char is_selected_thread(struct DataStream* ds, unsigned short thread_id) {

}


void read_frames(struct DataStream* ds, unsigned int num_frames, char** out) {

}


struct DataStream** separate_threads(struct DataStream* ds) {

}


void decode_samples(struct DataStream* ds, unsigned long num_samples, float** out, unsigned long** valid_samples) {

}


void close(struct DataStream* ds) {
    if (ds->_input_file_handle != NULL) {
        fclose(ds->_input_file_handle);
    }

    // TODO: free memory
}
