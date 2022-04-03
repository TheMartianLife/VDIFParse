// vdifparse_types.h - defines the InputStream type and associated enum types
// used throughout the library.
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
#ifndef VDIFPARSE_TYPES_H
#define VDIFPARSE_TYPES_H

enum InputMode { FileMode, StreamMode };
enum InputFormat { VDIF, VDIF_LEGACY, CODIF };
enum DataType { RealData, ComplexData };
enum GapPolicy  { SkipInvalid, InsertInvalid };

// may need different cases for different VDIF versions in the future
// for current keys, see https://vlbi.org/vlbi-standards/vdif/
enum ExtendedDataVersion { 
    None = 0, 
    NICT = 1, 
    ALMA = 2, 
    NRAO = 3, 
    CornerTurned = 4, 
    Haystack = 171,
};

struct InputStream {
    enum InputMode mode;
    enum InputFormat format;
    unsigned int format_version;
    unsigned char header_length;
    unsigned char valid_flag;
    // TODO reference epoch + seconds offset
    unsigned long int frame_length;
    enum DataType data_type;
    unsigned long int num_channels;
    unsigned int bits_per_sample;
    char* station_id; // unsigned 16-bit int or 2-char ASCII
    // TODO middle fields
    unsigned int extended_data_version;

    unsigned long int frame_header_length;
    unsigned long int frame_data_length;
    unsigned long int sample_rate;
    unsigned char* station_name;

    enum GapPolicy gap_policy;

    const unsigned char* raw_data_buffer;
};

#endif // VDIFPARSE_TYPES_H
