// vdifparse_utils.c - provides general utility functions such as for printing
// structs, converting between enum and string types, and more.
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
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>

#include "vdifparse_utils.h"

#define _1e9 1000000000
#define _1e6 1000000
#define _1e3 1000


void raise_exception(const char *format, ...) {
    va_list args;
    
    va_start(args, format);
    fprintf(stderr, "\033[0;31mERROR: ");
    vfprintf(stderr, format, args);
    fprintf(stderr, "\033[0m\n");
    va_end(args);
    
    exit(EXIT_FAILURE);
}


void raise_warning(const char *format, ...) {
    va_list args;
    
    va_start(args, format);
    fprintf(stderr, "\033[0;33mWARNING: ");
    vfprintf(stderr, format, args);
    fprintf(stderr, "\033[0m\n");
    va_end(args);
}

char* string_for_datetime(datetime dt) {
    char* time_str = malloc(32 * sizeof(char));
    strftime(time_str, 32, "%Y-%m-%dT%H:%M:%SZ", &dt);
    return time_str;
}


char* string_for_input_mode(enum InputMode mode) {
    switch (mode) {
        case FileMode: return "FileMode";
        case StreamMode: return "StreamMode";
    }
}


char* string_for_data_format(enum DataFormat format) {
    switch (format) {
        case VDIF: return "VDIF";
        case CODIF: return "CODIF";
        case VDIF_LEGACY: return "VDIFL";
    }
}


char* string_for_data_type(enum DataType type) {
    switch (type) {
        case RealData: return "real";
        case ComplexData: return "complex";
    }
}

char* string_for_gap_policy(enum GapPolicy policy) {
    switch (policy) {
        case SkipInvalid: return "SkipInvalid";
        case InsertInvalid: return "InsertInvalid";
    }
}


char* string_for_edv(enum VDIFExtendedDataVersion version) {
    switch (version) {
        case None: return "None"; 
        case NICT: return "NICT"; 
        case ALMA: return "ALMA";
        case NRAO: return "NRAO";
        case Multiplex: return "Multiplex";
        case Haystack: return "Haystack";
    }
}


char* string_for_hertz(uint32_t hertz) {
    char* output = malloc(128 * sizeof(char));
    int whole, part = 0;
    if (hertz > _1e9) {
        whole = hertz / _1e9;
        part = (hertz % _1e9) / _1e6;
        sprintf(output, "%d.%d GHz", whole, part);
    } else if (hertz > 1e6) {
        whole = hertz / _1e6;
        part = (hertz % _1e6) / _1e3;
        sprintf(output, "%d.%d MHz", whole, part);
    } else if (hertz > 1e3) {
        whole = hertz / _1e3;
        part = hertz % _1e3;
        sprintf(output, "%d.%d kHz", whole, part);
    } else {
        sprintf(output, "%d Hz", hertz);
    }
    return output;
}

static inline int is_ascii_char(char byte) {
    return (byte > 0x2f && byte < 0x5b) || (byte > 0x60 && byte < 0x7b);
}

char* string_for_ascii(uint64_t sequence) {
    char* ascii = malloc(9 * sizeof(char));
    int valid_chars = 0;
    for (int i = 7; i >= 0; i--) {
        char byte = ((sequence >> (8 * i)) & 0xff);
        if (is_ascii_char(byte)) {
            ascii[valid_chars] = byte;
            valid_chars++;
        }
    }
    ascii[valid_chars] = TERM_CHAR;
    return ascii;
}

unsigned int* string_to_numeric(const char* string_value) {
    char* non_digits;
    unsigned int* num_ptr = malloc(sizeof(unsigned int));
    unsigned int num_value = (unsigned int)strtoul(string_value, &non_digits, 10);
    if (string_value[0] != TERM_CHAR && *non_digits != TERM_CHAR) {
        // cast to numeric failed
        return (unsigned int*)NULL;
    }
    num_ptr = &num_value;
    return num_ptr;
}

int split_string(const char* string_value, const char* separators, char*** out) {
    int length = strlen(string_value);
    char** tokens = NULL;
    int num_tokens = 0;
    char* token = strtok(strdup(string_value), separators);
    while (token != NULL) {
        num_tokens++;
        tokens = realloc(tokens, num_tokens * sizeof(char*));
        tokens[num_tokens - 1] = token;
        token = strtok(NULL, separators);
    }
    *out = tokens;
    return num_tokens;
}

void print_stream(DataStream ds) {
    fprintf(stdout, "DataStream\n");
    fprintf(stdout, "_input_mode: %s\n", string_for_input_mode(ds.input.mode));
    fprintf(stdout, "_gap_policy: %s\n", string_for_gap_policy(ds.gap_policy));
    fprintf(stdout, "_buffered_frames: %u\n", ds.num_buffered_frames);
}

void print_frame(DataFrame df) {
    fprintf(stdout, "DataFrame_%s\n", df.format == CODIF ? "CODIF" : "VDIF");
    fprintf(stdout, "_station_id: %s\n", get_station_id(df));
    fprintf(stdout, "_thread_id: %u\n", get_thread_id(df));
    fprintf(stdout, "_frame_number: %lu\n", get_frame_number(df));
    fprintf(stdout, "_data_type: %s numbers\n", string_for_data_type(get_data_type(df)));
    fprintf(stdout, "_num_channels: %lu\n", get_num_channels(df));
    fprintf(stdout, "_bits_per_sample: %u bit(s)\n", get_bits_per_sample(df));
    fprintf(stdout, "_num_samples: %llu\n", get_num_samples(df));
    fprintf(stdout, "_start_time: %s\n", string_for_datetime(get_start_time(df)));
    // TODO extended data fields
}
