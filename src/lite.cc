#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <stdexcept>
#include "lite.h"

#define TOKEN_OPEN "{%"
#define TOKEN_CLOSE "%}"
#define TOKEN_VARIABLE "$"
#define TOKEN_VARIABLE_SEPARATOR "."
#define TOKEN_FOR "for"
#define TOKEN_IF "if"
#define TOKEN_ENDIF "endif"
#define TOKEN_ENDFOR "endfor"

#define OP_EQUAL "=="
#define OP_NOTEQUAL "!="
#define OP_LESSTHAN "<"
#define OP_LESSOREQUAL "<="
#define OP_GREATERTHAN ">"
#define OP_GREATEREQUAL ">="

namespace lite {
  std::string data::get() {
    throw std::logic_error("Not a value");
  }

  data * data::get(int i) {
    throw std::logic_error("Not a list");
  }

  data * data::get(std::string key) {
    throw std::logic_error("Not a map");
  }

  data_value::data_value(std::string value) : value_(value), type_(VALUE) {}

  data_type data_value::get_type() {
    return type_;
  }

  std::string data_value::get() {
    return value_;
  }

  size_t data_value::size() {
    return value_.size();
  }

  data_list::data_list(std::vector<data *> values) : values_(values), type_(LIST) {}

  data_type data_list::get_type() {
    return type_;
  }

  data * data_list::get(int i) {
    if((size_t)i < values_.size()) {
      return values_[i];
    } 

    return NULL;
  }

  size_t data_list::size() {
    return values_.size();
  }

  data_map::data_map(std::map<std::string, data *> values) : values_(values), type_(MAP) {}

  data_type data_map::get_type() {
    return type_;
  }

  data * data_map::get(std::string key) {
    return values_[key];
  }

  size_t data_map::size() {
    return values_.size();
  }

  // ...

  void token::set_childs(std::vector<token *> t) {
    childs_.clear();
    std::copy(t.begin(), t.end(), std::back_inserter(childs_));
  }

  std::vector<token *> token::get_childs() {
    return childs_;
  }

  data * token::get_data(std::map<std::string, data *> data) {
    throw std::logic_error("No data for this this token");
  }

  token_variable::token_variable(std::string & data) : 
    type_(VARIABLE), data_(data) {};

  token_type token_variable::get_type() {
    return type_;
  }

  std::string token_variable::get_text(std::map<std::string, data *> data) {
    return get_data_value(make_data(data), data_);
  }

  data * token_variable::get_data(std::map<std::string, data *> data) {
    return get_data_r(make_data(data), data_);
  }

  std::string token_variable::get_data_value(data * datas, std::string key) {
    if(datas->get_type() != MAP) {
      return "";
    }

    size_t dot_pos = key.find_first_of(TOKEN_VARIABLE_SEPARATOR);
    if(dot_pos == std::string::npos) {
      if(datas->get(key) != NULL && datas->get(key)->get_type() == VALUE) {
        return datas->get(key)->get();
      } else {
        return "";
      }
    } else {
      std::string pre = key.substr(0, dot_pos);
      std::string post = key.substr(dot_pos + 1);
      if(datas->get(pre) != NULL && datas->get(pre)->get_type() == MAP) {
        return get_data_value(datas->get(pre), post);
      } else {
        return "";
      }
    }

    return "";
  }

  data * token_variable::get_data_r(data * datas, std::string key) {
    if(datas->get_type() != MAP) {
      return NULL;
    }

    size_t dot_pos = key.find_first_of(TOKEN_VARIABLE_SEPARATOR);
    if(dot_pos == std::string::npos) {
      if(datas->get(key) != NULL) {
        return datas->get(key);
      } else {
        return NULL;
      }
    } else {
      std::string pre = key.substr(0, dot_pos);
      std::string post = key.substr(dot_pos + 1);
      if(datas->get(pre) != NULL && datas->get(pre)->get_type() == MAP) {
        return get_data_r(datas->get(pre), post);
      } else {
        return NULL;
      }
    }

    return NULL;
  }

  token_text::token_text(std::string & data) : 
    type_(TEXT), data_(data) {};

  token_type token_text::get_type() {
    return type_;
  }

  std::string token_text::get_text(std::map<std::string, data *> data) {
    return data_;
  }

  token_none::token_none() : type_(NONE) {};

  token_type token_none::get_type() {
    return type_;
  }

  std::string token_none::get_text(std::map<std::string, data *> data) {
    throw std::logic_error("Malformated template (unexpected token NONE)");
  }

  void token_none::set_childs(std::vector<token *> t) {
    throw std::logic_error("This token can't have child");
  }

  std::vector<token *> token_none::get_childs() {
    throw std::logic_error("This token can't have child");
  }
  
  token_end::token_end() : type_(END) {};

  token_type token_end::get_type() {
    return type_;
  }

  std::string token_end::get_text(std::map<std::string, data *> data) {
    throw std::logic_error("Malformated template (unexpected token END)");
  }

  void token_end::set_childs(std::vector<token *> t) {
    throw std::logic_error("This token can't have child");
  }

  std::vector<token *> token_end::get_childs() {
    throw std::logic_error("This token can't have child");
  }

  token_if::token_if(std::vector<std::string> & data) : 
    type_(IF), data_(data) {}
  
  token_type token_if::get_type() {
    return type_;
  }

  std::string token_if::get_text(std::map<std::string, data *> data) {
    std::string result;

    if(is_true(data)) {
      std::vector<token *>::iterator it;
      std::vector<token *> childs = get_childs();
      for(it = childs.begin(); it < childs.end(); it++) {
        result.append((*it)->get_text(data));
      }
    }

    return result;
  }

  bool token_if::is_true(std::map<std::string, data *> data) {
    bool result = false;
    bool negative = false;
    size_t var_pos = 1;
    if(data_[1] == "not") {
      negative = true;
      var_pos = 2;
    }

    std::string var_name = data_[var_pos];

    token * t = new token_variable(var_name);
    if(NULL != t->get_data(data)) {
      std::string var_value = t->get_text(data);
      if(data_.size() > var_pos + 1) {
        std::string op = data_[var_pos + 1];
        std::string value = data_[var_pos + 2];
        if(op == OP_EQUAL) {
          result = (var_value == value);
        } else if(op == OP_NOTEQUAL) {
          result = (var_value != value);
        } else if(op == OP_LESSTHAN) {
          result = (var_value < value);
        } else if(op == OP_LESSOREQUAL) {
          result = (var_value <= value);
        } else if(op == OP_GREATERTHAN) {
          result = (var_value > value);
        } else if(op == OP_GREATEREQUAL) {
          result = (var_value >= value);
        } else {
          result = false;
        }
      } else {
        result = true;
      }
    }

    delete t;
    
    if(negative) {
      return !result;
    }
    return result;
  }

  token_for::token_for(std::vector<std::string> & data) : 
    type_(FOR), data_(data) {}
  
  token_type token_for::get_type() {
    return type_;
  }

  std::string token_for::get_text(std::map<std::string, data *> data) {
    std::string result;
    std::map<std::string, lite::data *> data_copy(data);
    std::string var_name = data_[1];
    std::string in_name = data_[3];

    token * t = new token_variable(in_name);
    lite::data * v = t->get_data(data);
    if(NULL == v || LIST != v->get_type()) {
      return "";
    }

    for(size_t i = 0; i < v->size(); i++) {
      data_copy[var_name] = v->get(i);

      std::vector<token *>::iterator it;
      std::vector<token *> childs = get_childs();
      for(it = childs.begin(); it < childs.end(); it++) {
        result.append((*it)->get_text(data_copy));
      }
    }

    return result;
  }

  // ...

  std::string trim(std::string str) {
    std::string::size_type pos = str.find_last_not_of(' ');
    if(pos != std::string::npos) {
      str.erase(pos + 1);
      pos = str.find_first_not_of(' ');
      if(pos != std::string::npos) str.erase(0, pos);
    } else {
      str.erase(str.begin(), str.end());
    }
    return str;
  }

  std::string squeeze(const std::string & str) {
    std::string result;
    bool keep = true;
    for(size_t i = 0; i < str.size(); i++) {
      char c = str[i];
      if(c == ' ' || c == '\t') {
        if(keep) {
          keep = false;
          result.append(1, ' ');
        } 
      } else {
        keep = true;
        result.append(1, c);
      }
    }

    return result;
  }

  std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
      elems.push_back(trim(item));
    }   
    return elems;
  }   

  std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    return split(s, delim, elems);
  }  

  // ...

  data * make_data(std::string value) {
    return(new data_value(value));
  }

  data * make_data(std::vector<data *> values) {
    return(new data_list(values));
  }

  data * make_data(std::map<std::string, data *> values) {
    return(new data_map(values));
  }

  // ...

  std::vector<token *> tokenize(const std::string & tmpl_text) {
    std::vector<token *> tokens;
    size_t last_pos = 0;

    while(true) {
      // Find TOKEN_OPEN
      size_t open_pos = tmpl_text.find(TOKEN_OPEN, last_pos);
      if(open_pos == std::string::npos) {
        std::string end_text = tmpl_text.substr(last_pos);
        if(end_text.size() > 0) {
          tokens.push_back(new token_text(end_text));
        }
        break;
      }

      std::string pre_text = tmpl_text.substr(last_pos, open_pos - last_pos);
      if(pre_text.size() > 0) {
        tokens.push_back(new token_text(pre_text));
      }

      // Find TOKEN_CLOSE
      open_pos = open_pos + 2;
      size_t close_pos = tmpl_text.find(TOKEN_CLOSE, open_pos);
      if(close_pos == std::string::npos) {
        throw std::logic_error("Malformated template (missing %})");
        break;
      }

      // Get token data
      std::string token_data = trim(squeeze(tmpl_text.substr(open_pos, close_pos - open_pos)));
      tokens.push_back(parse_token(token_data));

      last_pos = close_pos + 2;
    }

    tokens.push_back(new token_none());

    return tokens;
  }

  token * parse_token(const std::string & token_data) {
    token * t = NULL;

    if(token_data[0] == '$') {
      std::string token_var_name = token_data.substr(1);
      t = new token_variable(token_var_name);
    } else {
      if(token_data == "end") {
        t = new token_end();
      } else {
        std::vector<std::string> data = split(token_data, ' ');
        if(data[0] == "if") {
          t = new token_if(data);
        } else if(data[0] == "for") {
          t = new token_for(data); 
        } else {
          throw std::logic_error("Malformated template (invalid token " + data[0] + ")");
        }
      }
    }

    return t;
  }

  std::vector<token *> gen_tree(std::vector<token *> & tokens, token_type until) {
    std::vector<token *> tree;

    while(!tokens.empty()) {
      token * current_token = tokens[0];
      tokens.erase(tokens.begin());

      if(current_token->get_type() == FOR) {
        current_token->set_childs(gen_tree(tokens, END));
      } else if(current_token->get_type() == IF) {
        current_token->set_childs(gen_tree(tokens, END));
      } else if(current_token->get_type() == until) {
        break;
      }

      tree.push_back(current_token);
    }

    return tree;
  }

  std::string render(const std::string & tmpl_text, std::map<std::string, data *> data) {
    std::string result;
    std::vector<token *> tokens = tokenize(tmpl_text);
    std::vector<token *> tree = gen_tree(tokens);

    std::vector<token *>::iterator it;
    for(it = tree.begin(); it < tree.end(); it++) {
      result.append((*it)->get_text(data));
    }

    for(it = tree.begin(); it < tree.end(); it++) {
      delete (*it);
    }

    /*
    for(it = tokens.begin(); it < tokens.end(); it++) {
      delete (*it);
    }
    */

    return result;
  }

  std::string render_file(const std::string & filename, std::map<std::string, data *> data) {
    std::string line;
    std::string tmpl_data;

    std::ifstream myfile(filename.c_str());
    if (myfile.is_open()) {
      while (myfile.good()) {
        getline (myfile,line);
        tmpl_data.append(line + "\n");
      }
      myfile.close();
    } else {
      throw std::runtime_error("Can't open file " + filename);
    }

    return render(tmpl_data, data);
  }
}

