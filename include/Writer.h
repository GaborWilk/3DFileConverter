#ifndef WRITER_H
#define WRITER_H

#include "Core.h"


namespace conv {

class Writer {
public:
  virtual ~Writer() = default;

  virtual void write(const std::string& pathToFile, const MeshData& data) = 0;
};

} // namespace conv


#endif // WRITER_H
