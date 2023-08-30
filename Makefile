CFLAGS+=-g

main: main.o ctx.o gen.o

.PHONY: clean
clean:
	rm -rf *.o main
