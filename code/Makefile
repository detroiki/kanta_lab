FLAGS = -std=c++11 -static -Wall

## DOWNSAMPLE
downsample: bin/main_downsample.o bin/file_helpers.o bin/string_helpers.o 
	g++ $(FLAGS) bin/main_downsample.o bin/file_helpers.o bin/string_helpers.o -o exec/downsample

bin/main_downsample.o: src/downsample/downsample_main.cpp
	$(info Buildung Main Reduce...)  g++ -c $(FLAGS) -o bin/main_downsample.o src/downsample/downsample_main.cpp

## DOWNSAMPLE
rem_dups: bin/rem_dups_main.o bin/file_helpers.o bin/string_helpers.o 
	g++ $(FLAGS) bin/rem_dups_main.o bin/file_helpers.o bin/string_helpers.o -o exec/remove_duplicates

bin/rem_dups_main.o: src/remove_duplicates/rem_dups_main.cpp
	$(info Buildung Main Reduce...)  g++ -c $(FLAGS) -o bin/rem_dups_main.o src/remove_duplicates/rem_dups_main.cpp

## MINIMAL
minimal: bin/main_minimal.o bin/file_helpers.o bin/string_helpers.o 
	g++ $(FLAGS) bin/main_minimal.o bin/file_helpers.o bin/string_helpers.o -o exec/minimal

bin/main_minimal.o: src/minimal/minimal_main.cpp
	$(info Buildung Main Reduce...)  g++ -c $(FLAGS) -o bin/main_minimal.o src/minimal/minimal_main.cpp

## REDUCE
reduce: bin/main_reduce.o bin/file_helpers.o bin/string_helpers.o 
	g++ $(FLAGS) bin/main_reduce.o bin/file_helpers.o bin/string_helpers.o -o exec/reduce

bin/main_reduce.o: src/reduce/reduce_main.cpp
	$(info Buildung Main Reduce...)  g++ -c $(FLAGS) -o bin/main_reduce.o src/reduce/reduce_main.cpp


## MISSINGNESS
missing: bin/missing_main.o bin/missing.o bin/string_helpers.o bin/file_helpers.o bin/missing_write_res.o
	g++ $(FLAGS) bin/missing_main.o bin/missing.o bin/string_helpers.o bin/file_helpers.o bin/missing_write_res.o -o exec/missing

bin/missing_main.o: src/missing/missing_main.cpp
	$(info Buildung Main Missing...)  g++ -c $(FLAGS) -o bin/missing_main.o src/missing/missing_main.cpp

bin/missing.o: src/missing/missing.cpp
	$(info Buildung Missing...)  g++ -c $(FLAGS) -o bin/missing.o src/missing/missing.cpp

bin/missing_write_res.o: src/missing/missing_write_res.cpp
	$(info Buildung Writing Results Missing...)  g++ -c $(FLAGS) -o bin/missing_write_res.o src/missing/missing_write_res.cpp

## COUNTING
counting: bin/main_counting.o bin/counting.o bin/string_helpers.o  bin/file_helpers.o bin/counting_write_res.o
	g++ $(FLAGS) bin/main_counting.o bin/counting.o bin/string_helpers.o  bin/file_helpers.o bin/counting_write_res.o -o exec/counting

bin/main_counting.o: src/counting/counting_main.cpp
	$(info Buildung Main Counting...)  g++ -c $(FLAGS) -o bin/main_counting.o src/counting/counting_main.cpp

bin/counting.o: src/counting/counting.cpp
	$(info Buildung Counting...)  g++ -c $(FLAGS) -o bin/counting.o src/counting/counting.cpp

bin/counting_write_res.o: src/counting/counting_write_res.cpp
	$(info Buildung  Writing Results Counting...)  g++ -c $(FLAGS) -o bin/counting_write_res.o src/counting/counting_write_res.cpp

## HELPERS
bin/string_helpers.o: src/string_helpers.cpp
	$(info Buildung String Helpers...)  g++ -c $(FLAGS) -o bin/string_helpers.o src/string_helpers.cpp

bin/file_helpers.o: src/file_helpers.cpp
	$(info Buildung File Helpers...)  g++ -c $(FLAGS) -o bin/file_helpers.o src/file_helpers.cpp

run_reduce: exec/reduce
	cat /home/kira/duni/helsinki/DSGE/Code/kanta_lab/code/tests/processed/data/all_minimal.csv | exec/reduce tests/ /home/kira/duni/helsinki/DSGE/Code/kanta_lab/code/tests/test_data/omop_map.csv /home/kira/duni/helsinki/DSGE/Code/kanta_lab/code/tests/test_data/omop_names.csv

run_missing: exec/missing
	exec/missing tests/processed/data/test_reduced.csv test_reduced tests/ 16

run_counting: exec/counting
	exec/counting file_1 tests/

run_minimal: exec/minimal
	exec/minimal tests/ file_1 /home/kira/duni/helsinki/DSGE/Code/kanta_lab/docu/thl_sote_fix_name_map.tsv
run_minimal_app: exec/minimal
	exec/minimal tests/ file_2 /home/kira/duni/helsinki/DSGE/Code/kanta_lab/docu/thl_sote_fix_name_map.tsv

run_downsample: exec/downsample
	exec/downsample tests/ /home/kira/duni/helsinki/DSGE/Code/kanta_lab/code/tests/processed/reports/sampled_ids_10.txt 10
clean:
	$(info Cleaning up...)
	rm bin/*.o
	rm exec/*