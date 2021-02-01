#ifndef WRITESTL_H
#define WRITESTL_H

#include "Writer.h"


namespace conv {

class WriteStl : public Writer {
public:
  WriteStl() = default;
  virtual ~WriteStl() = default;

  virtual void write(const std::string& pathToFile, const MeshData& data);
};

} // namespace conv


#endif // WRITESTL_H
