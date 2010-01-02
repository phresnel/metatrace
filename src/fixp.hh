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
