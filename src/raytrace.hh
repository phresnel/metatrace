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
                                vector::scale<surface_normal, scalar::mul<scalar::epsilon, scalar::c5>>
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
                                                        diffuse_color,                
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
