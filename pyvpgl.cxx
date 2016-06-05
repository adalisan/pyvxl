#include "pyvpgl.h"
#include <boost/python.hpp>
#include <boost/python/args.hpp>
#include <boost/noncopyable.hpp>
#include <vpgl/vpgl_proj_camera.h>
#include <vpgl/vpgl_affine_camera.h>
#include <vpgl/vpgl_perspective_camera.h>
#include <vgl/vgl_point_3d.h>
#include <vgl/vgl_point_2d.h>
#include <vgl/vgl_homg_point_2d.h>
#include <vgl/vgl_vector_3d.h>
#include <vgl/vgl_vector_2d.h>
#include <vgl/vgl_homg_point_2d.h>
#include <vgl/vgl_homg_point_3d.h>

#include "pyvxl_util.h"

using namespace boost::python;

// Helper functions

template<class T>
vgl_point_2d<double> vpgl_project_point(T const& cam, vgl_point_3d<double> const& pt)
{
  double u,v;
  cam.project(pt.x(), pt.y(), pt.z(), u, v);
  return vgl_point_2d<double>(u,v);
}

template<class T>
vgl_vector_2d<double> vpgl_project_vector(T const& cam, vgl_vector_3d<double> const& vec)
{
  double u,v;
  cam.project(vec.x(), vec.y(), vec.z(), u, v);
  return vgl_vector_2d<double>(u,v);
}

template<class T>
vgl_homg_point_2d<double> vpgl_project_homg_point(T const& cam, vgl_homg_point_3d<double> const& x)
{
  return cam.project(x);
}

void pyvxl::wrap_vpgl()
{
  class_<vpgl_proj_camera<double> >("vpgl_proj_camera")
    .def(init<vnl_matrix_fixed<double,3,4> >())
    .def("__str__", stream2str<vpgl_proj_camera<double> >)
    .def("project", vpgl_project_homg_point<vpgl_proj_camera<double> >)
    .def("project", vpgl_project_point<vpgl_proj_camera<double> >)
    .def("project", vpgl_project_vector<vpgl_proj_camera<double> >)
    .def("get_matrix", &vpgl_affine_camera<double>::get_matrix, return_value_policy<return_by_value>());

  class_<vpgl_affine_camera<double>, bases<vpgl_proj_camera<double> > >("vpgl_affine_camera")
    .def(init<vnl_matrix_fixed<double,3,4> >())
    .def("__str__", stream2str<vpgl_proj_camera<double> >);

  class_<vpgl_calibration_matrix<double> >("vpgl_calibration_matrix")
    .def(init<vnl_matrix_fixed<double,3,3> >())
    .def(init<double, vgl_point_2d<double> >());

  class_<vpgl_perspective_camera<double>, bases<vpgl_proj_camera<double> > >("vpgl_perspective_camera")
    .def(init<vpgl_calibration_matrix<double>, vgl_rotation_3d<double>, vgl_vector_3d<double> >())
    .def("__str__", stream2str<vpgl_perspective_camera<double> >);
}
