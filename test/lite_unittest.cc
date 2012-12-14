#include "lite.h"
#include "config-test.h"
#include "gtest/gtest.h"

TEST(Lite, Simple) {
  std::string tmpl = "Hello {% $who %}{% $sign %}\nHow are you ?";

  lite::map * data = new lite::map();
  data->set("who", "world");
  data->set("sign", "!");

  lite::parser p = lite::parser::string(tmpl);
  std::string result = p.render(data);
  EXPECT_EQ("Hello world!\nHow are you ?", result);

  delete data;
}

TEST(Lite, IfAndIfNot) {
  std::string tmpl = "Hello {% if who %}{% $who %}{% end %}{% if not who %}World{% end %}{% $sign %}";

  lite::map *data = new lite::map();
  data->set("sign", "!");

  std::string result1 = lite::render(tmpl, data);
  EXPECT_STREQ("Hello World!", result1.c_str());

  data->set("who", "Lite");
  std::string result2 = lite::render(tmpl, data);
  EXPECT_STREQ("Hello Lite!", result2.c_str());

  delete data;
}
 
TEST(Lite, IfAndIfNotEqual) {
  std::string tmpl = "Hello {% if who == Lite %}Lite{% end %}{% if not who == Lite %}World{% end %}{% $sign %}";
  lite::data *data = new lite::map();
  data->set("sign", "!");
  data->set("who", "???");

  std::string result1 = lite::render(tmpl, data);
  EXPECT_STREQ("Hello World!", result1.c_str());

  data->set("who", "Lite");
  std::string result2 = lite::render(tmpl, data);
  EXPECT_STREQ("Hello Lite!", result2.c_str());

  delete data;
}

TEST(Lite, IfAndIfNotNotEqual) {
  std::string tmpl = "Hello {% if who != Lite %}World{% end %}{% if not who != Lite %}Lite{% end %}{% $sign %}";
  lite::data *data = new lite::map();
  data->set("sign", "!");
  data->set("who", "???");

  std::string result1 = lite::render(tmpl, data);
  EXPECT_STREQ("Hello World!", result1.c_str());

  data->set("who", "Lite");
  std::string result2 = lite::render(tmpl, data);
  EXPECT_STREQ("Hello Lite!", result2.c_str());

  delete data;
}

TEST(Lite, For) {
  std::string tmpl = "{% for val in count %}* {% $val %}\n{% end %}";

  lite::data *data = new lite::map();
  lite::data *list = new lite::list();
  list->set("one");
  list->set("two");
  list->set("three");

  data->set("count", list);

  std::string result = lite::render(tmpl, data);
  EXPECT_STREQ("* one\n* two\n* three\n", result.c_str());

  delete list;
  delete data;
}

TEST(Lite, RenderTemplateFile) {
  std::string tmpl(LITE_TESTS_PATH(lite.template));
  lite::data *data = new lite::map();
  data->set("who", "world");

  std::string result = lite::render_file(tmpl, data);

  EXPECT_EQ("Hello world!", result);

  delete data;
}

TEST(Lite, ForWithListOfMap) {
  lite::data *data = new lite::map();

  lite::data *record_list = new lite::list();

  lite::data *record_one = new lite::map();
  record_one->set("name", "record_one");
  record_one->set("primary_key", "record_one_pk");
  record_one->set("foreign_key", "record_one_fk");
  record_list->set(record_one);

  lite::data *record_two = new lite::map();
  record_two->set("name", "record_two");
  record_two->set("primary_key", "record_two_pk");
  record_two->set("foreign_key", "record_two_fk");
  record_list->set(record_two);

  data->set("records", record_list);

  std::string tmpl = "{% for rec in records %}{% $rec.name %}:\n  * {% $rec.primary_key %}\n  * {% $rec.foreign_key %}\n{% end %}"; 
  std::string result = lite::render(tmpl, data);

  EXPECT_EQ("record_one:\n  * record_one_pk\n  * record_one_fk\nrecord_two:\n  * record_two_pk\n  * record_two_fk\n", result);

  delete record_two;
  delete record_one;
  delete record_list;
  delete data;
}

TEST(Lite, ForWithMap) {
  lite::data *map = new lite::map();
  map->set("en", "Hello World!");
  map->set("fr", "Bonjour Monde!");
  map->set("es", "Hola Mundo!");
  map->set("it", "Cioa Tutti!");

  lite::data *root = new lite::map();
  root->set("hello", map);

  std::string tmpl = "{%for key value in hello%}* {% $key %} = {% $value %}\n{% end %}";

  std::string result = lite::render(tmpl, root);

  EXPECT_EQ("* en = Hello World!\n* fr = Bonjour Monde!\n* es = Hola Mundo!\n* it = Cioa Tutti!\n", result);

  delete root;
  delete map;
}
