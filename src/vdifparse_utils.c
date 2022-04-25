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


datetime time_for_epoch_seconds(uint32_t epoch, uint32_t seconds) {
    uint16_t year = 2000 + (epoch / 2);
    uint8_t month = 1 + ((epoch % 2) * 6);
    char* iso_epoch = malloc(32 * sizeof(char));
    sprintf(iso_epoch, "%02d-%02d-01T00:00:00+0000", year, month);
    datetime dt; // make a time object
    strptime(iso_epoch, "%Y-%m-%dT%H:%M:%S%z", &dt);
    time_t time = mktime(&dt);
    time += seconds;
    return *gmtime(&time);
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

void print_stream(DataStream ds) {
    // TODO
}

void print_frame(DataFrame df) {
    if (df.format == CODIF) {

    } else {
        VDIFHeader* head = df.vdif->header;
        char time_str[32];
        datetime timestamp = time_for_epoch_seconds(head->reference_epoch, head->seconds_from_epoch);
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %I:%M %p", &timestamp);
        fprintf(stdout, "VDIF Frame #%lu, begin %s\n", (unsigned long)head->data_frame_number, time_str);
        fprintf(stdout, "- Invalid: %hu\n", head->invalid_flag);
        fprintf(stdout, "- Legacy format: %hu\n", head->legacy_mode);
        fprintf(stdout, "- VDIF version: %hu\n", head->vdif_version_number);
        fprintf(stdout, "- Number of channels: %lu\n", (unsigned long)pow(2, head->log2_num_channels));        
        fprintf(stdout, "- Frame length: %lu bytes\n", (unsigned long)head->frame_length * 8);
        fprintf(stdout, "- Data type: %s\n", string_for_data_type(head->data_type));
        fprintf(stdout, "- Bits per sample: %u bit(s)\n", head->bits_per_sample + 1);
        fprintf(stdout, "- Thread ID: %u\n", head->thread_id);
        fprintf(stdout, "- Station ID: %s\n", string_for_ascii((uint64_t)head->station_id));
        // TODO extended data fields
    }
}
