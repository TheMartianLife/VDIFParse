// vdifparse_types.c - defines the struct and enum types used throughout the 
// library to represent data streams, threads, channels, frames and attributes.
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

#include "vdifparse_types.h"

struct DataStream init_stream(enum InputMode mode) {
    struct DataStream ds = {mode};
    return ds;
}

void* init_input(enum InputMode mode) {
    void* di;
    switch (mode) { 
        case FileMode: di = calloc(1, sizeof(struct DataStreamInput_File));
        case StreamMode: di = calloc(1, sizeof(struct DataStreamInput_Stream));
    }
    return di;
}

void* init_frame(enum DataFormat format) {
    void* df;
    void* header;
    void* data;
    if (format == VDIF || format == VDIF_LEGACY) {
        df = (struct DataFrame_VDIF*)calloc(1, sizeof(struct DataFrame_VDIF));
        header = (struct VDIFHeader*)malloc(sizeof(struct VDIFHeader));
        if (format == VDIF) {
            data = calloc(1, VDIF_EXTENDED_DATA_BYTES);
        }
        ((struct DataFrame_VDIF*)df)->header = header;
        ((struct DataFrame_VDIF*)df)->extended_data = data;
    } else if (format == CODIF) {
        df = (struct DataFrame_CODIF*)calloc(1, sizeof(struct DataFrame_CODIF));
        header = (struct CODIFHeader*)malloc(sizeof(struct CODIFHeader));
        data = calloc(1, CODIF_METADATA_BYTES);
        ((struct DataFrame_VDIF*)df)->header = header;
        ((struct DataFrame_VDIF*)df)->extended_data = data;
    }
    return df;
}

char* get_error_message(int error_code) {
    switch(error_code) {
        case SUCCESS: return "Success.";
        case FAILED_TO_OPEN_FILE: return "Could not open file.";
        case FILE_HEADER_INVALID: return "First bytes of file were not a valid header. File may be misaligned or malformed.";
        case UNRECOGNISED_VERSION: return "Version field was unrecognised value. Cannot interpret data with unknown format.";
        case REACHED_END_OF_FILE: return "Reached EOF before completing requested action. No more frames available.";
        // TODO other types of errors...
        default: return "INVALID STATUS CODE";
    }
}

unsigned int get_header_length(enum DataFormat format) {
    switch (format) {
        case VDIF_LEGACY: return 16;
        case VDIF: return 32;
        case CODIF: return 64;
    }
}

FILE* get_file_handle(struct DataStream ds) {
    if (ds.mode == FileMode && ds.input != NULL) {
        return ((struct DataStreamInput_File*)ds.input)->file_handle;
    }
    return (FILE*)NULL;
}

unsigned int should_buffer_frame(struct DataStream ds, const void* frame) {
    // TODO check if selected thread, if frame is invalid and gap policy is SkipInvalid, etc.
    return 1;
}