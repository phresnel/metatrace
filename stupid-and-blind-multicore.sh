#!/bin/bash

mkdir -p supertrace

TOTAL_WIDTH=256
TOTAL_HEIGHT=256

TILE_WIDTH=5
TILE_HEIGHT=1

if [ ${1} = "join" ] ; then
        do_join="yes"
        render="no"
elif [ $1 = "multi" ] ; then 
        do_join="no"
        render="yes"
        y_start=$2
        n_th=$3
else
        do_join="yes"
        render="yes"
        y_start=0
        n_th=1
fi;



if [ $render = "yes" ] ; then
        for ((y=$y_start; y<TOTAL_HEIGHT; y+=((TILE_HEIGHT*$n_th)) )); do
                for ((x=0; x<TOTAL_WIDTH; x+=TILE_WIDTH)); do
                        mkdir -p supertrace/$y/
                        binname=$x.$y.out
                        g++-4.4 -o $binname -std=c++0x -ftemplate-depth-10000  \
                            -DTOTAL_WIDTH=$TOTAL_WIDTH   \
                            -DTOTAL_HEIGHT=$TOTAL_HEIGHT \
                            -DTILE_WIDTH=$TILE_WIDTH     \
                            -DTILE_HEIGHT=$TILE_HEIGHT   \
                            -DTOTAL_X=$x -DTOTAL_Y=$y    \
                        main.cc && ./$binname > supertrace/$y/$x.ppm
                        rm $binname
                done;
                pushd supertrace/$y/
                convert +append $(ls *.ppm | sort -n) ../$y.ppm
                popd
        done;
fi;
if [ $do_join = "yes" ] ; then
        echo "joining..."
        pushd supertrace
        convert -append $(ls *.ppm | sort -n) final.ppm
        popd
fi;
