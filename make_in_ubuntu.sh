#!/bin/bash
#sudo apt-get install automake autoconf libtool
aclocal -I aclocal
autoconf
libtoolize --automake --copy --force
automake --add-missing
automake
./configure CXXFLAGS='-O0 -g' LDFLAGS='-O0 -g' 
