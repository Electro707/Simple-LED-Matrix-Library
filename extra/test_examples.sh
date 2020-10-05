#!/bin/bash

# Bash program to test out all examples, to make sure that they compile fine before releasing a software version
# TODO: Maybe add an option to upload the program, watch it do something, then manually move on to the next example
EXAMPLES_FOLDER="../examples/*"
for e in $EXAMPLES_FOLDER 
do 
    echo "Testing example: $e"
    arduino-cli compile --fqbn arduino:avr:uno --warnings all $e
done
