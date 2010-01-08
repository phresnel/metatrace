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

namespace scalar {
        /*struct fixp {
                long long bits;
        };*/
        typedef long long bits_type;
        
        template <bits_type bits_> struct fixp { 
                enum : bits_type { value = bits_ };
        };
        template <bool value_> struct boolean { 
                enum : bool { value = value_ };
        };


        template <unsigned int shift> struct get_shift_mask {
                enum { value = (1<<(shift-1)) | get_shift_mask<shift-1>::value };
        };
        template <> struct get_shift_mask<0> {
                enum { value = 0 };
        };

        enum {
                shift = config::scalar_shift,
                shift_mask = get_shift_mask<shift>::value
        };

        typedef fixp<config::heron_initial_max_recursion<<shift> heron_initial_max_recursion;

        template <typename ...> struct add;
        template <typename lhs, typename rhs> struct add<lhs,rhs>
        : fixp<lhs::value + rhs::value> {};
        template <typename lhs, typename rhs, typename ...REST> struct add<lhs,rhs,REST...>
        : add<add<lhs,rhs>, REST...> {};       
        
        template <typename ...> struct sub;
        template <typename lhs, typename rhs> struct sub<lhs,rhs>
        : fixp<lhs::value - rhs::value> {};
        template <typename lhs, typename rhs, typename ...REST> struct sub<lhs,rhs,REST...>
        : sub<sub<lhs,rhs>, REST...> {};       
                
        /*template <typename lhs, typename rhs> struct mul {
                enum : bits_type { value = (lhs::value*rhs::value) >> shift };
        };*/
        template <typename ...> struct mul;
        template <typename lhs, typename rhs> struct mul<lhs,rhs>
        : fixp<((lhs::value*rhs::value) >> shift)> {};
        template <typename lhs, typename rhs, typename ...REST> struct mul<lhs,rhs,REST...>
        : mul<mul<lhs,rhs>, REST...> {}; 
                
        template <typename lhs, typename rhs> struct div {
                enum : bits_type { value = (lhs::value<<shift)/rhs::value };
        };
        template <typename rhs> struct inc : add<rhs,fixp<(1<<shift)>> {};        
        template <typename rhs> struct sqr : mul<rhs,rhs> {};
        template <typename rhs> struct neg : fixp<-rhs::value> {};


        template <typename lhs, typename rhs> struct equal
        : boolean<lhs::value==rhs::value> {};
                
        template <typename lhs, typename rhs> struct less
        : boolean<lhs::value<rhs::value> {};
                
        template <typename lhs, typename rhs> struct greater
        : boolean<(lhs::value>rhs::value)> {};
                
        template <typename lhs, typename rhs> struct less_equal
        : boolean<lhs::value<=rhs::value> {};
        
        template <typename lhs, typename rhs> struct greater_equal
        : boolean<lhs::value>=rhs::value> {};

        template <typename rhs> struct sign {
                enum { value = less<rhs,fixp<0>>::value ? -1 : +1 };
        };
        
        template <typename rhs> struct reciprocal : div<fixp<(1<<shift)>, rhs> {};

        template <typename rhs> struct to_int {
                enum { value = rhs::value >> shift };
        };
        template <int rhs> struct from_int : fixp<rhs<<shift> {};
                
        //=======================================================================================
        // min, max
        //=======================================================================================
        template <typename lhs, typename rhs> struct max : ift<greater<lhs,rhs>::value,lhs,rhs> {};
        template <typename lhs, typename rhs> struct min : ift<greater<lhs,rhs>::value,lhs,rhs> {};

        //*********************************************************************
        // some constants
        //*********************************************************************
        typedef fixp<0>               c0;
        typedef fixp<1<<(shift-3)>    c0_125;
        typedef fixp<1<<(shift-2)>    c0_25; 
        typedef fixp<1<<(shift-1)>    c0_5;
        typedef add <c0_25, c0_5>     c0_75;
        typedef fixp<1<<shift>        c1, c1_0;
        typedef add<c1, c0_25>        c1_25;
        typedef add<c1, c0_5>         c1_5;
        typedef inc<c1>               c2, c2_0;
        typedef inc<c2>               c3, c3_0;
        typedef inc<c3>               c4, c4_0;
        typedef inc<c4>               c5, c5_0;
        typedef inc<c5>               c6, c6_0;
        typedef inc<c6>               c7, c7_0;
        typedef inc<c7>               c8, c8_0;
        typedef inc<c8>               c9, c9_0;
        typedef inc<c9>               c10, c10_0;
        typedef add<c10,c10>          c20, c20_0;
        typedef add<c20, c10>         c30, c30_0;
        typedef add<c30, c10>         c40, c40_0;
        typedef add<c40, c10>         c50, c50_0;
        typedef add<c50, c10>         c60, c60_0;
        typedef add<c60, c10>         c70, c70_0;
        typedef add<c70, c10>         c80, c80_0;
        typedef add<c80, c10>         c90, c90_0;
        typedef add<c90, c10>         c100, c100_0;
        
        typedef neg<c1>               cn1, cn1_0;
        typedef neg<c2>               cn2, cn2_0;
        typedef neg<c2>               cn3, cn3_0;
        typedef neg<c2>               cn4, cn4_0;
        typedef neg<c5>               cn5, cn5_0;
        typedef neg<c5>               cn6, cn6_0;
        typedef neg<c5>               cn7, cn7_0;
        typedef neg<c5>               cn8, cn8_0;
        typedef neg<c5>               cn9, cn9_0;
        typedef neg<c10>              cn10, cn10_0;
        
        typedef fixp<1>               epsilon;
        typedef fixp<0xFFF>           reflection_offset_epsilon; // TODO: needs serious research


        //*********************************************************************
        // heron method
        //*********************************************************************

        template <unsigned int rec, typename guess, typename x>
        struct heron_refine
        : heron_refine<
                rec-1,
                div<add<guess,div<x,guess>>, c2>,
                x
        >
        {};
        
        template <typename guess, typename x>
        struct heron_refine<0, guess, x> {
                typedef guess type;
        };
        
        
        // see http://en.literateprograms.org/Newton-Raphson%27s_method_for_root_finding_(C)
        template <typename i, typename n> struct heron_initial; // fwd decl

        // case 0: i == c0
        template <typename i, bool val_eq_c0>
        struct heron_initial_case0 {
                typedef i type;
        };
        
        // case 1: i > c0?
        // yes
        template <typename i, bool val_gt_c0>
        struct heron_initial_case1 {
                typedef div<add<i,sub<i,c1>>, c2> type;
        };
        // no
        template <typename i>
        struct heron_initial_case1<i, false> {
                typedef void type;
        };
        
        // case 2: i < n?
        // yes
        template <typename i, typename n, bool i_lt_n>
        struct heron_initial_case2 {                
                typedef typename heron_initial<inc<i>, n>::type type;
        };
        // no
        template <typename i, typename n>
        struct heron_initial_case2<i, n, false> {
                typedef void type;
        };
        
        template <typename i, typename n>
        struct heron_initial {
        private:
                typedef sub<mul<i,i>, n> val;
                
                enum : bool { val_eq_c0 = equal<val,c0>::value };
                typedef typename heron_initial_case0<i,val_eq_c0>::type case0;
                
                enum : bool { val_gt_c0 = val_eq_c0 ? 0 : greater<val,c0>::value };
                typedef typename heron_initial_case1<i,val_gt_c0>::type case1;
                
                enum : bool { i_lt_n = val_gt_c0 ? 0 : (less<i,n>::value && less<i,heron_initial_max_recursion>::value) };
                typedef typename heron_initial_case2<i,n, i_lt_n>::type case2;
                
                typedef c1 case3;
        public:
                typedef ift<
                        val_eq_c0,
                        case0,
                        ift<
                                val_gt_c0,
                                case1,
                                ift<
                                        i_lt_n,
                                        case2,
                                        case3
                                >
                        >
                > type;
        };
        
        template <typename x, bool is_zero = equal<x,c0>::value> struct sqrt_impl {
                static_assert (greater_equal<x,c0>::value, "sqrt<> is only defined for x>=0");
                typedef typename heron_refine<
                        3u,
                        typename heron_initial<c0, x>::type, 
                        x
                >::type type;
        };
        
        template <typename x> struct sqrt_impl<x,true> {
                typedef c0 type;
        };
        
        template <typename x> struct sqrt
        : sqrt_impl<x>::type {};                
       
        /*
        // using heron method
        fixp heron_sqrt (unsigned int rec, fixp guess, const fixp x) {
                if (rec == 0)
                        return guess;
                return heron_sqrt (
                        rec-1,
                        div ( add (guess, div(x,guess)), c2),
                        x
                );
        }
        
        // see http://en.literateprograms.org/Newton-Raphson%27s_method_for_root_finding_(C)
        fixp heron_sqrt_initial_guess_impl (fixp i, fixp n) {
                const fixp val = sub (mul (i, i), n);
                return
                        equal(val,c0)   ? i :
                        greater(val,c0) ? div(add(i,sub(i,c1)), c2) :
                        less(i,n)       ? heron_sqrt_initial_guess_impl (inc(i), n) :
                        c0
                ;
        }

        fixp heron_sqrt_initial_guess (fixp x) {
                return heron_sqrt_initial_guess_impl (c0, x);
        }

        fixp sqrt (fixp x) {
                return heron_sqrt (3, heron_sqrt_initial_guess (x), x);
        }
        */
} // namespace scalar
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
// color math
//=======================================================================================
namespace color {
        template <int r_, int g_, int b_> struct rgb {
                static_assert (r_>=0 && r_<=255, "you bailed 0<=r<=255");
                static_assert (g_>=0 && g_<=255, "you bailed 0<=g<=255");
                static_assert (b_>=0 && b_<=255, "you bailed 0<=b<=255");
                enum {
                        r = r_,
                        g = g_,
                        b = b_
                };
        };

        template <typename r_, typename g_, typename b_> struct rgbf {
                typedef r_ r;
                typedef g_ g;
                typedef b_ b;
        };
        
        
        //=======================================================================================
        // fractional rgb
        //=======================================================================================        
        template <typename rgbf_> struct rgbf_to_rgb_impl {
                enum : long long {
                        r_ = scalar::to_int<scalar::mul<scalar::from_int<255>,typename rgbf_::r>>::value,
                        g_ = scalar::to_int<scalar::mul<scalar::from_int<255>,typename rgbf_::g>>::value,
                        b_ = scalar::to_int<scalar::mul<scalar::from_int<255>,typename rgbf_::b>>::value,
                        r = r_<0?0:r_>255?255:r_,
                        g = g_<0?0:g_>255?255:g_,
                        b = b_<0?0:b_>255?255:b_
                };
                typedef rgb<r,g,b> type;
        };

        template <typename rgbf_> struct rgbf_to_rgb
        : rgbf_to_rgb_impl<rgbf_>::type {};

        template <typename rgbf_, typename factor_> struct mul_rgbf
        : rgbf<
                scalar::mul<typename rgbf_::r, factor_>,
                scalar::mul<typename rgbf_::g, factor_>,
                scalar::mul<typename rgbf_::b, factor_>
        >
        {};

        template <typename lhs_, typename rhs_> struct add_rgbf
        : rgbf<
                scalar::add<typename lhs_::r,  typename rhs_::r>,
                scalar::add<typename lhs_::g,  typename rhs_::g>,
                scalar::add<typename lhs_::b,  typename rhs_::b>
        >
        {};

        template <typename lhs_, typename rhs_> struct filter_rgbf
        : rgbf<
                scalar::mul<typename lhs_::r,  typename rhs_::r>,
                scalar::mul<typename lhs_::g,  typename rhs_::g>,
                scalar::mul<typename lhs_::b,  typename rhs_::b>
        >
        {};
}
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
// vector math
//=======================================================================================
namespace vector {
        template <typename x_, typename y_, typename z_> struct vector {
                typedef x_ x;
                typedef y_ y;
                typedef z_ z;
        };


        template <typename vec> struct length_sq 
        : scalar::add<
                scalar::mul<typename vec::x, typename vec::x>,
                scalar::mul<typename vec::y, typename vec::y>,
                scalar::mul<typename vec::z, typename vec::z>
        > {};


        template <typename vec> struct length 
        : scalar::sqrt<
                length_sq<vec>
        > {};


        template <typename lhs, typename rhs> struct add
        : vector<
                scalar::add<typename lhs::x, typename rhs::x>,
                scalar::add<typename lhs::y, typename rhs::y>,
                scalar::add<typename lhs::z, typename rhs::z>
        > {};
        

        template <typename lhs, typename rhs> struct sub
        : vector<
                scalar::sub<typename lhs::x, typename rhs::x>,
                scalar::sub<typename lhs::y, typename rhs::y>,
                scalar::sub<typename lhs::z, typename rhs::z>
        > {};


        template <typename lhs, typename rhs> struct scale
        : vector<
                scalar::mul<typename lhs::x, rhs>,
                scalar::mul<typename lhs::y, rhs>,
                scalar::mul<typename lhs::z, rhs>
        > {};


        template <typename lhs, typename rhs> struct dot
        : scalar::add<
                scalar::mul<typename lhs::x, typename rhs::x>,
                scalar::mul<typename lhs::y, typename rhs::y>,
                scalar::mul<typename lhs::z, typename rhs::z>
        > {};

        
        template <typename vec> struct normalize_impl {
        private:
                typedef length<vec> len;
                typedef scalar::reciprocal<len> rlen;
        public:
                typedef scale<vec, rlen> type;
        };
        template <typename vec> struct normalize
        : normalize_impl<vec>::type{};


        template <typename incident, typename normal> struct reflect
        : sub<
                incident, 
                scale<
                        normal, 
                        scalar::mul<
                                dot<incident, normal>, 
                                scalar::c2
                        >
                >
        > {};


        template <typename vec> struct normal_to_rgb
        : color::rgb<
                scalar::to_int<scalar::mul<scalar::from_int<255>, scalar::add<scalar::mul<typename vec::x,scalar::c0_5>, scalar::c0_5>>>::value,
                scalar::to_int<scalar::mul<scalar::from_int<255>, scalar::add<scalar::mul<typename vec::y,scalar::c0_5>, scalar::c0_5>>>::value,
                scalar::to_int<scalar::mul<scalar::from_int<255>, scalar::add<scalar::mul<typename vec::z,scalar::c0_5>, scalar::c0_5>>>::value
        > {};
                
        template <typename vec> struct normal_to_rgbf
        : color::rgbf<
                scalar::add<scalar::mul<typename vec::x,scalar::c0_5>, scalar::c0_5>,
                scalar::add<scalar::mul<typename vec::y,scalar::c0_5>, scalar::c0_5>,
                scalar::add<scalar::mul<typename vec::z,scalar::c0_5>, scalar::c0_5>
        > {};
}
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
// ray
//=======================================================================================
namespace ray {
        template <typename position_, typename direction_> struct ray {
                typedef position_ position;
                typedef direction_ direction;
        };
        
        template <typename ray, typename factor> struct madd_impl {
                typedef vector::add<
                        typename ray::position,
                        vector::scale<
                                typename ray::direction,
                                factor
                        >
                > type;
        };
        template <typename ray, typename factor> struct madd
        : madd_impl<ray,factor>::type{};        
}
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
        // plane
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        enum aa_plane_direction {
                x, y, z
        };
        template <typename distance_, aa_plane_direction direction, template <typename position, typename normal> class material_> struct aa_plane;



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

        template <typename value_, template <typename position, typename normal> class material_>
        struct aa_plane<value_, aa_plane_direction::x, material_> {
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
                        typedef material_<point_of_intersection, normal> material;
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

        template <typename value_, template <typename position, typename normal> class material_>
        struct aa_plane<value_, aa_plane_direction::y, material_> {
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
                        typedef material_<point_of_intersection, normal> material;
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

        template <typename value_, template <typename position, typename normal> class material_>
        struct aa_plane<value_, aa_plane_direction::z, material_> {
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
                        typedef material_<point_of_intersection, normal> material;
                };
                
        };
}
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
                                        scalar::less<
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
                                                scalar::c0_125 // TODO: should use smaller value
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
                                            (scalar::template greater<
                                                typename lightRayIntersect::distance,
                                                typename vector::template length<diff>
                                            >::value
                                            || // dist>0 (epsilon self intersect)
                                            scalar::template less<
                                                typename lightRayIntersect::distance,
                                                scalar::epsilon//scalar::c0_125
                                            >::value
                                            )
                                        )                                        
                                #endif
                        };
                
                        typedef ift<
                                unobstructed,
                                scalar::max<
                                        scalar::epsilon/*c0_125*/,
                                        scalar::div<
                                                vector::dot<direction, typename intersection::normal>,
                                                scalar::add<vector::length<diff>, scalar::c1>
                                        >
                                >,
                                scalar::c0
                        > intensity;
                        /*typedef ift<
                                unobstructed,
                                frac0<1,1>,
                                frac0<0,1>
                        > intensity;*/
                public:
                        typedef ::color::rgbf<
                                scalar::mul<typename color::r, typename intersection::material::color::r, intensity>,
                                scalar::mul<typename color::g, typename intersection::material::color::g, intensity>,
                                scalar::mul<typename color::b, typename intersection::material::color::b, intensity>
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
// raytrace
//=======================================================================================
namespace raytrace {
        
        template <typename far_plane> struct simple_camera {
                template <typename u, typename v> struct generate {
                private:
                        typedef vector::normalize<
                                vector::vector<u,v,far_plane>
                        > direction;
                
                        typedef vector::vector<
                                scalar::c0, scalar::c0, scalar::c0
                        > position;
                public:
                        typedef ray::ray<
                                position,
                                direction
                        > type;
                };
        };
        
        
        template <
                typename intersection,
                typename ray,
                template <typename, unsigned int> class raytrace,
                unsigned int depth_max,
                bool is_mirror = (depth_max>0)
                                && (intersection::does_intersect)
                                && (scalar::greater<typename intersection::material::reflection, scalar::epsilon>::value)
        > class whitted_mirror {
                typedef typename ray::direction                           ray_dir;
                typedef typename ray::position                            ray_pos;
                typedef typename intersection::normal                     surface_normal;
                typedef typename intersection::point_of_intersection      POI;
                typedef typename vector::reflect<ray_dir, surface_normal> reflect_dir;
                typedef typename ::ray::ray<
                        vector::add<
                                POI, 
                                //vector::scale<reflect_dir, scalar::mul<scalar::epsilon, scalar::c5>>
                                vector::scale<surface_normal, scalar::reflection_offset_epsilon>
                        >, 
                        reflect_dir
                > reflect_ray;
        public:
                typedef typename raytrace<reflect_ray, depth_max-1>::color color;                
        };

        template <
                typename intersection,
                typename ray,
                template <typename, unsigned int> class raytrace,
                unsigned int depth_max
        > struct whitted_mirror<intersection, ray, raytrace, depth_max, false> {
                typedef color::rgbf<scalar::c0,scalar::c0,scalar::c0> color;
        };

        template <
                typename CameraT, 
                typename ObjectsT, 
                typename LightsT
        > struct whitted_style {
                typedef CameraT camera;
                typedef ObjectsT objects;
                typedef LightsT lights;
                
                template <typename ray, unsigned int depth_max_> struct raytrace {
                private:
                        enum { depth_max = depth_max_ };                

                        typedef vector::normal_to_rgbf<typename ray::direction> background_color;
                
                        typedef typename ObjectsT::template intersect<ray> intersection;
                        enum {
                                does_intersect = intersection::does_intersect
                        };

                        typedef typename lights::template shade<intersection>::lit_color diffuse_color;
                private:
                        typedef typename whitted_mirror<
                                intersection,
                                ray,
                                whitted_style::raytrace,
                                depth_max
                        >::color mirror_color;

                        typedef typename intersection::material::reflection reflection;
                        typedef typename intersection::material::color      surface_color;
                public:
                        typedef ift<
                                intersection::does_intersect,
                                color::add_rgbf<
                                        color::mul_rgbf<
                                                diffuse_color,
                                                scalar::sub<scalar::c1,reflection>
                                        >,
                                        color::mul_rgbf<
                                                color::filter_rgbf<
                                                        surface_color,
                                                        mirror_color
                                                >,
                                                reflection
                                        >
                                >,
                                background_color
                        > color;
                };
        };
        
        template <
                template<typename,typename,typename> class RendererT,                
                typename CameraT, 
                typename ObjectsT, 
                typename LightsT                
        > struct scene {
                
                template <unsigned int x, unsigned int width, unsigned int y, unsigned int height> struct kernel {
                public:                                                
                        // Map 0..width to -0.5..+0.5.
                        typedef scalar::sub< 
                                scalar::div<scalar::from_int<x+config::virtual_x>, scalar::from_int<config::virtual_width>>, 
                                scalar::c0_5
                        > u;
                        typedef scalar::neg<scalar::sub<
                                scalar::div<scalar::from_int<y+config::virtual_y>, scalar::from_int<config::virtual_height>>,
                                scalar::c0_5
                        >> v;
                
                        // Generate primary ray.
                        typedef typename CameraT::template generate<u,v>::type ray;
                
                        typedef typename RendererT<
                                CameraT, ObjectsT, LightsT
                        >::template raytrace<ray,15> raytrace;
                public:                        
                        typedef typename color::rgbf_to_rgb<typename raytrace::color> color;
                        
                        // only for debugging
                        /*
                        typedef typename raytrace::distance distance;
                        typedef typename raytrace::normal_color normal_color;
                        typedef typename raytrace::direction_color direction_color;
                        typedef typename raytrace::intersection::normal normal;
                        typedef typename raytrace::intersection::point_of_intersection point_of_intersection;                        
                        enum { does_intersect = raytrace::does_intersect };
                        */
                };
        };
}
