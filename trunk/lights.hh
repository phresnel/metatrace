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

//=======================================================================================
// lights
//=======================================================================================
namespace lights {
        template <typename position_, typename color_, typename objects_> struct point_light {
                typedef position_ position;
                typedef color_ color;
                typedef objects_ objects;
                
                template <typename intersection> struct shade {
                private:
                        typedef vector::sub<
                                position, 
                                typename intersection::point_of_intersection
                        > diff;
                        typedef vector::normalize<diff> direction;
                
                        typedef typename ray::template ray<
                                // bah, epsilon add
                                vector::template add <                
                                        vector::template scale<
                                                direction,
                                                frac0<1,10>
                                        >,
                                        typename intersection::point_of_intersection
                                >,
                                direction
                        > lightRay;
                
                        typedef typename objects::template intersect<lightRay> lightRayIntersect;

                        enum {
                                unobstructed = 
                                #if 0
                                        1
                                #else
                                        !config::trace_shadows ||
                                        (!lightRayIntersect::does_intersect
                                         || 
                                            (tinplate::template gt<
                                                typename lightRayIntersect::distance,
                                                typename vector::template length<diff>
                                            >::value
                                            || // dist>0 (epsilon self intersect)
                                            tinplate::template lt<
                                                typename lightRayIntersect::distance,
                                                frac0<1,10>
                                            >::value
                                            )
                                        )                                        
                                #endif
                        };
                
                        typedef ift<
                                unobstructed,
                                max<
                                        frac0<0,1>,
                                        vector::dot<direction, typename intersection::normal>
                                >,
                                frac0<0,1>
                        > intensity;
                        /*typedef ift<
                                unobstructed,
                                frac0<1,1>,
                                frac0<0,1>
                        > intensity;*/
                public:
                        typedef tinplate::rgbf<
                                mul<typename color::r, typename intersection::color::r, intensity>,
                                mul<typename color::g, typename intersection::color::g, intensity>,
                                mul<typename color::b, typename intersection::color::b, intensity>
                        > lit_color;
                
                        /*typedef tinplate::rgbf<
                                intensity,
                                intensity,
                                intensity
                        > lit_color;*/
                        
                        // display light ray obstruction
                        /*typedef tinplate::rgbf<
                                ift<unobstructed,frac0<0,1>,frac0<1,1>>,
                                ift<unobstructed,frac0<1,1>,frac0<0,1>>,
                                frac0<0,1>
                        > lit_color;*/
                        
                };
        };
}
