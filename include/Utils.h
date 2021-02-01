#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>


namespace utils {

/*
 * function to check whether the numeric value is signed
 * returns -1 if it is negative, 1 otherwise
 */
template <typename T>
inline int8_t getSignValue(T value) {
  if (value < T(0)) {
    return -1;
  }

  return 1;
}

/*
 * function to split string using a given delimiter
 */
inline std::vector<std::string> split(const std::string& s, const char delimiter) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(s);

  while (std::getline(tokenStream, token, delimiter)) {
    tokens.emplace_back(token);
  }

  return tokens;
}

/*
 * function to find the position(s) of a given character in a string
 */
inline std::vector<size_t> findPosition(const std::string& str, const char toFind) {
  std::vector<size_t> charPositions;

  for (size_t i = 0u; i < str.size(); ++i) {
    if (toFind == str[i]) {
      charPositions.emplace_back(i);
    }
  }

  return charPositions;
}

} // namespace utils


#endif // UTILS_H
