#include "FileConverter.h"
#include "Utils.h"

namespace conv {

constexpr double COORD_VALUE_MIN = std::numeric_limits<double>::min();
constexpr double COORD_VALUE_MAX = std::numeric_limits<double>::max();
constexpr double COORD_OFFSET_VALUE = 10.0;


void FileConverter::transform() {
  /* nothing to do if there were no transformations */
  if (data_.transformOperations.empty()) {
    return;
  }

  /* construct identity matrix */
  glm::dmat4 transformMatrix(1.0);

  /*
   * summarize the operations
   * multiply the matrices first and then the vertices is faster,
   * than multiply each matrices with the vertices
   */
  for (const auto& op : data_.transformOperations) {
    transformMatrix *= op;
  }

  /*
   * transform vertices
   * doing perspective projection: after carrying out the matrix multiplication,
   * the component 'w' will be equal to the value of 'z' and the other three will not change
   * therefore, to map back into the real plane we must perform perspective divide by
   * dividing each component by 'w'
   */
  for (auto& v : data_.geometricVertices) {
    glm::dvec4 helper(v.x, v.y, v.z, 1.0);
    glm::dvec4 result = helper * transformMatrix;
    v.x = (result.x / result.w);
    v.y = (result.y / result.w);
    v.z = (result.z / result.w);
  }

  /*
   * transform vertex normals
   * multiply vertex normals by the transpose of the inverse of the transformation matrix:
   * N' = N ∗ M−1T
   */
  transformMatrix = glm::transpose(glm::inverse(transformMatrix));
  for (auto& n : data_.vertexNormals) {
    glm::dvec4 helper(n.x, n.y, n.z, 1.0);
    glm::dvec4 result = helper * transformMatrix;
    n.x = (result.x / result.w);
    n.y = (result.y / result.w);
    n.z = (result.z / result.w);
  }
}

void FileConverter::calculateBoundaryPoints() {
  glm::dvec3 minCoords(COORD_VALUE_MAX, COORD_VALUE_MAX, COORD_VALUE_MAX);
  glm::dvec3 maxCoords(COORD_VALUE_MIN, COORD_VALUE_MIN, COORD_VALUE_MIN);

  /* set minimum and maximum coordinates based on the vertices */
  for (const auto& v : data_.geometricVertices) {
    minCoords.x = (minCoords.x > v.x) ? v.x : minCoords.x;
    minCoords.y = (minCoords.y > v.y) ? v.y : minCoords.y;
    minCoords.z = (minCoords.z > v.z) ? v.z : minCoords.z;

    maxCoords.x = (maxCoords.x < v.x) ? v.x : maxCoords.x;
    maxCoords.y = (maxCoords.y < v.y) ? v.y : maxCoords.y;
    maxCoords.z = (maxCoords.z < v.z) ? v.z : maxCoords.z;
  }

  data_.polygonBoundaries.emplace_back(minCoords);
  data_.polygonBoundaries.emplace_back(maxCoords);
}

bool FileConverter::isPointOutsideOfBoundaries(const glm::dvec3& point) {
  /* calculate boundary points of the polygon */
  calculateBoundaryPoints();  

  /* point is outside if one of its coordinates is outside or equal to the min or max coordinates */
  if (point.x <= data_.polygonBoundaries[0].x || point.x >= data_.polygonBoundaries[1].x ||
      point.y <= data_.polygonBoundaries[0].y || point.y >= data_.polygonBoundaries[1].y ||
      point.z <= data_.polygonBoundaries[0].z || point.z >= data_.polygonBoundaries[1].z) {
    return true;
  }

  return false;
}

double FileConverter::calculateSignedVolume(const glm::dvec3& pointA, const glm::dvec3& pointB,
                                            const glm::dvec3& pointC, const glm::dvec3& pointD) const {
   /*
   * Let SignedVolume(a,b,c,d) denote the signed volume of the tetrahedron a, b, c and d.
   * It can be calculated as follows: SignedVolume(a,b,c,d) = (1.0/6.0)* dot(cross(b-a, c-a), d-a)
   */
  glm::dvec3 crossProduct = glm::cross(pointB - pointA, pointC - pointA);

  return (1.0 / 6.0) * glm::dot(crossProduct, pointD - pointA);
}

bool FileConverter::hasIntersection(const glm::dvec3& pointP, const glm::dvec3& pointQ,
                                    const glm::dvec3& pointA, const glm::dvec3& pointB,
                                    const glm::dvec3& pointC) const {
  /*
   * Let Pa, Pb and Pc denote a given triangle.
   * Pick two points P and Q on the line very far away in both directions.
   * Let SignedVolume(a,b,c,d) denote the signed volume of the tetrahedron a, b, c and d.
   *
   * If SignedVolume(q1, p1, p2, p3) and SignedVolume(q2, p1, p2, p3) have different signs AND
   * SignedVolume(q1, q2, p1, p2), SignedVolume(q1, q2, p2, p3) and SignedVolume(q1, q2, p3, p1)
   * have the same sign, then there is an intersection.
   */

  double signedVolumePABC = calculateSignedVolume(pointP, pointA, pointB, pointC);
  double signedVolumeQABC = calculateSignedVolume(pointQ, pointA, pointB, pointC);
  double signedVolumePQAB = calculateSignedVolume(pointP, pointQ, pointA, pointB);
  double signedVolumePQBC = calculateSignedVolume(pointP, pointQ, pointB, pointC);
  double signedVolumePQCA = calculateSignedVolume(pointP, pointQ, pointC, pointA);

  int8_t signOfPABC = utils::getSignValue(signedVolumePABC);
  int8_t signOfQABC = utils::getSignValue(signedVolumeQABC);
  int8_t signOfPQAB = utils::getSignValue(signedVolumePQAB);
  int8_t signOfPQBC = utils::getSignValue(signedVolumePQBC);
  int8_t signOfPQCA = utils::getSignValue(signedVolumePQCA);

  return ((signOfPABC != signOfQABC) && (signOfPQAB == signOfPQBC && signOfPQAB == signOfPQCA));
}

double FileConverter::calculateAreaOfTriangle(const glm::dvec3& pointA, const glm::dvec3& pointB, const glm::dvec3& pointC) const {
  double area = 0.0;

  /*
   * Calculating the area of a triangle step-by-step:
   * 1) calculate cross porduct of the two direction vectors (Pb - Pa, Pc - Pa)
   * 2) calculate the magnitude of the result vector (= area of a parallelogram)
   * 3) half the magnitude (= area of a triangle)
   */

  glm::dvec3 crossProduct = glm::cross(pointB - pointA, pointC - pointA);
  area = 0.5f * sqrtf((crossProduct.x * crossProduct.x) +
                      (crossProduct.y * crossProduct.y) +
                      (crossProduct.z * crossProduct.z));

  return area;
}

bool FileConverter::intersectsTriangle(const glm::dvec3& probePoint, const glm::dvec3& pointA,
                                       const glm::dvec3& pointB, const glm::dvec3& pointC) const {
  /*
   * The probe point intersects the triangle if the points are coplanar.
   * They are coplanar if and only if one of the areas of the four triangles has an area equal to the sum
   * of the other three areas (e.g. P = PA + PB + PC).
   */

  /* calculate area of (probePoint, A, B) triangle */
  double areaOfPAB = calculateAreaOfTriangle(probePoint, pointA, pointB);

  /* calculate area of (probePoint, A, C) triangle */
  double areaOfPAC = calculateAreaOfTriangle(probePoint, pointA, pointC);

  /* calculate area of (probePoint, B, C) triangle */
  double areaOfPBC = calculateAreaOfTriangle(probePoint, pointB, pointC);

  /* calculate area of the original triangle */
  double areaOfABC = calculateAreaOfTriangle(pointA, pointB, pointC);

  return areaOfABC == (areaOfPAB + areaOfPAC + areaOfPBC);
}

void FileConverter::setInputFormat(InputType input) {
  /* set proper read object type */
  switch (input) {
  case InputType::INPUT_TYPE_OBJ:
    reader_ = std::make_unique<ReadObj>();
    break;
  default:
    /* Not supported reading converter type */
    throw std::runtime_error("Not supported read object type");
  }
}

void FileConverter::setOutputFormat(OutputType output) {
  /* set proper write object type */
  switch (output) {
  case OutputType::OUTPUT_TYPE_STL:
    writer_ = std::make_unique<WriteStl>();
    break;
  default:
    /* Not supported writing converter type */
    throw std::runtime_error("Not supported write object type");
  }
}

void FileConverter::read(const std::string& pathToFile) {
  /* clear data structure */
  data_.clear();

  /* read file and store data internally */
  reader_->read(pathToFile, data_);
}

void FileConverter::write(const std::string& pathToFile) {
  /* write internally stored data into file */
  writer_->write(pathToFile, data_);
}

void FileConverter::rotate(const glm::dvec3& rotate) {
  /* set rotation by axis X */
  if (0.0 != rotate.x) {
    glm::dmat4 rotateMatrix = { {1,       0,             0,        0},
                                {0, cos(rotate.x), -sin(rotate.x), 0},
                                {0, sin(rotate.x), cos(rotate.x),  0},
                                {0,       0,             0,        1} };

    data_.transformOperations.emplace_back(rotateMatrix);
  }

  /* set rotation by axis Y */
  if (0.0 != rotate.y) {
    glm::dmat4 rotateMatrix = { {cos(rotate.y),   0, sin(rotate.y),  0},
                                {      0,         1,       0,        0},
                                {-sin(rotate.y),  0, cos(rotate.y),  0},
                                {      0,         0,       0,        1} };

    data_.transformOperations.emplace_back(rotateMatrix);
  }

  /* set rotation by axis Z */
  if (0.0 != rotate.z) {
    glm::dmat4 rotateMatrix = { {cos(rotate.z),	-sin(rotate.z),	 0, 0},
                                {sin(rotate.z),	 cos(rotate.z),  0, 0},
                                {      0,                0,      1, 0},
                                {      0,                0,      0, 1} };

    data_.transformOperations.emplace_back(rotateMatrix);
  }

  /* transform vertices and normals */
  transform();

  /* update triangles */
  data_.updateTriangles();
}

void FileConverter::scale(const glm::dvec3& scale) {
  glm::dmat4 scaleMatrix = { {scale.x,     0,        0,    0},
                             {  0,      scale.y,     0,    0},
                             {	0,         0,     scale.z, 0},
                             {	0,         0,        0,    1} };

  data_.transformOperations.emplace_back(scaleMatrix);

  /* transform vertices and normals */
  transform();

  /* update triangles */
  data_.updateTriangles();
}

void FileConverter::translate(const glm::dvec3& translate) {
  glm::dmat4 translateMatrix = { {1, 0, 0, translate.x},
                                 {0, 1, 0, translate.y},
                                 {0, 0, 1, translate.z},
                                 {0, 0, 0,      1     } };

  data_.transformOperations.emplace_back(translateMatrix);

  /* transform vertices and normals */
  transform();

  /* update triangles */
  data_.updateTriangles();
}

bool FileConverter::isPointInside(const glm::dvec3& point) {
  /* check whether the point is outside the boundary box */
  if (isPointOutsideOfBoundaries(point)) {
    return false;
  }

  /* get point outside of the boundary box called infinity */
  glm::dvec3 infinityPoint(data_.polygonBoundaries[1].x + COORD_OFFSET_VALUE,
                           data_.polygonBoundaries[1].y + COORD_OFFSET_VALUE,
                           data_.polygonBoundaries[1].z + COORD_OFFSET_VALUE);

  std::vector<glm::dvec3> intersectionPoints;

  /* use ray casting algoritm to determine whether the point is inside */
  for (const auto& triangle : data_.triangles) {
    glm::dvec3 vertex1 = triangle.vertices[0];
    glm::dvec3 vertex2 = triangle.vertices[1];
    glm::dvec3 vertex3 = triangle.vertices[2];

    /* check whether there is an intersection */
    if (!hasIntersection(point, infinityPoint, vertex1, vertex2, vertex3)) {
      continue;
    }

    /* get normal vector */
    glm::dvec3 normalVector = glm::cross(vertex2 - vertex1, vertex3 - vertex1);

    /*
     * equation of the line is: p(t) = point + t * (infinityPoint - point)
     * equation of the plane is: dot(point - vertex1, normalVector) = 0, where
     * normalVector = cross(vertex2 - vertex1, vertex3 - vertex1)
     * t = -dot(point - vertex1, normalVector) / dot(infinityPoint - point, normalVector)
     * and if 0 < t < 1, then
     * intersectionPoint = point + t * (infinityPoint - point)
     */
    double dotProductPN = glm::dot(point - vertex1, normalVector);
    double dotProductIN = glm::dot(infinityPoint - point, normalVector);
    double t = -(dotProductPN / dotProductIN);
    /* precondition check (0 < t < 1 must be fulfilled) */
    if (t <= 0.0 || t >= 1.0) {
      continue;
    }

    /* calculate intersection point */
    glm::dvec3 intersectionPoint = point + t * (infinityPoint - point);

    /* check if the point is on the plane of the triangle */
    if (intersectsTriangle(intersectionPoint, vertex1, vertex2, vertex3)) {
      /* check if the point has not been found before */
      if (std::find(intersectionPoints.begin(), intersectionPoints.end(), intersectionPoint) == intersectionPoints.end()) {
        intersectionPoints.emplace_back(intersectionPoint);
      }
    }
  }

  /* the point is inside if the number of intersections is odd */
  return (intersectionPoints.size() & 1u);
}

double FileConverter::volume() const {
  double volume = 0.0;

  /* calculate the signed volume of a given tetrahedron based on a given triangle and topped off at the origin */
  for (const auto& t : data_.triangles) {
    volume += calculateSignedVolume(glm::dvec3 {0.0, 0.0, 0.0}, t.vertices[0], t.vertices[1], t.vertices[2]);
  }

  return std::fabs(volume);
}

double FileConverter::surface() const {
  double surface = 0.0;

  /* surface = area of all of the triangles that make up the polygon mesh */
  for (const auto& t : data_.triangles) {
    surface += calculateAreaOfTriangle(t.vertices[0], t.vertices[1], t.vertices[2]);
  }

  return surface;
}

} // namespace conv
