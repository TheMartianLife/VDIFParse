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


char* time_for_epoch_seconds(unsigned int epoch, unsigned long int seconds) {
    unsigned int year = 2000 + (epoch / 2);
    unsigned int month = 1 + ((epoch % 2) * 6);
    char* iso_epoch= sprintf("%d-%d-01T00:00:00+0000", year, month);
    struct tm tm; // make a time object
    strptime(iso_epoch, "%Y-%m-%dT%H:%m::%S%z", &tm);
    // TODO check leap seconds in C time_t handling
    tm.tm_sec += seconds;
    return mktime(&tm);
}


char* string_for_input_format(enum InputFormat format) {
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


char* string_for_edv(enum ExtendedDataVersion version) {
    switch (version) {
        case None: return "None"; 
        case NICT: return "NICT"; 
        case ALMA: return "ALMA";
        case NRAO: return "NRAO";
        case Multiplex: return "Multiplex";
        case Haystack: return "Haystack";
    }
}


char* string_for_frequency(unsigned long int frequency) {
    char* string = (char*)malloc(128 * sizeof(char));
    int whole, part = 0;
    if (frequency > _1e9) {
        whole = frequency / _1e9;
        part = (frequency % _1e9) / _1e6;
        sprintf(string, "%d.%d GHz", whole, part);
    } else if (frequency > 1e6) {
        whole = frequency / _1e6;
        part = (frequency % _1e6) / _1e3;
        sprintf(string, "%d.%d MHz", whole, part);
    } else if (frequency > 1e3) {
        whole = frequency / _1e3;
        part = frequency % _1e3;
        sprintf(string, "%d.%d kHz", whole, part);
    } else {
        sprintf(string, "%ld Hz", frequency);
    }
    return string;
}