include ../Makefile.inc

GEN_EXE = demo_SIGWINCH new_intr no_echo test_tty_functions 

EXERCISE_FILES_EXE = # ex.show_tty_attr

EXE = ${GEN_EXE} ${LINUX_EXE} ${EXERCISE_FILES_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

clean : 
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation
