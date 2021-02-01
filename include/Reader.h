#ifndef READER_H
#define READER_H

#include "Core.h"


namespace conv {

class Reader {
public:
  virtual ~Reader() = default;

  virtual void read(const std::string& pathToFile, MeshData& data) = 0;
};

} // namespace conv


#endif // READER_H
