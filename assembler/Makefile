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

TARGET_NAME = bin/am4asm

bin/obj/%.o: src/%.c | bin/obj/
	$(CC) $(COPTS) $(OBJECT_FLAG) -o $@ src/$(basename $(notdir $@)).c $(LIBS)

$(TARGET_NAME): $(OBJECTS) src/main.c | bin/obj/
	$(CC) $(COPTS) -o $@ $^

run: $(TARGET_NAME)
	$(TARGET_NAME)

clean:
	rm -r bin

%/:
	mkdir -p $@
