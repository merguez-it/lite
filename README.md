# lite

LIghtweight Template Engine for C++ inspired by [cpptemplate](https://bitbucket.org/ginstrom/cpptemplate)

So, why ? - Because I want something simple Boost and Qt free.

*This is a Work In Progress.* So test it but don't use it.

## Dependencies

### Compiling

* [cmake](http://www.cmake.org/)

### Documentation

* [doxygen](http://doxygen.org/) (optional)
* [graphviz](http://graphviz.org/) (optional)

### Testing 

* [gtest](http://code.google.com/p/googletest/) 

## Compiling

Install [cmake](http://www.cmake.org/cmake/help/install.html), [gtest](http://code.google.com/p/googletest/), [doxygen](http://www.stack.nl/~dimitri/doxygen/install.html) and [graphviz](http://www.graphviz.org/Download.php), then :

### On MacOSX, or Linux

    cd <path to lite root>
    mkdir build
    cd build
    cmake ..
    make
    make test
    make doc

### On Windows

You need to install [MinGW](http://www.mingw.org/wiki/InstallationHOWTOforMinGW). Add the bin directory of your MinGW installation in your PATH. Then, open a Windows (not MSYS) console and :

    cd <path to lite root>
    mkdir build
    cd build
    cmake -G "MinGW Makefiles" ..
    mingw32-make
    mingw32-make test
    mingw32-make doc

If you want to use Visual Studio... Try it ;)

## Usage

### Simple

```cpp
std::string tmpl = "Hello {% $who %}{% $sign %}\nHow are you ?";

std::map<std::string, lite::data *> data;
data["who"] = lite::make_data("world");
data["sign"] = lite::make_data("!");

std::string result = lite::render(tmpl, data);
```

### IF

```cpp
std::string tmpl = "Hello " 
  "{% if name %}{% $name %}{% end %}" 
  "{% if not name %}world{% end %}";

std::map<std::string, lite::data *> data;
data["person"] = lite::make_data("you");

std::string result = lite::render(tmpl, data);
```

You can use 

    $var 
    $var == value
    $var != value
    $var < value
    $var > value
    $var <= value
    $var >= value

Or its negation 

    not $var
    not $var == value
    not $var != value
    not $var < value
    not $var > value
    not $var <= value
    not $var >= value

### FOR

(*WIP*)

```cpp
std::string tmpl = "{% for number in sequence %}"
  "* {% $number %}\n"
  "{% end %}"; 

std::vector<lite::data *> list;
list.push_back(lite::make_data("1"));
list.push_back(lite::make_data("2"));
list.push_back(lite::make_data("3"));

std::map<std::string, lite::data *> data;
data["sequence"] = lite::make_data(list);

std::string result = lite::render(tmpl, data);
```

