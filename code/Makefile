FLAGS = -std=c++11 -static -Wall

## Minimal
minimal: bin/minimal_main.o bin/file_helpers.o bin/string_helpers.o  bin/minimal_utils.o
	g++ $(FLAGS) bin/minimal_main.o bin/file_helpers.o bin/string_helpers.o bin/minimal_utils.o -o exec/minimal

bin/minimal_main.o: src/minimal/minimal_main.cpp
	$(info Buildung Main map_omop...)  g++ -c $(FLAGS) -o bin/minimal_main.o src/minimal/minimal_main.cpp

bin/minimal_utils.o: src/minimal/minimal_utils.cpp
	$(info Buildung Main map_omop...)  g++ -c $(FLAGS) -o bin/minimal_utils.o src/minimal/minimal_utils.cpp

## Mapping OMOP
map_omop: bin/main_map_omop.o bin/file_helpers.o bin/string_helpers.o bin/read_omop_file.o bin/get_omop_lab_source.o bin/get_omop_id_and_name.o
	g++ $(FLAGS) bin/main_map_omop.o bin/file_helpers.o bin/string_helpers.o bin/read_omop_file.o bin/get_omop_lab_source.o bin/get_omop_id_and_name.o -o exec/map_omop

bin/main_map_omop.o: src/map_omop/map_omop_main.cpp
	$(info Buildung Main map_omop...)  g++ -c $(FLAGS) -o bin/main_map_omop.o src/map_omop/map_omop_main.cpp 

bin/read_omop_file.o: src/map_omop/read_omop_file.cpp
	$(info Buildung read_omop_file...)  g++ -c $(FLAGS) -o bin/read_omop_file.o src/map_omop/read_omop_file.cpp

bin/get_omop_lab_source.o: src/map_omop/get_omop_lab_source.cpp
	$(info Buildung get_omop_lab_source...)  g++ -c $(FLAGS) -o bin/get_omop_lab_source.o src/map_omop/get_omop_lab_source.cpp

bin/get_omop_id_and_name.o: src/map_omop/get_omop_id_and_name.cpp
	$(info Buildung get_omop_id_and_name...)  g++ -c $(FLAGS) -o bin/get_omop_id_and_name.o src/map_omop/get_omop_id_and_name.cpp

## Getting lab data, where OMOP concept covers at least 5% of population
top_lab_data: bin/top_lab_data_main.o bin/file_helpers.o bin/string_helpers.o bin/top_lab_data_utils.o
	g++ $(FLAGS) bin/top_lab_data_main.o bin/file_helpers.o bin/string_helpers.o bin/top_lab_data_utils.o -o exec/top_lab_data

bin/top_lab_data_main.o: src/top_lab_data/top_lab_data_main.cpp
	$(info Buildung Main map_omop...)  g++ -c $(FLAGS) -o bin/top_lab_data_main.o src/top_lab_data/top_lab_data_main.cpp

bin/top_lab_data_utils.o: src/top_lab_data/top_lab_data_utils.cpp
	$(info Buildung Main map_omop...)  g++ -c $(FLAGS) -o bin/top_lab_data_utils.o src/top_lab_data/top_lab_data_utils.cpp

## col_tabs
col_tabs: bin/main_col_tabs.o bin/update_col_tabs.o bin/string_helpers.o  bin/file_helpers.o bin/col_tabs_write_res.o
	g++ $(FLAGS) bin/main_col_tabs.o bin/update_col_tabs.o bin/string_helpers.o  bin/file_helpers.o bin/col_tabs_write_res.o -o exec/col_tabs

bin/main_col_tabs.o: src/col_tabs/col_tabs_main.cpp
	$(info Buildung Main col_tabs...)  g++ -c $(FLAGS) -o bin/main_col_tabs.o src/col_tabs/col_tabs_main.cpp

bin/update_col_tabs.o: src/col_tabs/update_col_tabs.cpp
	$(info Buildung col_tabs...)  g++ -c $(FLAGS) -o bin/update_col_tabs.o src/col_tabs/update_col_tabs.cpp

bin/col_tabs_write_res.o: src/col_tabs/col_tabs_write_res.cpp
	$(info Buildung  Writing Results col_tabs...)  g++ -c $(FLAGS) -o bin/col_tabs_write_res.o src/col_tabs/col_tabs_write_res.cpp

## _cross_tabs
_cross_tabs: bin/_cross_tabs_main.o bin/omop_update_cross_tabs.o bin/string_helpers.o  bin/file_helpers.o bin/_cross_tabs_write_res.o
	g++ $(FLAGS) bin/_cross_tabs_main.o bin/omop_update_cross_tabs.o bin/string_helpers.o  bin/file_helpers.o bin/_cross_tabs_write_res.o -o exec/_cross_tabs

bin/_cross_tabs_main.o: src/_cross_tabs/_cross_tabs_main.cpp
	$(info Buildung Main _cross_tabs...)  g++ -c $(FLAGS) -o bin/_cross_tabs_main.o src/_cross_tabs/_cross_tabs_main.cpp

bin/omop_update_cross_tabs.o: src/_cross_tabs/omop_update_cross_tabs.cpp
	$(info Buildung _cross_tabs...)  g++ -c $(FLAGS) -o bin/omop_update_cross_tabs.o src/_cross_tabs/omop_update_cross_tabs.cpp

bin/_cross_tabs_write_res.o: src/_cross_tabs/_cross_tabs_write_res.cpp
	$(info Buildung  Writing Results _cross_tabs...)  g++ -c $(FLAGS) -o bin/_cross_tabs_write_res.o src/_cross_tabs/_cross_tabs_write_res.cpp

## HELPERS
bin/string_helpers.o: src/string_helpers.cpp
	$(info Buildung String Helpers...)  g++ -c $(FLAGS) -o bin/string_helpers.o src/string_helpers.cpp

bin/file_helpers.o: src/file_helpers.cpp
	$(info Buildung File Helpers...)  g++ -c $(FLAGS) -o bin/file_helpers.o src/file_helpers.cpp

run_map_omop: exec/map_omop
	cat /home/kira/duni/helsinki/DSGE/Code/kanta_lab/code/tests/processed/data/all_minimal.csv | exec/map_omop tests/ /home/kira/duni/helsinki/DSGE/Code/kanta_lab/code/tests/test_data/omop_concept_map.csv /home/kira/duni/helsinki/DSGE/Code/kanta_lab/code/tests/test_data/omop_names.csv

final_fixing: bin/final_fixing_main.o bin/file_helpers.o bin/string_helpers.o  bin/final_fixing_utils.o
	g++ $(FLAGS) bin/final_fixing_main.o bin/file_helpers.o bin/string_helpers.o bin/final_fixing_utils.o -o exec/final_fixing

bin/final_fixing_main.o: src/final_fixing/final_fixing_main.cpp
	$(info Buildung Main map_omop...)  g++ -c $(FLAGS) -o bin/final_fixing_main.o src/final_fixing/final_fixing_main.cpp

bin/final_fixing_utils.o: src/final_fixing/final_fixing_utils.cpp
	$(info Buildung Main map_omop...)  g++ -c $(FLAGS) -o bin/final_fixing_utils.o src/final_fixing/final_fixing_utils.cpp

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