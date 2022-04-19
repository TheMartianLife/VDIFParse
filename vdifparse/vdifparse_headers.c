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


struct DataFrame* parse_header(struct DataStream* ds, unsigned int* data) {
    switch (ds->format) {
        case VDIF: return parse_vdif_header(ds, data);
        case VDIF_LEGACY: return parse_vdif_header(ds, data);
        case CODIF: return parse_codif_header(ds, data);
    }
}


struct DataFrame* parse_vdif_header(struct DataStream* ds, unsigned int* data) {
    if (ds->frame_header_length == 0) {
        // stream fields are not set, so this must be the first header
        // let's get the fields that stay the same throughout a stream
        unsigned char legacy_mode = (unsigned char)((data[0] >> 30) & 0x2);
        ds->format = (legacy_mode) ? VDIF_LEGACY : VDIF;
        ds->frame_header_length = (unsigned char)((legacy_mode) ? 16 : 32);
    }
    unsigned short thread_id = (unsigned short)((data[3] >> 16) & 0x3ff);
    if (!ds->threads[thread_id]) {
        // this must be the first header with this thread ID
        // let's get the fields that stay the same throughout a thread
        struct DataThread* dt = _init_thread();
        dt->frame_length = (unsigned long)((data[2] & 0xffffff) * 8);
        dt->num_channels = pow(2, (unsigned long)((data[2] >> 24) & 0x1f));
        dt->bits_per_sample = (unsigned int)((data[3] >> 26) & 0x1f) + 1;
        // the station ID requires a bit of special decoding
        unsigned char station_id1 = ((data[3] >> 8) & 0xff);
        char* id_string = (char*)malloc(8 * sizeof(char));
        if (station_id1 <= ASCII_0) {
            sprintf(id_string, "%d", (data[3] & 0xffff));
        } else {
            // NOTE: anything >0x7f isn't a valid ASCII char but as per 
            // the spec any station id not starting with a character 
            // <0x30 should still be interpreted as two ASCII characters
            // even though this may result in a nonsense id.
            unsigned char station_id2 = (data[3] & 0xff);
            sprintf(id_string, "%c%c", station_id1, station_id2);
        }
        dt->station_id = id_string;
        ds->threads[thread_id] = dt;
    }
    // now, let's get the fields that may be specific to this frame
    struct DataFrame* df = _init_frame();
    df->thread_id = thread_id;
    df->format_version = (unsigned char)(data[2] >> 29);
    df->invalid_flag = (unsigned char)(data[0] >> 31);
    df->reference_epoch = (unsigned int)((data[1] >> 24) & 0x3f);
    df->seconds_from_epoch = (unsigned long)(data[0] & 0x3fffffff);
    df->frame_number = (unsigned long)(data[1] & 0xffffff);
    df->data_type = (unsigned char)(data[3] >> 31);
    // and the extended data (if there is any)
    if (ds->format == VDIF) {
        // (if format == VDIF_LEGACY, then EDV = None by default)
        df->extended_data_version = (unsigned int)(data[4] >> 24);
        // further information from any included metadata fields
        parse_vdif_extended_data(df);
    }
    return df;
}


void parse_vdif_extended_data(struct DataFrame* df) {
    switch (df->extended_data_version) {
        case None: return;
        case NICT: parse_vdif_extended_data_nict(df);
        case ALMA: parse_vdif_extended_data_alma(df);
        case NRAO: parse_vdif_extended_data_nrao(df);
        case Multiplex: parse_vdif_extended_data_corner_turned(df);
        case Haystack: parse_vdif_extended_data_haystack(df);
    }
}


void parse_vdif_extended_data_nict(struct DataFrame* df) {

}


void parse_vdif_extended_data_alma(struct DataFrame* df) {

}


void parse_vdif_extended_data_nrao(struct DataFrame* df) {

}


void parse_vdif_extended_data_corner_turned(struct DataFrame* df) {

}


void parse_vdif_extended_data_haystack(struct DataFrame* df) {

}


struct DataFrame* parse_codif_header(struct DataStream* ds, unsigned int* data) {

}