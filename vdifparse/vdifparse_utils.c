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

void print_attributes(struct InputStream* in) {
    char* data_format = string_for_input_format(in->format);
    int format_version = in->format_version;
    char* data_type = string_for_data_type(in->data_type);
    unsigned int edv = in->extended_data_version;
    fprintf(vp_stdout, "Format: %s (v%d) \n", data_format, format_version);
    fprintf(vp_stdout, "Channels: %ld\n", in->num_channels);
    fprintf(vp_stdout, "Data type: %s numbers\n", data_type);
    fprintf(vp_stdout, "Bits per sample: %d\n", in->bits_per_sample);
    fprintf(vp_stdout, "Station ID: %s\n", in->station_id);
    fprintf(vp_stdout, "EDV: %d (%s)\n", edv, string_for_edv(edv));
    fprintf(vp_stdout, "Data length: %ld bytes \n", in->frame_data_length);
    if (in->sample_rate != 0) {
        char* frequency = string_for_frequency(in->sample_rate);
        fprintf(vp_stdout, "Sample rate: %s", frequency);
    }
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
        case CornerTurned: return "Corner Turned";
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