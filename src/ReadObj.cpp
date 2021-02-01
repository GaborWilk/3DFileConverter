#include "ReadObj.h"
#include "Utils.h"

namespace conv {

void ReadObj::read(const std::string& pathToFile, MeshData& data) {
  if (pathToFile.empty()) {
    throw std::invalid_argument(std::string("No path to the input file: ") + pathToFile);
  }

  std::ifstream file(pathToFile, std::ios::in);
  if (!file.is_open()) {
    throw std::runtime_error(std::string("Cannot open file for read: ") + pathToFile);
  }

  /* parsing file line by line */
  std::string input;
  while (std::getline(file, input)) {
    std::istringstream line(input);
    std::string lineType;
    line >> lineType;

    /*
     * geometric vertex
     * v x y z (w)
     */
    if (lineType == "v") {
      glm::dvec4 v{0.0, 0.0, 0.0, 1.0};
      line >> v.x >> v.y >> v.z >> v.w;
      data.geometricVertices.emplace_back(v);
    }

    /*
     * texture vertex
     * vt u v (w)
     */
    if (lineType == "vt") {
      glm::dvec3 vt{0.0, 0.0, 0.0};
      line >> vt.x >> vt.y >> vt.z;
      data.textureVertices.emplace_back(vt);
    }

    /*
     * vertex normal
     * vn i j k
     */
    if (lineType == "vn") {
      glm::dvec3 vn{0.0, 0.0, 0.0};
      line >> vn.x >> vn.y >> vn.z;
      data.vertexNormals.emplace_back(vn);
    }

    /*
     * face
     * f v
     * f v/vt
     * f v//vn
     * f v/vt/vn
     */
    if (lineType == "f") {
      std::string faceType;
      std::string tmp;
      while (line >> tmp) {
        faceType += (tmp + " ");
      }

      Face f;

      /* split face parameters among spaces */
      auto parameters = utils::split(faceType, ' ');
      for (auto p : parameters) {
        /* get the number of slashes */
        auto occurrences = utils::findPosition(p, '/');
        if (occurrences.empty()) {
          /* case is v1 v2 v3 ... */
          int v = std::stoi(p);

          /* handle negative indices */
          v = (v > 0) ? v : (v + data.geometricVertices.size() + 1);

          f.geometricVertexReferences.emplace_back(v);

          /* case is v1/vt1 v2/vt2 v3/vt3 ... */
        } else if (occurrences.size() == 1u) {
          auto refs = utils::split(p, '/');

          /* handle negative indices */
          int v = std::stoi(refs[0]);
          int vt = std::stoi(refs[1]);
          v = (v > 0) ? v : (v + data.geometricVertices.size() + 1);
          vt = (vt > 0) ? vt : (vt + data.textureVertices.size() + 1);

          f.geometricVertexReferences.emplace_back(v);
          f.textureVertexReferences.emplace_back(vt);
        } else {
          /* distance between the positions of the slashes */
          auto dist = occurrences[1] - occurrences[0];

          /* case is v1//vn1 v2//vn2 v3//vn3 ... */
          if (dist == 1u) {
            auto refs = utils::split(p, '/');

            /* handle negative indices */
            /* refs[1] equals nothing between // */
            int v = std::stoi(refs[0]);
            int vn = std::stoi(refs[2]);
            v = (v > 0) ? v : (v + data.geometricVertices.size() + 1);
            vn = (vn > 0) ? vn : (vn + data.vertexNormals.size() + 1);

            f.geometricVertexReferences.emplace_back(v);
            f.vertexNormalReferences.emplace_back(vn);

            /* case is v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ... */
          } else {
            auto refs = utils::split(p, '/');

            /* handle negative indices */
            int v = std::stoi(refs[0]);
            int vt = std::stoi(refs[1]);
            int vn = std::stoi(refs[2]);
            v = (v > 0) ? v : (v + data.geometricVertices.size() + 1);
            vt = (vt > 0) ? vt : (vt + data.textureVertices.size() + 1);
            vn = (vn > 0) ? vn : (vn + data.vertexNormals.size() + 1);

            f.geometricVertexReferences.emplace_back(v);
            f.textureVertexReferences.emplace_back(vt);
            f.vertexNormalReferences.emplace_back(vn);
          }
        }
      }

      data.faces.emplace_back(f);
    }
  }

  /* update triangles */
  data.updateTriangles();
}

} // namespace conv

