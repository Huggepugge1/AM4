CC = gcc
COPTS = -Wall -Wextra -pedantic
OBJECT_FLAG = -c

LINKER = ld

OBJECTS = $(addprefix bin/obj/, \
	$(addsuffix .o, 			\
	$(filter-out main, 			\
	$(basename 					\
	$(notdir 					\
	$(wildcard src/*.c))))))

LIBS =

%.o: | bin/obj/
	$(CC) $(COPTS) $(OBJECT_FLAG) -o $@ src/$(basename $(notdir $@)).c $(LIBS)

bin/am4: $(OBJECTS) src/main.c | bin/obj/
	$(CC) $(COPTS) -o $@ $^

run: bin/am4
	./bin/am4

clean:
	rm -r bin

%/:
	mkdir -p $@
