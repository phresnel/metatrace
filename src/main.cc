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

#ifndef TOTAL_WIDTH
  #define TOTAL_WIDTH 8
  //#define TOTAL_WIDTH 2
#endif

#ifndef TOTAL_HEIGHT
  #define TOTAL_HEIGHT TOTAL_WIDTH
#endif

#ifndef TILE_WIDTH
  #define TILE_WIDTH TOTAL_WIDTH
#endif

#ifndef TILE_HEIGHT
  #define TILE_HEIGHT TOTAL_HEIGHT
#endif

#ifndef TOTAL_X
  #define TOTAL_X 0
#endif

#ifndef TOTAL_Y
  #define TOTAL_Y 0
#endif

namespace config {
        enum {
                scalar_shift = 20,
                heron_initial_max_recursion = 500,
                trace_shadows = 1,
                
                screen_width = TILE_WIDTH,
                screen_height = TILE_HEIGHT,
                
                virtual_x = TOTAL_X,
                virtual_y = TOTAL_Y,
                virtual_width = TOTAL_WIDTH,
                virtual_height = TOTAL_HEIGHT,
        };
};

#include "control-flow.hh"
#include "fixp.hh"
#include "rgb.hh"
#include "vector.hh"
#include "ray.hh"

#include "materials.hh"

#include "sphere.hh"
#include "aa_plane.hh"
#include "object_list.hh"

#include "lights.hh"
#include "raytrace.hh"


namespace scene {
        
};


#include <iostream>
#include <string>
#include <sstream>

template <typename f_>
std::string ts () {
        using namespace scalar;
        enum { sign = sign<f_>::value };
        typedef ift<(sign<0), neg<f_>, f_> f;
                
        std::stringstream ss;
        ss << (sign<0?"-":"");
        ss << (f::value>>shift);
        ss << '.';
        ss << static_cast<int>(100000*static_cast<double>(f::value&shift_mask) / shift_mask);
        return ss.str();
}


template <unsigned int,unsigned int,unsigned int,unsigned int> class null_kernel {
        
public:
        typedef color::rgb<122,122,122> color;
};

void ppm (size_t x, size_t y, int r, int g, int b) {
        r = r < 0 ? 0 : r > 255 ? 255 : r;
        g = g < 0 ? 0 : g > 255 ? 255 : g;
        b = b < 0 ? 0 : b > 255 ? 255 : b;
        std::cout << r << ' ' << g << ' ' << b << '\n';
}


int main () {                
        typedef objects::list<
                // plumbing
                objects::aa_plane<
                        scalar::cn10,
                        objects::aa_plane_direction::x,
                        material::material<
                                material::const_reflection<scalar::c0>::type,
                                material::const_color<color::rgbf<scalar::c1,scalar::c0,scalar::c0>>::type
                        >::type
                >,
                objects::aa_plane<
                        scalar::c10,
                        objects::aa_plane_direction::x,
                        material::material<
                                material::const_reflection<scalar::c0>::type,
                                material::const_color<color::rgbf<scalar::c0,scalar::c1,scalar::c0>>::type
                        >::type
                >,
                objects::aa_plane<
                        scalar::cn2,
                        objects::aa_plane_direction::y,
                        material::material<
                                material::const_reflection<scalar::c0>::type,
                                material::const_color<color::rgbf<scalar::c1,scalar::c1,scalar::c1>>::type
                        >::type
                >,
                objects::aa_plane<
                        scalar::c8,
                        objects::aa_plane_direction::y,
                        material::material<
                                material::const_reflection<scalar::c0>::type,
                                material::const_color<color::rgbf<scalar::c1,scalar::c1,scalar::c1>>::type
                        >::type
                >,
                objects::aa_plane<
                        scalar::c10,
                        objects::aa_plane_direction::z,
                        material::material<
                                material::const_reflection<scalar::c0_75>::type,
                                material::const_color<color::rgbf<scalar::c1,scalar::c1,scalar::c1>>::type
                        >::type
                >,
                objects::aa_plane<
                        scalar::cn2,
                        objects::aa_plane_direction::z,
                        material::material<
                                material::const_reflection<scalar::c0_75>::type,
                                material::const_color<color::rgbf<scalar::c1,scalar::c1,scalar::c1>>::type
                        >::type
                >,
                
                // porcelain
                objects::sphere<
                        vector::vector<scalar::from_int<-5>,scalar::c4,scalar::c7>,
                        scalar::c3,
                        material::material<
                                material::const_reflection<scalar::c0_75>::type,
                                material::const_color<color::rgbf<scalar::c1,scalar::c0_75,scalar::c0_75>>::type//material::normal2rgb_color
                        >::type
                >,
                objects::sphere<
                        vector::vector<scalar::c1_25,scalar::c4,scalar::c6>,
                        scalar::c1,
                        material::material<
                                material::const_reflection<scalar::c0_75>::type,
                                material::const_color<color::rgbf<scalar::c0_75,scalar::c0_75,scalar::c1>>::type//material::normal2rgb_color
                        >::type
                >                
        > objects;

        typedef raytrace::scene<
                raytrace::whitted_style,
                raytrace::simple_camera<scalar::c0_25>,
                objects,
                lights::point_light<
                        vector::vector<scalar::c7,scalar::c6,scalar::c4>,
                        color::rgbf<scalar::c10,scalar::c10,scalar::c10>,
                        objects
                >
        > scene;


        typedef for2<
                0, config::screen_width,
                0, config::screen_height,
                scene::kernel
        > raytrace;
        std::cout << "P3\n" 
                << raytrace::width << ' ' << raytrace::height << '\n' 
                << "255\n";
        raytrace::query (ppm);
}
