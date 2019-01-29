include ../Makefile.inc

GEN_EXE = sched_set sched_view t_setpriority 

LINUX_EXE = demo_sched_fifo t_sched_setaffinity t_sched_getaffinity

COURSE_FILES_EXE = sched_nonrealtime

EXERCISE_FILES_EXE = # ex.rtsched

EXE = ${GEN_EXE} ${LINUX_EXE} ${COURSE_FILES_EXE} ${EXERCISE_FILES_EXE}

LDLIBS = ${IMPL_LDLIBS} #CF[ ${LINUX_LIBRT} ]

all : ${EXE}

allgen : ${GEN_EXE}

clean : 
	${RM} ${EXE} *.o

sched_nonrealtime : sched_nonrealtime.o
	${CC} -o $@ sched_nonrealtime.o ${CFLAGS} ${LDLIBS} ${LINUX_LIBRT}

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation
