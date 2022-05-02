// vdifparse_decode.h - provides functions to decode frame data from encoded
// sample representations to (real or complex) float arrays.
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

#include "vdifparse_decode.h"
#include "vdifparse_lookup.h"

#define REP_OFFSET 0
#define REP_2sCOMP 1
#define REP_FLOAT 2
#define REP_INVALID 3

int decode_frame(DataStream ds, const DataFrame* df, unsigned long num_samples, float*** out, unsigned long** valid_samples) {
    unsigned long decoded_samples = 0;
    int encoding = REP_OFFSET;
    if (df->format == CODIF) {
        encoding = df->codif->header->sample_representation;
    }
    
    unsigned long long sample = 0; 
    // TODO scrub for cursor if mid-frame
    unsigned long long frame_samples = get_num_samples(*df);
    // TODO what to do if not encoded? e.g. CODIF float type?
    float** lookup = get_lookup_table(get_bits_per_sample(*df), get_data_type(*df));
    // TODO wth do you do if a sample size if over 1 byte?

    // iterate over frame samples

    for (int i = 0; i < get_num_samples(*df); i++) {
        // TODO remove
        unsigned long word = df->vdif->data[i / 8];
        float* byte = lookup[word & 0xff];
        (*out)[0][decoded_samples] = byte[0];
        (*out)[1][decoded_samples] = byte[1];
        decoded_samples++;
        (*out)[0][decoded_samples] = byte[2];
        (*out)[1][decoded_samples] = byte[3];
        decoded_samples++;
        byte = lookup[(word >> 8) & 0xff];
        (*out)[0][decoded_samples] = byte[0];
        (*out)[1][decoded_samples] = byte[1];
        decoded_samples++;
        (*out)[0][decoded_samples] = byte[2];
        (*out)[1][decoded_samples] = byte[3];
        decoded_samples++;
        if (decoded_samples >= num_samples) {
            break;
        }
    }
    // TODO remove
    (*valid_samples)[0] = decoded_samples;
    (*valid_samples)[1] = decoded_samples;

    // TODO some sort of "incorporate partial result" function (for thread safety) here

    return decoded_samples;
}