FROM randomdude/gcc-cross-x86_64-elf

RUN sed -i -e 's/deb.debian.org/archive.debian.org/g' \
           -e 's|security.debian.org|archive.debian.org/|g' \
           -e '/stretch-updates/d' /etc/apt/sources.list
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