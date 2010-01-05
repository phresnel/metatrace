#!/bin/bash
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Copyright (C) 2009  Sebastian Mach (*1983)
# * mail: phresnel/at/gmail/dot/com
# * http:#phresnel.org
# * http:#picogen.org
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http:#www.gnu.org/licenses/>.
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

NUM_THREADS=4

for ((t=0; t<NUM_THREADS; ++t)); do
        ./render.sh multi $t $NUM_THREADS &
done;
# sync
for ((t=0; t<NUM_THREADS; ++t)); do
        while [ ! -e alphacentauri/$t.done ] ; do 
                sleep 3
        done;
        echo "synced " $t " of " NUM_THREADS
done;

./render.sh join