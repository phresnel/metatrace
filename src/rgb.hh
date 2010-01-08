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
