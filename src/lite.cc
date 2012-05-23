#include <iostream>
#include <sstream>
#include <stdexcept>
#include "lite.h"

#define TOKEN_OPEN "{%"
#define TOKEN_CLOSE "%}"
#define TOKEN_VARIABLE "$"
#define TOKEN_FOR "for"
#define TOKEN_IF "if"
#define TOKEN_ENDIF "endif"
#define TOKEN_ENDFOR "endfor"

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

  token_variable::token_variable(std::string & data) : 
    type_(VARIABLE), data_(data) {};

  token_type token_variable::get_type() {
    return type_;
  }

  std::string token_variable::get_text(std::map<std::string, data *> data) {
    //return data[data_]->get();
    return get_data_value(make_data(data), data_);
  }

  std::string token_variable::get_data_value(data * datas, std::string key) {
    if(datas->get_type() != MAP) {
      return "";
    }

    size_t dot_pos = key.find_first_of(".");
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
    throw std::logic_error("This token can't have value");
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
    throw std::logic_error("This token can't have value");
  }

  void token_end::set_childs(std::vector<token *> t) {
    throw std::logic_error("This token can't have child");
  }

  std::vector<token *> token_end::get_childs() {
    throw std::logic_error("This token can't have child");
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
        // TODO
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

    return result;
  }
}

/*
int main(void) {
  std::string tmpl = "Hello {% $who %}{% $sign %}\nHow are you ?";
  std::map<std::string, lite::data *> data;
  data["who"] = lite::make_data("world");
  data["sign"] = lite::make_data("!");

  std::vector<lite::data *> list;
  list.push_back(lite::make_data("1"));
  list.push_back(lite::make_data("2"));
  list.push_back(lite::make_data("3"));

  data["list"] = lite::make_data(list);

  std::string result = lite::render(tmpl, data);
  std::cout << "----------------------------" << std::endl;
  std::cout << "[" << result << "]" << std::endl;
}
*/

