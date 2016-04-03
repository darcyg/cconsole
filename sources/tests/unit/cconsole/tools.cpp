/// tools.hpp
#include <cconsole/tools.hpp>
#include <boost/format.hpp>
#include <gtest/gtest.h>
#include <sstream>

TEST(cconsole_tools, startswith) {
  EXPECT_PRED2( crf::startswith, "", "" );
  EXPECT_PRED2( crf::startswith, " ", "" );
  EXPECT_PRED2( crf::startswith, " ", " " );
  EXPECT_PRED2( crf::startswith, "abcd", "a" );
  EXPECT_PRED2( crf::startswith, "abcd", "ab" );
  EXPECT_PRED2( crf::startswith, "abcd", "abc" );
  EXPECT_PRED2( crf::startswith, "abcd", "abcd" );

  EXPECT_FALSE(crf::startswith( "abcd", " a" ));
  EXPECT_FALSE(crf::startswith( "abcd", " ab" ));
  EXPECT_FALSE(crf::startswith( "abcd", "b" ));
  EXPECT_FALSE(crf::startswith( "abcd", "__" ));
}

TEST(cconsole_tools, fist_line_of) {
  auto const first_line  = "cpu  5 4 3 2 0 1";
  auto const second_line = "cpu1 3 2 1 0 0 0 ";
  auto const third_line  = "cpu2 5 4 3 2 1 0";
  auto const fourth_line = "";
  {
     auto const text = ( boost::format{"%1%\n%2%\n%3%\n%4%\n"} % first_line % second_line % third_line % fourth_line ).str();
     std::stringstream ss{text};

     auto const actual_line = crf::first_line_of(ss);
     auto const expected_line = first_line;
     EXPECT_EQ( actual_line, expected_line );
  }
  {
     auto const text = ( boost::format{"%1%\n%2%\n%3%\n%4%\n"} % first_line % second_line % third_line % fourth_line ).str();
     std::stringstream ss{text};
     auto const actual_line = crf::first_line_of( ss, [] (std::string const& line) noexcept { return crf::startswith( line, "cpu1 " ); });
     auto const expected_line = second_line;
     EXPECT_EQ( actual_line, expected_line );
  }
  {
     auto const text = ( boost::format{"%1%\n%2%\n%3%\n%4%\n"} % first_line % second_line % third_line % fourth_line ).str();
     std::stringstream ss{text};
     auto const actual_line = crf::first_line_of( ss, [] (std::string const& line) noexcept { return crf::startswith( line, "cpu10 " ); });
     EXPECT_TRUE(actual_line.empty( ));
  }
}

