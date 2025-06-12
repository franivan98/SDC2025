#!/bin/bash

IMG=2025-05-13-raspios-bookworm-armhf-lite.img

sudo qemu-system-arm \
    -M raspi2 \
    -kernel /usr/share/qemu-efi-aarch64/QEMU-EFI.fd \
    -dtb /usr/share/qemu/device-tree/raspi2.dtb \