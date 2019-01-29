include ../Makefile.inc

GEN_EXE = anon_mmap mmcat mmcopy t_mmap

LINUX_EXE = t_remap_file_pages

COURSE_FILES_EXE = mmap_create

EXE = ${GEN_EXE} ${LINUX_EXE} ${COURSE_FILES_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

clean : 
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation
