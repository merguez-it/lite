#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <stdexcept>
#include "lite.h"

lite::parser lite::parser::file(const std::string & path) {
  std::string line;
  std::string tmpl_data;

  std::ifstream myfile(path.c_str());
  if (myfile.is_open()) {
    while (myfile.good()) {
      getline (myfile,line);
      tmpl_data.append(line);
    }
    myfile.close();
  } else {
    throw std::runtime_error("Can't open file " + path);
  }

  return parser(tmpl_data);
}
lite::parser lite::parser::string(const std::string & str) {
  return parser(str);
}
lite::parser::parser(const std::string & str) : template_(str) {
  tokenize();
};
lite::parser::~parser() {
  std::vector<token *>::iterator it;
  for(it = tree_.begin(); it < tree_.end(); it++) {
    delete (*it);
  }
}

std::string lite::parser::render(lite::data *data) {
  std::string result;

  std::vector<token *>::iterator it;
  for(it = tree_.begin(); it < tree_.end(); it++) {
    result.append((*it)->to_string(data));
  }

  return result;
}

void lite::parser::tokenize() {
  size_t last_pos = 0;

  while(true) {
    // Find TOKEN_OPEN
    size_t open_pos = template_.find(TOKEN_OPEN, last_pos);
    if(open_pos == std::string::npos) {
      std::string end_text = template_.substr(last_pos);
      if(end_text.size() > 0) {
        tokens_.push_back(new token_text(end_text));
      }
      break;
    }

    std::string pre_text = template_.substr(last_pos, open_pos - last_pos);
    if(pre_text.size() > 0) {
      tokens_.push_back(new token_text(pre_text));
    }

    // Find TOKEN_CLOSE
    open_pos = open_pos + 2;
    size_t close_pos = template_.find(TOKEN_CLOSE, open_pos);
    if(close_pos == std::string::npos) {
      throw std::logic_error("Malformated template (missing %})");
      break;
    }

    // Get token data
    std::string token_data = trim(squeeze(template_.substr(open_pos, close_pos - open_pos)));
    tokens_.push_back(parse_token(token_data));

    last_pos = close_pos + 2;
  }

  tokens_.push_back(new token_none());
  tree_ = gen_tree();
}

std::vector<lite::token *> lite::parser::gen_tree(lite::token_type until) {
  std::vector<token *> tree_tmp_;
  while(!tokens_.empty()) {
    token * current_token = tokens_[0];
    tokens_.erase(tokens_.begin());
    if(current_token->type() == FOR) {
      current_token->childs(gen_tree(END));
    } else if(current_token->type() == IF) {
      current_token->childs(gen_tree(END));
    } else if(current_token->type() == until) {
      delete current_token;
      break;
    }

    tree_tmp_.push_back(current_token);
  }
  return tree_tmp_;
}

lite::token * lite::parser::parse_token(const std::string & token_data) {
  token * t = NULL;

  if(token_data[0] == TOKEN_VARIABLE) {
    std::string token_var_name = token_data.substr(1);
    t = new token_variable(token_var_name);
  } else if(token_data == TOKEN_END) {
    t = new token_end();
  } else {
    std::vector<std::string> data = split(token_data, ' ');
    if(data[0] == TOKEN_IF) {
      t = new token_if(data);
    } else if(data[0] == TOKEN_FOR) {
      t = new token_for(data);
    } else {
      throw std::logic_error("Malformated template (invalid token " + data[0] + ")");
    }
  }

  return t;
}

std::string lite::parser::trim(std::string str) {
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
std::string lite::parser::squeeze(const std::string & str) {
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
std::vector<std::string>& lite::parser::split(const std::string &s, char delim, std::vector<std::string> &elems) {
  std::stringstream ss(s);
  std::string item;
  while(std::getline(ss, item, delim)) {
    elems.push_back(trim(item));
  }
  return elems;
}
std::vector<std::string> lite::parser::split(const std::string &s, char delim) {
  std::vector<std::string> elems;
  return split(s, delim, elems);
}

std::string lite::render(const std::string & tmpl, lite::data * data) {
  lite::parser p = lite::parser::string(tmpl);
  return p.render(data);
}
std::string lite::render_file(const std::string & path, lite::data * data) {
  lite::parser p = lite::parser::file(path);
  return p.render(data);
}
