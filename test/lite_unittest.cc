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

TEST(Lite, IfAndIfNot) {
  std::string tmpl = "Hello {% if who %}{% $who %}{% end %}{% if not who %}World{% end %}{% $sign %}";
  std::map<std::string, lite::data *> data;
  data["sign"] = lite::make_data("!");

  std::string result1 = lite::render(tmpl, data);
  EXPECT_STREQ("Hello World!", result1.c_str());

  data["who"] = lite::make_data("Lite");
  std::string result2 = lite::render(tmpl, data);
  EXPECT_STREQ("Hello Lite!", result2.c_str());
}

TEST(Lite, IfAndIfNotEqual) {
  std::string tmpl = "Hello {% if who == Lite %}Lite{% end %}{% if not who == Lite %}World{% end %}{% $sign %}";
  std::map<std::string, lite::data *> data;
  data["sign"] = lite::make_data("!");
  data["who"] = lite::make_data("???");

  std::string result1 = lite::render(tmpl, data);
  EXPECT_STREQ("Hello World!", result1.c_str());

  data["who"] = lite::make_data("Lite");
  std::string result2 = lite::render(tmpl, data);
  EXPECT_STREQ("Hello Lite!", result2.c_str());
}

TEST(Lite, IfAndIfNotNotEqual) {
  std::string tmpl = "Hello {% if who != Lite %}World{% end %}{% if not who != Lite %}Lite{% end %}{% $sign %}";
  std::map<std::string, lite::data *> data;
  data["sign"] = lite::make_data("!");
  data["who"] = lite::make_data("???");

  std::string result1 = lite::render(tmpl, data);
  EXPECT_STREQ("Hello World!", result1.c_str());

  data["who"] = lite::make_data("Lite");
  std::string result2 = lite::render(tmpl, data);
  EXPECT_STREQ("Hello Lite!", result2.c_str());
}

TEST(Lite, For) {
  std::string tmpl = "{% for val in count %}* {% $val %}\n{% end %}";
  std::map<std::string, lite::data *> data;

  std::vector<lite::data *> list;
  list.push_back(lite::make_data("one"));
  list.push_back(lite::make_data("two"));
  list.push_back(lite::make_data("three"));

  data["count"] = lite::make_data(list);

  std::string result = lite::render(tmpl, data);
  EXPECT_STREQ("* one\n* two\n* three\n", result.c_str());
}
