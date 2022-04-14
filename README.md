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
// == Example usage in StreamMode
struct InputStream* in1 = open_stream();
// TODO: stream data in

// (do you work)
close(in1);

// == Example usage in FileMode
struct InputStream* in2 = open_file("input_file.vdif");
// in2 now contains first buffer of data, 
// and has parsed first header for attributes

// (do you work)
close(in2);
```

**Configuration**

```c
// TODO: set thread attributes

// configure whether to skip or include data gaps
set_gap_policy(in, InsertInvalid);

// TODO: output thread selection

// TODO: seek to timestamp
```

**Data Processing and Output**

```c
// output raw data (such as to a new file)
read_frames(in, num_frames_to_read, output_buffer);

// decode and output data (such as for input to a software spectrometer)
decode_samples(in, num_samples_to_read, output_buffer);

// TODO: fanning multi-thread input into multiple single-thread outputs?
```

## License

This project is licensed under the terms of the [GNU General Public License, version 3](https://www.gnu.org/licenses/gpl-3.0.en.html). **This is a [copyleft](https://www.gnu.org/licenses/copyleft.en.html) license.**
