#include <stdio.h>
#include "vdifparse.h"

int main(int argc, char** argv) {
    char* test_file_path = "/Users/mars/University/Coursework/data/m0921_Mp_264_042000.vdif";

    // Data stream attributes
    struct DataStream* ds = open_file(test_file_path, VDIF);
    print_stream_attributes(ds);
    printf("\n");

    // Data thread attributes
    struct DataThread* dt = ds->threads[ds->_frame_buffer->thread_id];
    print_thread_attributes(dt);
    printf("\n");

    // set_channel_attributes();

    // char* test_file_path2 = "/Users/mars/University/Coursework/data/m1010_yg_no0003.vdif";
    // struct InputStream* in2 = open_file(test_file_path2);
    // print_attributes(in2);
    // printf("\n");
}
