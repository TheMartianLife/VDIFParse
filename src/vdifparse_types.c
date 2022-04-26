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
#include <math.h>

#include "vdifparse_types.h"
#include "vdifparse_utils.h"

DataStream init_stream(enum InputMode mode) {
    DataStreamInput input = init_input(mode);
    DataStream ds = { .input = input };
    return ds;
}

DataStreamInput init_input(enum InputMode mode) {
    DataStreamInput di = { .mode = mode };
    switch (mode) { 
        case FileMode: di.file = calloc(1, sizeof(DataStreamInput_File));
            break;
        case StreamMode: di.stream = calloc(1, sizeof(DataStreamInput_Stream));
            break;
    }
    return di;
}

DataFrame init_frame(enum DataFormat format) {
    DataFrame df = { .format = format };
    if (format == VDIF || format == VDIF_LEGACY) {
        df.vdif = calloc(1, sizeof(DataFrame_VDIF));
        VDIFHeader* header = calloc(1, sizeof(VDIFHeader));
        df.vdif->header = header;
        if (format == VDIF) {
            VDIFExtendedData* data = calloc(1, VDIF_EXTENDED_DATA_BYTES);
            df.vdif->extended_data = data;
        }
    } else if (format == CODIF) {
        df.codif = calloc(1, sizeof(DataFrame_CODIF));
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
                    case 1: ds->data_rate = (unsigned int)arg_value;
                        break;
                    case 2: ds->num_channels = (unsigned int)arg_value;
                        break;
                    case 3: ds->bits_per_sample = (unsigned int)arg_value;
                        break;
                    case 4: ds->num_threads = (unsigned int)arg_value;
                        break;
                    default: return BAD_FORMAT_DESIGNATOR; // too many args
                }
            }
            arg = strtok(NULL, "_-");
            arg_num++;
        }
        if (arg_num < 5) {
            // num threads arg was omitted, so use default value
            ds->num_threads = (unsigned int)1;
        }
    }
    return SUCCESS;
}

int ingest_structured_filename(DataStream* ds, const char* file_path) {
    // char* filename = basename(file_path);
    // TODO this
    return SUCCESS;
}

unsigned int get_frame_length(DataFrame df) {
    if (df.format == CODIF) {
        return get_data_length(df) + get_header_length(df);
    } else {
        return df.vdif->header->frame_length * 8;
    }
}

unsigned int get_header_length(DataFrame df) {
    switch (df.format) {
        case VDIF_LEGACY: return 16;
        case VDIF: return 32;
        case CODIF: return 64;
    }
}

unsigned int get_data_length(DataFrame df) {
    if (df.format == CODIF) {
        return df.codif->header->data_array_length * 8;
    } else {
        return get_frame_length(df) - get_header_length(df);
    }
}

enum DataType get_data_type(DataFrame df) {
    if (df.format == CODIF) {
        return (enum DataType)df.codif->header->data_type;
    } else {
        return (enum DataType)df.vdif->header->data_type;
    }
}

unsigned long get_frame_number(DataFrame df) {
    if (df.format == CODIF) {
        return df.codif->header->frame_number;
    } else {
        return df.vdif->header->frame_number;
    }    
}

unsigned int get_thread_id(DataFrame df) {
    if (df.format == CODIF) {
        return df.codif->header->thread_id;
    } else {
        return df.vdif->header->thread_id;
    }    
}

unsigned long get_num_channels(DataFrame df) {
    if (df.format == CODIF) {
        return df.codif->header->num_channels;
    } else {
        return (unsigned long)pow(2, df.vdif->header->log2_num_channels);
    }
}

unsigned int get_bits_per_sample(DataFrame df) {
    if (df.format == CODIF) {
        return df.codif->header->bits_per_sample;
    } else {
        return df.vdif->header->bits_per_sample + 1;
    } 
}


unsigned int get_reference_epoch_month(DataFrame df) {
    uint8_t reference_epoch;
    if (df.format == CODIF) {
        reference_epoch = df.codif->header->reference_epoch;
    } else {
        reference_epoch = df.vdif->header->reference_epoch;
    } 
    return (reference_epoch % 2) == 0 ? 1 : 7;
}

unsigned int get_reference_epoch_year(DataFrame df) {
    uint8_t reference_epoch;
    if (df.format == CODIF) {
        reference_epoch = df.codif->header->reference_epoch;
        return 2020 + (reference_epoch / 2);
    } else {
        reference_epoch = df.vdif->header->reference_epoch;
        return 2000 + (reference_epoch / 2);
    } 
}

unsigned long get_seconds_from_epoch(DataFrame df) {
    if (df.format == CODIF) {
        return df.codif->header->seconds_from_epoch;
    } else {
        return df.vdif->header->seconds_from_epoch;
    } 
}

char* get_station_id(DataFrame df) {
    unsigned int station_id;
    if (df.format == CODIF) {
        station_id = df.codif->header->station_id;
    } else {
        station_id = df.vdif->header->station_id;
    } 
    char* ascii_id = string_for_ascii((uint64_t)station_id);
    if (strlen(ascii_id) == 2) {
        return ascii_id; // valid 2-char ascii id
    }
    // otherwise it must be a numeric id
    char* numeric_id = malloc(6 * sizeof(char));
    sprintf(numeric_id, "%d", station_id);
    return numeric_id;
}

unsigned long long get_num_samples(DataFrame df) {
    int multiplier = (int)get_data_type(df) + 1; // real=1*bits, complex=2*bits
    unsigned int bits_per_sample = get_bits_per_sample(df) * multiplier;
    unsigned long num_channels = get_num_channels(df);
    // TODO work out for various word widths + padding
    unsigned long long frame_bytes = get_frame_length(df);
    return 0;
}

datetime get_start_time(DataFrame df) {
    unsigned int year = get_reference_epoch_year(df);
    unsigned int month = get_reference_epoch_month(df);
    unsigned long seconds = get_seconds_from_epoch(df);
    char* iso_string = malloc(32 * sizeof(char));
    sprintf(iso_string, "%02d-%02d-01T00:00:00+0000", year, month);
    datetime dt; // time as struct of components
    strptime(iso_string, "%Y-%m-%dT%H:%M:%S%z", &dt);
    time_t time = mktime(&dt); // time as seconds from C epoch
    time += seconds; // now to add seconds offset
    return *gmtime(&time); // and turn it back into a datetime struct
}

FILE* get_file_handle(DataStreamInput di) {
    if (di.mode == FileMode) {
        return di.file->file_handle;
    }
    raise_exception("data stream input file handle was NULL or input type was not FileMode.");
    return (FILE*)NULL;
}

unsigned int should_buffer_frame(DataStream ds, const DataFrame df) {
    // TODO check if selected thread, if frame is invalid and gap policy is SkipInvalid, etc.
    return 1;
}