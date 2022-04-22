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


time_t time_for_epoch_seconds(uint32_t epoch, uint32_t seconds) {
    uint8_t year = 2000 + (epoch / 2);
    uint8_t month = 1 + ((epoch % 2) * 6);
    char* iso_epoch = (char*)calloc(32, sizeof(char));
    sprintf(iso_epoch, "%d-%d-01T00:00:00+0000", year, month);
    struct tm tm; // make a time object
    strptime(iso_epoch, "%Y-%m-%dT%H:%m::%S%z", &tm);
    // TODO check leap seconds in C time_t handling
    tm.tm_sec += seconds;
    return mktime(&tm);
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
    char* output = (char*)calloc(128, sizeof(char));
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