#!bin/bash
#use for check lab5

gxemul -E testmips -C R3000 -M 64 -d gxemul/fs.img gxemul/vmlinux
