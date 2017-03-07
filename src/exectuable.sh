#! /bin/sh
# Created Time: 2016-04-23 14:34:33
#

cd ../build

echo "current exe path:"
pwd

if [ "$#" != "0" ]; then
    echo "para count : "$#
    echo "para list : "$*
    bin/nlp-correlation $*
else
    bin/nlp-correlation
fi

