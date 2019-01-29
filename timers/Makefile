include ../Makefile.inc

GEN_EXE = cpu_burner cpu_multi_burner cpu_multithread_burner \
	ptmr_null_evp ptmr_sigev_signal ptmr_sigev_thread \
	real_timer t_nanosleep timed_read

LINUX_EXE = demo_timerfd t_clock_nanosleep

COURSE_FILES_EXE = ptmr_simple_sigev_signal ptmr_simple_sigev_thread simple_timed_read t_clock_nanosleep_abstime timerfd_simple

EXERCISE_FILES_EXE = # ex.cpu_watchdog ex.timed_read_ptmr

EXE = ${GEN_EXE} ${LINUX_EXE} ${COURSE_FILES_EXE} ${EXERCISE_FILES_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

LDLIBS = ${IMPL_LDLIBS} ${LINUX_LIBRT} ${IMPL_THREAD_FLAGS}
	# Many of the programs in this directory need the 
	# realtime library, librt; to keep this Makefile simple,
	# we link *all* of the programs against that library.
	# Some of the programs in this directory use POSIX thread
	# and so require building with '-pthread'. To keep the Makefile
	# simple, we build all of the programs with '-pthread'

clean : 
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation
