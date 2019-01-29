include ../Makefile.inc

GEN_EXE = bad_longjmp display_env longjmp \
      necho setjmp_vars setjmp_vars_opt t_getenv

LINUX_EXE = modify_env

COURSE_FILES_EXE = simple_longjmp

EXE = ${GEN_EXE} ${LINUX_EXE} ${COURSE_FILES_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

setjmp_vars:
	${CC} -o $@ setjmp_vars.c ${CFLAGS} -Wno-error ${LDLIBS}

clean : 
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation
