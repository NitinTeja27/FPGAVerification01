#!/bin/bash

# Generate Test Cases
cd "/Users/nitinema.co/my_stuff/FPGA Assisted Verification/TestCaseGenerator/" && g++ test_case_generator.cpp -o test_case_generator && "/Users/nitinema.co/my_stuff/FPGA Assisted Verification/TestCaseGenerator/"test_case_generator

# Run Golden Model and Save Output
for input_file in test_case_*.txt; do
    output_file="output_$(echo "$input_file" | sed 's/test_case_//;s/\.txt//').txt"
    cd ..
    cp "TestCaseGenerator/$input_file" "GoldenModel/input.txt"
    cd "/Users/nitinema.co/my_stuff/FPGA Assisted Verification/GoldenModel/" && g++ golden_model.cpp -o golden_model && "/Users/nitinema.co/my_stuff/FPGA Assisted Verification/GoldenModel/"golden_model
    cd ..
    touch "TestCaseGenerator/$output_file"
    cp "GoldenModel/output.txt" "TestCaseGenerator/$output_file"
    cd TestCaseGenerator
done