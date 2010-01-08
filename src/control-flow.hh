#ifndef AMALGAM
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2009  Sebastian Mach (*1983)
// * mail: phresnel/at/gmail/dot/com
// * http://phresnel.org
// * http://picogen.org
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif

//*********************************************************************
// control flow
//*********************************************************************
template <bool cond, typename if_T, typename else_T>
struct ift : if_T {};

template <typename if_T, typename else_T>
struct ift<false, if_T, else_T> : else_T {};

        
        


// for2
// TODO: only where u=0, there shall be a "next_vertical" defined
/* ORDINARY VARIANT, BUT YIELDS WIDTH*HEIGHT RECURSION, KEPT FOR REFERENCE
template <
        unsigned int u, unsigned int width,
        unsigned int v, unsigned int height,
        template <unsigned int,unsigned int,unsigned int,unsigned int> class kernel
> struct for2 {
        typedef typename kernel<u, width, v, height>::color color;

        typedef for2<u+1,width, v,height, kernel> next;

        template <typename F> static void query (F f) {
                f (u, v, get_rgb<color>::r, get_rgb<color>::g, get_rgb<color>::b);
                return next::query (f); // hopefully get a tail call
        }
};

template <
        unsigned int width,
        unsigned int v, unsigned int height,
        template <unsigned int,unsigned int,unsigned int,unsigned int> class kernel
> struct for2<width,width, v, height, kernel> {
        typedef for2<0,width, v+1,height, kernel> next;

        template <typename F> static void query (F f) {}
                return next::query (f);
        }
};

template <
        unsigned int u, unsigned int width,
        unsigned int height,
        template <unsigned int,unsigned int,unsigned int,unsigned int> class kernel
> struct for2<u,width, height, height, kernel> {

        //typedef typename kernel<u, width, height, height>::color color;

        template <typename F> static void query (F f) {}
};
*/


// non stupid variant that only recurses downwards at the beginning of each line
// the maximum recursion for this is only width+height (vs. width*height in the other variant)
// compare:
//    assume a maximum of 200 recursion for templates (std btw declares only 17 to be reliable)
//    with width*height, the maximum screen size is e.g. 14*14
//    with width+height, it is 100+100 (200 pixels vs. 10000 pixels)
template <
        unsigned int u, unsigned int width,
        unsigned int v, unsigned int height,
        template <unsigned int,unsigned int,unsigned int,unsigned int> class kernel
> struct for2 {
        typedef typename kernel<u, width, v, height>::color color;

        typedef for2<u+1,width, v,height, kernel> next;

        template <typename F> static void query (F f) {
                f (u, v, color::r, color::g, color::b);
                return next::query (f); // hopefully get a tail call
        }
};

// vertical iteration for u=0
template <
        unsigned int width_,
        unsigned int v, unsigned int height_,
        template <unsigned int,unsigned int,unsigned int,unsigned int> class kernel
> struct for2<0,width_, v, height_, kernel> {
private:
        typedef typename kernel<0, width_, v, height_>::color color;
        typedef for2<1,width_, v,height_, kernel> next_h;
        typedef for2<0,width_, v+1,height_, kernel> next_v;

public:
        enum { width = width_, height = height_ };
        template <typename F> static void query (F f) {
                f (0, v, color::r, color::g, color::b);
                next_h::query (f);
                return next_v::query (f); // hopefully get a tail call
        }
};

// dead end for u=width
template <
        unsigned int width_,
        unsigned int v, unsigned int height_,
        template <unsigned int,unsigned int,unsigned int,unsigned int> class kernel
> struct for2<width_,width_, v, height_, kernel> {
        enum { width = width_, height = height_ };
        template <typename F> static void query (F f) {}
};

// dead end for u=0, v=height (the u=0 was needed for disambiguation only)
template <
        unsigned int width_,
        unsigned int height_,
        template <unsigned int,unsigned int,unsigned int,unsigned int> class kernel
> struct for2<0,width_, height_,height_, kernel> {
        enum { width = width_, height = height_ };
        template <typename F> static void query (F f) {}
};


