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

#include <stdio.h>
#include <stdlib.h>

#define vp_stdout stdout // TODO: remove
#define vp_stderr stderr // TODO: remove

enum InputMode { FileMode, StreamMode };
enum InputFormat { VDIF, VDIF_LEGACY, CODIF };
enum DataType { RealData, ComplexData };
enum GapPolicy  { SkipInvalid, InsertInvalid };

// may need different cases for different VDIF versions in the future
// for current keys, see https://vlbi.org/vlbi-standards/vdif/
enum ExtendedDataVersion { 
    None = 0,       // see vlbi.org/wp-content/uploads/ ...
    NICT = 1,       // ... /2019/03/vdif_extension_0x01.pdf
    ALMA = 2,       // ... /2019/03/alma-vdif-edv.pdf
    NRAO = 3,       // ... /2019/03/vdif_extension_0x03.pdf
    Multiplex = 4,  // ... /2019/03/edv4description.pdf
    Haystack = 171, // ... /2019/03/vdif_extension_0xab.pdf
};


// All Data Frames must have the same Data Frame Header length, Data Array length, #channels, #bit/sample and Station ID.

struct DataStream {
    // fields that remain the same between frames and data threads
    enum InputMode mode; // StreamMode (inits empty) or FileMode (opens file)
    enum InputFormat format; // file format (VDIF, CODIF, VDIF_LEGACY)
    unsigned char frame_header_length; // size of frame header in bytes

    struct ThreadAttributes** thread_attrs; // information about each thread
    enum GapPolicy gap_policy; // how invalid samples should be treated

    // internal (utility) attributes
    unsigned int _cursor_epoch;
    unsigned long int _cursor_second;
    FILE* _input_file_handle;
    const unsigned char* _raw_data_buffer;
};

struct ThreadAttributes {
    // information gleaned from headers
    unsigned char format_version; // version of VDIF/CODIF used
    unsigned long int frame_length; // size of frame in bytes, incl. header
    unsigned long int num_channels;
    unsigned int bits_per_sample;
    char* station_id; // 16-bit uint or 2-char ASCII code of source device(s)
    unsigned int extended_data_version;
    // fields that change between frames
    unsigned char valid_flag; // 0 if source device(s) detected malformation
    unsigned long int seconds_from_epoch;
    unsigned int reference_epoch;
    unsigned long int frame_number;
    enum DataType data_type; // whether data represents real or complex numbers
    unsigned short int thread_id;
    // TODO: CODIF/EDV fields
    unsigned long int sample_rate;
    unsigned char* station_name;

    float frequency;
    float bandwidth;
    unsigned char* channel_name;

};

struct DataStream* _init_stream();
struct ThreadAttributes* _init_thread_attributes();
void print_attributes(struct DataStream* in, unsigned short int thread_num);

#endif // VDIFPARSE_TYPES_H
