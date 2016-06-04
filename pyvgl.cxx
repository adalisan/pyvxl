#include "pyvgl.h"
#include <boost/python.hpp>
#include <boost/python/args.hpp>
#include <vgl/vgl_vector_2d.h>
#include <vgl/vgl_point_2d.h>
#include <vgl/vgl_vector_3d.h>
#include <vgl/vgl_point_3d.h>
#include <vgl/algo/vgl_rotation_3d.h>

#include "pyvxl_util.h"

using namespace boost::python;


// Helper functions
template<class T>
long three(T const&) { return 3;}

template<class T>
long two(T const&) { return 2;}


template<class T>
double vgl_getitem_3d(T const& a, long i)
{
  // wrap around
  if (i < 0) {
    i += 3;
  }
  if (i==0) {
    return a.x();
  }
  else if (i==1) {
    return a.y();
  }
  else if (i==2) {
    return a.z();
  }
  else {
    PyErr_SetString(PyExc_IndexError, "index out of range");
    boost::python::throw_error_already_set();
  }
  return 0; // to avoid compiler warning
}

template<class T>
double vgl_getitem_2d(T const& a, long i)
{
  // wrap around
  if (i < 0) {
    i += 2;
  }
  if (i==0) {
    return a.x();
  }
  else if (i==1) {
    return a.y();
  }
  else {
    PyErr_SetString(PyExc_IndexError, "index out of range");
    boost::python::throw_error_already_set();
  }
  return 0; // to avoid compiler warning
}

template<class T>
double vgl_get_x(T const& a) { return a.x(); }
template<class T>
double vgl_get_y(T const& a) { return a.y(); }
template<class T>
double vgl_get_z(T const& a) { return a.z(); }


void pyvxl::wrap_vgl()
{
  class_<vgl_point_2d<double> > ("vgl_point_2d")
    .def(init<double,double>())
    .def("__len__", two<vgl_point_2d<double> >)
    .def("__getitem__", vgl_getitem_2d<vgl_point_2d<double> >)
    .def("__str__", stream2str<vgl_point_2d<double> >)
    .add_property("x", &vgl_get_x<vgl_point_2d<double> >)
    .add_property("y", &vgl_get_y<vgl_point_2d<double> >);

  class_<vgl_vector_2d<double> > ("vgl_vector_2d")
    .def(init<double,double>())
    .def("__len__", two<vgl_vector_2d<double> >)
    .def("__getitem__",vgl_getitem_2d<vgl_vector_2d<double> >)
    .def("__str__", stream2str<vgl_vector_2d<double> >)
    .add_property("x", &vgl_vector_2d<double>::x)
    .add_property("y", &vgl_vector_2d<double>::y)
    .def("length", &vgl_vector_2d<double>::length);

  class_<vgl_point_3d<double> > ("vgl_point_3d")
    .def(init<double,double,double>())
    .def("__len__", three<vgl_point_3d<double> >)
    .def("__getitem__", vgl_getitem_3d<vgl_point_3d<double> >)
    .def("__str__", stream2str<vgl_point_3d<double> >)
    .add_property("x", vgl_get_x<vgl_point_3d<double> >)
    .add_property("y", vgl_get_y<vgl_point_3d<double> >)
    .add_property("z", vgl_get_z<vgl_point_3d<double> >);

  class_<vgl_vector_3d<double> > ("vgl_vector_3d")
    .def(init<double,double,double>())
    .def("__len__", three<vgl_vector_3d<double> >)
    .def("__getitem__", vgl_getitem_3d<vgl_vector_3d<double> >)
    .def("__str__", stream2str<vgl_vector_3d<double> >)
    .add_property("x", &vgl_vector_3d<double>::x)
    .add_property("y", &vgl_vector_3d<double>::y)
    .add_property("z", &vgl_vector_3d<double>::z)
    .def("length", &vgl_vector_3d<double>::length);

  class_ <vgl_rotation_3d<double> > ("vgl_rotation_3d")
    .def(init<vnl_vector_fixed<double,4> >())
    .def(init<vnl_matrix_fixed<double,3,3> >())
    .def("__str__", stream2str<vgl_rotation_3d<double> >)
    .def("as_matrix", &vgl_rotation_3d<double>::as_matrix);

}
