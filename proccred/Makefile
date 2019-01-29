include ../Makefile.inc

GEN_EXE =

LINUX_EXE = idshow

EXERCISE_FILES_EXE = # ex.setuid_expt

EXE = ${GEN_EXE} ${LINUX_EXE} ${EXERCISE_FILES_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

clean : 
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation
