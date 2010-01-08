#!/bin/bash
SRC=../src
BUILD=../build
mkdir -p $BUILD
cat $SRC/copyright-header > $BUILD/amalgam.cc
echo -e "#define AMALGAM\n" >> $BUILD/amalgam.cc
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
        >> $BUILD/amalgam.cc
