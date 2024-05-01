
OPT = -g3 -O0
OP3 = -O3

ifeq ($(OS), Windows_NT)
        EXEC=exe
else
        EXEC=out
endif


main: seed.c includes 
	$(CC) $(OPT) seed.c -o run.$(EXEC)
	@echo DONE!

training: training.c includes
	$(CC) $(OPT) training.c -o learn.$(EXEC)

building:
	mkdir -p ./build

includes: console.h

create: begin.c
	$(CC) $(OP3) begin.c -o begin.$(EXEC)

help:
	@echo main
	@echo training
	@echo create
	@echo building
	@echo help
	@echo clean

clean:
	rm -r ./build
	rmdir ./build
	rm $(RUN).$(EXEC)

