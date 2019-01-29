include ../Makefile.inc

GEN_EXE = t_getpwent t_getpwnam_r

LINUX_EXE = check_password idshow

COURSE_FILES_EXE = show_eff_ids

EXE = ${GEN_EXE} ${LINUX_EXE} ${COURSE_FILES_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

clean : 
	${RM} ${EXE} *.o

check_password : check_password.o
	${CC} -o $@ check_password.o ${LDFLAGS} ${LDLIBS} ${LINUX_LIBCRYPT}

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation
