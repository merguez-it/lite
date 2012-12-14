#include "lite.h"
#include "gtest/gtest.h"

TEST(Token, SimpleVariable) {
  lite::data *root = new lite::map();
  root->set("key1", "value1");
  root->set("key2", "value2");

  lite::token_variable *var = new lite::token_variable("key1");
  ASSERT_EQ(lite::VARIABLE, var->type());
  ASSERT_EQ("value1", var->to_string(root));

  delete root;
  delete var;
}

TEST(Token, MapVariable) {
  lite::data *root = new lite::map();
  lite::data *map = new lite::map();
  map->set("sub1", "value1");
  map->set("sub2", "value2");
  root->set("key1", map);
  root->set("key2", "value3");

  lite::token *var = new lite::token_variable("key1.sub1");
  ASSERT_EQ(lite::VARIABLE, var->type());
  ASSERT_EQ("value1", var->to_string(root));

  delete root;
  delete map;
  delete var;
}

TEST(Token, ListVariable) {
  lite::data *root = new lite::map();
  lite::data *list = new lite::list();
  list->set("value1");
  list->set("value2");
  root->set("key1", list);
  root->set("key2", "value3");

  lite::token *var = new lite::token_variable("key1.0");
  ASSERT_EQ(lite::VARIABLE, var->type());
  ASSERT_EQ("value1", var->to_string(root));

  delete root;
  delete list;
  delete var;
}

TEST(Token, Text) {
  lite::token *text = new lite::token_text("Hello World");
  ASSERT_EQ(lite::TEXT, text->type());
  ASSERT_EQ("Hello World", text->to_string(NULL));

  delete text;
}

TEST(Token, None) {
  lite::token *none = new lite::token_none();
  ASSERT_EQ(lite::NONE, none->type());

  delete none;
}

TEST(Token, End) {
  lite::token *end = new lite::token_end();
  ASSERT_EQ(lite::END, end->type());

  delete end;
}

TEST(Token, If) {
  std::vector<std::string> if_clause;
  if_clause.push_back("if");
  if_clause.push_back("value");
  if_clause.push_back("==");
  if_clause.push_back("toto");
  
  std::vector<lite::token *> if_childs;
  lite::token *text = new lite::token_text("ok");
  if_childs.push_back(text);

  lite::token *t_if = new lite::token_if(if_clause);
  t_if->childs(if_childs);

  ASSERT_EQ(lite::IF, t_if->type());

  lite::data *data = new lite::map();
  data->set("value", "toto");
  data->set("other", "nothing");

  ASSERT_EQ("ok", t_if->to_string(data));

  delete data;
  delete t_if;
  delete text;
}

TEST(Token, ForList) {
  std::vector<std::string> for_clause;
  for_clause.push_back("for");
  for_clause.push_back("v");
  for_clause.push_back("in");
  for_clause.push_back("list");

  std::vector<lite::token *> for_childs;
  lite::token *var = new lite::token_variable("v");
  lite::token *lf = new lite::token_text("\n");
  for_childs.push_back(var);
  for_childs.push_back(lf);

  lite::token *t_for = new lite::token_for(for_clause);
  t_for->childs(for_childs);

  lite::data *root = new lite::map();
  lite::data *list = new lite::list();
  list->set("A");
  list->set("B");
  list->set("C");
  root->set("list", list);
  root->set("other", "nothing");

  ASSERT_EQ(lite::FOR, t_for->type());
  ASSERT_EQ("A\nB\nC\n", t_for->to_string(root));
  ASSERT_EQ(2, root->size());

  delete list;
  delete root;
  delete t_for;
  delete lf;
  delete var;
}

TEST(Token, ForMap) {
  std::vector<std::string> for_clause;
  for_clause.push_back("for");
  for_clause.push_back("k");
  for_clause.push_back("v");
  for_clause.push_back("in");
  for_clause.push_back("map");

  std::vector<lite::token *> for_childs;
  lite::token *k = new lite::token_variable("k");
  lite::token *eq = new lite::token_text("=");
  lite::token *v = new lite::token_variable("v");
  lite::token *lf = new lite::token_text("\n");
  for_childs.push_back(k);
  for_childs.push_back(eq);
  for_childs.push_back(v);
  for_childs.push_back(lf);

  lite::token *t_for = new lite::token_for(for_clause);
  t_for->childs(for_childs);

  lite::data *root = new lite::map();
  lite::data *map = new lite::map();
  map->set("A", "1");
  map->set("B", "2");
  map->set("C", "3");
  root->set("map", map);
  root->set("other", "nothing");

  ASSERT_EQ(lite::FOR, t_for->type());
  ASSERT_EQ("A=1\nB=2\nC=3\n", t_for->to_string(root));
  ASSERT_EQ(2, root->size());

  delete map;
  delete root;
  delete t_for;
  delete lf;
  delete v;
  delete eq;
  delete k;
}
