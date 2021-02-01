#ifndef CORE_H
#define CORE_H

#include <glm.hpp>

#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <memory>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>


namespace conv {

/* enum class for read object types */
enum class InputType : uint8_t {
  INPUT_TYPE_OBJ = 0u,
  INPUT_TYPE_UNKNOWN = std::numeric_limits<uint8_t>::max()
};

/* enum class for write object types */
enum class OutputType : uint8_t {
  OUTPUT_TYPE_STL = 0u,
  OUTPUT_TYPE_UNKNOWN = std::numeric_limits<uint8_t>::max()
};

/* structure to store 'f' parameter */
struct Face {
  std::vector<uint32_t> geometricVertexReferences;
  std::vector<uint32_t> textureVertexReferences;
  std::vector<uint32_t> vertexNormalReferences;

  void clear() {
    geometricVertexReferences.clear();
    textureVertexReferences.clear();
    vertexNormalReferences.clear();
  }
};

/* structure to store a given triangle with its vertices and normal vector */
struct Triangle {
  std::array<glm::dvec3, 3> vertices;
  glm::dvec3 normal;
};

/* internal data structure to store information about the given mesh */
struct MeshData {
  MeshData() = default;
  ~MeshData() = default;

  /* clear internally stored data */
  void clear() {
    geometricVertices.clear();
    textureVertices.clear();
    vertexNormals.clear();
    faces.clear();
    triangles.clear();
    polygonBoundaries.clear();
    transformOperations.clear();
  }

  /* update triangles after transformations */
  void updateTriangles() {
    /* clear triangles if needed */
    if (!triangles.empty()) {
      triangles.clear();
    }

    /* triangulate faces (assuming n>3-gons are convex and coplanar) */
    for (auto& f : faces) {
      for (size_t i = 1u; (i + 1) < f.geometricVertexReferences.size(); ++i) {
        Triangle t;

        /* calculate triangle vertices */
        t.vertices[0] = geometricVertices[f.geometricVertexReferences[0] - 1];
        t.vertices[1] = geometricVertices[f.geometricVertexReferences[i] - 1];
        t.vertices[2] = geometricVertices[f.geometricVertexReferences[i + 1] - 1];

        /* calculate normal vector */
        glm::dvec3 crossProduct = glm::cross(t.vertices[1] - t.vertices[0], t.vertices[2] - t.vertices[0]);
        t.normal = glm::normalize(crossProduct);

        triangles.emplace_back(t);
      }
    }
  }

  /* parameter v */
  std::vector<glm::dvec4> geometricVertices;

  /* parameter vt */
  std::vector<glm::dvec3> textureVertices;

  /* parameter vn */
  std::vector<glm::dvec3> vertexNormals;

  /* parameter f */
  std::vector<Face> faces;

  /* storage for the triangles that make the surface of the polygon mesh */
  std::vector<Triangle> triangles;

  /* boundary points of the given polygon */
  std::vector<glm::dvec3> polygonBoundaries;

  /* storage for the arbitrary number of transformations */
  std::vector<glm::dmat4> transformOperations;
};

} // namespace conv


#endif // CORE_H
