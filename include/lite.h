#include <vector>
#include <map>
#include <string>

#define LITE_VERSION "0.0.1"

namespace lite {
  enum token_type {
    NONE,
    VARIABLE,
    TEXT,
    FOR,
    IF,
    END
  };

  enum data_type {
    VALUE,
    LIST,
    MAP
  };

  class data {
    public:
      virtual data_type get_type() = 0;
      virtual std::string get();
      virtual data * get(int i);
      virtual data * get(std::string key);
      virtual size_t size() = 0;
  };

  class data_value : public data {
    public:
      data_value(std::string value);
      data_type get_type();
      std::string get();
      size_t size();

    private:
      std::string value_;
      data_type type_;
  };

  class data_list : public data {
    public:
      data_list(std::vector<data *> values);
      data_type get_type();
      data * get(int i);
      size_t size();

    private:
      std::vector<data *> values_;
      data_type type_;
  };

  class data_map : public data {
    public:
      data_map(std::map<std::string, data*> values);
      data_type get_type();
      data * get(std::string key);
      size_t size();

    private:
      std::map<std::string, data *> values_;
      data_type type_;
  };

  class token;
  class token {
    public:
      virtual ~token() {};
      virtual token_type get_type() = 0;
      virtual std::string get_text(std::map<std::string, data *> data) = 0;
      virtual data * get_data(std::map<std::string, data *> data);
      virtual void set_childs(std::vector<token *> t);
      virtual std::vector<token *> get_childs();

    protected:
      std::vector<token *> childs_;
  };

  class token_variable : public token {
    public:
      token_variable(std::string & data);

      token_type get_type();
      std::string get_text(std::map<std::string, data *> data);
      data * get_data(std::map<std::string, data *> data);

    private:
      std::string get_data_value(data * data, std::string key);
      data * get_data_r(data * data, std::string key);

    private:
      token_type type_;
      std::string data_;
  };

  class token_text : public token {
    public:
      token_text(std::string & data);

      token_type get_type();
      std::string get_text(std::map<std::string, data *> data);

    private:
      token_type type_;
      std::string data_;
  };

  class token_none : public token {
    public:
      token_none();

      token_type get_type();
      std::string get_text(std::map<std::string, data *> data);
      void set_childs(std::vector<token *> t);
      std::vector<token *> get_childs();

    private:
      token_type type_;
  };

  class token_end : public token {
    public:
      token_end();

      token_type get_type();
      std::string get_text(std::map<std::string, data *> data);
      void set_childs(std::vector<token *> t);
      std::vector<token *> get_childs();

    private:
      token_type type_;
  };

  class token_if : public token {
    public:
      token_if(std::vector<std::string> & data);

      token_type get_type();
      std::string get_text(std::map<std::string, data *> data);

    private:
      bool is_true(std::map<std::string, data *> data);

    private:
      token_type type_;
      std::vector<std::string> data_;
  };

  class token_for : public token { 
    public:
      token_for(std::vector<std::string> & data);

      token_type get_type();
      std::string get_text(std::map<std::string, data *> data);

    private:
      token_type type_;
      std::vector<std::string> data_;
  }; 

  std::string trim(std::string str); 
  std::string squeeze(const std::string & str);
  std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems); 
  std::vector<std::string> split(const std::string &s, char delim); 

  data * make_data(std::string value); 
  data * make_data(std::vector<data *> values); 
  data * make_data(std::map<std::string, data *> values); 

  std::vector<token *> tokenize(const std::string & tmpl_text);
  token * parse_token(const std::string & token_data);
  std::vector<token *> gen_tree(std::vector<token *> & tokens, token_type until = NONE); 

  std::string render(const std::string & tmpl_text, std::map<std::string, data *> data);
  std::string render_file(const std::string & filename, std::map<std::string, data *> data);
}

