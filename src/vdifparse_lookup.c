// vdifparse_lookup.c - provides LookupHolder type and initialises lookup tables
// for offset binary-encoded data when required (lazily).
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

#include <stdlib.h>
#include <complex.h>

#include "vdifparse_lookup.h"

// a single global variable to save wasted work
// even though I know globals are bad
// but trying to singleton in a non-OO language would be worse
// so forgive me this one evil
LookupHolder lookup_holder = { NULL };

static inline float luts_2level(char key) { 
    return (key) ? 1.0 : -1.0;
}
static inline float luts_4level(char key) { 
    const float lookup[4] = {-M5A_2BIT_HIGH, -1.0, 1.0, M5A_2BIT_HIGH};
    return lookup[(int)key];
}
static inline float luts_16level(char key) {
    return ((float)key - 8.0) / FOUR_BIT_1_SIGMA;
}
static inline float luts_high(key) {
    return ((float)key - 128.0) / 3.3;
}

static float** make_lookup_table_real(char num_bits) {
    float** new_luts = malloc(256 * sizeof(float*));
    char num_segments = 8 / num_bits;
    char key;
    for (int i = 0; i < 256; i++) {
        new_luts[i] = malloc(num_segments * sizeof(float));
        for (int j = 0; j < num_segments; j++) {
            switch (num_bits) {
                case 1: key = (i >> j) & 0b1;
                    new_luts[i][j] = luts_2level(key);
                    break;
                case 2: key = (i >> (2 * j)) & 0b11;
                    new_luts[i][j] = luts_4level(key);
                    break;
                case 4: key = (i >> (4 * j)) & 0b1111;
                    new_luts[i][j] = luts_16level(key);
                    break;
                case 8: new_luts[i][j] = luts_high(i);
                    break;
                default: return (float**)NULL;
            }
        }
    }
    return new_luts;
}

static float** make_lookup_table_complex(char num_bits) {
    float** new_luts = malloc(256 * sizeof(float*));
    char num_segments = 8 / num_bits;
    float value;
    char key_q;
    char key_i;
    for (int i = 0; i < 256; i++) {
        new_luts[i] = malloc(num_segments * sizeof(float));
        for (int j = 0; j < num_segments; j++) {
            switch (num_bits) {
                case 1: key_q = (i >> (2 * j)) & 0b1;
                    key_i = (i >> ((2 * j) + 1)) & 0b1;
                    value = luts_2level(key_q) + (luts_2level(key_i) * I);
                    break;
                case 2: key_q = (i >> (4 * j)) & 0b11;
                    key_i = (i >> ((4 * j) + 2)) & 0b11;
                    value = luts_4level(key_q) + (luts_4level(key_i) * I);
                    break;
                case 4: key_q = j & 0b1111;
                    key_i = (j >> 4) & 0b1111;
                    value = luts_16level(key_q) + (luts_16level(key_i) * I);
                    break;
                default: return (float**)NULL;
            }
            new_luts[i][j] = value;
        }
    }
    return new_luts;
}

static float** get_luts(LookupHolder* lookup, char num_bits, enum DataType type) {
    if (type == RealData) {
        switch (num_bits) {
            case 1: return lookup->luts1bit;
            case 2: return lookup->luts2bit;
            case 4: return lookup->luts4bit;
            case 8: return lookup->luts8bit;
            default: break;
        }
    }
    if (type == ComplexData) {
        switch (num_bits) {
            case 1: return lookup->luts1bit_complex;
            case 2: return lookup->luts2bit_complex;
            case 4: return lookup->luts4bit_complex;
            default: break;
        }
    }
    return (float**)NULL;
}

static void set_luts(LookupHolder* lookup, char num_bits, enum DataType type, float*** luts) {
    if (type == RealData) {
        switch (num_bits) {
            case 1: lookup->luts1bit = *luts;
                return;
            case 2: lookup->luts2bit = *luts;
                return;
            case 4: lookup->luts4bit = *luts;
                return;
            case 8: lookup->luts8bit = *luts;
                return;
            default: return;
        }
    }
    if (type == ComplexData) {
        switch (num_bits) {
            case 1: lookup->luts1bit_complex = *luts;
                return;
            case 2: lookup->luts2bit_complex = *luts;
                return;
            case 4: lookup->luts4bit_complex = *luts;
                return;
            default: return;
        }
    }
}

float** get_lookup_table(char num_bits, enum DataType type) {
    LookupHolder* lookup = &lookup_holder;
    float** luts = get_luts(lookup, num_bits, type);
    if (luts != NULL) { return luts; }
    float** new_luts;
    switch (type) {
        case RealData: new_luts = make_lookup_table_real(num_bits);
            break;
        case ComplexData: new_luts = make_lookup_table_complex(num_bits);
            break;
    }
    set_luts(lookup, num_bits, type, &new_luts);
    return get_luts(lookup, num_bits, type);
}