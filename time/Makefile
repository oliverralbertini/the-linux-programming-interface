include ../Makefile.inc

GEN_EXE = calendar_time show_time process_time strtime t_stime

COURSE_FILES_EXE = cal_time process_time_test

EXERCISE_FILES_EXE = # ex.date_math

EXE = ${GEN_EXE} ${LINUX_EXE} ${COURSE_FILES_EXE} ${EXERCISE_FILES_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

clean : 
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

cal_time: cal_time.o
	${CC} -o $@ cal_time.o ${CFLAGS} ${LDLIBS} ${LINUX_LIBRT}

process_time_test: process_time_test.o
	${CC} -o $@ process_time_test.o ${CFLAGS} ${LDLIBS} ${LINUX_LIBRT}

${EXE} : ${TLPI_LIB}		# True as a rough approximation
