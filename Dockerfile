FROM randomdude/gcc-cross-x86_64-elf

RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install -y nasm
RUN apt-get install -y xorriso
RUN apt-get install -y grub-efi
RUN apt-get install -y mtools
RUN apt-get install -y grub-common
RUN apt-get install -y grub-pc-bin

VOLUME /root/env
WORKDIR /root/env