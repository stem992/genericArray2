CFLAGS = -g -O0 -Wall -Wextra -Wpedantic -I include -I Resources/

mkdir:
	mkdir -p bin
	mkdir -p build

test: bin/generic_array_test
	cd bin && ./generic_array_test

main : bin/generic_array_app
	cd bin && ./generic_array_app ../records/records.csv


bin/generic_array_test : build/generic_array_test.o build/generic_array.o build/unity.o
	$(CC) $^ -o $@

bin/generic_array_app : build/generic_array_app.o build/generic_array.o
	$(CC) $^ -o $@


build/generic_array_test.o: test/generic_array_test.c Resources/unity.h
	$(CC) $(CFLAGS) -c test/generic_array_test.c -o build/generic_array_test.o

build/generic_array_app.o: app/generic_array_app.c
	$(CC) $(CFLAGS) -c app/generic_array_app.c -o build/generic_array_app.o

build/generic_array.o: src/generic_array.c include/generic_array.h
	$(CC) $(CFLAGS) -c src/generic_array.c -o build/generic_array.o
	
build/unity.o : Resources/unity.c Resources/unity.h
	$(CC) $(CFLAGS) -c Resources/unity.c -o build/unity.o

	
clean:
	$(RM) build/*
	$(RM) bin/*
	

