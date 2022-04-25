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

#define BUFFER_FRAMES 1
#define CODIF_VERSION 0b111
#define CODIF_METADATA_BYTES 20
#define VDIF_EXTENDED_DATA_BYTES 16

enum StatusCode {
    SUCCESS = 0,
    UNKNOWN_FORMAT = -1,
    FAILED_TO_OPEN_FILE = -2,
    FILE_HEADER_INVALID = -3,
    UNRECOGNISED_VERSION = -4,
    REACHED_END_OF_FILE = -5,
    BAD_FORMAT_DESIGNATOR = -6,
}; // NOTE: keep codes < 0 so that result >= 0 indicates success

enum InputMode { FileMode, StreamMode };
enum DataFormat { VDIF=1, VDIF_LEGACY, CODIF };
enum DataType { RealData, ComplexData };
enum GapPolicy  { SkipInvalid, InsertInvalid };

// MARK: Data stream input types

typedef struct DataStreamInput_File {
    FILE* file_handle;
} DataStreamInput_File;

typedef struct DataStreamInput_Stream {
    unsigned int buffer_depth;
} DataStreamInput_Stream;

typedef struct {
    const enum InputMode mode;
    union {
        DataStreamInput_File* file;
        DataStreamInput_Stream* stream;
    };
} DataStreamInput;

// MARK: VDIF format types

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

typedef struct VDIFExtendedData_None {
    uint32_t extended_data1 : 24;
    uint8_t extended_data_version : 8;
    uint32_t extended_data2 : 32;
    uint32_t extended_data3 : 32;
    uint32_t extended_data4 : 32;
} VDIFExtendedData_None;

typedef struct VDIFExtendedData_NICT {
    uint32_t sample_rate : 23;
    uint8_t unit_flag : 1;
    uint8_t extended_data_version : 8;
    uint32_t synch_pattern : 32;
    uint64_t station_name : 64;
} VDIFExtendedData_NICT;

typedef struct VDIFExtendedData_ALMA {
    // TODO decode "magic nibble" ?
    uint32_t synch_identifier : 24;
    uint8_t extended_data_version : 8;
    uint32_t extended_data2 : 32;
    uint32_t extended_data3 : 32;
    uint32_t extended_data4 : 32;
} VDIFExtendedData_ALMA;

typedef struct VDIFExtendedData_NRAO {
    uint32_t sample_rate : 23;
    uint8_t unit_flag : 1;
    uint8_t extended_data_version : 8;
    uint32_t synch_pattern : 32;
    uint32_t tuning_word : 32;
    uint8_t personality_type : 8;
    uint8_t minor_revision : 4;
    uint8_t major_revision : 4;
    uint8_t electronic_sideband : 1;
    uint8_t sub_band : 3;
    uint8_t intermediate_frequency : 4;
    uint8_t digital_backend : 4;
    uint8_t unassigned : 4;
} VDIFExtendedData_NRAO;

typedef struct VDIFExtendedData_Multiplex {
    uint32_t unassigned : 16;
    uint32_t validity_mask_length : 8;
    uint32_t extended_data_version : 8;
    uint32_t synch_pattern : 32;
    uint64_t validity_mask : 64;
} VDIFExtendedData_Multiplex;

typedef struct VDIFExtendedData_Haystack {
    uint32_t extended_data1 : 24;
    uint8_t extended_data_version : 8;
    uint32_t extended_data2 : 32;
    uint32_t extended_data3 : 32;
    uint32_t extended_data4 : 32;
} VDIFExtendedData_Haystack;

typedef struct {
    enum VDIFExtendedDataVersion version;
    union {
        VDIFExtendedData_None* none;
        VDIFExtendedData_NICT* nict;
        VDIFExtendedData_ALMA* alma;
        VDIFExtendedData_NRAO* nrao;
        VDIFExtendedData_Multiplex* multiplex;
        VDIFExtendedData_Haystack* haystack;
    };
}   VDIFExtendedData;

typedef struct VDIFHeader {
    uint32_t seconds_from_epoch : 30;
    uint8_t legacy_mode : 1;
    uint8_t invalid_flag : 1;
    uint32_t data_frame_number : 24;
    uint8_t reference_epoch : 6;
    uint8_t unassigned_field : 2;
    uint32_t frame_length : 24;
    uint8_t log2_num_channels : 5;
    uint8_t vdif_version_number : 3;
    uint16_t station_id : 16;
    uint16_t thread_id : 10;
    uint8_t bits_per_sample : 5;
    uint8_t data_type : 1;
} VDIFHeader;

typedef struct DataFrame_VDIF {
    VDIFHeader* header;
    enum VDIFExtendedDataVersion edv;
    VDIFExtendedData* extended_data;
    uint32_t* data;
} DataFrame_VDIF;

// MARK: CODIF format types

// may need different cases for different CODIF versions in the future
// TODO: no idea where the canonical source for this is?
enum CODIFMetadataVersion { 
    Empty = 0x0000, 
    CryoPaf = 0x5043,
    // TODO are there others?
};

typedef struct CODIFMetadata_None {
    uint32_t synch_pattern : 32;
    uint16_t metadata_version : 16;
    uint16_t metadata1 : 16;
    uint64_t metadata2 : 64;
    uint64_t metadata3 : 64;
} CODIFMetadata_None;

typedef struct {
    enum CODIFMetadataVersion version;
    union {
        CODIFMetadata_None* none;
    };
} CODIFMetadata;

typedef struct CODIFHeader {
    // TODO fields
    uint32_t frame_length : 32;
    // TODO fields
} CODIFHeader;

typedef struct DataFrame_CODIF {
    CODIFHeader* header;
    enum CODIFMetadataVersion mdv;
    CODIFMetadata* metadata;
    uint32_t* data;
} DataFrame_CODIF;

typedef struct {
    const enum DataFormat format;
    union {
        DataFrame_VDIF* vdif;
        DataFrame_CODIF* codif;
    };
} DataFrame;

// MARK: Stream types

typedef struct DataStream {
    DataStreamInput input;
    enum DataFormat format;

    unsigned int data_rate;
    unsigned int num_channels;
    unsigned int bits_per_sample;
    unsigned int num_threads;

    enum GapPolicy gap_policy;

    unsigned int buffered_frames;
    DataFrame frames[BUFFER_FRAMES];
} DataStream;

char* get_error_message(int error_code);

DataStream init_stream(enum InputMode mode);
DataStreamInput init_input(enum InputMode mode);
DataFrame init_frame(enum DataFormat format);

unsigned int get_header_length(enum DataFormat format);
unsigned int get_frame_length(DataFrame df);

int ingest_format_designator(DataStream* ds, const char* format_designator);
int ingest_structured_filename(DataStream* ds, char* file_path);

FILE* get_file_handle(DataStream ds);

static inline void set_data_format(DataStream* ds, enum DataFormat format) { ds->format = format; }
static inline void set_data_rate(DataStream* ds, unsigned int data_rate ) { ds->data_rate = data_rate; }
static inline void set_num_channels(DataStream* ds, unsigned int num_channels ) { ds->num_channels = num_channels; }
static inline void set_bits_per_sample(DataStream* ds, unsigned int bits_per_sample ) { ds->bits_per_sample = bits_per_sample; }
static inline void set_num_threads(DataStream* ds, unsigned int num_threads ) { ds->num_threads = num_threads; }

unsigned int should_buffer_frame(DataStream ds, const DataFrame frame);

#endif // VDIFPARSE_TYPES_H
