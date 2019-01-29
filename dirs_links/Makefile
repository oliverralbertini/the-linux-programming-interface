include ../Makefile.inc

GEN_EXE = bad_symlink file_type_stats list_files list_files_readdir_r \
	nftw_dir_tree t_dirbasename t_unlink view_symlink 

LINUX_EXE =

COURSE_FILES_EXE = nftw_dtree

EXERCISE_FILES_EXE = # ex.find_symlinks ex.openat_expmt

EXE = ${GEN_EXE} ${LINUX_EXE} ${COURSE_FILES_EXE} ${EXERCISE_FILES_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

list_files_readdir_r:
	${CC} -o $@ list_files_readdir_r.c ${CFLAGS} -Wno-error ${LDLIBS}

clean : 
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation
