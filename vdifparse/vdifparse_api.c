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

struct DataStream* open_file(char* file_path, enum InputFormat format) {
    struct DataStream* in = _init_stream(FileMode, format);
    open_file_input(in, file_path);
    if (in->_input_file_handle == NULL) { // check it actually opened
        fprintf(vp_stderr, "File %s could not be opened.", file_path);
        exit(1); // TODO: make this recoverable maybe?
    }
    // TODO: init threat atributes
    return in;
}


struct DataStream* open_sink(struct DataStream* in, enum InputFormat format) {
    return _init_stream(StreamMode, format);
}


void set_thread_attributes(struct DataStream* in, unsigned short thread_num, float frequency, float bandwidth, char* channel_name) {
    struct DataThread* thread = in->threads[thread_num];
    if (!thread) {
        thread =  _init_thread_attributes();
    }
    thread->frequency = frequency;
    thread->bandwidth = bandwidth;
    thread->channel_name = channel_name;
    in->threads[thread_num] = thread;
}


void set_gap_policy(struct DataStream* in, enum GapPolicy policy) {
    in->gap_policy = policy;
}


void set_cursor(struct DataStream* in, unsigned int epoch, unsigned long int second) {

}


void ingest_data(struct DataStream* in, unsigned int num_bytes, char** data) {

}


void select_thread(struct DataStream* in, unsigned short thread_num) {

}

void select_all_threads(struct DataStream* in) {

}


struct ThreadAttributes* get_thread_attributes(struct DataStream* in, unsigned short thread_num) {
    return in->threads[thread_num]
}


enum GapPolicy get_gap_policy(struct DataStream* in) {
    return in->gap_policy;
}

unsigned long int* get_cursor(struct DataStream* in) {

}


unsigned short* get_selected_threads(struct DataStream* in) {

}


void read_frames(struct DataStream* in, unsigned int num_frames, char** out) {

}


struct DataStream** separate_threads(struct DataStream* in) {

}


void decode_samples(struct DataStream* in, unsigned long int num_samples, float** out, unsigned long int** valid_samples) {

}


void close(struct DataStream* in) {
    if (in->_input_file_handle != NULL) {
        fclose(in->_input_file_handle);
    }

    // TODO: free memory
}
