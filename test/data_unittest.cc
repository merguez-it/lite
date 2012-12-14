#include "lite.h"
#include "gtest/gtest.h"

TEST(Data, String) {
  lite::data *var = new lite::string("Hello World!");

  ASSERT_EQ(lite::STRING, var->type());
  ASSERT_EQ("Hello World!", var->str());
  ASSERT_EQ(12, var->size());

  (*var) = "Hola Mundo!";

  ASSERT_EQ(lite::STRING, var->type());
  ASSERT_EQ("Hola Mundo!", var->str());
  ASSERT_EQ(11, var->size());

  delete var;
}

TEST(Data, List) {
  lite::data *list = new lite::list();

  lite::data *hw = new lite::string("Hello World!");
  lite::data *hm = new lite::string("Hola Mundo!");

  list->set(hw);
  (*list) << hm;
  list->set("Bonjour Monde!");
  (*list) << "Ciao Tutti!";

  ASSERT_EQ(lite::LIST, list->type());
  ASSERT_EQ(4, list->size());
  ASSERT_EQ("Hello World!", list->at(0)->str());
  ASSERT_EQ("Hola Mundo!", (*list)[1]->str());
  ASSERT_EQ("Bonjour Monde!", list->at(2)->str());
  ASSERT_EQ("Ciao Tutti!", (*list)[3]->str());

  delete hm;
  delete hw;
  delete list;
}

TEST(Data, Map) {
  lite::data *map = new lite::map();

  lite::data *hw = new lite::string("Hello World!");
  map->set("en", hw);
  map->set("fr", "Bonjour Monde!");

  ASSERT_EQ(lite::MAP, map->type());
  ASSERT_EQ(2, map->size());
  ASSERT_EQ("Hello World!", map->at("en")->str());
  ASSERT_EQ("Bonjour Monde!", (*map)["fr"]->str());

  delete hw;
  delete map;
}
