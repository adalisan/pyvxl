#include "pyvgl.h"
#include <boost/python.hpp>
#include <boost/python/args.hpp>
#include <vgl/vgl_vector_2d.h>
#include <vgl/vgl_point_2d.h>
#include <vgl/vgl_vector_3d.h>
#include <vgl/vgl_point_3d.h>
#include <vgl/vgl_homg_point_2d.h>
#include <vgl/vgl_homg_point_3d.h>
#include <vgl/algo/vgl_rotation_3d.h>

#include "pyvxl_util.h"

using namespace boost::python;


// Helper functions
template<class T>
long two(T const&) { return 2;}

template<class T>
long three(T const&) { return 3;}

template<class T>
long four(T const&) { return 4;}


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
double vgl_getitem_homg_2d(T const& a, long i)
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
    return a.w();
  }
  else {
    PyErr_SetString(PyExc_IndexError, "index out of range");
    boost::python::throw_error_already_set();
  }
  return 0; // to avoid compiler warning
}

template<class T>
double vgl_getitem_homg_3d(T const& a, long i)
{
  // wrap around
  if (i < 0) {
    i += 4;
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
  else if (i==3) {
    return a.w();
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
template<class T>
double vgl_get_w(T const& a) { return a.w(); }

template<class T>
void wrap_vgl_point_2d(std::string const& class_name)
{
  class_<vgl_point_2d<T> > (class_name.c_str())
    .def(init<T,T>())
    .def("__len__", two<vgl_point_2d<T> >)
    .def("__getitem__", vgl_getitem_2d<vgl_point_2d<T> >)
    .def("__str__", stream2str<vgl_point_2d<T> >)
    .add_property("x", &vgl_get_x<vgl_point_2d<T> >)
    .add_property("y", &vgl_get_y<vgl_point_2d<T> >);
}

template<class T>
void wrap_vgl_homg_point_2d(std::string const& class_name)
{
  class_<vgl_homg_point_2d<T> > (class_name.c_str())
    .def(init<T,T>())
    .def(init<T,T,T>())
    .def(init<vgl_point_2d<T> >())
    .def("__len__", three<vgl_homg_point_2d<T> >)
    .def("__getitem__", vgl_getitem_homg_2d<vgl_homg_point_2d<T> >)
    .def("__str__", stream2str<vgl_homg_point_2d<T> >)
    .add_property("x", &vgl_get_x<vgl_homg_point_2d<T> >)
    .add_property("y", &vgl_get_y<vgl_homg_point_2d<T> >)
    .add_property("w", &vgl_get_w<vgl_homg_point_2d<T> >);
}

template<class T>
void wrap_vgl_point_3d(std::string const& class_name)
{
  class_<vgl_point_3d<T> > (class_name.c_str())
    .def(init<T,T,T>())
    .def("__len__", three<vgl_point_3d<T> >)
    .def("__getitem__", vgl_getitem_3d<vgl_point_3d<T> >)
    .def("__str__", stream2str<vgl_point_3d<T> >)
    .add_property("x", vgl_get_x<vgl_point_3d<T> >)
    .add_property("y", vgl_get_y<vgl_point_3d<T> >)
    .add_property("z", vgl_get_z<vgl_point_3d<T> >);
}

template<class T>
void wrap_vgl_homg_point_3d(std::string const& class_name)
{
  class_<vgl_homg_point_3d<T> > (class_name.c_str())
    .def(init<T,T,T>())
    .def(init<T,T,T,T>())
    .def(init<vgl_point_3d<T> >())
    .def("__len__", four<vgl_homg_point_3d<T> >)
    .def("__getitem__", vgl_getitem_homg_3d<vgl_homg_point_3d<T> >)
    .def("__str__", stream2str<vgl_homg_point_3d<T> >)
    .add_property("x", &vgl_get_x<vgl_homg_point_3d<T> >)
    .add_property("y", &vgl_get_y<vgl_homg_point_3d<T> >)
    .add_property("z", &vgl_get_z<vgl_homg_point_3d<T> >)
    .add_property("w", &vgl_get_w<vgl_homg_point_3d<T> >);
}

template<class T>
void wrap_vgl_vector_2d(std::string const& class_name)
{
  class_<vgl_vector_2d<T> > (class_name.c_str())
    .def(init<T,T>())
    .def("__len__", two<vgl_vector_2d<T> >)
    .def("__getitem__",vgl_getitem_2d<vgl_vector_2d<T> >)
    .def("__str__", stream2str<vgl_vector_2d<T> >)
    .add_property("x", &vgl_vector_2d<T>::x)
    .add_property("y", &vgl_vector_2d<T>::y)
    .def("length", &vgl_vector_2d<T>::length);
}

template <class T>
void wrap_vgl_vector_3d(std::string const& class_name)
{
  class_<vgl_vector_3d<T> > (class_name.c_str())
    .def(init<T,T,T>())
    .def("__len__", three<vgl_vector_3d<T> >)
    .def("__getitem__", vgl_getitem_3d<vgl_vector_3d<T> >)
    .def("__str__", stream2str<vgl_vector_3d<T> >)
    .add_property("x", &vgl_vector_3d<T>::x)
    .add_property("y", &vgl_vector_3d<T>::y)
    .add_property("z", &vgl_vector_3d<T>::z)
    .def("length", &vgl_vector_3d<T>::length);
}

template<class T>
void wrap_vgl_rotation_3d(std::string const& class_name)
{
  class_ <vgl_rotation_3d<T> > (class_name.c_str())
    .def(init<vnl_vector_fixed<T,4> >())
    .def(init<vnl_matrix_fixed<T,3,3> >())
    .def("__str__", stream2str<vgl_rotation_3d<T> >)
    .def("as_matrix", &vgl_rotation_3d<T>::as_matrix);
}

void pyvxl::wrap_vgl()
{
  wrap_vgl_point_2d<double>("vgl_point_2d");
  wrap_vgl_homg_point_2d<double>("vgl_homg_point_2d");
  wrap_vgl_point_3d<double>("vgl_point_3d");
  wrap_vgl_homg_point_3d<double>("vgl_homg_point_3d");
  wrap_vgl_vector_2d<double>("vgl_vector_2d");
  wrap_vgl_vector_3d<double>("vgl_vector_3d");
  wrap_vgl_rotation_3d<double>("vgl_rotation_3d");
}
