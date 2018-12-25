# CLCC


This program is a driver for calling installed OpenCL compilers to see compiler warnings.

## Building

This program requires both at least one OpenCL driver, but also the OpenCL headers to be installed.

```bash
mkdir build
cd build
cmake .. -G Ninja
ninja
```

## Usage

```bash
# clcc -h
Usage:
	clcc [-p platform] [-d device] [-q] file [ -- options... ]
	clcc -l
	clcc -h
Options
	file	the file to compile
	options	the options to pass to the OpenCL compiler
	-d	the OpenCL device to use
	-h	prints this message
	-h	lists the installed OpenCL devices and platforms
	-p	the OpenCL platform to use
	-q	print less output
	--	only options passed to the OpenCL compiler after this
```

`device` and `platform` accept arbitrary regular expressions which they match case insensitively to those reported by OpenCL.

`file` is the path -- relative or absolute -- to a OpenCL source file.

`options` correspond to any compile options that should be passed to the OpenCL compiler.

