include ../Makefile.inc

GEN_EXE = fork_exec hello_world

LINUX_EXE =

COURSE_FILES_EXE = fork_exec hello_world

EXE = ${GEN_EXE} ${LINUX_EXE} ${COURSE_FILES_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

clean : 
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation
