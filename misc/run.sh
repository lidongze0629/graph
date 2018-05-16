#!/bin/bash

if [ $# != 1 ] ; then 
    echo "USAGE: $0 <query>"
    echo "example: $0 4"
    exit 1; 
fi 

query=$1

SHELL_FOLDER=$(dirname $(readlink -f "$0"))

BASE_DIR=${SHELL_FOLDER%/*}

COMMAND="${BASE_DIR}/build/graph-engine $query ${BASE_DIR}/result/twitter_$query.res"

echo "COMMAND = $COMMAND"

$COMMAND

echo "compare result "
diff ${BASE_DIR}/result/twitter_$query.res ${BASE_DIR}/result/twitter-sssp-$query.dat
