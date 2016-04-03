/// __startswith.hpp
#pragma once
#include <cstring>
#include <string>

namespace crf {
inline bool startswith(std::string const& str, char const * const what) noexcept __attribute__((always_inline)) __attribute__((flatten));
} // namespace crf

inline bool crf::startswith(std::string const& str, char const * const what) noexcept {
  if( str.length() < std::strlen( what )) return false;

  auto const posn = str.find( what );
  return( posn == 0u );
}

