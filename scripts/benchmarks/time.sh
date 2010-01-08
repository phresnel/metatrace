#!/bin/bash
BUILDS=../../build
WIDTH=1
HEIGHT=1

function RunTest {
        WIDTH=$1
        HEIGHT=$2
        echo "running $WIDTH"x"$HEIGHT ("$(($WIDTH*$HEIGHT)) " pixels)"
        time g++-4.4 -ftemplate-depth-10000 -std=c++0x \
                -DTOTAL_WIDTH=$WIDTH -DTOTAL_HEIGHT=$WIDTH \
                $BUILDS/amalgam.cc \
                -o $WIDTH"x"$HEIGHT".out"
        echo -e "\n"
}

for ((i=0; i<=3; ++i)); do
        let "WIDTH = $WIDTH + $WIDTH"
        RunTest $WIDTH $HEIGHT
        let "HEIGHT = $HEIGHT + $HEIGHT"
        RunTest $WIDTH $HEIGHT
done;
