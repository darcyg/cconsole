/// tools.hpp
#include <cconsole/tools.hpp>
#include <gtest/gtest.h>


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

