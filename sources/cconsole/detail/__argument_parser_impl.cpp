/// __argument_parser_impl.cpp
#include <cconsole/detail/__argument_parser.hpp>
#include <unordered_map>
#include <tuple>

using crf::cli::argument_parser;

struct argument_parser::__impl {
public:
  using argument_descriptor_t = std::tuple<
     std::string       ///< help message
    ,crf::cli::type_t  ///< type of the argument value
    ,bool              ///< is the argument required
    ,bool>;            ///< is the argument positional

  static argument_descriptor_t make_descriptor(std::string help, crf::cli::type_t type = type_t{false}, bool required = false, bool positional = false);

  explicit __impl(std::string description);
  //produces the usage message
  std::string to_string() const noexcept;

private:
  std::string _description;
  ///arg.name -> arg.descriptor
  ///name is either --${name} for a flag or ${NAME} for a positional
  ///argument
  std::unordered_map<std::string, argument_descriptor_t> _args;
};

argument_parser::__impl::argument_descriptor_t 
argument_parser::__impl::make_descriptor(std::string help, crf::cli::type_t type, bool required, bool positional) {
  return std::make_tuple(help, type, required, positional);
}

argument_parser::__impl::__impl(std::string description)
: _description{description}
{
  _args.insert("--help", make_descriptor("prints this help message"));
}

