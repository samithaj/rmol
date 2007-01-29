#!/bin/sh

INSTALL_DIR=`grep "^prefix =" ../Makefile | cut -d"=" -d" " -f3`
TST_PROG=./optimise
RMOL_API_VERSION=`grep "^RMOL_API_VERSION =" ../Makefile | cut -d"=" -d" " -f3`
RMOL_LIBRARY_NAME=`grep "^RMOL_LIBRARY_NAME =" ../Makefile | cut -d"=" -d" " -f3`
RMOL_LIB=lib${RMOL_LIBRARY_NAME}-${RMOL_API_VERSION}.so

if [ ! -x ${TST_PROG} ];
then
	echo "The sample program does not seem to have been compiled. Try 'make test' first."
	exit -1
fi

if [ "$1" = "-h" -o "$1" = "-H" -o "$1" = "--h" -o "$1" = "--help" ];
then
	echo "Usage: $0 [<Number of random draws> <Cabin capacity> <CSV input filename>]"
	echo "By default:"
	echo "  * The number of random draws is 10,000."
	echo "  * The cabin capacity is 500 seats."
	echo "  * If no input filename is provided, an internal sample with 3 classes is used."
	echo
	echo "The library (${RMOL_LIB}) is installed in: ${INSTALL_DIR}/lib"
	exit 0
fi

${TST_PROG} $1 $2 $3