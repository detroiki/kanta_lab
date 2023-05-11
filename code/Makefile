FLAGS = -std=c++11 -static -Wall

## DOWNSAMPLE
downsample: bin/main_downsample.o bin/file_helpers.o bin/string_helpers.o 
	g++ $(FLAGS) bin/main_downsample.o bin/file_helpers.o bin/string_helpers.o -o exec/downsample

bin/main_downsample.o: src/downsample/downsample_main.cpp
	$(info Buildung Main map_omop...)  g++ -c $(FLAGS) -o bin/main_downsample.o src/downsample/downsample_main.cpp

## DOWNSAMPLE
rem_dups: bin/rem_dups_main.o bin/file_helpers.o bin/string_helpers.o 
	g++ $(FLAGS) bin/rem_dups_main.o bin/file_helpers.o bin/string_helpers.o -o exec/remove_duplicates

bin/rem_dups_main.o: src/remove_duplicates/rem_dups_main.cpp
	$(info Buildung Main map_omop...)  g++ -c $(FLAGS) -o bin/rem_dups_main.o src/remove_duplicates/rem_dups_main.cpp

## MINIMAL
minimal: bin/main_minimal.o bin/file_helpers.o bin/string_helpers.o 
	g++ $(FLAGS) bin/main_minimal.o bin/file_helpers.o bin/string_helpers.o -o exec/minimal

bin/main_minimal.o: src/minimal/minimal_main.cpp
	$(info Buildung Main map_omop...)  g++ -c $(FLAGS) -o bin/main_minimal.o src/minimal/minimal_main.cpp

## map_omop
map_omop: bin/main_map_omop.o bin/file_helpers.o bin/string_helpers.o bin/read_omop_file.o
	g++ $(FLAGS) bin/main_map_omop.o bin/file_helpers.o bin/string_helpers.o bin/read_omop_file.o -o exec/map_omop

bin/main_map_omop.o: src/map_omop/map_omop_main.cpp
	$(info Buildung Main map_omop...)  g++ -c $(FLAGS) -o bin/main_map_omop.o src/map_omop/map_omop_main.cpp

bin/read_omop_file.o: src/map_omop/read_omop_file.cpp
	$(info Buildung map_omop...)  g++ -c $(FLAGS) -o bin/read_omop_file.o src/map_omop/read_omop_file.cpp

## MISSINGNESS
missing: bin/missing_main.o bin/missing.o bin/string_helpers.o bin/file_helpers.o bin/missing_write_res.o
	g++ $(FLAGS) bin/missing_main.o bin/missing.o bin/string_helpers.o bin/file_helpers.o bin/missing_write_res.o -o exec/missing

bin/missing_main.o: src/missing/missing_main.cpp
	$(info Buildung Main Missing...)  g++ -c $(FLAGS) -o bin/missing_main.o src/missing/missing_main.cpp

bin/missing.o: src/missing/missing.cpp
	$(info Buildung Missing...)  g++ -c $(FLAGS) -o bin/missing.o src/missing/missing.cpp

bin/missing_write_res.o: src/missing/missing_write_res.cpp
	$(info Buildung Writing Results Missing...)  g++ -c $(FLAGS) -o bin/missing_write_res.o src/missing/missing_write_res.cpp

## col_tabs
col_tabs: bin/main_col_tabs.o bin/update_col_tabs.o bin/string_helpers.o  bin/file_helpers.o bin/col_tabs_write_res.o
	g++ $(FLAGS) bin/main_col_tabs.o bin/update_col_tabs.o bin/string_helpers.o  bin/file_helpers.o bin/col_tabs_write_res.o -o exec/col_tabs

bin/main_col_tabs.o: src/col_tabs/col_tabs_main.cpp
	$(info Buildung Main col_tabs...)  g++ -c $(FLAGS) -o bin/main_col_tabs.o src/col_tabs/col_tabs_main.cpp

bin/update_col_tabs.o: src/col_tabs/update_col_tabs.cpp
	$(info Buildung col_tabs...)  g++ -c $(FLAGS) -o bin/update_col_tabs.o src/col_tabs/update_col_tabs.cpp

bin/col_tabs_write_res.o: src/col_tabs/col_tabs_write_res.cpp
	$(info Buildung  Writing Results col_tabs...)  g++ -c $(FLAGS) -o bin/col_tabs_write_res.o src/col_tabs/col_tabs_write_res.cpp

## omop_cross_tabs
omop_cross_tabs: bin/omop_cross_tabs_main.o bin/omop_update_cross_tabs.o bin/string_helpers.o  bin/file_helpers.o bin/omop_cross_tabs_write_res.o
	g++ $(FLAGS) bin/omop_cross_tabs_main.o bin/omop_update_cross_tabs.o bin/string_helpers.o  bin/file_helpers.o bin/omop_cross_tabs_write_res.o -o exec/omop_cross_tabs

bin/omop_cross_tabs_main.o: src/omop_cross_tabs/omop_cross_tabs_main.cpp
	$(info Buildung Main omop_cross_tabs...)  g++ -c $(FLAGS) -o bin/omop_cross_tabs_main.o src/omop_cross_tabs/omop_cross_tabs_main.cpp

bin/omop_update_cross_tabs.o: src/omop_cross_tabs/omop_update_cross_tabs.cpp
	$(info Buildung omop_cross_tabs...)  g++ -c $(FLAGS) -o bin/omop_update_cross_tabs.o src/omop_cross_tabs/omop_update_cross_tabs.cpp

bin/omop_cross_tabs_write_res.o: src/omop_cross_tabs/omop_cross_tabs_write_res.cpp
	$(info Buildung  Writing Results omop_cross_tabs...)  g++ -c $(FLAGS) -o bin/omop_cross_tabs_write_res.o src/omop_cross_tabs/omop_cross_tabs_write_res.cpp

## HELPERS
bin/string_helpers.o: src/string_helpers.cpp
	$(info Buildung String Helpers...)  g++ -c $(FLAGS) -o bin/string_helpers.o src/string_helpers.cpp

bin/file_helpers.o: src/file_helpers.cpp
	$(info Buildung File Helpers...)  g++ -c $(FLAGS) -o bin/file_helpers.o src/file_helpers.cpp

run_map_omop: exec/map_omop
	cat /home/kira/duni/helsinki/DSGE/Code/kanta_lab/code/tests/processed/data/all_minimal.csv | exec/map_omop tests/ /home/kira/duni/helsinki/DSGE/Code/kanta_lab/code/tests/test_data/omop_group_id_map.csv /home/kira/duni/helsinki/DSGE/Code/kanta_lab/code/tests/test_data/omop_names.csv

run_missing: exec/missing
	exec/missing tests/processed/data/test_map_omopd.csv test_map_omopd tests/ 16

run_col_tabs: exec/col_tabs
	exec/col_tabs file_1 tests/

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