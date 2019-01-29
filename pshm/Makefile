include ../Makefile.inc

GEN_EXE = pshm_create pshm_read pshm_write pshm_unlink

LINUX_EXE =

COURSE_FILES_EXE = pshm_create_simple pshm_seqnum_get pshm_seqnum_init

EXERCISE_FILES_EXE = # ex.pshm_xfr_reader ex.pshm_xfr_writer

EXE = ${GEN_EXE} ${LINUX_EXE} ${COURSE_FILES_EXE} ${EXERCISE_FILES_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

LDLIBS = ${IMPL_LDLIBS} ${LINUX_LIBRT} ${IMPL_THREAD_FLAGS}  
	# All of the programs in this directory need the 
	# realtime library, librt.
	# Several of the programs in this directory use POSIX semaphores
	# and so require building with '-pthread'. To keep the Makefile
	# simple, we build all of the programs with '-pthread'

ex.pshm_xfr_writer ex.pshm_xfr_reader: pshm_xfr.h

pshm_xfr_test:
	dd if=/dev/urandom of=/tmp/infile count=100000
	./ex.pshm_xfr_writer < /tmp/infile &
	./ex.pshm_xfr_reader > /tmp/outfile
	@echo
	@if diff -q /tmp/infile /tmp/outfile; then echo "Test passed" ; else echo "Test FAILED"; fi

clean : 
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation
