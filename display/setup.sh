#!/bin/sh

rm -f /dev/es6display

mknod /dev/es6display0 c 249 0
chmod 666 /dev/es6display
