#!/bin/bash
arduino-cli compile --fqbn adafruit:samd:adafruit_feather_m4_can --build-property "compiler.cpp.extra_flags=\"-DFRONT_NODE=1\"" Nodes.ino
arduino-cli compile --fqbn adafruit:samd:adafruit_feather_m4_can --build-property "compiler.cpp.extra_flags=\"-DSCREEN_NODE=1\"" Nodes.ino
arduino-cli compile --fqbn adafruit:samd:adafruit_feather_m4_can --build-property "compiler.cpp.extra_flags=\"-DVEHICLE_DYNAMICS_NODE=1\"" Nodes.ino
