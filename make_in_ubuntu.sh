#!/bin/bash
aclocal
autoconf
libtoolize --automake --copy --force
#automake --add-missing
automake
./configure
