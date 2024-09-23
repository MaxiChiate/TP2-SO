#!/bin/bash
if
[ "$1" = "-d" ];
then
  qemu-system-x86_64 -s -S -hda Image/x64BareBonesImage.qcow2 -m 512 -soundhw pcspk -audiodev pa,id=RPDSink
else
  qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -soundhw pcspk -audiodev pa,id=RPDSink
  fi