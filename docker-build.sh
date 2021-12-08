#/bin/bash

SCRIPT_HOME="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

pushd ${SCRIPT_HOME}
    docker build -f ./Dockerfile-base -t padamose-build-base .
    docker build -f ./Dockerfile --no-cache -t padamose-build .
    docker run -it padamose-build
popd
