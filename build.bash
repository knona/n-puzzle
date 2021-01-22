#!/bin/bash

set -e

./scripts/install-sfml.bash
./scripts/install-boost.bash
cmake .
make
