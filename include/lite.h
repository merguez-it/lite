#include <vector>
#include <map>
#include <string>

#define LITE_VERSION "0.0.2"

#define TOKEN_OPEN "{%"
#define TOKEN_CLOSE "%}"
#define TOKEN_VARIABLE '$'
#define TOKEN_VARIABLE_SEPARATOR "."
#define TOKEN_FOR "for"
#define TOKEN_FOR_IN "in"
#define TOKEN_IF "if"
#define TOKEN_END "end"

#define OP_NOT "not"
#define OP_EQUAL "=="
#define OP_NOTEQUAL "!="
#define OP_LESSTHAN "<"
#define OP_LESSOREQUAL "<="
#define OP_GREATERTHAN ">"
#define OP_GREATEREQUAL ">="

namespace lite {
  enum token_type {
    NONE,     // 0
    VARIABLE, // 1
    TEXT,     // 2
    FOR,      // 3
    IF,       // 4
    END       // 5
  };

  enum data_type {
    STRING,
    LIST,
    MAP
  };

  class string;
  class list;
  class map;
  class data { 
    public:
      virtual ~data() {}
      virtual data_type type() const = 0;
      virtual size_t size() const = 0;

      // string 
      virtual std::string str() const;
      virtual string * operator=(const std::string & str);
      virtual string * operator=(const char *str);
      virtual string * operator=(data * str);

      // list
      virtual data * operator[](int i);
      virtual data * at(int i);
      virtual void operator<<(const std::string & str);
      virtual void operator<<(const char *str);
      virtual void operator<<(data * d);
      virtual void set(data *d);
      virtual void set(const std::string & str);
      virtual void set(const char *str);

      // map
      virtual std::vector<std::string> keys() const;
      virtual data * operator[](std::string str);
      virtual data * at(std::string str);
      virtual void set(std::string key, data* value);
      virtual void set(std::string key, std::string value);
  };

  class string : public data {
    public:
      string() : data_(), type_(STRING) {};
      string(const std::string & str) : data_(str), type_(STRING) {};
      string(const char *str) : data_(str), type_(STRING) {};

      data_type type() const;
      size_t size() const;

      std::string str() const;
      string * operator=(const std::string & str);
      string * operator=(const char *str);
      string * operator=(data * str);

    private:
      std::string data_;
      data_type type_;
  };

  class list : public data {
    public:
      ~list();
      list() : type_(LIST) {}

      data_type type() const;
      size_t size() const;

      data * operator[](int i);
      data * at(int i);
      void operator<<(data * d);
      void operator<<(const std::string & str);
      void operator<<(const char *str);
      void set(data *d);
      void set(const std::string & str);
      void set(const char *str);

    private:
      std::vector<data*> data_;
      std::vector<data*> data_c_;
      data_type type_;
  };

  class map : public data {
    public:
      ~map();
      map() : type_(MAP) {}

      data_type type() const;
      size_t size() const;

      std::vector<std::string> keys() const;
      data * operator[](std::string str);
      data * at(std::string str);
      void set(std::string key, data* value);
      void set(std::string key, std::string value);

    private:
      std::map<std::string, data*> data_;
      std::vector<data*> data_c_;
      std::vector<std::string> keys_;
      data_type type_;
  };

  class token;
  class token {
    public:
      virtual ~token() {}
      virtual token_type type() const = 0;
      virtual std::string to_string(lite::data *data) const = 0;
      virtual lite::data * to_data(lite::data *data);
      virtual void childs(std::vector<token *> childs);
      virtual std::vector<token *> childs();
    
    protected:
      std::vector<token *> childs_;
  };

  class token_variable : public token {
    public:
      token_variable(const std::string & name);

      token_type type() const;
      std::string to_string(lite::data *data) const;
      data * to_data(lite::data *data);

    private:
      lite::data * get_value(lite::data *data, const std::string & key) const;

    private:
      std::string name_;
      token_type type_;
  };

  class token_text : public token {
    public:
      token_text(const std::string & text);

      token_type type() const;
      std::string to_string(lite::data *data) const;

    private:
      std::string text_;
      token_type type_;
  };

  class token_none : public token {
    public:
      token_none();

      token_type type() const;
      std::string to_string(lite::data *data) const;
      void childs(std::vector<token *> childs);
      std::vector<token *> childs();

    private:
      token_type type_;
  };

  class token_if : public token {
    public:
      token_if(const std::vector<std::string> & data);

      token_type type() const;
      std::string to_string(lite::data *data) const;

    private:
      bool is_true(lite::data *data) const;

    private:
      std::vector<std::string> data_;
      token_type type_;
  };

  class token_for : public token {
    public:
      token_for(const std::vector<std::string> & data);

      token_type type() const;
      std::string to_string(lite::data *data) const;

    private:
      std::string to_string_from_list(lite::data *data, std::string to, std::string in) const;
      std::string to_string_from_map(lite::data *data, std::string key, std::string value, std::string in) const;

    private:
      std::vector<std::string> data_;
      token_type type_;
  };

  class token_end : public token {
    public:
      token_end();

      token_type type() const;
      std::string to_string(lite::data *data) const;
      void childs(std::vector<token *> childs);
      std::vector<token *> childs();

    private:
      token_type type_;
  };

  class parser {
    public:
      static parser file(const std::string & path);
      static parser string(const std::string & str);
      ~parser();

      std::string render(lite::data *data);

    private:
      parser(const std::string & str);
      void tokenize();
      std::vector<token *> gen_tree(token_type until = NONE);
      token * parse_token(const std::string & token_data);

      std::string trim(std::string str);
      std::string squeeze(const std::string & str);
      std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);
      std::vector<std::string> split(const std::string &s, char delim);

    private:
      std::string template_;
      std::vector<token *> tree_;
      std::vector<token *> tokens_;
  };

  std::string render(const std::string & tmpl, lite::data * data);
  std::string render_file(const std::string & path, lite::data * data);
}

