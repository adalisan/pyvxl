#include "pyvnl.h"
#include <boost/python.hpp>
#include <boost/python/args.hpp>
#include <vnl/vnl_vector.h>
#include <vnl/vnl_matrix.h>
#include <vnl/vnl_matrix_fixed.h>
#include "pyvxl_util.h"

using namespace boost::python;

using namespace pyvxl;


// VNL HELPER FUNCTIONS

double vnl_vector_getitem(vnl_vector<double> const& v, long i)
{
  // wrap around
  if (i < 0) {
    i += v.size();
  }
  if ((i < 0) || (i >= v.size())) {
    PyErr_SetString(PyExc_IndexError, "index out of range");
    boost::python::throw_error_already_set();
  }
  return v[i];
}

long vnl_vector_len(vnl_vector<double> const& v)
{
  return v.size();
}

vnl_vector<double> vnl_matrix_getitem(vnl_matrix<double> const& m, long r)
{
  if (r < 0) {
    r += m.rows();
  }
  if ((r < 0) || (r >= m.rows())) {
    PyErr_SetString(PyExc_IndexError, "index out of range");
    boost::python::throw_error_already_set();
  }
  return m.get_row(r);
}

template<class T, unsigned R, unsigned C>
vnl_vector<double> vnl_matrix_fixed_getitem(vnl_matrix_fixed<T,R,C> const& m, long r)
{
  return vnl_matrix_getitem(m.as_ref(), r);
}

long vnl_matrix_len(vnl_matrix<double> const& m)
{
  return m.rows();
}

template<class T, unsigned R, unsigned C>
long vnl_matrix_fixed_len(vnl_matrix_fixed<T,R,C> const& m)
{
  return vnl_matrix_len(m.as_ref());
}

tuple vnl_matrix_shape(vnl_matrix<double> const& m)
{
  return make_tuple(m.rows(), m.cols());
}

template<class T, unsigned R, unsigned C>
tuple vnl_matrix_fixed_shape(vnl_matrix_fixed<T,R,C> const& m)
{
  return make_tuple(R,C);
}


void pyvxl::wrap_vnl()
{

  class_<vnl_vector<double> >("vnl_vector")
    .def(init<size_t>())
    .def(init<size_t, double>())
    .def("get", &vnl_vector<double>::get)
    .def("size", &vnl_vector<double>::size)
    .def("__len__", vnl_vector_len)
    .def("__str__", stream2str<vnl_vector<double> >)
    .def("__getitem__", vnl_vector_getitem);

  class_<vnl_matrix<double> >("vnl_matrix")
    .def(init<size_t,size_t>())
    .def("get", &vnl_matrix<double>::get)
    .add_property("shape", &vnl_matrix_shape)
    .def("__str__", stream2str<vnl_matrix<double> >)
    .def("__getitem__", vnl_matrix_getitem)
    .def("__len__", vnl_matrix_len);

  class_<vnl_matrix_fixed<double,3,3> >("vnl_matrix_fixed_3x3")
    .def("get", &vnl_matrix_fixed<double,3,3>::get)
    .add_property("shape", &vnl_matrix_fixed_shape<double,3,3>)
    .def("__str__", stream2str<vnl_matrix_fixed<double,3,3> >)
    .def("__getitem__", vnl_matrix_fixed_getitem<double,3,3>)
    .def("__len__", vnl_matrix_fixed_len<double,3,3>);

  class_<vnl_matrix_fixed<double,3,4> >("vnl_matrix_fixed_3x4")
    .def("get", &vnl_matrix_fixed<double,3,4>::get)
    .add_property("shape", &vnl_matrix_fixed_shape<double,3,4>)
    .def("__str__", stream2str<vnl_matrix_fixed<double,3,4> >)
    .def("__getitem__", vnl_matrix_fixed_getitem<double,3,4>)
    .def("__len__", vnl_matrix_fixed_len<double,3,4>);
}
