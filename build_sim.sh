#!/bin/bash
set -e

mkdir -p build bin
cd build
cmake ..
cmake --build .

for exe in electrode_brain_sim test_background_noise test_neuron test_adder test_electrode test_top; do
  if [ -f "$exe" ]; then
    cp "$exe" ../bin/
  fi
done

