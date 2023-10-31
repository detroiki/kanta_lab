FLAGS = -std=c++11 -static -Wall


## HELPERS
../../bin/string_helpers.o: ../../src/string_helpers.cpp
	$(info Buildung String Helpers...)  g++ -c $(FLAGS) -o ../../bin/string_helpers.o ../../src/string_helpers.cpp

../../bin/file_helpers.o: ../../src/file_helpers.cpp
	$(info Buildung File Helpers...)  g++ -c $(FLAGS) -o ../../bin/file_helpers.o ../../src/file_helpers.cpp

../../bin/config_file_readers.o: config_file_readers.cpp
	$(info Buildung config file readr...)  g++ -c $(FLAGS) -o ../../bin/config_file_readers.o config_file_readers.cpp

../../bin/sumstats_utils.o: sumstats_utils.cpp
	$(info Buildung Main omop_sumstats...)  g++ -c $(FLAGS) -o ../../bin/sumstats_utils.o sumstats_utils.cpp

## OMOP Sumstats
omop_sumstats: ../../bin/omop_sumstats_main.o ../../bin/file_helpers.o ../../bin/string_helpers.o ../../bin/sumstats_utils.o ../../bin/config_file_readers.o
	g++ $(FLAGS) ../../bin/omop_sumstats_main.o ../../bin/file_helpers.o ../../bin/string_helpers.o ../../bin/sumstats_utils.o ../../bin/config_file_readers.o -o  ../../exec/omop_sumstats

../../bin/omop_sumstats_main.o: omop_sumstats_main.cpp
	$(info Buildung Main omop_sumstats...)  g++ -c $(FLAGS) -o ../../bin/omop_sumstats_main.o omop_sumstats_main.cpp

## Indiv Sumstats
indv_sumstats: ../../bin/indv_sumstats_main.o ../../bin/file_helpers.o ../../bin/string_helpers.o ../../bin/sumstats_utils.o ../../bin/config_file_readers.o
	g++ $(FLAGS) ../../bin/indv_sumstats_main.o ../../bin/file_helpers.o ../../bin/string_helpers.o ../../bin/sumstats_utils.o ../../bin/config_file_readers.o -o  ../../exec/indv_sumstats

../../bin/indv_sumstats_main.o: indv_sumstats_main.cpp
	$(info Buildung Main indv_sumstats...)  g++ -c $(FLAGS) -o ../../bin/indv_sumstats_main.o indv_sumstats_main.cpp

## OMOP Lab mapping
omop_lab_map: ../../bin/omop_lab_map_main.o ../../bin/file_helpers.o ../../bin/string_helpers.o ../../bin/sumstats_utils.o ../../bin/config_file_readers.o
	g++ $(FLAGS) ../../bin/omop_lab_map_main.o ../../bin/file_helpers.o ../../bin/string_helpers.o ../../bin/sumstats_utils.o ../../bin/config_file_readers.o -o  ../../exec/omop_lab_map

../../bin/omop_lab_map_main.o: omop_lab_map_main.cpp
	$(info Buildung Main omop_lab_map_main...)  g++ -c $(FLAGS) -o ../../bin/omop_lab_map_main.o omop_lab_map_main.cpp

clean:
	$(info Cleaning up...)
	rm ../../bin/*.o
	rm  ../../exec/*