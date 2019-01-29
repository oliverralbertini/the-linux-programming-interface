include ../Makefile.inc

GEN_EXE = atomic_append bad_exclusive_open copy \
	multi_descriptors seek_io t_readv t_truncate

LINUX_EXE = large_file

EXERCISE_FILES_EXE = # ex.fd_sharing ex.seek_write ex.tee

EXE = ${GEN_EXE} ${LINUX_EXE} ${EXERCISE_FILES_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

tee_test:
	@SRC=/tmp/$$$$.src ; \
	SRC2=/tmp/$$$$.src2 ; \
	TFILE1=/tmp/$$$$.tfile1 ; \
	TFILE2=/tmp/$$$$.tfile2 ; \
	date > $$SRC ; \
	cat $$SRC $$SRC > $$SRC2 ; \
	cat $$SRC | ./ex.tee $$TFILE1 1> $$TFILE2 ; \
	diff $$SRC $$TFILE1 && diff $$SRC $$TFILE2 && \
		echo "Test 1 passed" || echo "Test 1 failed" ; \
	cat $$SRC | ./ex.tee -a $$TFILE1 1> $$TFILE2 ; \
	diff $$SRC2 $$TFILE1 && diff $$SRC $$TFILE2 &&\
		echo "Test 2 (append) passed" || \
		echo "Test 2 (append) failed" ; \
	cat $$SRC | ./ex.tee $$TFILE1 1> $$TFILE2 ; \
	diff $$SRC $$TFILE1 && diff $$SRC $$TFILE2 &&\
		echo "Test 3 (truncate) passed" || \
		echo "Test 3 (truncate) failed"

clean : 
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation
