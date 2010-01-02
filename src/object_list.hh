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
