cc -g -Wall -fPIC -shared -o libfoo.so foo.c 
cc -g -Wall -o main main.c libfoo.so -ldl
LD_LIBRARY_PATH=. ./main 
