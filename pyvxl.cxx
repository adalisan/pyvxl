#include <boost/python.hpp>
#include "pyvnl.h"
#include "pyvgl.h"
#include "pyvpgl.h"

using namespace boost::python;


BOOST_PYTHON_MODULE(vxl)
{
  pyvxl::wrap_vnl();
  pyvxl::wrap_vgl();
  pyvxl::wrap_vpgl();
}
