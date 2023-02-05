FLAGS = -std=c++11 -static -Wall

compile: bin/main.o bin/counting.o bin/split.o bin/write_res.o
	g++ $(FLAGS) bin/main.o bin/counting.o bin/split.o bin/write_res.o -o missing

bin/main.o: src/main.cpp
	$(info Buildung Main...)  g++ -c $(FLAGS) -o bin/main.o src/main.cpp

bin/counting.o: src/counting.cpp
	$(info Buildung counting...)  g++ -c $(FLAGS) -o bin/counting.o src/counting.cpp

bin/split.o: src/split.cpp
	$(info Buildung split...)  g++ -c $(FLAGS) -o bin/split.o src/split.cpp

bin/write_res.o: src/write_res.cpp
	$(info Buildung write_res...)  g++ -c $(FLAGS) -o bin/write_res.o src/write_res.cpp

run: ./missing
	./missing test_data/test_selectcols.csv
clean:
	$(info Cleaning up...)
	rm bin/*.o
	rm missing