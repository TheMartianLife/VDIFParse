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

int decode_frame(DataStream ds, const DataFrame* df, unsigned long num_samples, float** out, unsigned long* valid_samples) {
    unsigned long decoded_samples = 0;
    // TODO scrub for cursor if mid-frame
    
    // TODO decode

    // TODO some sort of incorporate partial result function (for thread safety) here

    return decoded_samples;
}