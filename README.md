# Project Build and Run Script

The `mk_file.sh` script compiles and runs your project using Docker, with options for debugging, normal execution, and warnings-only mode.

## Usage

Run the script with the following flags:

- `-d` : Debug mode (compiles and runs the project with QEMU in debug mode).
- `-r` : Run mode (compiles and runs the project normally).
- `-w` : Warnings mode (compiles and displays only warnings).
- `-e` : Error mode (compiles and display only errors).
