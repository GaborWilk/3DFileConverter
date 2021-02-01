# 3DFileConverter

3DFileConverter is a general purpose 3D file converter tool. It reads files to gather 3D data, which is stored in an internal data structure and has basic transformation functionalities (scale, rotation and translatation). The original file (or the modified data) can be written into an output file with other fileformat.
The implementation is based on the C++17 standard (CMake 3.8 is required).


**Note:** 

The current version works only with the following file formats:
- input file: 
  - **.obj** (with only v, vn, vt and f parameters, see [OBJ Format](http://paulbourke.net/dataformats/obj/) for more details)
- output file: 
  - **.stl** (binary, see [STL Format](https://en.wikipedia.org/wiki/STL_(file_format)) for more details)

For more details (concrete example), see the main.cpp file, where you can see how to use the functions. The tool reads the cube.obj file and writes it out into cube.stl file under the res directory.

## Usage

```cpp
/* include necessary header file */
#include "FileConverter.h"

using namespace fileconv;

/* get FileConverter object */
auto& fc = FileConverter::getInstance();

/* set input and output converter types */
fc.setInputFormat(InputType::INPUT_TYPE_OBJ);
fc.setOutputFormat(OutputType::OUTPUT_TYPE_STL);

/* read file */
fc.read("path/to/input/file");

/* check whether point is inside the polygon */
glm::dvec3 point(1.0, 2.0, 3.0);
bool isInside = fc.isPointInside(point);

/* get the volume of the mesh */
double result = fc.volume();

/* get the surface of the mesh */
double result = fc.surface();

/* set scale */
glm::dvec3 scale(2.0, 2.0, 2.0);
fc.scale(scale);

/* set rotate */
glm::dvec3 rotate(-3.0, -3.0, -3.0);
fc.rotate(rotate);

/* set translate */
glm::dvec3 translate(4.0, 4.0, 4.0);
fc.translate(translate);

/* write file */
fc.write("path/to/output/file");
```

## 3rd party libraries
The tool uses the [OpenGL Mathematics](https://glm.g-truc.net/0.9.9/index.html) library for mathematical computations.

The [Catch2](https://github.com/catchorg/Catch2) is used for unit testing purposes only.

Note: Catch2 is a single-header file to test the tool's functionalities. Because of the large size of the file the compilation time can be above average, but it does not functionally affect the tool's performance.

## License
[MIT](https://choosealicense.com/licenses/mit/)