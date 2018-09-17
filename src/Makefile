.KEEP_STATE:

CC=gcc
OBJ=../obj
EXE=..
CCFLAGS=-Wall -g `pkg-config --cflags gtk+-2.0`
LDFLAGS=-lm `pkg-config --libs gtk+-2.0`

#compilation
OBJ_FILES=compute.o main.o
OBJ_FILES_AUTO=compute_auto.o ${OBJ}/compute.o

all: imaProjet.exe imaProjet_auto.exe

.c.o:
	${CC} ${CCFLAGS} -o $@ -c $<

imaProjet.exe:${OBJ_FILES}
	${CC} -o ${EXE}/$@ ${OBJ_FILES} ${LDFLAGS}
	mv *.o ${OBJ}

imaProjet_auto.exe:${OBJ_FILES_AUTO}
	${CC} -o ${EXE}/$@ ${OBJ_FILES_AUTO} ${LDFLAGS}
	mv *.o ${OBJ}

clean:
	\rm -f ${OBJ}/*.o ./*/core ${EXE}/*.exe

install:
	cp *.h ${INC}

.SUFFIXES: .exe .o .c .h
