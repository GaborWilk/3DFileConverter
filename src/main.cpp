#include "FileConverter.h"


using namespace conv;

int main() {
  /* set file paths */
  const std::string INPUT_FILE = "../3dfc/res/cube.obj";
  const std::string OUTPUT_FILE = "../3dfc/res/cube.stl";

  /* testing converter from .obj to binary .stl */
  /* catch exceptions in the meantime if there are any */
  try {
    /* get FileConverter object */
    auto& fc = FileConverter::getInstance();

    /* set input and output converter types */
    fc.setInputFormat(InputType::INPUT_TYPE_OBJ);
    fc.setOutputFormat(OutputType::OUTPUT_TYPE_STL);

    /* read .obj file */
    fc.read(INPUT_FILE);

    /* check whether point is inside */
    glm::dvec3 point(1.9, 1.9, 1.0);
    if (fc.isPointInside(point)) {
      std::cout << std::fixed << std::setprecision(4) << "The point [" << point.x << ", " << point.y << ", " << point.z << "] is inside the polygon" << std::endl;
    } else {
      std::cout << std::fixed << std::setprecision(4) << "The point [" << point.x << ", " << point.y << ", " << point.z << "] is outside the polygon" << std::endl;
    }

    /* get the volume of the mesh */
    std::cout << std::fixed << std::setprecision(4) << "Volume of the mesh: " << fc.volume() << std::endl;
    /* get the surface of the mesh */
    std::cout << std::fixed << std::setprecision(4) << "Surface of the mesh: " << fc.surface() << std::endl;

    /* set scale */
    glm::dvec3 scale(2.0, 2.0, 2.0);
    fc.scale(scale);

    /* set rotation */
    glm::dvec3 rotate(-3.0, -3.0, -3.0);
    fc.rotate(rotate);

    /* set translation */
    glm::dvec3 translate(4.0, 4.0, 4.0);
    fc.translate(translate);

    /* write .stl file */
    fc.write(OUTPUT_FILE);

    /* check whether point is inside */
    point = { -4.0, 4.0, 6.66 };
    if (fc.isPointInside(point)) {
      std::cout << std::fixed << std::setprecision(4) << "The point [" << point.x << ", " << point.y << ", " << point.z << "] is inside the polygon" << std::endl;
    } else {
      std::cout << std::fixed << std::setprecision(4) << "The point [" << point.x << ", " << point.y << ", " << point.z << "] is outside the polygon" << std::endl;
    }

    /* get the volume of the mesh */
    std::cout << std::fixed << std::setprecision(4) << "Volume of the mesh: " << fc.volume() << std::endl;
    /* get the surface of the mesh */
    std::cout << std::fixed << std::setprecision(4) << "Surface of the mesh: " << fc.surface() << std::endl;

  /* error handling */
  } catch (const std::invalid_argument& ia) {
    std::cerr << "*** [ERR] Invalid argument: " << ia.what() << " ***" << std::endl;
  } catch (const std::runtime_error& re) {
    std::cerr << "*** [ERR] Runtime error: " << re.what() << " ***" << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "*** [ERR] Exception occurred: " << e.what() << " ***" << std::endl;
  } catch (...) {
    std::cerr << "*** [ERR] Unknown error occurred ***" << std::endl;
  }

  return 0;
}
