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
// objects
//=======================================================================================
namespace objects {
        
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // plane
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        enum aa_plane_direction {
                x, y, z
        };
        template <typename distance_, aa_plane_direction direction, typename color> struct aa_plane;



        template <typename ray_, typename plane_, bool does_intersect> struct aa_plane_x_eins {
        private:
                typedef ray_ ray;
                typedef plane_ plane;
                typedef typename plane::value plane_value;
                typedef typename ray::position::x rayp_x;
                typedef typename ray::direction::x rayd_x;
        
                typedef scalar::sub<rayp_x, plane_value> height;
        public:
                typedef scalar::div<height, scalar::neg<rayd_x>> distance;
                typedef ::ray::madd<ray, distance> point_of_intersection;                
        };
        
        template <typename ray_, typename plane_> struct aa_plane_x_eins<ray_, plane_, false> {
                typedef scalar::neg<scalar::c1> distance;                
                typedef vector::vector<scalar::c0,scalar::c0,scalar::c0> point_of_intersection;                
        };

        template <typename value_, typename color_> struct aa_plane<value_, aa_plane_direction::x, color_> {
                typedef value_ value;
                
                template <typename ray_> struct intersect {
                        typedef ray_ ray;
                        typedef typename ray::position::x rayp_x;
                        typedef typename ray::direction::x rayd_x;
                        typedef typename scalar::c0 c0;

                        enum {
                                hit_side = scalar::greater<rayp_x, value>::value ? 1 : -1,
                                does_intersect = (hit_side>0 && scalar::less<rayd_x,c0>::value)
                                              || (hit_side<0 && scalar::greater<rayd_x,c0>::value)
                        };
                private:
                        typedef aa_plane_x_eins<ray, aa_plane, does_intersect> eins;
                public:                        
                        typedef typename eins::distance distance;
                        typedef typename eins::point_of_intersection point_of_intersection;
                        typedef vector::vector<
                                ift<(hit_side>0), scalar::c1, scalar::cn1>,
                                scalar::c0,                                
                                scalar::c0
                        > normal;
                        typedef color_ color;
                };
                
        };






        template <typename ray_, typename plane_, bool does_intersect> struct aa_plane_y_eins {
        private:
                typedef ray_ ray;
                typedef plane_ plane;
                typedef typename plane::value plane_value;
                typedef typename ray::position::y rayp_y;
                typedef typename ray::direction::y rayd_y;
        
                typedef scalar::sub<rayp_y, plane_value> height;
        public:
                typedef scalar::div<height, scalar::neg<rayd_y>> distance;
                typedef ::ray::madd<ray, distance> point_of_intersection;                
        };
        
        template <typename ray_, typename plane_> struct aa_plane_y_eins<ray_, plane_, false> {
                typedef scalar::neg<scalar::c1> distance;                
                typedef vector::vector<scalar::c0,scalar::c0,scalar::c0> point_of_intersection;                
        };

        template <typename value_, typename color_> struct aa_plane<value_, aa_plane_direction::y, color_> {
                typedef value_ value;
                
                template <typename ray_> struct intersect {
                        typedef ray_ ray;
                        typedef typename ray::position::y rayp_y;
                        typedef typename ray::direction::y rayd_y;
                        typedef typename scalar::c0 c0;

                        enum {
                                hit_side = scalar::greater<rayp_y, value>::value ? 1 : -1,
                                does_intersect = (hit_side>0 && scalar::less<rayd_y,c0>::value)
                                              || (hit_side<0 && scalar::greater<rayd_y,c0>::value)
                        };
                private:
                        typedef aa_plane_y_eins<ray, aa_plane, does_intersect> eins;
                public:                        
                        typedef typename eins::distance distance;
                        typedef typename eins::point_of_intersection point_of_intersection;
                        typedef vector::vector<
                                scalar::c0,
                                ift<(hit_side>0), scalar::c1, scalar::cn1>,
                                scalar::c0
                        > normal;
                        typedef color_ color;
                };
                
        };
        
        
        
        
        
        
        template <typename ray_, typename plane_, bool does_intersect> struct aa_plane_z_eins {
        private:
                typedef ray_ ray;
                typedef plane_ plane;
                typedef typename plane::value plane_value;
                typedef typename ray::position::z rayp_z;
                typedef typename ray::direction::z rayd_z;
        
                typedef scalar::sub<rayp_z, plane_value> height;
        public:
                typedef scalar::div<height, scalar::neg<rayd_z>> distance;
                typedef ::ray::madd<ray, distance> point_of_intersection;                
        };
        
        template <typename ray_, typename plane_> struct aa_plane_z_eins<ray_, plane_, false> {
                typedef scalar::neg<scalar::c1> distance;                
                typedef vector::vector<scalar::c0,scalar::c0,scalar::c0> point_of_intersection;                
        };

        template <typename value_, typename color_> struct aa_plane<value_, aa_plane_direction::z, color_> {
                typedef value_ value;
                
                template <typename ray_> struct intersect {
                        typedef ray_ ray;
                        typedef typename ray::position::z rayp_z;
                        typedef typename ray::direction::z rayd_z;
                        typedef typename scalar::c0 c0;

                        enum {
                                hit_side = scalar::greater<rayp_z, value>::value ? 1 : -1,
                                does_intersect = (hit_side>0 && scalar::less<rayd_z,c0>::value)
                                              || (hit_side<0 && scalar::greater<rayd_z,c0>::value)
                        };
                private:
                        typedef aa_plane_z_eins<ray, aa_plane, does_intersect> eins;
                public:                        
                        typedef typename eins::distance distance;
                        typedef typename eins::point_of_intersection point_of_intersection;
                        typedef vector::vector<
                                scalar::c0,
                                scalar::c0,
                                ift<(hit_side>0), scalar::c1, scalar::cn1>
                        > normal;
                        typedef color_ color;
                };
                
        };
}
