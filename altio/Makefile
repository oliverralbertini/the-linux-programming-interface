include ../Makefile.inc

GEN_EXE = demo_sigio poll_pipes select_mq self_pipe t_select

LINUX_EXE = epoll_flags_fork epoll_input multithread_epoll_wait

COURSE_FILES_EXE = t_select_simple

EXERCISE_FILES_EXE = # ex.epoll_pipes ex.is_chat_cl ex.is_chat_sv ex.poll_pipes_write

EXE = ${GEN_EXE} ${LINUX_EXE} ${COURSE_FILES_EXE} ${EXERCISE_FILES_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

clean : 
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation

multithread_epoll_wait: multithread_epoll_wait.o
	${CC} -o $@ multithread_epoll_wait.o \
		${CFLAGS} ${LDLIBS} ${IMPL_THREAD_FLAGS}

