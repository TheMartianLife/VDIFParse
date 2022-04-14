// vdifparse_headers.c - provides functions for interpreting format header 
// fields into stream attributes.
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

#include "vdifparse_headers.h"

void parse_vdif_header(struct InputStream* in) {
    // parse header field values
    unsigned int *header_words = (unsigned int *)(in->raw_data_buffer);
    // WORD 0, BYTES 7-0
    unsigned int word0 = header_words[0];
    in->valid_flag = (unsigned char)(word0 >> 31);
    unsigned char legacy_mode = (unsigned char)((word0 >> 30) & 0x2);
    in->format = (legacy_mode) ? VDIF_LEGACY : VDIF;
    in->seconds_from_epoch = (unsigned long int)(word0 & 0x3fffffff);
    // WORD 1, BYTES 7-0
    unsigned int word1 = header_words[1];
    unsigned int reference_epoch = (unsigned int)((word1 >> 24) & 0x3f);
    in->reference_epoch_month = (unsigned char)(1 + ((reference_epoch % 2) * 6)); // Jan or July
    in->reference_epoch_year = (unsigned short int)(2000 + (reference_epoch / 2));
    in->frame_number = (unsigned long int)(word1 & 0xffffff);
    // WORD 2, BYTES 7-0
    unsigned int word2 = header_words[2];
    in->format_version = (unsigned char)(word2 >> 29);
    in->num_channels = pow(2, (unsigned long int)((word2 >> 24) & 0x1f));
    in->frame_length = (unsigned long int)((word2 & 0xffffff) * 8);
    // WORD 3, BYTES 7-0
    unsigned int word3 = header_words[3];
    in->data_type = (unsigned char)(word3 >> 31);
    in->bits_per_sample = (unsigned int)((word3 >> 26) & 0x1f) + 1;
    in->thread_id = (unsigned short int)((word3 >> 16) & 0x3ff);
    unsigned char station_id1 = ((word3 >> 8) & 0xff);
    char* id_string = (char*)malloc(8 * sizeof(char));
    if (station_id1 == ASCII_0) {
        // NOTE: anything > 0x7f isn't a valid ASCII char
        sprintf(id_string, "%d", (word3 & 0xffff));
    } else {
        unsigned char station_id2 = (word3 & 0xff);
        sprintf(id_string, "%c%c", station_id1, station_id2);
    }
    in->station_id = id_string;
    // WORD 4, BITS 31-24
    if (in->format == VDIF) {
        // (if format == VDIFL, then EDV = None by default)
        unsigned int word4 = header_words[4];
        in->extended_data_version = (unsigned int)(word4 >> 24);
        // further information from any included metadata fields
        parse_vdif_extended_data(in);
    }
    // now some inferred attributes
    in->frame_header_length = (unsigned char)((in->format == VDIF) ? 32 : 16);
    in->frame_data_length = in->frame_length - in->frame_header_length;
}

void parse_codif_header(struct InputStream* in) {
    unsigned int *header_words = (unsigned int *)(in->raw_data_buffer);
    in->format = CODIF;

    unsigned int word0 = header_words[0];
    unsigned int word1 = header_words[1];
    unsigned int word2 = header_words[2];
    unsigned int word3 = header_words[3];
    unsigned int word4 = header_words[4];
    unsigned int word5 = header_words[5];
    unsigned int word6 = header_words[6];
    unsigned int word7 = header_words[7];
}

void parse_vdif_extended_data(struct InputStream* in) {
    switch (in->extended_data_version) {
        case None: return;
        case NICT: parse_vdif_extended_data_nict(in);
        case ALMA: parse_vdif_extended_data_alma(in);
        case NRAO: parse_vdif_extended_data_nrao(in);
        case CornerTurned: parse_vdif_extended_data_corner_turned(in);
        case Haystack: return parse_vdif_extended_data_haystack(in);
    }
}

void parse_vdif_extended_data_nict(struct InputStream* in) {
    unsigned int *header_words = (unsigned int *)(in->raw_data_buffer);

    unsigned int word4 = header_words[4];
    // unsigned char megahertz = (unsigned char)((word4 >> 23) & 0x1);
    // unsigned long int sample_rate = (unsigned long int)(word4 & 0xfffffe);
    // in->sample_rate = (megahertz) ? sample_rate * 1e6 : sample_rate * 1e3;

    unsigned int word6 = header_words[6];
    unsigned int word7 = header_words[7];
}

void parse_vdif_extended_data_alma(struct InputStream* in) {
    unsigned int *header_words = (unsigned int *)(in->raw_data_buffer);
    unsigned int word4 = header_words[4];
    unsigned int word5 = header_words[5];
    unsigned int word6 = header_words[6];
    unsigned int word7 = header_words[7];
}

void parse_vdif_extended_data_nrao(struct InputStream* in) {
    unsigned int *header_words = (unsigned int *)(in->raw_data_buffer);
    unsigned int word4 = header_words[4];
    unsigned int word5 = header_words[5];
    unsigned int word6 = header_words[6];
    unsigned int word7 = header_words[7];
}

void parse_vdif_extended_data_corner_turned(struct InputStream* in) {
    unsigned int *header_words = (unsigned int *)(in->raw_data_buffer);
    unsigned int word4 = header_words[4];
    unsigned int word5 = header_words[5];
    unsigned int word6 = header_words[6];
    unsigned int word7 = header_words[7];
}

void parse_vdif_extended_data_haystack(struct InputStream* in) {
    unsigned int *header_words = (unsigned int *)(in->raw_data_buffer);
    unsigned int word4 = header_words[4];
    unsigned int word5 = header_words[5];
    unsigned int word6 = header_words[6];
    unsigned int word7 = header_words[7];
}