<img align="right" width="150" src=docs/logo.png>

# VDIFparse

> :warning: **WARNING**: This project is in early development and is not yet suitable for public use. This repo is public for feedback purposes only.

**© 2022 Mars Buttfield-Addison**

A simple C library for parsing stream- or file-based radio telescope data in **VDIF**[^1] or **CODIF**[^2] formats.

[^1]: VLBI Data Interchange Format (source: [vlbi.org](https://vlbi.org/wp-content/uploads/2019/03/VDIF_specification_Release_1.1.1.pdf))
[^2]: CSIRO Oversampled Data Interchange Format (source: CSIRO internal)

## Usage

VDIFparse can be given data to parse in either of two ways: **from streamed or piped input** or **from a file**. These loosely correlate with the two ways a user is likely to interact with VDIF data: **live** (as data comes in from a telescope or similar source) or **later** (on historical data at rest).

**Usage with Live Data:** `StreamMode`

```c
in = open_stream();

close(in);
```

**Usage with Historical Data:** `FileMode`

```c
in = open_file("input_file.vdif");

close(in);
```

## License

This project is licensed under the terms of the [GNU General Public License, version 3](https://www.gnu.org/licenses/gpl-3.0.en.html). **This is a [copyleft](https://www.gnu.org/licenses/copyleft.en.html) license.**