#*============================---(source-start)---============================*#

#*---(current variables)--------------*#
BASE    = eos
DEBUG   = eos_debug
UNIT    = eos_unit
DIR     = /home/system/eos.goddess_of_dawn_and_startup
#*---(compilier variables)------------*#
# must have "-x c" on gcc line so stripped files work with alternate extensions
COMP    = gcc -c -std=gnu89 -x c -g -pg -Wall -Wextra
INCS    = -I/usr/local/include 
LINK    = gcc
LIBDIR  = -L/usr/local/lib
LIBS    = ${LIBDIR} -Wl,-Bstatic  -lyDLST        -lySEC        -lyEXEC        -lySTR                -Wl,-Bdynamic  -lrt
LIBD    = ${LIBDIR} -Wl,-Bstatic  -lyDLST_debug  -lySEC_debug  -lyEXEC_debug  -lySTR_debug  -lyLOG  -Wl,-Bdynamic  -lrt
LIBU    = ${LIBDIR} -Wl,-Bstatic  -lyDLST_debug  -lySEC_debug  -lyEXEC_debug  -lySTR_debug  -lyLOG  -Wl,-Bdynamic  -lrt  -lyUNIT  -lyVAR
#*---(file lists)---------------------*#
HEADS   = ${BASE}.h
OBJS    = ${BASE}_main.os ${BASE}_prog.os ${BASE}_conf.os ${BASE}_exec.os ${BASE}_util.os
OBJD    = ${BASE}_main.o  ${BASE}_prog.o  ${BASE}_conf.o  ${BASE}_exec.o  ${BASE}_util.o
#OBJU    = ${UNIT}.o       ${BASE}_prog.o  ${BASE}_conf.o  ${BASE}_exec.o  ${BASE}_util.o  ${BASE}_test.o
#*---(make variables)-----------------*#
COPY    = cp -f
CLEAN   = rm -f
PRINT   = @printf
STRIP   = @grep -v -e " DEBUG_" -e " yLOG_" 



#*---(MAIN)---------------------------*#
all                : ${BASE}_main.o ${BASE}_prog.o ${BASE}_conf.o ${BASE}_exec.o



#*---(objects)------------------------*#

${BASE}_main.o   : ${HEADS}       ${BASE}_main.c
	${COMP}    ${BASE}_main.c                        ${INC}
	${STRIP}   ${BASE}_main.c    > ${BASE}_main.cs
	${COMP}    ${BASE}_main.cs  -o ${BASE}_main.os   ${INC}

${BASE}_prog.o   : ${HEADS}       ${BASE}_prog.c
	${COMP}    ${BASE}_prog.c                        ${INC}
	${STRIP}   ${BASE}_prog.c    > ${BASE}_prog.cs
	${COMP}    ${BASE}_prog.cs  -o ${BASE}_prog.os   ${INC}

${BASE}_conf.o   : ${HEADS}       ${BASE}_conf.c
	${COMP}    ${BASE}_conf.c                        ${INC}
	${STRIP}   ${BASE}_conf.c    > ${BASE}_conf.cs
	${COMP}    ${BASE}_conf.cs  -o ${BASE}_conf.os   ${INC}

${BASE}_exec.o   : ${HEADS}       ${BASE}_exec.c
	${COMP}    ${BASE}_exec.c                        ${INC}
	${STRIP}   ${BASE}_exec.c    > ${BASE}_exec.cs
	${COMP}    ${BASE}_exec.cs  -o ${BASE}_exec.os   ${INC}



#*---(housecleaning)------------------*#
clean              :
	${PRINT}  "\n---cleaning------------------------------\n"
	${CLEAN} ${BASE}
	${CLEAN} ${BASE}*.o
	${CLEAN} ${BASE}*.cs
	${CLEAN} ${BASE}*.os
	${CLEAN} ${BASE}*.Sc
	${CLEAN} ${BASE}*.So
	${CLEAN} *~
	${CLEAN} temp*
	${CLEAN} ${BASE}_unit.c
	${CLEAN} ${DEBUG}
	#---(complete)------------------------#

bigclean           :
	${PRINT}  "\n---bigclean------------------------------\n"
	${CLEAN} .*.swp
	#---(complete)------------------------#

install            :
	${PRINT}  "\n---installing----------------------------\n"

remove             :
	${PRINT}  "\n---removing------------------------------\n"


#install            : 
#	${PRINTF} "\n---installing----------------------------\n"
#	${COPY}   eos        /sbin/
#	chmod     0700       /sbin/eos
#	chown     root:root  /sbin/eos
#	sha1sum   eos
#	ln --force --physical /sbin/eos   /sbin/eos_debug

#remove             :
#	rm -f  /sbin/eos
#	rm -f  /sbin/eos_debug



#*============================----(source-end)----============================*#
