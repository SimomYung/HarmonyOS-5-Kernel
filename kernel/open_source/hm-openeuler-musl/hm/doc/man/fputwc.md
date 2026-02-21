# fputwc()
Put a wide-character code on a stream

## Synopsis
```c
#include <stdio.h>
#include <wchar.h>

wint_t fputwc(wchar_t wc, FILE *stream);
```

## Arguments 
- **wc**
A wide-character code will be written.
&nbsp;
- **stream**
The output stream is pointed to by *stream*.

## Library
libc

## Description
The fputwc() function writes the character corresponding to the wide-character code *wc* to the output stream pointed to by *stream*, at the position indicated by the associated file-position indicator for the stream, then advances the indicator appropriately. If the file cannot support positioning requests, or if the stream was opened with append mode, the character is appended to the output stream. If an error occurs while writing the character, the shift state of the output file is left in an undefined state.

## Returns
- **wc**
Put the wide-character code successfully, *wc* is returned. 
&nbsp;
- **WEOF**
An error occurs, *WEOF* is returned and error indicator for the stream is set, the *errno* is set.

## Errors
- **EAGAIN**
The *O_NONBLOCK* flag is set for the file descriptor underlying *stream*, and the write operation will be delayed.
&nbsp;
- **EBADF**
The file descriptor underlying *stream* is not valid open for writing.
&nbsp;
- **EFBIG**
Attempt to write to a file that exceeds the maximum file size, or attempt to write to a file that exceeds the file size of the process, or attempt to write to a file at or beyond the offset maximum.
&nbsp;
- **ENOSPC**
There is no free space on the device to store the file.
&nbsp;
- **ENOMEM**
Insufficient memory space is available.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO

