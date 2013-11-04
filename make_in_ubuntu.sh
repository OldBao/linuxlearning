#!/bin/bash
sudo apt-get install automake autoconf libtool
aclocal
autoconf
libtoolize --automake --copy --force
automake --add-missing
automake
./configure
