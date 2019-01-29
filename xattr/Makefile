include ../Makefile.inc

GEN_EXE = 

LINUX_EXE = t_setxattr xattr_view

COURSE_FILES_EXE = setxattr xattr_view_simple

EXE = ${GEN_EXE} ${LINUX_EXE} ${COURSE_FILES_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

clean : 
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation
