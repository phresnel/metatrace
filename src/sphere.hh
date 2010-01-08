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

//=======================================================================================
// objects
//=======================================================================================
namespace objects {
        
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // sphere
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        
        template <typename intersection, bool neg> struct sphere_zweitens;
        template <typename intersection> struct sphere_zweitens<intersection,true> {
        private:
                typedef typename intersection::ray ray;
                typedef typename intersection::ray_pos ray_pos;
                typedef typename intersection::ray_dir ray_dir;

                typedef typename scalar::sqrt<typename intersection::det_sq> det;

                typedef scalar::sub<typename intersection::b,det> i1;
                typedef scalar::add<typename intersection::b,det> i2;                
                enum { 
                        i1_negative = scalar::less<i1,scalar::c0>::value,
                        i2_not_negative = scalar::greater<i2,scalar::c0>::value,
                };
        public:
                enum {
                        hit_side = i2_not_negative
                                 ? i1_negative ? -1 : +1
                                 : +-0,
                        does_intersect = hit_side != 0
                };
                
                typedef ift< 
                        (hit_side<0),
                        i2,
                        ift< 
                                (hit_side>0),
                                i1,
                                scalar::cn1
                        >
                > distance;
        };
        template <typename intersection> struct sphere_zweitens<intersection,false> {
                enum { 
                        hit_side = 0,
                        does_intersect = 0
                };
                typedef scalar::cn1 distance;
        };

        
        template <typename intersection, bool neg> struct sphere_drittens;
        template <typename intersection> struct sphere_drittens<intersection,true> {
                typedef typename intersection::ray ray;
                typedef typename intersection::distance distance;
                typedef typename ::ray::template madd<ray, distance> point_of_intersection;
                typedef vector::sub<
                        point_of_intersection,
                        typename intersection::sphere_position
                > direction_;
                /*typedef vector::vector<
                        ift<
                                scalar::can_reduce_precision<typename direction_::x>::value,
                                scalar::precision_reduce<typename direction_::x>,
                                typename direction_::y
                        >,
                        ift<
                                scalar::can_reduce_precision<typename direction_::y>::value,
                                scalar::precision_reduce<typename direction_::y>,
                                typename direction_::y
                        >,
                        ift<
                                scalar::can_reduce_precision<typename direction_::y>::value,
                                scalar::precision_reduce<typename direction_::y>,
                                typename direction_::y
                        >
                > direction;*/
                
                typedef vector::normalize<direction_> normal;
        };
        template <typename intersection> struct sphere_drittens<intersection,false> {
                typedef vector::vector<scalar::c1,scalar::c1,scalar::c1> normal;
                typedef vector::vector<scalar::c0,scalar::c0,scalar::c0> point_of_intersection;
        };
                        
                        
        template <
                typename position_, 
                typename radius_, 
                template <typename position, typename normal> class material_
        > 
        struct sphere {
        private:
                typedef position_ position;
                typedef radius_ radius;
                typedef scalar::mul<radius_, radius_> radius_sq;
        public:
                
                template <typename ray_> struct intersect {
                private:
                        typedef ray_ ray;
                        typedef typename ray::position ray_pos;
                        typedef typename ray::direction ray_dir;                        
                        typedef position sphere_position;

                        typedef vector::sub<ray_pos,position> v;

                        typedef scalar::neg< vector::dot<v, ray_dir> > b;
                        typedef scalar::mul<b,b> bb;
                
                        typedef vector::length_sq<v> sqrV;
                
                        typedef scalar::add<scalar::sub<bb,sqrV>, radius_sq> det_sq;

                        enum { det_not_negative = scalar::greater<det_sq,scalar::c0>::value };                       
                        
                        friend class sphere_zweitens<intersect, det_not_negative>;
                        typedef sphere_zweitens<intersect, det_not_negative> zwo;
                        
                public:
                        enum {
                                hit_side = zwo::hit_side,
                                does_intersect = zwo::does_intersect
                        };                        
                        
                        typedef typename zwo::distance distance;
                private:
                        friend class sphere_drittens<intersect, does_intersect>;
                        typedef sphere_drittens<intersect, does_intersect> drei;
                public:        
                        typedef typename drei::point_of_intersection point_of_intersection;
                        typedef typename drei::normal normal;                        
                        typedef material_<point_of_intersection, normal> material;
                };
        };
}
