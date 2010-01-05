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

namespace material {
        
        
        template <typename color_> struct const_color {
                template <typename position, typename normal>
                struct type : color_ {};
        };
        
        template <typename position, typename normal>
        struct normal2rgb_color : vector::normal_to_rgbf<normal> {};
        
        template <typename reflection_> struct const_reflection {
                template <typename position, typename normal>
                struct type : reflection_ {};
        };
        
        // TODO: implement checkered board color
        
        template <
                template <typename pos, typename dir> class reflection_,
                template <typename pos, typename dir> class color_
        > struct material {
                template <typename position, typename normal> struct type {
                        typedef reflection_<position, normal> reflection;
                        typedef color_<position, normal>      color;
                };
        };
}
