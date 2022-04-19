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
        // TODO invariant fields
}

void update_vdif_header_attributes(struct InputStream* in) {
    // TODO variant fields
}

void parse_vdif_extended_data(struct InputStream* in) {
    switch (in->extended_data_version) {
        case None: return;
        case NICT: parse_vdif_extended_data_nict(in);
        case ALMA: parse_vdif_extended_data_alma(in);
        case NRAO: parse_vdif_extended_data_nrao(in);
        case Multiplex: parse_vdif_extended_data_corner_turned(in);
        case Haystack: return parse_vdif_extended_data_haystack(in);
    }
}


void parse_vdif_extended_data_nict(struct InputStream* in) {
    unsigned int *header_words = (unsigned int *)(in->_raw_data_buffer);
    // WORD 4, BYTES 7-0
    unsigned int word4 = header_words[4];
    // WORD 5, BYTES 7-0
    unsigned int word5 = header_words[5];
    // WORD 6, BYTES 7-0
    unsigned int word6 = header_words[6];
    // WORD 7, BYTES 7-0
    unsigned int word7 = header_words[7];
}


void parse_vdif_extended_data_alma(struct InputStream* in) {
    unsigned int *header_words = (unsigned int *)(in->_raw_data_buffer);
    // WORD 4, BYTES 7-0
    unsigned int word4 = header_words[4];
    // WORD 5, BYTES 7-0
    unsigned int word5 = header_words[5];
    // WORD 6, BYTES 7-0
    unsigned int word6 = header_words[6];
    // WORD 7, BYTES 7-0
    unsigned int word7 = header_words[7];
}


void parse_vdif_extended_data_nrao(struct InputStream* in) {
    unsigned int *header_words = (unsigned int *)(in->_raw_data_buffer);
    // WORD 4, BYTES 7-0
    unsigned int word4 = header_words[4];
    // WORD 5, BYTES 7-0
    unsigned int word5 = header_words[5];
    // WORD 6, BYTES 7-0
    unsigned int word6 = header_words[6];
    // WORD 7, BYTES 7-0
    unsigned int word7 = header_words[7];
}


void parse_vdif_extended_data_corner_turned(struct InputStream* in) {
    unsigned int *header_words = (unsigned int *)(in->_raw_data_buffer);
    // WORD 4, BYTES 7-0
    unsigned int word4 = header_words[4];
    // WORD 5, BYTES 7-0
    unsigned int word5 = header_words[5];
    // WORD 6, BYTES 7-0
    unsigned int word6 = header_words[6];
    // WORD 7, BYTES 7-0
    unsigned int word7 = header_words[7];
}


void parse_vdif_extended_data_haystack(struct InputStream* in) {
    unsigned int *header_words = (unsigned int *)(in->_raw_data_buffer);
    // WORD 4, BYTES 7-0
    unsigned int word4 = header_words[4];
    // WORD 5, BYTES 7-0
    unsigned int word5 = header_words[5];
    // WORD 6, BYTES 7-0
    unsigned int word6 = header_words[6];
    // WORD 7, BYTES 7-0
    unsigned int word7 = header_words[7];
}


void parse_codif_header(struct InputStream* in) {
    unsigned int *header_words = (unsigned int *)(in->_raw_data_buffer);
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

void update_codif_header_attributes(struct InputStream* in) {
    // TODO
}