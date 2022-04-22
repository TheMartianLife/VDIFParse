#include <stdio.h>
#include "vdifparse.h"


void test(char* description, uint8_t condition) {
    if (condition) {
        fprintf(stdout, "%s: \033[0;32mTrue\033[0m\n", description);
    } else {
        fprintf(stdout, "%s: \033[0;31mFalse\033[0m\n", description);
    }
}

int main(int argc, char** argv) {
    fprintf(stdout, "===BEGIN TEST===\n");
    char* test_file_path = "/Users/mars/test_data/m0921_Mp_264_042000.vdif";

    struct DataStream ds = open_file(test_file_path); 
    struct DataFrame_VDIF* df = (struct DataFrame_VDIF*)ds.frames[0];
    struct VDIFHeader* header = df->header;

    // Data stream attributes
    test("Correct format", ds.format == VDIF);
    test("Correct header length", get_header_length(ds.format) == 32);
    test("Correct default gap policy", get_gap_policy(ds) == SkipInvalid);

    // Data thread attributes
    // test("Correct frame length", header->frame_length == 8032);
    // test("Correct num channels", header->num_channels == 2);
    // test("Correct bits per sample", header->bits_per_sample == 2);
    // test("Correct data type", header->data_type == RealData);
    // test("Correct station ID", header->station_id == "Tt");

    // Data channel attributes


    // Data frame attributes
    // test("Correct thread ID", header->thread_id == 0);
    // test("Correct format version", header->format_version == 0);
    // test("Correct invalid flag", header->invalid_flag == 0);
    // test("Correct seconds from epoch", header->seconds_from_epoch == 7100400);
    // test("Correct reference epoch", header->reference_epoch == 43);
    // test("Correct frame number", header->frame_number == 0);
    // test("Correct EDV", header->extended_data_version == 0);

    // other API functions
    set_gap_policy(&ds, InsertInvalid);
    test("Successfully get default gap policy", get_gap_policy(ds) == InsertInvalid);

    char* test_file_path2 = "/Users/mars/University/Coursework/data/m1010_yg_no0003.vdif";

    fprintf(stdout, "===END TEST===\n");
}
