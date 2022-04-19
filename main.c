#include <stdio.h>
#include "vdifparse.h"

int main(int argc, char** argv) {
    char* test_file_path = "/Users/mars/University/Coursework/data/m0921_Mp_264_042000.vdif";
    struct InputStream* in = open_file(test_file_path);
    printf("\n");
    print_attributes(in);
    printf("\n");

    // char* test_file_path2 = "/Users/mars/University/Coursework/data/m1010_yg_no0003.vdif";
    // struct InputStream* in2 = open_file(test_file_path2);
    // print_attributes(in2);
    // printf("\n");
}
