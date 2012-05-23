#include "lite.h"
#include "gtest/gtest.h"

TEST(Lite, Simple) {
  std::string tmpl = "Hello {% $who %}{% $sign %}\nHow are you ?";
  std::map<std::string, lite::data *> data;
  data["who"] = lite::make_data("world");
  data["sign"] = lite::make_data("!");

  std::string result = lite::render(tmpl, data);

  EXPECT_EQ("Hello world!\nHow are you ?", result);
}

