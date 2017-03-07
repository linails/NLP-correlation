#! /bin/sh
# Created Time: 2016-06-27 13:25:31
#


if [ -d ../build ]; then
    echo "dir build exist"
    cd ../build/
    make
else
    mkdir ../build
    echo "mkdir ../build"
    ./build.sh
fi

