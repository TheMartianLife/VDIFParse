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
#include "vdifparse_input.h"
#include "vdifparse_utils.h"

#define FD_DRATE_ARG 0
#define FD_NCHAN_ARG 1
#define FD_NBITS_ARG 2
#define FD_NTHREAD_ARG 3

#define DWORD_BITS 32

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
    char* designator = strdup(format_designator); // duplicate const
    // first, let's see if this is a "simple" data stream
    char** combined_streams;
    int num_streams = split_string(format_designator, "+", &combined_streams);
    if (num_streams > 1) {
        // TODO: support complex data streams?
        ds->is_compound_datastream = 1;
    } else if (num_streams == 1) {
        char** stream_args;
        int num_args = split_string(combined_streams[0], "_-", &stream_args);
        // check whether args beging with optional format prefix
        int offset = (string_to_numeric(stream_args[0]) == NULL);
        for (int i = FD_DRATE_ARG; i <= FD_NTHREAD_ARG; i ++) {
            // check if optional thread count arg has been omitted
            if (i == FD_NTHREAD_ARG && num_args <= (FD_NTHREAD_ARG + offset)) {
                ds->num_threads = 1;
                continue;
            }
            // otherwise get int of string arg value and assign
            unsigned int* value = string_to_numeric(stream_args[i + offset]);
            if (value == NULL) { return BAD_FORMAT_DESIGNATOR; }
            switch (i) {
                case FD_DRATE_ARG: ds->data_rate = *value;
                    break;
                case FD_NCHAN_ARG: ds->num_channels = *value;
                    break;
                case FD_NBITS_ARG: ds->bits_per_sample = *value;
                    break;
                case FD_NTHREAD_ARG: ds->num_threads = *value;
                    break;
                default: break; // to appease the compiler         
            }
        }
        return SUCCESS;
    }
    return BAD_FORMAT_DESIGNATOR;
}

static int ingest_aux_info(DataStream* ds, const char* string_value) {
    if (strlen(string_value) < 3) { return FAILURE; }
    char* code = "xx"; // for first 2 chars
    memcpy(code, &string_value, 2);
    char* value; // for the remaining chars
    memcpy(value, &string_value[2], strlen(string_value) - 2);
    if (strcasecmp(code, "st") == 0) {
        // start time
    } else if (strcasecmp(code, "fd") == 0) {
        // format designator maybe present, or may just say "compound"
        if (strcasecmp(value, "compound") == 0) {
            ds->is_compound_datastream = 1;
        } else {
            return ingest_format_designator(ds, value);
        }
    } else {
        return BAD_FILE_NAME;
    }
    return SUCCESS; 
}

int ingest_structured_filename(DataStream* ds, const char* file_path) {
    char* file_name = basename(strdup(file_path)); // duplicate const
    char** args;
    int num_args = split_string(file_name, "_", &args);
    if (num_args < 4) { return BAD_FILE_NAME; }
    int status = SUCCESS;
    for (int i = 0; i < num_args; i++) {
        if (i == num_args - 1) {
            // last arg should contain file extension
            // TODO ingest data format from this if possible?
            char* separator = strchr(args[i], '.');
            if(separator != NULL) { // if there is a dot in this arg
                *separator = '\0'; // keep only what was before the dot
            }
        }
        switch (i) {
            case 0: // experiment name
                // TODO save somewhere?
                break;
            case 1: // station code
                // TODO save somewhere?
                break;
            case 2: // scan name
                // TODO save somewhere?
                break;
            case 3: // start time or aux info
                // TODO save somewhere?
                break;
            default: // aux info
                status = ingest_aux_info(ds, args[i]);
        }
        if (status != SUCCESS) { return status; }
    }
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
    unsigned long long frame_bytes = get_frame_length(df);
    if (df.format == CODIF) {
        // sample block length (in 64-bit words) already accounts for padding
        unsigned long sample_bytes = df.codif->header->sample_block_length * 8;
        return frame_bytes / sample_bytes;
    } else {
        // calculate size of segment (AKA complete sample)
        unsigned long long segment_bits = (bits_per_sample * num_channels);
        unsigned long long frame_words = (frame_bytes / 4);
        if (segment_bits > DWORD_BITS) {
            // complete samples are taking more than a word each
            if (bits_per_sample > DWORD_BITS) {
                // then padding will mean each sample will be 2 words long
                return (frame_words / 2) / num_channels;
            } else if (bits_per_sample == DWORD_BITS) {
                // each sample will be exactly 1 word long
                return frame_words / num_channels;
            } else {
                // otherwise it's just the number of samples in the frame
                unsigned int samples_per_word = DWORD_BITS / bits_per_sample;
                // divided by the number of channels that make up a complete
                return (frame_words * samples_per_word) / num_channels;
            }
        } else {
            // however many complete samples fit in each 32-bit word
            unsigned int segments_per_word = DWORD_BITS / segment_bits;
            // times the number of 32-bit words in the frame
            return frame_words * segments_per_word;
        }
    }
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

int get_next_buffer_frame(DataStream* ds, DataFrame** frame) {
    // TODO thread lock
    unsigned int next_frame_num = ds->num_processed_frames;
    int status;
    if (next_frame_num >= ds->num_buffered_frames && ds->input.mode == FileMode) {
        // buffer more frames from file
        status = buffer_frames(ds, BUFFER_FRAMES);
    }
    ds->num_processed_frames++;
    // if we succeeded in finding more frames
    if (next_frame_num < ds->num_buffered_frames) {
        *frame = &ds->frames[next_frame_num];
        // TODO thread unlock
        return SUCCESS;
    } else {
        *frame = (DataFrame*)NULL;
        // TODO thread unlock
        if (status == SUCCESS) { status = FAILURE; }
        return (ds->input.mode == FileMode) ? status : REACHED_END_OF_BUFFER;
    }
}

unsigned int should_buffer_frame(DataStream ds, const DataFrame df) {
    // TODO check if selected thread, if frame is invalid and gap policy is SkipInvalid, etc.
    return 1;
}