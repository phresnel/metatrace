[`ORIGINAL GITORIOUS PROJECT META`](https://gitorious.org/metatrace)

# Metatrace

Metatrace is a C++ compile time ray tracer ^0 (as of the initial release, it is a whitted style one).

It is mostly inane stuff. except, maybe, that it's a nice gadget to test compiler performance and conformance, especially but not exclusively in the Template and Metaprogramming corner of C++.

It requires basic C++0x support; with g++, make sure to have at least 4.4 installed.

----------

## Extern
* [Announcement](http://ompf.org/forum/viewtopic.php?p=16900)
* [git origin](http://savannah.nongnu.org/projects/metatrace)
* [download release packages](http://code.google.com/p/metatrace/downloads/list)

-----------

## Features

### graphics viewpoint:
* linear object list
* recursive ray tracing (whitted style)
* mirror reflections
* spheres, axis aligned planes
* quite easily extensible (considering the circumstances)
* rgb colors
* scalar type is fixed point
* not fast

### c++ viewpoint:
* uses variadic templates in some contexts (e.g. scene, some math-operations)
* fairly heavy compile time number crunching (brings back the "slow" in "ray tracing")
* strongly typed enums, to avoid ugly enum hacks like "FORCE_DWORD=0xEFFFFFFF" and the like

### bash viewpoint:
* contains a bash script to compile on multiple cores

_... more to come._

--------------

## Example Rendering

![an example rendering](https://i.stack.imgur.com/lX8c8.png)

---------------

_^0: For the googles, it is also a c++ compile time raytracer and a c++ meta ray tracer (and unsurprisingly, it is even a c++ meta raytracer (and surely, a c++ template ray tracer)) :D_

----------------


`ORIGINAL README`


      metatrace

    A mostly useless (but funny) C++ Compile Time Ray Tracer.




    -------------------------------------------------------------------------------
    todo's for this document:
    * Proper formatting
    * Expand 'Scene configuration'
    -------------------------------------------------------------------------------



    ===============================================================================
     DESCRIPTION
    ===============================================================================




    ===============================================================================
     "USAGE"
    ===============================================================================


    -------------------------------------------------------------------------------
     Compiling (for gcc users)
    -------------------------------------------------------------------------------

     0) Prerequisites
     
     If you know how to invoke g++ 4.4 or newer on your box, skip this section.
     
     Basic C++0x support is required. Make sure you have at least g++ 
     version 4.4 installed:
     
       $ g++ --version
       g++-4.4 (Debian 4.4.0-5) 4.4.0
       Copyright (C) 2009 Free Software Foundation, Inc.
       This is free software; see the source for copying conditions.  There is NO
       warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
       
     If you have less than 4.4, grab a newer copy from your distro or from 
     http://gcc.gnu.org.
     
     It might also be that you have 4.4 or newer installed, but it might not be the
     default. In that case (in bash), to see your options:
     
       $ g++<tab><tab>
       g++      g++-4.3  g++-4.4
       
     So instead of just "g++", type "g++-4.4".
     

     1) Option 1.
     
     Simply compile src/main.cc in C++0x mode:
     
       $ g++ -std=c++0x src/main.cc
       
     You probably get a plethora of error messages, so increase the template
     recursion limit (sidenote: the Holy Standard states that portable programs
     shall not rely on recursion depths greater than 17); a depth of 10000 was
     enough for me in all cases:
     
       $ g++ -std=c++0x  -ftemplate-depth-10000 src/main.cc
     
     If all went fine (after possibly hours and days of compiling, depending on 
     image size), you'll reap a classy "a.out". That binary consists of pure
     output, no more calculations (we did it at compile time); actually, it prints 
     a PPM file. So:
     
       $ ./a.out > result.ppm

     Open up that file in an image viewer of your choice, et voila.
     
     
     2) Option 2.
     
     g++ <= 4.4 happens to choke on larger template instantiation counts. Further,
     it does not have multithreaded compilation support at the moment. So  I have
     written a (stupid) script that invokes g++ many, many times under the hood and
     finally joins all output into a single image.
     
     Unfortunately, that script relies on ImageMagick at the moment, so make sure
     to have that installed.
     
     Assume you want to have 4 compile threads (e.g. because you have a quad core),
     then type:
     
       $ ./render.sh multi 1 4 &
       $ ./render.sh multi 2 4 &
       $ ./render.sh multi 3 4 &
       $ ./render.sh multi 4 4 &
       
     If all compiling is done,
     
       $ ./render.sh join
       
     You'll reap "supertrace/final.ppm".
     
     
    -------------------------------------------------------------------------------
     Configuring image resolution
    -------------------------------------------------------------------------------

     If you use ./render.sh , open it in your editor, and see
     the following fields:

      TOTAL_WIDTH=1024
      TOTAL_HEIGHT=1024
      
     As you guess, those set the size of the final result.

     If you compile src/main.cc yourself, open src/main.cc and find at the top
     the right options. You can also #define the size directly on the command line

      $ g++ -std=c++0x -ftemplate-depth-10000 \
            -DTOTAL_WIDTH=64 -DTOTAL_HEIGHT=64 \
            src/main.cc


    -------------------------------------------------------------------------------
     Scene configuration
    -------------------------------------------------------------------------------
     See main().



    ===============================================================================
     AUTHOR
    ===============================================================================
    Written by Sebastian Mach / phresnel (*1983).



    ===============================================================================
     COPYING / LICENSE OF metatrace AND THIS DOCUMENT (README)
    ===============================================================================
    Copyright (C) 2009 Sebastian Mach (*1983)
                     - phresnel@gmail.com
                     - http://phresnel.org

    This program is free software:  you  can redistribute it and/or modify it under
    the terms of the GNU General Public License as published by the  Free  Software
    Foundation,  either  version 3 of the  License,  or  (at your option) any later
    version.

    This  program  is distributed in the hope that it will be useful,  but  WITHOUT 
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY  or  FITNESS
    FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

    You should have received a copy of the  GNU  General Public License along  with
    this program.  If not, see <http://www.gnu.org/licenses/>.
