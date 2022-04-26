<img align="right" src="docs/logo.png" style="padding:10px;width:20%;">

# VDIF Parse

> :warning: **WARNING**: This project is in early development and is not yet suitable for public use. **This repo is public for feedback purposes only.**

A simple C library for parsing and manipulating stream- or file-based radio telescope data in **VDIF**[^1] or **CODIF**[^2] formats.

[^1]: VLBI Data Interchange Format (source: [vlbi.org](https://vlbi.org/wp-content/uploads/2019/03/VDIF_specification_Release_1.1.1.pdf))
[^2]: CSIRO Oversampled Data Interchange Format (source: CSIRO internal)

## Usage

VDIFparse can be given data to parse in either of two ways: 

* **from streamed or piped input** (`StreamMode`), or 
* **from a file** (`FileMode`). 

These loosely correlate with the two ways a user is likely to interact with VDIF data: **live** (as data comes in from a telescope or similar source) or **later** (on historical data at rest).

**Data Input**

```c
// OPTION A: StreamMode (open a data sink to buffer data into)
struct DataStream ds_stream = open_sink();
// (configure the data stream here)
ingest_data(&ds_stream, num_bytes, &source_data);
// (use the data stream here)
close(&ds_stream);

// OPTION B: FileMode (open a file to read from)
struct DataStream ds_file = open_file("gre53_ef_scan035_fd1024-16-2-16.vdif");
// (configure and use the data stream here)
close(&ds_file);
```
**Configuration**

```c
// TODO: set thread attributes
set_thread_attributes(&ds, 0, 1414.0, 16, "Ch01");
// 0 = thread number
// 1414.0 = radio frequency (MHz)
// 16 = bandwidth (MHz)
// "Ch01" = name of the signal

// TODO: set attributes of multi-channel thread

// configure whether to skip or include data gaps
set_gap_policy(&ds, InsertInvalid);

// TODO: output thread selection

// TODO: seek to timestamp
```

**Data Processing and Output**

```c
// output raw data (such as to a new file)
read_frames(&ds, num_frames_to_read, &output_buffer);

// decode and output data (such as for input to a software spectrometer)
decode_samples(&ds, num_samples_to_read, &output_buffer, &valid_samples);

// TODO: fanning multi-thread input into multiple single-thread outputs?
```

**Data Inspection**

```c
// the top-level data stream has some simple fields
enum DataFormat format = ds.format;
enum GapPolicy = ds.gap_policy;
unsigned int num_buffered_frames = ds.buffered_frames;

// but sub-structs require getting to ensure safe use of union types 
// whose fields may be either VDIF or CODIF formats
DataFrame df = df.frames[0];
unsigned long num_channels = get_num_channels(df);
char* station_id = get_station_id(df);

// TODO fields that vary
```

## Limitations

* Limit to the number of channels per stream: 2<sup>16</sup> (65,536) instead of theoretical 2<sup>31</sup> (2,147,483,648)

## License

This project is licensed under the terms of the [GNU General Public License, version 3](https://www.gnu.org/licenses/gpl-3.0.en.html). **This is a [copyleft](https://www.gnu.org/licenses/copyleft.en.html) license.**
