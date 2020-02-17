#!/bin/sh

pushd () {
    command pushd "$@" > /dev/null
}

popd () {
    command popd "$@" > /dev/null
}

SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null && pwd )"
  SOURCE="$(readlink "$SOURCE")"
  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null && pwd )"

echo ""
echo "================================================================================"
echo "Building the Envelope Protection Solution on Linux/Unix"
echo "================================================================================"
echo ""
echo "Current location is ${DIR}"
echo ""

echo "--------------------------------------------------------------------------------"
echo "Building with the auto-tools from the root directory"
echo "--------------------------------------------------------------------------------"
#pushd "${DIR}"
autoreconf -ivf
./configure
make
make check
#popd

