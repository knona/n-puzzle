#!/bin/bash

./scripts/install-sfml.bash
./scripts/install-boost.bash
cmake .
make
