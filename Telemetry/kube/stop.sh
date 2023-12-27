#!/bin/bash

export CODE_DIR=$(dirname $( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd ))
echo $CODE_DIR

envsubst < prometheus.yaml | kubectl delete -f -

