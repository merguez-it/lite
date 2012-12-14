#include <string>
#include <stdexcept>
#include "lite.h"

std::string lite::data::str() const {
  throw std::logic_error("Not a value");
}
lite::string * lite::data::operator=(const std::string & str) {
  throw std::logic_error("Not a value");
}
lite::string * lite::data::operator=(const char *str) {
  throw std::logic_error("Not a value");
}
lite::string * lite::data::operator=(data * str) {
  throw std::logic_error("Not a value");
}
lite::data * lite::data::operator[](int i) {
  throw std::logic_error("Not a list");
}
lite::data * lite::data::at(int i) {
  throw std::logic_error("Not a list");
}
void lite::data::operator<<(data * d) {
  throw std::logic_error("Not a list");
}
void lite::data::operator<<(const std::string & str) {
  throw std::logic_error("Not a list");
}
void lite::data::operator<<(const char *str) {
  throw std::logic_error("Not a list");
}
void lite::data::set(lite::data *d) {
  throw std::logic_error("Not a list");
}
void lite::data::set(const std::string & str) {
  throw std::logic_error("Not a list");
}
void lite::data::set(const char *str) {
  throw std::logic_error("Not a list");
}
std::vector<std::string> lite::data::keys() const {
  throw std::logic_error("Not a map");
}
lite::data * lite::data::operator[](std::string str) {
  throw std::logic_error("Not a map");
}
lite::data * lite::data::at(std::string str) {
  throw std::logic_error("Not a map");
}
void lite::data::set(std::string key, lite::data* value) {
  throw std::logic_error("Not a map");
}
void lite::data::set(std::string key, std::string value) {
  throw std::logic_error("Not a map");
}

lite::data_type lite::string::type() const {
  return type_;
}
size_t lite::string::size() const {
  return data_.size();
}
std::string lite::string::str() const {
  return data_;
}
lite::string * lite::string::operator=(const std::string & str) {
  data_ = str;
  return this;
}
lite::string * lite::string::operator=(const char *str) {
  data_ = std::string(str);
  return this;
}
lite::string * lite::string::operator=(data * str) {
  if(lite::STRING == str->type()) {
    data_ = str->str();
  } else {
    throw std::logic_error("Not a string");
  }
  return this;
}

lite::list::~list() {
  std::vector<data*>::iterator it;
  for(it = data_c_.begin(); it != data_c_.end(); it++) {
    delete *it;
  }
}
lite::data_type lite::list::type() const {
  return type_;
}
size_t lite::list::size() const {
  return data_.size();
}
lite::data * lite::list::operator[](int i) {
  if(i > data_.size()) {
    throw std::out_of_range("Out of range");
  }
  return data_[i];
}
lite::data * lite::list::at(int i) {
  return (*this)[i];
}
void lite::list::operator<<(data * d) {
  data_.push_back(d);
}
void lite::list::operator<<(const std::string & str) {
  lite::data * _c = new lite::string(str);
  data_c_.push_back(_c);
  *this << _c;
}
void lite::list::operator<<(const char *str) {
  lite::data * _c = new lite::string(str);
  data_c_.push_back(_c);
  *this << _c;
}
void lite::list::set(lite::data *d) {
  *this << d;
}
void lite::list::set(const std::string & str) {
  *this << str;
}
void lite::list::set(const char *str) {
  *this << str;
}

lite::map::~map() {
  std::vector<data*>::iterator it;
  for(it = data_c_.begin(); it != data_c_.end(); it++) {
    delete *it;
  }
}
lite::data_type lite::map::type() const {
  return type_;
}
size_t lite::map::size() const {
  return data_.size();
}
std::vector<std::string> lite::map::keys() const {
  return keys_;
}
lite::data * lite::map::operator[](std::string str) {
  return data_[str];
}
lite::data * lite::map::at(std::string str) {
  return (*this)[str];
}
void lite::map::set(std::string key, lite::data* value) {
  if(std::find(keys_.begin(), keys_.end(), key) == keys_.end()) {
    keys_.push_back(key);
  }
  data_[key] = value;
}
void lite::map::set(std::string key, std::string value) {
  if(std::find(keys_.begin(), keys_.end(), key) == keys_.end()) {
    keys_.push_back(key);
  } else {
    lite::data *_d = data_[key];
    std::vector<data*>::iterator pos = std::find(data_c_.begin(), data_c_.end(), _d);
    if(pos == data_c_.end()) {
      throw std::out_of_range("Can't find data in map");
    }
    data_c_.erase(pos);
    delete _d;
  }
  lite::data * _c = new lite::string(value);
  data_c_.push_back(_c);
  data_[key] = _c;
}
