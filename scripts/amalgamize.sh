#!/bin/bash
SRC=../src
cat $SRC/copyright-header > amalgam.cc
echo -e "#define AMALGAM\n" >> amalgam.cc
cat \
        $SRC/"base-config.hh" \
        $SRC/"control-flow.hh" \
        $SRC/"fixp.hh" \
        $SRC/"rgb.hh" \
        $SRC/"vector.hh" \
        $SRC/"ray.hh" \
        $SRC/"materials.hh" \
        $SRC/"sphere.hh" \
        $SRC/"aa_plane.hh" \
        $SRC/"object_list.hh" \
        $SRC/"lights.hh" \
        $SRC/"raytrace.hh" \
        $SRC/"main.cc" \
        >> amalgam.cc
