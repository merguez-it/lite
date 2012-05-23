#include "lite.h"
#include "gtest/gtest.h"

TEST(Token, TokenVariableSimple) {
  std::string tv_key("a");
  std::string tv_value("A");
  std::map<std::string, lite::data *> data;
  data[tv_key] = lite::make_data(tv_value);

  lite::token * tv_a = new lite::token_variable(tv_key);
  EXPECT_EQ(lite::VARIABLE, tv_a->get_type());
  EXPECT_EQ(tv_value, tv_a->get_text(data));

  std::string tv_b_key("b");
  lite::token * tv_b = new lite::token_variable(tv_b_key);
  EXPECT_EQ(lite::VARIABLE, tv_b->get_type());
  EXPECT_EQ("", tv_b->get_text(data));
}

TEST(Token, TokenVariableMap) {
  // a.z = Z
  std::string tv_key("a.z");
  std::string tv_value("Z");
  std::map<std::string, lite::data *> data;
  std::map<std::string, lite::data *> values;
  values["z"] = lite::make_data("Z");
  data["a"] = lite::make_data(values);

  lite::token * tv_a = new lite::token_variable(tv_key);
  EXPECT_EQ(lite::VARIABLE, tv_a->get_type());
  EXPECT_EQ(tv_value, tv_a->get_text(data));

  std::string tv_b_key("b");
  lite::token * tv_b = new lite::token_variable(tv_b_key);
  EXPECT_EQ(lite::VARIABLE, tv_b->get_type());
  EXPECT_EQ("", tv_b->get_text(data));
}
