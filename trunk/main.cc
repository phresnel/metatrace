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

namespace config {
        enum {
                scalar_shift = 16,
                heron_initial_max_recursion = 500
        };
};

#include "control-flow.hh"
#include "fixp.hh"
#include "vector.hh"
#include "ray.hh"
#include "rgb.hh"

#include "objects.hh"


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
        typedef for2<
                0, 8,
                0, 8,
                null_kernel
        > raytrace;
        std::cout << "P3\n" 
                << raytrace::width << ' ' << raytrace::height << '\n' 
                << "255\n";
        raytrace::query (ppm);
}
