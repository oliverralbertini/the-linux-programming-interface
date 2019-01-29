include ../Makefile.inc

GEN_EXE = acct_on acct_view child_status closeonexec envargs exit_handlers \
	footprint fork_file_sharing fork_sig_sync \
	fork_stdio_buf fork_whos_on_first \
	make_zombie multi_SIGCHLD multi_wait necho orphan \
	t_execl t_execle t_execve t_execlp t_fork t_system \
	t_vfork vfork_fd_test

LINUX_EXE = demo_clone t_clone acct_v3_view

COURSE_FILES_EXE = fork_stdio_buf_simple simple_atexit zombie exec_status t_system_v2 simple_on_exit

EXERCISE_FILES_EXE = # ex.exec_cnt ex.fork_var_test ex.grandchild_zombie ex.inherit_alarm ex.make_link ex.nohup ex.simple_shell

EXE = ${GEN_EXE} ${LINUX_EXE} ${COURSE_FILES_EXE} ${EXERCISE_FILES_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

clean : 
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation
