cc -DNAME='"MW1"' -o mwrap_1.so -fPIC -shared mwrap.c -ldl
cc -DNAME='"MW2  "' -o mwrap_2.so -fPIC -shared mwrap.c -ldl
cc -DNAME='"MW3    "' -o mwrap_3.so -fPIC -shared mwrap.c -ldl

cc -o mtest -g -Wall mtest.c

LD_PRELOAD="./mwrap_1.so ./mwrap_2.so ./mwrap_3.so" ./mtest
