// vdifparse_api.c - provides public API functions to open, parse, manipulate,
// and split VDIF and CODIF files for use in related software.
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

#include "vdifparse_api.h"
#include "vdifparse_decode.h"
#include "vdifparse_input.h"
#include "vdifparse_utils.h"

// MARK: deal with error responses

char* get_error_message(int error_code) {
    switch(error_code) {
        case SUCCESS: return "Success.";
        case FAILURE: return "Failure.";
        case FAILED_TO_OPEN_FILE: return "Could not open file.";
        case FILE_HEADER_INVALID: return "First bytes of file were not a valid header. File may be misaligned or malformed.";
        case UNRECOGNISED_VERSION: return "Version field was unrecognised value. Cannot interpret data with unknown format.";
        case REACHED_END_OF_FILE: return "Reached EOF before completing requested action. No more frames available.";
        case REACHED_END_OF_BUFFER: return "Reached buffer depth limit before completing requested action. No more frames available.";
        case BAD_FORMAT_DESIGNATOR: return "Format designator did not follow ([a-zA-Z]+[_-])?\\d+-\\d+-\\d+(-\\d+)? expected format.";
        case BAD_FILE_NAME: return "File name did not follow expected <experiment>_<station>_<scan>[_<aux>...].<extension> format.";
        case FAILED_MALLOC: return "Could not allocate required memory.";
        // TODO other types of errors...
        default: return "INVALID STATUS CODE";
    }
}

// MARK: initialise stream object

DataStream open_file(const char* file_path) {
    DataStream ds = init_stream(FileMode);
    int status = peek_file(&ds, file_path);
    if (status != SUCCESS) {
        raise_exception("file %s could not be opened.", file_path);
    }
    status = ingest_structured_filename(&ds, file_path);
    if (status != SUCCESS) {
        raise_warning("filename was not structured to specifications.");
    }
    return ds;
}


DataStream open_sink() {
    // unfortunately nothing else can be known at this time
    return init_stream(StreamMode);
}

// MARK: configure objects

int set_format_designator(DataStream* ds, const char* format_designator) {
    int status = ingest_format_designator(ds, format_designator);
    if (status != SUCCESS) {
        raise_warning("format designator %s could not be parsed.", format_designator);
    }
    return status;
}

// MARK: process data

int init_decode_output(DataStream ds, unsigned long num_samples, float*** out, unsigned long** valid_samples) {
    float** new_out = malloc(ds.num_selected_channels * sizeof(float*));
    if (new_out == NULL) { return FAILED_MALLOC; }
    unsigned long* new_samples = malloc(ds.num_selected_channels * sizeof(float));
    if (new_samples == NULL) { return FAILED_MALLOC; }
    for (long i = 0; i < ds.num_selected_channels; i++) {
        new_out[i] = malloc(num_samples * sizeof(float));
        if (new_out[i] == NULL) { return FAILED_MALLOC; }
        new_samples[i] = 0;
    }
    *out = new_out;
    *valid_samples = new_samples;
    return SUCCESS;
}

int decode_samples(DataStream* ds, unsigned long num_samples, float*** out, unsigned long** valid_samples) {
    // if samples to decode is 0, we have already succeeded
    if (num_samples < 1) { return SUCCESS; }
    int status;
    // if output buffers are not set up yet, do that
    // and if that fails, return with the error arising from the attempt
    if (*out == NULL || *out[0] == NULL || *valid_samples == NULL) {
        status = init_decode_output(*ds, num_samples, out, valid_samples);
        if (status != SUCCESS) { return status; }
    }
    // otherwise we actually have to do work
    unsigned long decoded_samples = 0;
    int last_buffer = 0;
    DataFrame* next_frame = malloc(sizeof(DataFrame));
    int has_frame = (get_next_buffer_frame(ds, &next_frame) == SUCCESS);
    while (has_frame && decoded_samples < num_samples) {
        status = decode_frame(*ds, next_frame, num_samples - decoded_samples, out, valid_samples);
        // TODO remove above
        // status = decode_frame(*ds, next_frame, num_samples - decoded_samples, &out[decoded_samples], valid_samples);
        if (status < SUCCESS) { return status; }
        decoded_samples += status; // otherwise response = samples decoded
        has_frame = get_next_buffer_frame(ds, &next_frame); // get next_frame
    }
    // TODO replace for StreamMode
    if (decoded_samples < num_samples) { return REACHED_END_OF_FILE; }
    return SUCCESS;
}

void close(DataStream* ds) {
    // free DataStreamInput structs and fields
    switch (ds->input.mode) {
        case FileMode: fclose(ds->input.file->file_handle);
            free(ds->input.file);
            break;
        case StreamMode: 
            free(ds->input.stream);
            break;
    }
    // TODO free DataFrame structs and fields
}