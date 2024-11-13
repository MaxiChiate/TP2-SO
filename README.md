# Project Build and Run Script

The `mk_file.sh` script compiles and runs your project using Docker, with options for debugging, normal execution, and warnings-only mode.

## Usage

Run the script with the following flags:

- `-d` : Debug mode (compiles and runs the project with QEMU in debug mode).
- `-r` : Run mode (compiles and runs the project normally).
- `-w` : Warnings mode (compiles and displays only warnings).
- `-e` : Error mode (compiles and display only errors).
- `-a` : All mode (compiles and display both warnings and errors).
- `-b` : Buddy run mode (compiles with buddy mm and runs the project).

## Problems

Errors of a corrupted `mk_file.sh`, try: `./solve mk_file.sh` or `sed -i -e 's/\r$//' mk_file.sh`

Errors while running like: "qemu-system-x86_64: -hda Image/x64BareBonesImage.qcow2: Could not open 'Image/x64BareBonesImage.qcow2': No such file or directory" will probably be fixed doing `make clean` and running again.