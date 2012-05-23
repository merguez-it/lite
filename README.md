# lite

LIghtweight Template Engine for C++ inspired by [cpptemplate](https://bitbucket.org/ginstrom/cpptemplate)

So, why ? - Because I want something simple Boost and Qt free.

*This is a Work In Progress* So test it but don't use it.

## Dependencies

### Compiling

* [cmake](http://www.cmake.org/)

### Documentation

* [doxygen](http://doxygen.org/) (TODO)
* [graphviz](http://graphviz.org/) (optional - TODO)

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

### On Windows

You need to install [MinGW](http://www.mingw.org/wiki/InstallationHOWTOforMinGW). Add the bin directory of your MinGW installation in your PATH. Then, open a Windows (not MSYS) console and :

    cd <path to lite root>
    mkdir build
    cd build
    cmake -G "MinGW Makefiles" ..
    mingw32-make
    mingw32-make test

If you want to use Visual Studio... Try it ;)

