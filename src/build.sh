#! /bin/sh
# Created Time: 2016-04-23 14:26:21
#

# check build dir
if [ -d ../build ]; then
    echo "dir build exist"
else
    mkdir ../build
    echo "mkdir ../build"
fi

cd ../build
rm -rf *

c_compiler=
cxx_compiler=
system_type=
cmake_para=

if [ "`getconf LONG_BIT`" = "32" ]; then
    echo "32 bit system"
    system_type="i686"
else
    echo "64 bit system"
    system_type="x86_64"
fi

for eachline in `cat /etc/issue`; do
    if [ "`echo $eachline | awk -F "." '{print $1}'`" = "14" ]; then
        echo "system version : 14"
        c_compiler="x86_64-unknown-linux-gnu-gcc-4.9.2"
        cxx_compiler="x86_64-unknown-linux-gnu-g++"

        if [ "`which $c_compiler`" ]; then
            echo "$c_compiler is exist"
        else
            c_compiler="/opt/gcc-4.9.2/bin/x86_64-unknown-linux-gnu-gcc-4.9.2"
            cxx_compiler="/opt/gcc-4.9.2/bin/x86_64-unknown-linux-gnu-g++"
        fi

        break
    fi
done

if [ "$c_compiler" = "" ]; then
    echo "######################################################"
    echo ""
    echo "gcc -version"
    echo `gcc -v`
    echo ""
    echo "######################################################"
else
    echo "######################################################"
    echo ""
    echo "gcc -version"
    echo `$c_compiler -v`
    echo "######################################################"
    cmake_para="-DCMAKE_C_COMPILER=$c_compiler -DCMAKE_CXX_COMPILER=$cxx_compiler -DSYSTEM_TYPE=$system_type"
fi

cmake ../ $cmake_para

make
