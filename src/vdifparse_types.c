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

#include <string.h>
#include <libgen.h>
#include <ctype.h>

#include "vdifparse_types.h"

char* get_error_message(int error_code) {
    switch(error_code) {
        case SUCCESS: return "Success.";
        case FAILED_TO_OPEN_FILE: return "Could not open file.";
        case FILE_HEADER_INVALID: return "First bytes of file were not a valid header. File may be misaligned or malformed.";
        case UNRECOGNISED_VERSION: return "Version field was unrecognised value. Cannot interpret data with unknown format.";
        case REACHED_END_OF_FILE: return "Reached EOF before completing requested action. No more frames available.";
        case BAD_FORMAT_DESIGNATOR: return "Format designator did not follow ([a-zA-Z]+[_-])?\\d+-\\d+-\\d+(-\\d+)? expected format.";
        // TODO other types of errors...
        default: return "INVALID STATUS CODE";
    }
}

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

int ingest_format_name(struct DataStream* ds, char* format_name) {
    // TODO
}

int ingest_format_designator(struct DataStream* ds, char* format_designator) {
    // first, let's see if this is a "simple" data stream
    if (strchr(format_designator, '+') == NULL) {
        int length = strlen(format_designator);
        char* arg = calloc(length + 1, sizeof(char));
        int char_num = 0;
        char arg_num = 0;
        for (int i = 0; i < length; i++) {
            if (format_designator[i] == '-' || format_designator[i] == '_') {
                arg[char_num] = '\n';
                switch (arg_num) {
                    case 0: 
                        if (!isdigit(arg)) {
                            int status = ingest_format_name(ds, arg);
                            if (status != SUCCESS) {
                                return BAD_FORMAT_DESIGNATOR;
                            }
                            arg_num++;
                        }
                    case 1: ds->data_rate = (unsigned int)atoi(arg);
                        break;
                    case 2: ds->num_channels = (unsigned int)atoi(arg);
                        break;
                    case 3: ds->bits_per_sample = (unsigned int)atoi(arg);
                        break;
                    case 4: ds->num_threads = (unsigned int)atoi(arg);
                        break;
                }
                arg_num++;
            } else {
                arg[char_num] = format_designator[i];
                char_num++;
            }
        }
    } else {
        // TODO: complex data streams
    }
    return SUCCESS;
}

int ingest_structured_filename(struct DataStream* ds, char* file_path) {
    char* filename = basename(file_path);
    // TODO
    return SUCCESS;
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