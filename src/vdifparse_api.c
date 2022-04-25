// vdifparse_api.c - provides public API functions to open, parse, manipulate,
// and split VDIF and CODIF files for use in related software
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
        case BAD_FORMAT_DESIGNATOR: return "Format designator did not follow ([a-zA-Z]+[_-])?\\d+-\\d+-\\d+(-\\d+)? expected format.";
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
    // TODO remove
    buffer_frames(&ds, 1);
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

void close(DataStream* ds) {
    // free DataStreamInput structs and fields
    switch (ds->input.mode) {
        case FileMode: fclose(ds->input.file->file_handle);
            break;
        case StreamMode: 
            break;
    }
    // free DataFrame structs and fields

    free(ds);
}