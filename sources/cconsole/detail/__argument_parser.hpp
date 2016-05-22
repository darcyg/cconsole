/// __argument_parser.hpp
#ifndef _CRF_ARGUMENT_PARSER_HPP_
#define _CRF_ARGUMENT_PARSER_HPP_
#include <boost/noncopyable.hpp>
#include <boost/variant.hpp>
#include <string>
#include <memory>

namespace crf { namespace cli {

using type_t = boost::variant<bool, std::string, std::size_t>;

template <typename T = bool>
inline type_t type() noexcept {
  return type_t{T{}};
}

//--------------------------------------------------
class argument_parser : private boost::noncopyable {
//--------------------------------------------------
public:
  static auto constexpr _default_description = "";
  struct __arg_adder;

  explicit argument_parser(char const* description = _default_description);
  ~argument_parser() = default;

  __arg_adder operator() () noexcept;
  //argument::map_t parse(int argc, char* argv[]) const;

private:
  struct __impl;
  std::unique_ptr<__impl> _impl_ptr;

};

auto constexpr argument_parser::_default_description;
} // namespace cli
} // namespace crf

#endif // _CRF_ARGUMENT_PARSER_HPP_

