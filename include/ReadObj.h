#ifndef READOBJ_H
#define READOBJ_H

#include "Reader.h"

namespace conv {

class ReadObj : public Reader {
public:
  ReadObj() = default;
  virtual ~ReadObj() = default;

  virtual void read(const std::string& pathToFile, MeshData& data);
};

} // namespace conv

#endif // READOBJ_H
