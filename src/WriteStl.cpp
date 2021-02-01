#include "WriteStl.h"


namespace conv {

constexpr size_t HEADER_SIZE_IN_BYTES = 80u;

void WriteStl::write(const std::string& pathToFile, const MeshData& data) {
  if (pathToFile.empty()) {
    throw std::invalid_argument(std::string("No path to the output file: ") + pathToFile);
  }

  std::ofstream file(pathToFile, std::ios::out | std::ios::binary | std::ios::trunc);
  if (!file.is_open()) {
    throw std::runtime_error(std::string("Cannot open file for write: ") + pathToFile);
  }

  /* structure of the binary .stl format */
  /* UINT8[80] - Header */
  uint8_t header[HEADER_SIZE_IN_BYTES] = {0u};
  file.write(reinterpret_cast<char*>(header), sizeof(header));
  if (file.bad()) {
    throw std::runtime_error("Unable to write file at header");
  }

  /* UINT32 - Number of triangles (little endian) */
  uint32_t numOfTriangles = data.triangles.size();
  uint8_t byte[4];
  byte[0] = numOfTriangles & 0xFFu;
  byte[1] = (numOfTriangles >> 8u) & 0xFFu;
  byte[2] = (numOfTriangles >> 16u) & 0xFFu;
  byte[3] = (numOfTriangles >> 24u) & 0xFFu;
  file.write(reinterpret_cast<char*>(byte), sizeof(byte));
  if (file.bad()) {
    throw std::runtime_error("Unable to write file at number of triangles");
  }

  /* for each triangle */
  for (const auto& t : data.triangles) {
    /* REAL32[3] - Normal vector */
    glm::dvec3 normalVector = t.normal;
    file.write(reinterpret_cast<char*>(&normalVector), sizeof(normalVector));
    if (file.bad()) {
      throw std::runtime_error("Unable to write file at normal vector");
    }

    /* REAL32[3] - Vertex 1 */
    glm::dvec3 vertex1 = t.vertices[0];
    file.write(reinterpret_cast<char*>(&vertex1), sizeof(vertex1));
    if (file.bad()) {
      throw std::runtime_error("Unable to write file at vertex 1");
    }

    /* REAL32[3] - Vertex 2 */
    glm::dvec3 vertex2 = t.vertices[1];
    file.write(reinterpret_cast<char*>(&vertex2), sizeof(vertex2));
    if (file.bad()) {
      throw std::runtime_error("Unable to write file at vertex 2");
    }

    /* REAL32[3] - Vertex 3 */
    glm::dvec3 vertex3 = t.vertices[2];
    file.write(reinterpret_cast<char*>(&vertex3), sizeof(vertex3));
    if (file.bad()) {
      throw std::runtime_error("Unable to write file at vertex 3");
    }

    /* UINT16 - Attribute byte count */
    uint16_t attribute = 0u;
    file.write(reinterpret_cast<char*>(&attribute), sizeof(attribute));
    if (file.bad()) {
      throw std::runtime_error("Unable to write file at attribute byte count");
    }
  }
}

} // namespace conv
