#!/bin/bash

./scripts/install-sfml.bash
./scripts/install-boost.bash
# mkdir build && cd build && cmake .. && make
cmake .
make
