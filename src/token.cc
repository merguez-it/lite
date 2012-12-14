#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <stdexcept>
#include "lite.h"

void lite::token::childs(std::vector<lite::token *> childs) {
  childs_.clear();
  std::copy(childs.begin(), childs.end(), std::back_inserter(childs_));
}
std::vector<lite::token *> lite::token::childs() {
  return childs_;
}
lite::data * lite::token::to_data(lite::data *data) {
  throw std::logic_error("No data for this this token");
}

lite::token_variable::token_variable(const std::string & name) : name_(name), type_(VARIABLE) {}
lite::token_type lite::token_variable::type() const {
  return type_;
}
std::string lite::token_variable::to_string(lite::data *data) const {
  lite::data * _d = get_value(data, name_);
  return (NULL != _d)?_d->str():""; 
}
lite::data * lite::token_variable::to_data(lite::data *data) {
  lite::data *_d = get_value(data, name_);
  return _d;
}
#define IS_DIGIT(X) ((X).find_first_not_of("0123456789") == std::string::npos)
lite::data * lite::token_variable::get_value(lite::data *data, const std::string & key) const {
  lite::data *value;

  size_t dot_pos = key.find_first_of(TOKEN_VARIABLE_SEPARATOR);
  if(dot_pos == std::string::npos) {
    if(IS_DIGIT(key)) {
      int index = atoi(key.c_str());
      return (lite::LIST == data->type() && data->size() > index && NULL != data->at(index))?data->at(index):NULL;
    } else {
      return (lite::MAP == data->type() && NULL != data->at(key))?data->at(key):NULL;
    }
  } else {
    std::string pre = key.substr(0, dot_pos);
    std::string post = key.substr(dot_pos + 1);
    if(IS_DIGIT(pre)) {
      int index = atoi(pre.c_str());
      if(lite::LIST != data->type() || data->size() < index+1) {
        return NULL;
      }
      value = data->at(index);
    } else {
      if(lite::MAP != data->type()) {
        return NULL;
      }
      value = data->at(pre);
    }
    return get_value(value, post);
  }
}

lite::token_text::token_text(const std::string & text) : text_(text), type_(TEXT) {}
lite::token_type lite::token_text::type() const {
  return type_;
}
std::string lite::token_text::to_string(lite::data *data) const {
  return std::string(text_); 
}

lite::token_none::token_none() : type_(NONE) {}
lite::token_type lite::token_none::type() const {
  return type_;
}
std::string lite::token_none::to_string(lite::data *data) const {
  throw std::logic_error("Malformated template (unexpected token NONE)");
}
void lite::token_none::childs(std::vector<lite::token *> childs) {
  throw std::logic_error("This token can't have child");
}
std::vector<lite::token *> lite::token_none::childs() {
  throw std::logic_error("This token can't have child");
}

lite::token_end::token_end() : type_(END) {}
lite::token_type lite::token_end::type() const {
  return type_;
}
std::string lite::token_end::to_string(lite::data *data) const {
  throw std::logic_error("Malformated template (unexpected token END)");
}
void lite::token_end::childs(std::vector<lite::token *> childs) {
  throw std::logic_error("This token can't have child");
}
std::vector<lite::token *> lite::token_end::childs() {
  throw std::logic_error("This token can't have child");
}

lite::token_if::token_if(const std::vector<std::string> & data) : data_(data), type_(IF) {}
lite::token_type lite::token_if::type() const {
  return type_;
}
std::string lite::token_if::to_string(lite::data *data) const {
  std::string result;

  if(is_true(data)) {
    std::vector<lite::token *>::iterator it;
    std::vector<lite::token *> childs = childs_;
    for(it = childs.begin(); it < childs.end(); it++) {
      result.append((*it)->to_string(data));
    }
  }

  return result;
}
bool lite::token_if::is_true(lite::data *data) const {
  bool result = false;
  bool negative = false;
  size_t var_pos = 1;
  if(OP_NOT == data_[1]) {
    negative = true;
    var_pos = 2;
  }
  std::string var_name = data_[var_pos];
  lite::token *_t = new lite::token_variable(var_name);
  lite::data *_d = _t->to_data(data);
  if(NULL != _d) {
    std::string var_value = _d->str();
    if(data_.size() > var_pos + 1) {
      std::string _op = data_[var_pos+1];
      std::string _value = data_[var_pos+2];
      if(OP_EQUAL == _op) {
        result = (_value == var_value);
      } else if(OP_NOTEQUAL == _op) {
        result = (_value != var_value);
      } else if(OP_LESSTHAN == _op) {
        result = (_value > var_value);
      } else if(OP_LESSOREQUAL == _op) {
        result = (_value >= var_value);
      } else if(OP_GREATERTHAN == _op) {
        result = (_value < var_value);
      } else if(OP_GREATEREQUAL == _op) {
        result = (_value <= var_value);
      } else {
        throw std::logic_error("Malformated template (unexpected operator)");
      }
    } else {
      result = true;
    }
  }

  delete _t;
  if(negative) {
    return !result;
  }
  return result;
}

lite::token_for::token_for(const std::vector<std::string> & data) : data_(data), type_(FOR) {}
lite::token_type lite::token_for::type() const {
  return type_;
}
std::string lite::token_for::to_string(lite::data *data) const {
  std::string result;
  lite::data *data_copy = new lite::map((const lite::map&)*data);
  if(TOKEN_FOR_IN == data_[2]) {
    result = to_string_from_list(data_copy, data_[1], data_[3]);
  } else if(TOKEN_FOR_IN == data_[3]) {
    result = to_string_from_map(data_copy, data_[1], data_[2], data_[4]);
  } else {
    throw std::logic_error("Malformated template");
  }
  return result;
}
std::string lite::token_for::to_string_from_list(lite::data *data, std::string to, std::string in) const {
  std::string result;

  lite::token *_t = new lite::token_variable(in);
  lite::data *_d = _t->to_data(data);
  if(NULL == _d || lite::LIST != _d->type()) {
    return "";
  }

  for(size_t i = 0; i < _d->size(); i++) {
    data->set(to, _d->at(i));
    std::vector<lite::token *>::iterator it;
    std::vector<lite::token *> childs = childs_;
    for(it = childs.begin(); it < childs.end(); it++) {
      result.append((*it)->to_string(data));
    }
  }

  return result;
}
std::string lite::token_for::to_string_from_map(lite::data *data, std::string key, std::string value, std::string in) const {
  std::string result;

  lite::token *_t = new lite::token_variable(in);
  lite::data *_d = _t->to_data(data);
  if(NULL == _d || lite::MAP != _d->type()) {
    return "";
  }

  std::vector<std::string> _k = _d->keys();
  std::vector<std::string>::iterator it;
  for(it = _k.begin(); it < _k.end(); it++) {
    data->set(key, (*it));
    data->set(value, _d->at(*it));
    std::vector<lite::token *>::iterator it;
    std::vector<lite::token *> childs = childs_;
    for(it = childs.begin(); it < childs.end(); it++) {
      result.append((*it)->to_string(data));
    }
  }

  return result;
}
