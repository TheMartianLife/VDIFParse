// vdifparse_lookup.h - provides LookupHolder type and initialises lookup tables
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
#ifndef VDIFPARSE_LOOKUP_H
#define VDIFPARSE_LOOKUP_H

#define M5A_2BIT_HIGH 3.3359
#define FOUR_BIT_1_SIGMA 2.95
// #define BB_2BIT_HIGH 3.316505 // from baseband.py

#include "vdifparse_types.h"

typedef struct LookupHolder {
    float** luts1bit;
    float** luts2bit;
    float** luts4bit;
    float** luts8bit;
    float** luts1bit_complex;
    float** luts2bit_complex;
    float** luts4bit_complex;
} LookupHolder;

float** get_lookup_table(char num_bits, enum DataType type);

#endif // VDIFPARSE_LOOKUP_H
