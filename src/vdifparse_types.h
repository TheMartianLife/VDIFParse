// vdifparse_types.h - defines the struct and enum types used throughout the 
// library to represent data streams, threads, channels, frames and attributes.
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

#include <stdlib.h>
#include <stdio.h>

#define CODIF_VERSION 0b111
#define CODIF_METADATA_BYTES 20
#define VDIF_EXTENDED_DATA_BYTES 16

enum StatusCode {
    SUCCESS,
    FAILED_TO_OPEN_FILE,
    FILE_HEADER_INVALID,
    UNRECOGNISED_VERSION,
    REACHED_END_OF_FILE,
};

char* get_error_message(int error_code);

// MARK: Stream types and functions

enum InputMode { FileMode, StreamMode };
enum DataFormat { VDIF=1, VDIF_LEGACY, CODIF };
enum DataType { RealData, ComplexData };
enum GapPolicy  { SkipInvalid, InsertInvalid };

struct DataStream init_stream(enum InputMode mode);
void* init_input(enum InputMode mode);
void* init_frame(enum DataFormat format);

unsigned int get_header_length(enum DataFormat format);

struct DataStream {
    const enum InputMode mode;
    enum DataFormat format;
    unsigned int num_threads;

    enum GapPolicy gap_policy;

    unsigned int buffered_frames;
    void* input;
    void** frames;
};

// fields that may exist, and will return NULL if not
FILE* get_file_handle(struct DataStream ds);

// utility functions
unsigned int should_buffer_frame(struct DataStream ds, const void* frame);

struct DataStreamInput_File {
    FILE* file_handle;
};

struct DataStreamInput_Stream {

};

struct DataThread {

};

// inline unsigned int get_num_channels(struct DataThread dt);
// inline unsigned int get_extended_data_version(struct DataThread dt);

// MARK: VDIF format types and functions

struct VDIFHeader {
    uint32_t seconds_from_epoch : 30;
    uint32_t legacy_mode : 1;
    uint32_t valid_flag : 1;
    uint32_t data_frame_number : 24;
    uint32_t reference_epoch : 6;
    uint32_t unassigned_field : 2;
    uint32_t frame_length : 24;
    uint32_t log2_num_channels : 5;
    uint32_t vdif_version_number : 3;
    uint32_t station_id : 16;
    uint32_t thread_id : 10;
    uint32_t bits_per_sample : 5;
    uint32_t data_type : 1;
};

// may need different cases for different VDIF versions in the future
// for current keys, see https://vlbi.org/vlbi-standards/vdif/
enum VDIFExtendedDataVersion { 
    None = 0x00,       // see vlbi.org/wp-content/uploads/ ...
    NICT = 0x01,       // ... /2019/03/vdif_extension_0x01.pdf
    ALMA = 0x02,       // ... /2019/03/alma-vdif-edv.pdf
    NRAO = 0x03,       // ... /2019/03/vdif_extension_0x03.pdf
    Multiplex = 0x04,  // ... /2019/03/edv4description.pdf
    Haystack = 0xab, // ... /2019/03/vdif_extension_0xab.pdf
};

struct VDIFExtendedData {
    uint32_t extended_data1 : 24;
    uint32_t extended_data_version : 8;
    uint32_t extended_data2 : 32;
    uint32_t extended_data3 : 32;
    uint32_t extended_data4 : 32;
};

struct VDIFExtendedData_NICT {
    uint32_t sample_rate : 23;
    uint32_t unit_flag : 1;
    uint32_t extended_data_version : 8;
    uint32_t synch_pattern : 32;
    uint64_t station_name : 64;
};

struct VDIFExtendedData_ALMA {
    // TODO decode "magic nibble" ?
    uint32_t synch_identifier : 24;
    uint32_t extended_data_version : 8;
    uint32_t extended_data2 : 32;
    uint32_t extended_data3 : 32;
    uint32_t extended_data4 : 32;
};

struct VDIFExtendedData_NRAO {
    uint32_t sample_rate : 23;
    uint32_t unit_flag : 1;
    uint32_t extended_data_version : 8;
    uint32_t synch_pattern : 32;
    uint32_t tuning_word : 32;
    uint32_t personality_type : 8;
    uint32_t minor_revision : 4;
    uint32_t major_revision : 4;
    uint32_t electronic_sideband : 1;
    uint32_t sub_band : 3;
    uint32_t intermediate_frequency : 4;
    uint32_t digital_backend : 4;
    uint32_t unassigned : 4;
};

struct VDIFExtendedData_Multiplex {
    uint32_t unassigned : 16;
    uint32_t validity_mask_length : 8;
    uint32_t extended_data_version : 8;
    uint32_t synch_pattern : 32;
    uint64_t validity_mask : 64;
};

struct VDIFExtendedData_Haystack {
    uint32_t extended_data1 : 24;
    uint32_t extended_data_version : 8;
    uint32_t extended_data2 : 32;
    uint32_t extended_data3 : 32;
    uint32_t extended_data4 : 32;
};

struct DataFrame_VDIF {
    struct VDIFHeader* header;
    enum VDIFExtendedDataVersion edv;
    struct VDIFExtendedData* extended_data;
    uint32_t* data;
};

// inline unsigned long get_seconds_from_epoch(struct DataFrame_VDIF df);
// inline unsigned int get_legacy_mode(struct DataFrame_VDIF df);
// inline unsigned int get_valid_flag(struct DataFrame_VDIF df);
// inline unsigned long get_data_frame_number(struct DataFrame_VDIF df);

// MARK: CODIF format types and functions

struct CODIFHeader {
    // TODO fields
    uint64_t frame_length : 32;
    // TODO fields
};

// may need different cases for different CODIF versions in the future
// TODO: no idea where the canonical source for this is?
enum CODIFMetadataVersion { 
    Empty = 0x0000, 
    CryoPaf = 0x5043,
    // TODO are there others?
};

struct DataFrame_CODIF {
    const struct CODIFHeader* header;
    const enum CODIFMetadataVersion mdv;
    const struct CODIFMetadata* metadata;
    const uint32_t* data;
};

#endif // VDIFPARSE_TYPES_H
