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
    test("Correct header length", get_header_length(ds.format) == 32);
    test("Correct default gap policy", get_gap_policy(ds) == SkipInvalid);

    // Test set stream attributes
    set_gap_policy(&ds, InsertInvalid);
    test("Successfully get default gap policy", get_gap_policy(ds) == InsertInvalid);

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
    
    DataFrame_VDIF* df = (struct DataFrame_VDIF*)ds.frames[0];
    VDIFHeader* header = df->header; // header is correct right now

    printf("==DATA FRAME TESTS\n");

    // Test ingestion of header field information
    test("Correct frame length", header->frame_length == (8032 / 8));
    test("Correct num channels", header->log2_num_channels == log2(2));
    test("Correct bits per sample", header->bits_per_sample == (2 - 1));
    test("Correct data type", header->data_type == RealData);
    test("Correct thread ID", header->thread_id == 0);
    test("Correct format version", header->vdif_version_number == 0);
    test("Correct invalid flag", header->invalid_flag == 0);
    test("Correct seconds from epoch", header->seconds_from_epoch == 7100400);
    test("Correct reference epoch", header->reference_epoch == 43);
    test("Correct frame number", header->data_frame_number == 0);

    char* test_file_path2 = "/Users/mars/University/Coursework/data/m1010_yg_no0003.vdif";
}
