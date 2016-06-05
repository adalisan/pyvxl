#include "pyvnl.h"
#include <boost/python.hpp>
#include <boost/python/list.hpp>
#include <boost/python/args.hpp>
#include <boost/shared_ptr.hpp>
#include <vnl/vnl_vector.h>
#include <vnl/vnl_matrix.h>
#include <vnl/vnl_matrix_fixed.h>
#include <vnl/vnl_vector_fixed.h>
#include "pyvxl_util.h"

using namespace boost::python;

using namespace pyvxl;


// VNL HELPER FUNCTIONS

template <class T>
T vnl_vector_getitem(vnl_vector<T> const& v, long i)
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

template <class T>
long vnl_vector_len(vnl_vector<T> const& v)
{
  return v.size();
}

template <class T>
vnl_vector<T> vnl_matrix_getitem(vnl_matrix<T> const& m, long r)
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
vnl_vector<T> vnl_matrix_fixed_getitem(vnl_matrix_fixed<T,R,C> const& m, long r)
{
  return vnl_matrix_getitem<T>(m.as_ref(), r);
}

template<class T, unsigned N>
T vnl_vector_fixed_getitem(vnl_vector_fixed<T,N> const& v, long i)
{
  return vnl_vector_getitem<T>(v.as_ref(), i);
}

template <class T>
long vnl_matrix_len(vnl_matrix<T> const& m)
{
  return m.rows();
}

template<class T, unsigned R, unsigned C>
long vnl_matrix_fixed_len(vnl_matrix_fixed<T,R,C> const& m)
{
  return R;
}

template<class T, unsigned N>
long vnl_vector_fixed_len(vnl_vector_fixed<T,N> const& v)
{
  return N;
}

template<class T>
tuple vnl_matrix_shape(vnl_matrix<T> const& m)
{
  return boost::python::make_tuple(m.rows(), m.cols());
}

template<class T>
tuple vnl_vector_shape(vnl_vector<T> const& v)
{
  return boost::python::make_tuple(v.size());
}

template<class T, unsigned R, unsigned C>
tuple vnl_matrix_fixed_shape(vnl_matrix_fixed<T,R,C> const& m)
{
  return boost::python::make_tuple(R,C);
}

template<class T, unsigned N>
tuple vnl_vector_fixed_shape(vnl_vector_fixed<T,N> const& v)
{
  return boost::python::make_tuple(N);
}

template<class T, unsigned NR, unsigned NC>
boost::shared_ptr<vnl_matrix_fixed<T,NR,NC> > matrix_fixed_from_object(boost::python::object const& in)
{
  const size_t num_rows = boost::python::len(in);
  if (num_rows != NR) {
    std::stringstream errstr;
    errstr << "Input must have " << NR << " rows.";
    PyErr_SetString(PyExc_IndexError, errstr.str().c_str());
    boost::python::throw_error_already_set();
  }
  boost::shared_ptr<vnl_matrix_fixed<T,NR,NC> > out(new vnl_matrix_fixed<T,NR,NC>());
  vnl_matrix_fixed<T,NR,NC> &out_ref = *out;
  for (size_t r=0; r<NR; ++r) {
    const size_t num_cols = boost::python::len(in[r]);
    if (num_cols != NC) {
      std::stringstream errstr;
      errstr << "Input must have " << NC << " columns in all rows.";
      PyErr_SetString(PyExc_IndexError, errstr.str().c_str());
      boost::python::throw_error_already_set();
    }
    for (size_t c=0; c<NC; ++c) {
      out_ref(r,c) = extract<T>(in[r][c]);
    }
  }
  return out;
}

template<class T, unsigned N>
boost::shared_ptr<vnl_vector_fixed<T,N> > vector_fixed_from_object(boost::python::object const& in)
{
  const size_t n = boost::python::len(in);
  if (n != N) {
    std::stringstream errstr;
    errstr << "Input must have length " << N;
    PyErr_SetString(PyExc_IndexError, errstr.str().c_str());
    boost::python::throw_error_already_set();
  }
  boost::shared_ptr<vnl_vector_fixed<T,N> > out(new vnl_vector_fixed<T,N>());
  vnl_vector_fixed<T,N> &out_ref = *out;
  for (size_t i=0; i<N; ++i) {
    out_ref(i) = extract<T>(in[i]);
  }
  return out;
}

template <class T>
boost::shared_ptr<vnl_matrix<T> > matrix_from_object(boost::python::object const& in)
{
  const size_t num_rows = boost::python::len(in);
  if (num_rows == 0) {
    return boost::shared_ptr<vnl_matrix<T> >(new vnl_matrix<T>(0,0));
  }
  const size_t num_cols = boost::python::len(in[0]);
  boost::shared_ptr<vnl_matrix<T> > out(new vnl_matrix<T>(num_rows, num_cols));
  vnl_matrix<T> &out_ref = *out;
  for (size_t r=0; r<num_rows; ++r) {
    const size_t num_cols_this_row = boost::python::len(in[r]);
    if (num_cols_this_row != num_cols) {
      PyErr_SetString(PyExc_IndexError, "Input must have same number of elements in every row");
      boost::python::throw_error_already_set();
    }
    for (size_t c=0; c<num_cols; ++c) {
      out_ref(r,c) = extract<T>(in[r][c]);
    }
  }
  return out;
}

template <class T>
boost::shared_ptr<vnl_vector<T> > vector_from_object(boost::python::object const& in)
{
  const size_t size = boost::python::len(in);
  boost::shared_ptr<vnl_vector<T> > out(new vnl_vector<T>(size));
  vnl_vector<T> &out_ref = *out;
  for (size_t i=0; i<size; ++i) {
    out_ref(i) = extract<T>(in[i]);
  }
  return out;
}

template<class T>
void wrap_vnl_matrix(std::string const& class_name)
{
  class_<vnl_matrix<T> >(class_name.c_str())
    .def(init<size_t,size_t>())
    .def("__init__", make_constructor(matrix_from_object<T>))
    .def("get", &vnl_matrix<T>::get)
    .add_property("shape", &vnl_matrix_shape<T>)
    .def("__str__", stream2str<vnl_matrix<T> >)
    .def("__getitem__", vnl_matrix_getitem<T>)
    .def("__len__", vnl_matrix_len<T>)
    .def(self + vnl_matrix<T>())
    .def(self * vnl_vector<T>());
}

template<class T>
void wrap_vnl_vector(std::string const& class_name)
{
  class_<vnl_vector<T> >(class_name.c_str())
    .def(init<size_t>())
    .def(init<size_t, T>())
    .def("__init__", make_constructor(vector_from_object<T>))
    .def("get", &vnl_vector<T>::get)
    .def("size", &vnl_vector<T>::size)
    .def("__len__", vnl_vector_len<T>)
    .def("__str__", stream2str<vnl_vector<T> >)
    .def("__getitem__", vnl_vector_getitem<T>)
    .def(self + vnl_vector<T>());
}

template<class T, unsigned N>
void wrap_vnl_vector_fixed(std::string const& class_name)
{
  class_<vnl_vector_fixed<T,N> >(class_name.c_str())
    .def("__init__", make_constructor(vector_fixed_from_object<T,N>))
    .def("get", &vnl_vector_fixed<T,N>::get)
    .add_property("shape", &vnl_vector_fixed_shape<T,N>)
    .def("__str__", stream2str<vnl_vector_fixed<T,N> >)
    .def("__len__", vnl_vector_fixed_len<T,N>)
    .def("__getitem__", vnl_vector_fixed_getitem<T,N>)
    .def(self + vnl_vector<T>())
    .def(self + vnl_vector_fixed<T,N>());
}

template<class T, unsigned NR, unsigned NC>
void wrap_vnl_matrix_fixed(std::string const& class_name)
{
  class_<vnl_matrix_fixed<T,NR,NC> >(class_name.c_str())
    .def("__init__", make_constructor(matrix_fixed_from_object<T,NR,NC>))
    .def("get", &vnl_matrix_fixed<T,NR,NC>::get)
    .add_property("shape", &vnl_matrix_fixed_shape<T,NR,NC>)
    .def("__str__", stream2str<vnl_matrix_fixed<T,NR,NC> >)
    .def("__getitem__", vnl_matrix_fixed_getitem<T,NR,NC>)
    .def("__len__", vnl_matrix_fixed_len<T,NR,NC>)
    .def(self + vnl_matrix_fixed<T,NR,NC>())
    .def(self * vnl_vector<T>())
    .def(self * vnl_vector_fixed<T,NC>());
}


void pyvxl::wrap_vnl()
{
  wrap_vnl_vector<double>("vnl_vector");
  wrap_vnl_matrix<double>("vnl_matrix");
  wrap_vnl_matrix_fixed<double,3,3>("vnl_matrix_fixed_3x3");
  wrap_vnl_matrix_fixed<double,3,4>("vnl_matrix_fixed_3x4");
  wrap_vnl_vector_fixed<double,3>("vnl_vector_fixed_3");
  wrap_vnl_vector_fixed<double,4>("vnl_vector_fixed_4");
}
