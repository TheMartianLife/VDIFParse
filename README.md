<img align="right" width="100" src=docs/logo.png>

# VDIFparse


**© 2022 Mars Buttfield-Addison**

A simple C library for parsing stream- or file-based radio telescope data in **VDIF**[^1] or **CODIF**[^2] formats.

[^1]: VLBI Data Interchange Format
[^2]: CSIRO Oversampled Data Interchange Format

## Usage

VDIFparse can be given data to parse in either of two ways: **from streamed or piped input** or **from a file**. These loosely correlate with the two ways a user is likely to interact with VDIF data: **live** (as data comes in from a telescope or similar source) or **later** (on historical data at rest).

<details><summary>Usage with Live Data (Stream)</summary>

```c
in = open_stream();

close(in);
```

</details>

<details><summary>Usage with Historical Data (Files)</summary>

```c
in = open_file("input_file.vdif");

close(in);
```
</details>

## License

This project is licensed under the terms of the [GNU General Public License, version 3](https://www.gnu.org/licenses/gpl-3.0.en.html). **This is a [copyleft](https://www.gnu.org/licenses/copyleft.en.html) license.**