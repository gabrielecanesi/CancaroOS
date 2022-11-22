# CancaroOS
A useless x86-64 ELF kernel.

## Build (Linux/MacOS)
For generating an ISO (kernel.iso):

 - X86-64 EFI
    ```
    docker build -t elf-gcc .
    docker run -v $(pwd):/root/env -it elf-gcc
    make
    ```
- I386
    ```
    docker build -t elf-gcc .
    docker run -v $(pwd):/root/env -it elf-gcc
    make pc
    ```