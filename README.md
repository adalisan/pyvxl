## pyVXL: A Python interface for the [VXL](https://github.com/vxl/vxl) computer vision libraries

### Requirements:
- [CMake](https://cmake.org)
- [VXL](https://github.com/vxl/vxl)
- Python
- [Boost](www.boost.org) (Boost.Python specifically)

### Building:
You will need to point cmake to your VXL build/install directory via the `VXL_DIR` cmake variable.  If you want to use a version of python different than the one in your PATH, you will need to set `CMAKE_PREFIX_PATH` accordingly.
```
> cd <build dir>
> cmake -DVXL_DIR=<directory containing UseVXL.cmake> <source dir>
> make
```
This should create `vxl.so` (or `vxl.pyd` on windows), which can be imported into python.

### Example Usage:
#### VNL
```python
>>> import vxl
>>> m = vxl.vnl_matrix(((1,2),(3,4)))
>>> v = vxl.vnl_vector((10,20))
>>> print(m * v)
50 110
```
#### VGL/VPGL
```python
>>> import vxl
>>> K = vxl.vpgl_calibration_matrix(100.0, vxl.vgl_point_2d(320,240))
>>> R = vxl.vgl_rotation_3d()
>>> T = vxl.vgl_vector_3d(0,0,1000)
>>> cam = vxl.vpgl_perspective_camera(K,R,T)
>>> print(cam.project(vxl.vgl_point_3d(1,2,3)))
<vgl_point_2d 320.1,240.199>
```
