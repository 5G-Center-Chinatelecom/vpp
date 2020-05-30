#!/bin/bash
set -o errexit
set -o nounset
set -o pipefail
set -o errtrace

cd "$(dirname "${BASH_SOURCE}")/../.."

cp -v extras/docker/hooks/* .git/hooks/
