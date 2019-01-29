#!/bin/sh

rm -f *.a *.o *.so
\rm $(ls main* | grep -v '\.c$')

set -v
cc -fPIC -o mod_strong_pic.o -c mod.c
cc -shared -o mod_strong.so mod_strong_pic.o

cc -o mod_strong.o -c mod.c
ar r mod_strong.a mod_strong.o

cc -DMOD_WEAK -o mod_weak.o -c mod.c
ar r mod_weak.a mod_weak.o

cc -fPIC -DMOD_WEAK -c mod.c -o mod_weak_pic.o
cc -shared -o mod_weak.so mod_weak_pic.o

# ------------------------------------
# A weak symbol that is not defined gets the value 0

cc -DNO_MOD_H -DMAIN_PROVIDES_WEAK_DECL -o main_nolib main.c
./main_nolib

LD_PRELOAD=./mod_strong.so ./main_nolib

# ------------------------------------
# If main provides a weak declaration, at run time a shared library
# can provide the definition

cc -DMAIN_PROVIDES_WEAK_DECL -DNO_MOD_H -o main_lib_so main.c mod_strong.so
LD_LIBRARY_PATH=. ./main_lib_so

# ------------------------------------
# If main provides a weak FUNCTION, that will override a strong definition
# in the shared library (because symbol resolution is left-to-right on the 
# static link line, and the linker won't bother to look inside the library
# if it has already resolved the symbol)

cc -DMAIN_PROVIDES_WEAK_FUNC -o main_func_lib_so main.c mod_strong.so
LD_LIBRARY_PATH=. ./main_func_lib_so

# ------------------------------------
# If main provides a weak FUNCTION, that will override a strong definition
# in a static archive (because symbol resolution is left-to-right on the
# static link line, and the linker won't bother to look inside the archive
# if it has already resolved the symbol)

cc -DMAIN_PROVIDES_WEAK_FUNC -o main_func_lib_a main.c mod_strong.a
./main_func_lib_a

# ------------------------------------
# By contrast, the linker *always* links .o files, so that if a .o 
# provides a strong definition, that will override main's weak definition
# http://stackoverflow.com/questions/6177498/whats-the-difference-between-object-file-and-static-libraryarchive-file

cc -DMAIN_PROVIDES_WEAK_FUNC -o main_linked_to_mod_o main.c mod_strong.o
./main_linked_to_mod_o

# ------------------------------------
# If main provides a weak FUNCTION, but a static archive provides a
# strong version, and we employ the --whole-archive linker toggle, then
# the linker includes the whole of the archive into the executable
# and the archive's strong symbol overrides main's weak symbol

cc -DMAIN_PROVIDES_WEAK_FUNC -o main_func_lib_a_wa main.c \
       		-Wl,-whole-archive mod_strong.a -Wl,-no-whole-archive
./main_func_lib_a_wa

# ------------------------------------
# If main provides a weak FUNCTION, and a static archive provides a
# weak version, then even if we employ the --whole-archive linker toggle,
# main's weak symbol satisfies the link first

cc -DMAIN_PROVIDES_WEAK_FUNC -o main_func_lib_weak_a_wa main.c \
       		-Wl,-whole-archive mod_weak.a -Wl,-no-whole-archive
./main_func_lib_weak_a_wa

# ------------------------------------
# If main provides a weak FUNCTION, and a shared library provides a
# weak version, then even if we employ the --whole-archive linker toggle,
# main's weak symbol satisfies the static linker. IOW, --whole-archive
# has effect only for static archives, not shared libraries.

cc -DMAIN_PROVIDES_WEAK_FUNC -o main_func_lib_so_wa main.c \
       		-Wl,-whole-archive mod_strong.so -Wl,-no-whole-archive
LD_LIBRARY_PATH=. ./main_func_lib_so_wa

# ------------------------------------
# If main provides a weak declaration, and two shared libraries provide
# definitions, one strong, and one weak, then the definition from the
# first library mentioned in the static link line is used, even if that
# is the weak definition

cc -DMODNAME='"MOD_2"' -fPIC -o mod_2_strong_pic.o -c mod.c
cc -shared -o mod_2_strong.so mod_2_strong_pic.o
cc -DMAIN_PROVIDES_WEAK_DECL -DNO_MOD_H -o main_2_libs main.c mod_weak.so mod_2_strong.so
LD_LIBRARY_PATH=. ./main_2_libs

# ------------------------------------
# If a static archive provides a strong and a weak definition, then
# symbol that is used will be the first one that appears in the
# order that the objects lie inside the archive

cc -DMODNAME='"MOD_2"' -o mod_2_strong.o -c mod.c
ar r mod_2_libs.a mod_weak.o mod_2_strong.o

cc -o main_2_libs_a main.c mod_2_libs.a
./main_2_libs_a

# ------------------------------------
# If a static archive provides a strong and a weak definition, 
# and the link is done with the --whole-archive linker toggle,
# then the strong symbol predominates, regardless of the order of
# the objects inside the library

cc -DMODNAME='"MOD_2"' -o mod_2_strong.o -c mod.c
ar r mod_2_libs.a mod_weak.o mod_2_strong.o

cc -o main_2_libs_a_wa main.c \
	-Wl,-whole-archive mod_2_libs.a -Wl,-no-whole-archive

./main_2_libs_a_wa
