#!/bin/bash

export CODE_DIR=$(dirname $( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd ))
echo $CODE_DIR

cd prom
docker build . -t bssr_prom
cd ..
envsubst < prometheus.yaml | kubectl apply -f -

