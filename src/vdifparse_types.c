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

#include "vdifparse_types.h"
#include "vdifparse_utils.h"

#define TERM_CHAR '\0'
#define BUFFER_FRAMES 1


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

DataStream init_stream(enum InputMode mode) {
    DataStream ds = {mode};
    DataStreamInput input = init_input(mode);
    ds.input = &input;
    ds.frames = malloc(sizeof(DataFrame*));
    return ds;
}

DataStreamInput init_input(enum InputMode mode) {
    DataStreamInput di = {mode};
    switch (mode) { 
        case FileMode: di.file = malloc(sizeof(DataStreamInput_File));
        case StreamMode: di.stream = malloc(sizeof(DataStreamInput_Stream));
    }
    return di;
}

DataFrame init_frame(enum DataFormat format) {
    DataFrame df = {format};
    if (format == VDIF || format == VDIF_LEGACY) {
        df.vdif = malloc(sizeof(DataFrame_VDIF));
        VDIFHeader* header = calloc(1, sizeof(VDIFHeader));
        df.vdif->header = header;
        if (format == VDIF) {
            VDIFExtendedData* data = calloc(1, VDIF_EXTENDED_DATA_BYTES);
            df.vdif->extended_data = data;
        }
    } else if (format == CODIF) {
        df.codif = malloc(sizeof(DataFrame_CODIF));
        CODIFHeader* header = calloc(1, sizeof(CODIFHeader));
        df.codif->header = header;
        CODIFMetadata* data = calloc(1, CODIF_METADATA_BYTES);
        df.codif->metadata = data;
    }
    return df;
}

int ingest_format_designator(DataStream* ds, const char* format_designator) {
    // first, let's see if this is a "simple" data stream
    // TODO: support complex data streams?
    char* designator = strdup(format_designator);
    if (strchr(designator, '+') == NULL) {
        char* arg = strtok(designator, "_-"); // strsep(format_designator, "_-");
        int arg_num = 0;
        int arg_value = 0;
        char* non_digits;
        while (arg != NULL) {
            arg_value = strtoul(arg, &non_digits, 10);
            // check if arg is numeric based on success of cast to ul
            if (arg[0] != TERM_CHAR && *non_digits != TERM_CHAR) {
                // assume this is the format name
                // TODO interpret this for data type/format?
                if (arg_num != 0) {
                    return BAD_FORMAT_DESIGNATOR; // only first arg may be non-numeric
                }
            } else {
                switch (arg_num) {
                    case 0: arg_num++; 
                    case 1: set_data_rate(ds, (unsigned int)arg_value);
                        break;
                    case 2: set_num_channels(ds, (unsigned int)arg_value);
                        break;
                    case 3: set_bits_per_sample(ds, (unsigned int)arg_value);
                        break;
                    case 4: set_num_threads(ds, (unsigned int)arg_value);
                        break;
                    default: return BAD_FORMAT_DESIGNATOR; // too many args
                }
            }
            arg = strtok(NULL, "_-");
            arg_num++;
        }
        if (arg_num < 5) {
            // num threads arg was omitted, so use default value
            set_num_threads(ds, (unsigned int)1);
        }
    }
    return SUCCESS;
}

int ingest_structured_filename(DataStream* ds, char* file_path) {
    // char* filename = basename(file_path);
    // TODO this
    return SUCCESS;
}

unsigned int get_header_length(enum DataFormat format) {
    switch (format) {
        case VDIF_LEGACY: return 16;
        case VDIF: return 32;
        case CODIF: return 64;
    }
}

unsigned int get_frame_length(DataFrame df) {
    if (df.format == CODIF) {
        return df.codif->header->frame_length * 8;
    } else {
        return df.vdif->header->frame_length * 8;
    }
}

FILE* get_file_handle(DataStream ds) {
    if (ds.input != NULL && ds.input->mode == FileMode) {
        return ds.input->file->file_handle;
    }
    raise_exception("data stream file handle was NULL or input type was not FileMode.");
    return (FILE*)NULL;
}

unsigned int should_buffer_frame(DataStream ds, const DataFrame frame) {
    // TODO check if selected thread, if frame is invalid and gap policy is SkipInvalid, etc.
    return 1;
}