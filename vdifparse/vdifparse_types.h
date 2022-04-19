// vdifparse_types.h - defines the DataStream type and associated enum types
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

#define MAX_BITS_PER_SAMPLE 32
#define MAX_DATA_THREADS 1024
#define MAX_FRAME_LENGTH 134217728 // bytes
#define ASCII_0 0x30

enum InputMode { FileMode, StreamMode };
enum DataFormat { VDIF, VDIF_LEGACY, CODIF };
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
    enum DataFormat format; // file format (VDIF, CODIF, VDIF_LEGACY)
    unsigned char frame_header_length; // size of frame header in bytes

    unsigned short* selected_threads;
    struct DataThread** threads; // information about each thread
    enum GapPolicy gap_policy; // how invalid samples should be treated

    
    FILE* _input_file_handle;
    struct DataFrame* _frame_buffer;
    unsigned char _all_threads_selected;
    unsigned short _num_selected_threads;
};

struct DataThread {
    unsigned short thread_id;
    unsigned long frame_length; // size of frame in bytes, incl. header
    unsigned long num_channels;
    unsigned int bits_per_sample;
    char* station_id; // 16-bit uint or 2-char ASCII code of source device(s)

    struct DataChannel** channels;
    unsigned char _all_channels_set;
};

struct DataChannel {
    char* name;
    float frequency;
    float bandwidth;
};

struct DataFrame {
    unsigned short thread_id;
    unsigned char format_version; // version of VDIF/CODIF used
    unsigned char invalid_flag; // whether source device(s) detected malformation
    unsigned long seconds_from_epoch;
    unsigned int reference_epoch;
    unsigned long frame_number;
    enum DataType data_type; // whether data represents real or complex numbers
    unsigned int extended_data_version;
    // TODO: CODIF/EDV fields
    unsigned long sample_rate;
    unsigned char* station_name;

    unsigned int* data_buffer;
};

struct DataStream* _init_stream();
struct DataThread* _init_thread();
struct DataChannel* _init_channel();
struct DataFrame* _init_frame();

#endif // VDIFPARSE_TYPES_H
