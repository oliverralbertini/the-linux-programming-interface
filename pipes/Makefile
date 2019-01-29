include ../Makefile.inc

GEN_EXE = change_case fifo_seqnum_client fifo_seqnum_server \
	pipe_ls_wc pipe_sync popen_glob simple_pipe 

COURSE_FILES_EXE = pipe_ls_wc_simple simple_popen

EXERCISE_FILES_EXE = # ex.pipeline_builder ex.unique_tokens

EXE = ${GEN_EXE} ${LINUX_EXE} ${COURSE_FILES_EXE} ${EXERCISE_FILES_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

fifo_seqnum_client.o fifo_seqnum_server.o : fifo_seqnum.h

clean : 
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation

test_unique_tokens: ex.unique_tokens
	@TMPFILE=/tmp/unique_tokens.$$$$ ; \
	tr ' \t' '\012' < tokens.txt | sort -u > $$TMPFILE ; \
	./ex.unique_tokens tokens.txt | \
		diff -q /dev/stdin $$TMPFILE \
		&& echo 'Test passed' || echo 'Test failed'

test_count_unique_tokens: ex.count_unique_tokens
	@cnt1=$$(tr ' \t' '\012' < tokens.txt | sort -u | wc -l); \
	cnt2=$$(./ex.count_unique_tokens tokens.txt) ; \
	test $$cnt1 -eq $$cnt2 && echo 'Test passed' || echo 'Test failed'
