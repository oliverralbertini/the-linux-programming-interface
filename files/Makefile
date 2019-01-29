include ../Makefile.inc

GEN_EXE = t_chown t_stat t_umask t_utime t_utimes

LINUX_EXE = chiflag

COURSE_FILES_EXE = simple_stat

EXERCISE_FILES_EXE = # ex.chmod_addx

EXE = ${GEN_EXE} ${LINUX_EXE} ${COURSE_FILES_EXE} ${EXERCISE_FILES_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

clean : 
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation
