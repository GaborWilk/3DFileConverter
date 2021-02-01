#ifndef FILE_CONVERTER_H
#define FILE_CONVERTER_H

#include "ReadObj.h"
#include "WriteStl.h"


namespace conv {

class FileConverter {
public:
  /* singleton lazy initialization (thread-safe) */
  static FileConverter& getInstance() {
    static FileConverter instance;
    return instance;
  }

  /* no need for constructors because of singleton */
  FileConverter(const FileConverter&) = delete;
  FileConverter& operator= (const FileConverter&) = delete;
  FileConverter& operator= (FileConverter&&) = delete;
  FileConverter(FileConverter&&) = delete;

  /* function to set input converter type */
  void setInputFormat(InputType input);

  /* function to set output converter type */
  void setOutputFormat(OutputType output);

  /* function to read 3D polygon data from file */
  void read(const std::string& pathToFile);

  /* function to write 3D polygon data to file */
  void write(const std::string& pathToFile);

  /* function to rotate the internally stored 3D polygon */
  void rotate(const glm::dvec3& rotate);

  /* function to scale the internally stored 3D polygon */
  void scale(const glm::dvec3& scale);

  /* function to translate the internally stored 3D polygon */
  void translate(const glm::dvec3& translate);

  /* function to check whether the given point is inside the 3D polygon */
  bool isPointInside(const glm::dvec3& point);

  /* function to calculate the volume of the 3D polygon */
  double volume() const;

  /* function to calculate the surface of the 3D polygon */
  double surface() const;

private:
  explicit FileConverter() {
    data_ = MeshData();
    data_.clear();
  }
  ~FileConverter() = default;

  /* function to summarize the operations and transform every vertex and normal with it */
  void transform();

  /* function to calculate the boundary points of the 3D polygon */
  void calculateBoundaryPoints();

  /* function to check whether the given point is outside of the 3D polygon */
  bool isPointOutsideOfBoundaries(const glm::dvec3& point);

  /* function to calculate the signed volume of a given tetrahedron (A,B,C,D) */
  double calculateSignedVolume(const glm::dvec3& pointA, const glm::dvec3& pointB,
                               const glm::dvec3& pointC, const glm::dvec3& pointD) const;

  /* function to check if there is an intersection between the point P and triangle ABC */
  bool hasIntersection(const glm::dvec3& pointP, const glm::dvec3& pointQ,
                       const glm::dvec3& pointA, const glm::dvec3& pointB, const glm::dvec3& pointC) const;

  /* function to calculate the area of a given triangle */
  double calculateAreaOfTriangle(const glm::dvec3& pointA, const glm::dvec3& pointB, const glm::dvec3& pointC) const;

  /* function to check whether the given point intersects an arbitrary triangle */
  bool intersectsTriangle(const glm::dvec3& probePoint, const glm::dvec3& pointA,
                          const glm::dvec3& pointB, const glm::dvec3& pointC) const;


  /* private variable to store file reader object */
  std::unique_ptr<Reader> reader_;

  /* private variable to store file writer object */
  std::unique_ptr<Writer> writer_;

  /* private variable to store 3D polygon information internally */
  MeshData data_;
};

} // namespace conv


#endif // FILE_CONVERTER_H
