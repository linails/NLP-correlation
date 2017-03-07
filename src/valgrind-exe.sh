#! /bin/sh
# Created Time: 2017-01-3 12:52:33
#

# Note: valgrind 只对运行到的函数进行检测，没有运行到的函数，存在内存泄漏也检测不到

cd ../build

echo "current exe path:"
pwd

valg="valgrind --tool=memcheck --leak-check=full "

if [ "$#" != "0" ]; then
    echo "para count : "$#
    echo "para list : "$*
    ${valg}bin/nlp-correlation $*
else
    ${valg}bin/nlp-correlation
fi

