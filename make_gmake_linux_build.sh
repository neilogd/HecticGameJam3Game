#!/bin/bash

python Psybrus/reflection_parse.py HecticGameJam3Game
Psybrus/Tools/genie/genie-linux --platform=x64 --toolchain=linux-clang --boostpath=$BOOST_ROOT gmake
