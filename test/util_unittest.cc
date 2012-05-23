#include "lite.h"
#include "gtest/gtest.h"

TEST(Util, StringSqueeze) {
  EXPECT_EQ("hello",   lite::squeeze("hello"));
  EXPECT_EQ(" hello",  lite::squeeze(" hello"));
  EXPECT_EQ(" hello",  lite::squeeze("  hello"));
  EXPECT_EQ(" hello ", lite::squeeze("  hello "));
  EXPECT_EQ(" hello ", lite::squeeze("  hello  "));
  EXPECT_EQ(" hello ", lite::squeeze(" hello  "));
  EXPECT_EQ("hello ",  lite::squeeze("hello  "));

  EXPECT_EQ(" hello",  lite::squeeze("\thello"));
  EXPECT_EQ(" hello",  lite::squeeze("\t hello"));
  EXPECT_EQ(" hello ", lite::squeeze("\t hello\t"));
  EXPECT_EQ(" hello ", lite::squeeze("\t hello\t "));
  EXPECT_EQ(" hello ", lite::squeeze("\thello\t "));
  EXPECT_EQ("hello ",  lite::squeeze("hello\t "));

  EXPECT_EQ(" ", lite::squeeze("             "));
  EXPECT_EQ("0 1 2 3 4 5", lite::squeeze("0 1  2   3    4     5"));
}

