/// __find_first_line.hpp
#pragma once
#include <istream>
#include <string>

namespace crf {
template <typename UnaryPredicate>
std::string first_line_of(std::istream & is, UnaryPredicate && found) __attribute__((flatten));

inline std::string first_line_of(std::istream & is) noexcept __attribute__((always_inline));
} // namespace crf

template <typename UnaryPredicate>
std::string crf::first_line_of(std::istream & is, UnaryPredicate && found) {
  auto s = std::string{};
  while (std::getline( is, s ) && !found( s ));
  // if (! found( s )) throw .. ??
  return s;
}

inline std::string crf::first_line_of(std::istream & is) noexcept {
  return first_line_of( is, [] (std::string const&) noexcept { return true; });
}

