#include "lite.h"
#include "gtest/gtest.h"

TEST(Data, DataValue) {
  std::string value("Hello World");
  lite::data * dv = lite::make_data(value);

  EXPECT_EQ(11, dv->size());
  EXPECT_EQ(value, dv->get());
}

TEST(Data, DataList) {
  std::vector<lite::data *> list;
  list.push_back(lite::make_data("1"));
  list.push_back(lite::make_data("2"));
  list.push_back(lite::make_data("3"));
  lite::data * lv = lite::make_data(list); 

  EXPECT_EQ(3, lv->size());
  EXPECT_EQ("1", lv->get(0)->get());
  EXPECT_EQ("2", lv->get(1)->get());
  EXPECT_EQ("3", lv->get(2)->get());
  EXPECT_TRUE(NULL == lv->get(3));
}

TEST(Data, DataMap) {
  std::map<std::string, lite::data *> map;
  map["A"] = lite::make_data("1");
  map["B"] = lite::make_data("2");
  map["C"] = lite::make_data("3");
  lite::data * mv = lite::make_data(map);

  EXPECT_EQ(3, mv->size());
  EXPECT_EQ("1", mv->get("A")->get());
  EXPECT_EQ("2", mv->get("B")->get());
  EXPECT_EQ("3", mv->get("C")->get());
  EXPECT_TRUE(NULL == mv->get("D"));
}

