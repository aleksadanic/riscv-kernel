FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update && apt install -y \
    gcc-riscv64-unknown-elf \
    binutils-riscv64-unknown-elf \
    make \
    qemu-system-misc \
    gdb-multiarch \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /kernel