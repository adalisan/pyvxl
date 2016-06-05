#include "pyvnl.h"
#include <boost/python.hpp>
#include <boost/python/list.hpp>
#include <boost/python/args.hpp>
#include <boost/shared_ptr.hpp>
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

template<unsigned NR, unsigned NC>
boost::shared_ptr<vnl_matrix_fixed<double,NR,NC> > matrix_fixed_from_object(boost::python::object const& in)
{
  const size_t num_rows = boost::python::len(in);
  if (num_rows != NR) {
    std::stringstream errstr;
    errstr << "Input must have " << NR << " rows.";
    PyErr_SetString(PyExc_IndexError, errstr.str().c_str());
    boost::python::throw_error_already_set();
  }
  boost::shared_ptr<vnl_matrix_fixed<double,NR,NC> > out(new vnl_matrix_fixed<double,NR,NC>());
  vnl_matrix_fixed<double,NR,NC> &out_ref = *out;
  for (size_t r=0; r<NR; ++r) {
    const size_t num_cols = boost::python::len(in[r]);
    if (num_cols != NC) {
      std::stringstream errstr;
      errstr << "Input must have " << NC << " columns in all rows.";
      PyErr_SetString(PyExc_IndexError, errstr.str().c_str());
      boost::python::throw_error_already_set();
    }
    for (size_t c=0; c<NC; ++c) {
      out_ref(r,c) = extract<double>(in[r][c]);
    }
  }
  return out;
}

boost::shared_ptr<vnl_matrix<double> > matrix_from_object(boost::python::object const& in)
{
  const size_t num_rows = boost::python::len(in);
  if (num_rows == 0) {
    return boost::shared_ptr<vnl_matrix<double> >(new vnl_matrix<double>(0,0));
  }
  const size_t num_cols = boost::python::len(in[0]);
  boost::shared_ptr<vnl_matrix<double> > out(new vnl_matrix<double>(num_rows, num_cols));
  vnl_matrix<double> &out_ref = *out;
  for (size_t r=0; r<num_rows; ++r) {
    const size_t num_cols_this_row = boost::python::len(in[r]);
    if (num_cols_this_row != num_cols) {
      PyErr_SetString(PyExc_IndexError, "Input must have same number of elements in every row");
      boost::python::throw_error_already_set();
    }
    for (size_t c=0; c<num_cols; ++c) {
      out_ref(r,c) = extract<double>(in[r][c]);
    }
  }
  return out;
}

boost::shared_ptr<vnl_vector<double> > vector_from_object(boost::python::object const& in)
{
  const size_t size = boost::python::len(in);
  boost::shared_ptr<vnl_vector<double> > out(new vnl_vector<double>(size));
  vnl_vector<double> &out_ref = *out;
  for (size_t i=0; i<size; ++i) {
    out_ref(i) = extract<double>(in[i]);
  }
  return out;
}


void pyvxl::wrap_vnl()
{

  class_<vnl_vector<double> >("vnl_vector")
    .def(init<size_t>())
    .def(init<size_t, double>())
    .def("__init__", make_constructor(vector_from_object))
    .def("get", &vnl_vector<double>::get)
    .def("size", &vnl_vector<double>::size)
    .def("__len__", vnl_vector_len)
    .def("__str__", stream2str<vnl_vector<double> >)
    .def("__getitem__", vnl_vector_getitem);

  class_<vnl_matrix<double> >("vnl_matrix")
    .def(init<size_t,size_t>())
    .def("__init__", make_constructor(matrix_from_object))
    .def("get", &vnl_matrix<double>::get)
    .add_property("shape", &vnl_matrix_shape)
    .def("__str__", stream2str<vnl_matrix<double> >)
    .def("__getitem__", vnl_matrix_getitem)
    .def("__len__", vnl_matrix_len);

  class_<vnl_matrix_fixed<double,3,3> >("vnl_matrix_fixed_3x3")
    .def("__init__", make_constructor(matrix_fixed_from_object<3,3>))
    .def("get", &vnl_matrix_fixed<double,3,3>::get)
    .add_property("shape", &vnl_matrix_fixed_shape<double,3,3>)
    .def("__str__", stream2str<vnl_matrix_fixed<double,3,3> >)
    .def("__getitem__", vnl_matrix_fixed_getitem<double,3,3>)
    .def("__len__", vnl_matrix_fixed_len<double,3,3>);

  class_<vnl_matrix_fixed<double,3,4> >("vnl_matrix_fixed_3x4")
    .def("__init__", make_constructor(matrix_fixed_from_object<3,4>))
    .def("get", &vnl_matrix_fixed<double,3,4>::get)
    .add_property("shape", &vnl_matrix_fixed_shape<double,3,4>)
    .def("__str__", stream2str<vnl_matrix_fixed<double,3,4> >)
    .def("__getitem__", vnl_matrix_fixed_getitem<double,3,4>)
    .def("__len__", vnl_matrix_fixed_len<double,3,4>);
}
