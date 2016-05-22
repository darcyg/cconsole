/// __attribute.hpp
#ifndef _CRF_CLI_ATTRIBUTE_HPP_
#define _CRF_CLI_ATTRIBUTE_HPP_
#include <boost/variant.hpp>
#include <unordered_map>
#include <string>

namespace crf { namespace cli {
class attribute {
public:
  using name_t  = std::string;
  using value_t = boost::variant<std::string, std::size_t>;
  using map_t   = std::unordered_map<name_t, value_t>; 

};
} // namespace cli
} // namespace crf

#endif // _CRF_CLI_ATTRIBUTE_HPP_

