LTYPE=A18
TARGET=fft.a
CFLAGS=-O
PREFLAGS=
IDIRS=-I../
LLIBS=

LINT=lint -uhbaxpc
CTAGS=ctags
PRINT=@pr -t

CFILES=\
	fourier.c\
	ft.c\
	w.c
HFILES=\
	/usr/include/math.h\
	../complex.h\
	w.h
OBJECTS=\
	fourier.o\
	ft.o\
	w.o

.SUFFIXES: .i

$(TARGET):	$(OBJECTS)
	ar rv $@ $?
	ranlib $@

lint:
	$(LINT) $(PREFLAGS) $(IDIRS) $(CFILES) $(LLIBS) -lc

tags:	$(HFILES) $(CFILES)
	$(CTAGS) $(HFILES) $(CFILES)

print:	fft.man
	$(PRINT) fft.man tech complex.h w.h $(CFILES)

fft.man:	fft.3
	@nroff -man fft.3 > fft.man

.c.o:
	$(CC) $(CFLAGS) -c $(IDIRS) $<

.c.i:
	$(CC) $(CFLAGS) -P $(IDIRS) $<

.c.s:
	$(CC) $(CFLAGS) -S $(IDIRS) $<

fourier.o:\
	../complex.h\
	w.h

ft.o:\
	../complex.h\
	w.h

w.o:\
	../complex.h\
	w.h\
	/usr/include/math.h\
