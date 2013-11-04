#!/bin/bash
aclocal
automake --add-missing
automake
./configure
