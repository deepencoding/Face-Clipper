#!/usr/bin/bash

mkdir -p $2 

gst-launch-1.0 filesrc location=$1 ! decodebin ! videoconvert ! videoscale ! video/x-raw,width=640,height=640 ! jpegenc ! multifilesink location=./$2/%d.jpeg
