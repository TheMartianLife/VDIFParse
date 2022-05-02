#include <stdio.h>
#include <string.h>
#include <math.h>

#include "src/vdifparse_utils.h"
#include "vdifparse.h"


void test(char* description, uint8_t condition) {
    if (condition) {
        fprintf(stdout, "- %s: \033[0;32mTrue\033[0m\n", description);
    } else {
        fprintf(stdout, "- %s: \033[0;31mFalse\033[0m\n", description);
    }
}

int main(int argc, char** argv) {
    char* test_file_path = "/Users/mars/test_data/m0921_Mp_264_042000.vdif";

    DataStream ds = open_file(test_file_path); 

    printf("==DATA STREAM TESTS\n");

    // Test first init filemode stream with peek format
    test("Correct format", ds.format == VDIF);
    test("Correct default gap policy", ds.gap_policy == SkipInvalid);

    // Test set stream attributes
    set_gap_policy(&ds, InsertInvalid);
    test("Successfully set gap policy", ds.gap_policy == InsertInvalid);

    ds.num_selected_channels = 2; // TODO remove

    float** out = NULL;
    unsigned long* valid_samples = NULL;
    unsigned int num_samples = 4;
    decode_samples(&ds, num_samples, &out, &valid_samples);
    double samples[4][2] = { { -3.335900, -3.335900 }, 
        { -1.000000, -3.335900 },
        { 1.000000, 3.335900 }, 
        { 1.000000, 3.335900 }
    };
    int matching_samples = 1;
    for (int i = 0; i < num_samples; i++) {
        if (out[0][i] != (float)samples[i][0]) {
            matching_samples = matching_samples && 0;
        }
        if (out[1][i] != (float)samples[i][1]) {
            matching_samples = matching_samples && 0;
        }
    }
    test("Correct decode of first data samples", matching_samples);

    printf("==FORMAT DESIGNATOR TESTS\n");

    // Test use of format designator to provide information
    int status = set_format_designator(&ds, "VDIF-1024-16-2-4");
    test("Could parse format designator", status == SUCCESS);
    if (status == SUCCESS) {
        test("Correct data rate", ds.data_rate == 1024);
        test("Correct num channels", ds.num_channels == 16);
        test("Correct bits per sample", ds.bits_per_sample == 2);
        test("Correct num threads", ds.num_threads == 4);
    }
    
    DataFrame df = ds.frames[0];
    VDIFHeader* header = df.vdif->header;

    printf("==DATA FRAME TESTS\n");

    // Test ingestion of header field information
    test("Correct header length", get_header_length(df) == 32);
    test("Correct frame length", get_frame_length(df) == 8032);
    test("Correct num channels", get_num_channels(df) == 2);
    test("Correct bits per sample", get_bits_per_sample(df) == 2);
    test("Correct data type",  get_data_type(df) == RealData);
    test("Correct thread ID", get_thread_id(df) == 0);
    // test("Correct format version", header->vdif_version_number == 0);
    // test("Correct invalid flag", header->invalid_flag == 0);
    test("Correct seconds from epoch", get_seconds_from_epoch(df) == 7100400);
    // test("Correct reference epoch", header->reference_epoch == 43);
    test("Correct frame number", get_frame_number(df) == 0);

    print_stream(ds);
    print_frame(df);

    close(&ds);

    // char* test_file_path2 = "/Users/mars/University/Coursework/data/m1010_yg_no0003.vdif";
}
