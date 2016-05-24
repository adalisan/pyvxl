#include "pyvpgl.h"
#include <boost/python.hpp>
#include <boost/python/args.hpp>
#include <vpgl/vpgl_affine_camera.h>

using namespace boost::python;

// Helper functions

template<class T>
vgl_point_2d<double> vpgl_project(T const& cam, vgl_point_3d<double> const& pt)
{
  double u,v;
  cam.project(pt.x(), pt.y(), pt.z(), u, v);
  return vgl_point_2d<double>(u,v);
}


void pyvxl::wrap_vpgl()
{
  class_<vpgl_affine_camera<double> >("vpgl_affine_camera")
    .def(init<vnl_matrix_fixed<double,3,4> >())
    .def("project", &vpgl_project<vpgl_affine_camera<double> >)
    .def("get_matrix", &vpgl_affine_camera<double>::get_matrix, return_value_policy<return_by_value>());

}
