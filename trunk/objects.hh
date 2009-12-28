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
        // sphere
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        
        template <typename intersection, bool neg> struct sphere_zweitens;
        template <typename intersection> struct sphere_zweitens<intersection,true> {
        private:
                typedef typename intersection::ray ray;
                typedef typename intersection::ray_pos ray_pos;
                typedef typename intersection::ray_dir ray_dir;

                typedef typename tinplate::heron_sqrt<typename intersection::det_sq>::type det;

                typedef tinplate::sub<typename intersection::b,det> i1;
                typedef tinplate::add<typename intersection::b,det> i2;                
                enum { 
                        i1_negative = tinplate::ltz<i1>::value,
                        i2_not_negative = tinplate::gtz<i2>::value,
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
                                frac0<-1,1>
                        >
                > distance;
        };
        template <typename intersection> struct sphere_zweitens<intersection,false> {
                enum { 
                        hit_side = 0,
                        does_intersect = 0
                };
                typedef frac0<-1,1> distance;
        };

        
        template <typename intersection, bool neg> struct sphere_drittens;
        template <typename intersection> struct sphere_drittens<intersection,true> {
                typedef typename intersection::ray ray;
                typedef typename intersection::distance distance;
                typedef typename tinplate::ray::template madd<ray, distance> point_of_intersection;
                typedef vector::sub<
                        point_of_intersection,
                        typename intersection::sphere_position
                > direction_;
                /*typedef vector::vector<
                        ift<
                                tinplate::can_reduce_precision<typename direction_::x>::value,
                                tinplate::precision_reduce<typename direction_::x>,
                                typename direction_::y
                        >,
                        ift<
                                tinplate::can_reduce_precision<typename direction_::y>::value,
                                tinplate::precision_reduce<typename direction_::y>,
                                typename direction_::y
                        >,
                        ift<
                                tinplate::can_reduce_precision<typename direction_::y>::value,
                                tinplate::precision_reduce<typename direction_::y>,
                                typename direction_::y
                        >
                > direction;*/
                
                typedef vector::normalize<direction_> normal;
        };
        template <typename intersection> struct sphere_drittens<intersection,false> {
                typedef vector::vector<frac0<1,1>,frac0<1,1>,frac0<1,1>> normal;
                typedef vector::vector<frac0<0,1>,frac0<0,1>,frac0<0,1>> point_of_intersection;
        };
                        
                        
        template <typename position_, typename radius_> struct sphere {
        private:
                typedef position_ position;
                typedef radius_ radius;
                typedef tinplate::mul<radius_, radius_> radius_sq;
        public:
                
                template <typename ray_> struct intersect {
                private:
                        typedef ray_ ray;
                        typedef typename ray::position ray_pos;
                        typedef typename ray::direction ray_dir;                        
                        typedef position sphere_position;

                        typedef vector::sub<ray_pos,position> v;

                        typedef tinplate::neg< vector::dot<v, ray_dir> > b;
                        typedef tinplate::mul<b,b> bb;
                
                        typedef vector::length_sq<v> sqrV;
                
                        typedef tinplate::add<tinplate::sub<bb,sqrV>, radius_sq> det_sq;

                        enum { det_not_negative = tinplate::gtz<det_sq>::value };                       
                        
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
                        typedef vector::normal_to_rgbf<normal> color;
                };
        };
        
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // list
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        template <typename ...objects> struct list;

        template <typename current, typename ...more> struct list<current, more...> {
        private:
                template <typename ray_> struct intersect_impl {
                private:
                        typedef typename current::template intersect<ray_> alpha;
                        typedef typename list<more...>::template intersect<ray_> beta;
                        enum {
                                
                        };
                public:
                        typedef ift<
                                alpha::does_intersect && beta::does_intersect,
                                // if both intersect, take nearest
                                ift<
                                        tinplate::lt<
                                                typename alpha::distance,
                                                typename beta::distance
                                        >::value,
                                        alpha,
                                        beta
                                >,
                                // otherwise, take alpha if valid, otherwise beta
                                ift<
                                        alpha::does_intersect,
                                        alpha,
                                        beta
                                >
                        > type;
                };
        public:
                template <typename ray_> struct intersect
                : intersect_impl<ray_>::type
                {};
        };
        
        template <typename current> struct list<current> {
                template <typename ray_> struct intersect
                : current::template intersect<ray_>
                {};                
        };
}
