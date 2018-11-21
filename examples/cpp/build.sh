# /bin/bash

gcc -I./../../ -I./../../third_party/grishagins \
    -L./../../.build/lib minimize.cpp \
    -std=c++14 -lstdc++ -ladaptive_ags -lgrishagins -lm \
    -ominimize
