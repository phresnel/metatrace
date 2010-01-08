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

#ifndef TOTAL_WIDTH
  //#define TOTAL_WIDTH 8
  #define TOTAL_WIDTH 2
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
