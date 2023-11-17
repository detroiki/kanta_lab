orig_data_dir = /data/original_data/kela_lab/
res_dir = /home/kdetrois/upload/
date = 2023-11-15

# These are provided in the data folder
thl_sote_map_path = data/thl_sote_map_named.tsv
thl_lab_map_path = data/thl_lab_id_abbrv_map.csv
omop_map_path = data/omop_concept_map.tsv
new_omop_map_path = data/new_omop_mappings.tsv
sumstats_res_path = $(res_dir)processed/sumstats/kanta_lab_$(date)
min_count_for_new_omops = 1000

run_all:
	# Prepare directories
	mkdir -p $(res_dir)processed/data
	mkdir -p $(res_dir)processed/reports/problem_rows
	mkdir -p $(res_dir)processed/reports/counts/row_counts

	# Initial file processing
	make create_minimal
	make concat_files
	make compress_files

	# OMOP mapping and fixing
	make map_omop
	conda activate shared_env
	make fix_units
	make final_fixing

	# Creating summary statistics, round 1
	make create_lab_omop_map_overview
	make learn_lab_omop_map
	make add_new_omop_concepts

	# Creating final summary statistics
	make omop_sumstats
	make indv_sumstats

create_minimal: 
	for file_no in 1 2 3 4 5 6 7 8 9 10 ; do \
		echo "Processing file $$file_no" $(orig_data_dir)"finregistry_$$file_no.csv.finreg_IDs"; \
		cat $(orig_data_dir)"finregistry_$$file_no.csv.finreg_IDs" | exec/minimal $(res_dir) $$file_no $(date) $(thl_sote_map_path) $(thl_lab_map_path) False; \
	done 

concat_files:
	cat $(res_dir)processed/data/kanta_lab_file_1_$(date).csv | awk -F "," -f exec/concat_files_head.awk > $(res_dir)processed/data/kanta_lab_$(date)_minimal.csv ; \
	for file_no in 2 3 4 5 6 7 8 9 10 ; do \
		cat $(res_dir)"processed/data/kanta_lab_file_$$file_no"_$(date).csv | awk -F "," -f exec/concat_files.awk >> $(res_dir)processed/data/kanta_lab_$(date)_minimal.csv ; \
	done

map_omop:
	cat $(res_dir)processed/data/kanta_lab_$(date)_minimal.csv | exec/map_omop $(res_dir) $(omop_map_path) $(date)
	rm -f $(res_dir)processed/data/kanta_lab_$(date)_minimal.csv

fix_units:
	python3.9 exec/unit_fixing.py $(res_dir) $(res_dir)processed/data/kanta_lab_$(date)_omop.csv $(date)

final_fixing:
	cat $(res_dir)processed/data/kanta_lab_$(date)_fixed_units.csv | exec/final_fixing $(res_dir) $(date)
	rm -f $(res_dir)processed/data/kanta_lab_$(date)_fixed_units.csv

create_lab_omop_map_overview:
	cat $(res_dir)processed/data/kanta_lab_$(date)_final_fix.csv | exec/create_lab_omop_map_overview $(sumstats_res_path)

learn_lab_omop_map:
	cat $(res_dir)processed/data/kanta_lab_$(date)_final_fix.csv | exec/learn_lab_omop_map $(sumstats_res_path)

add_new_omop_concepts:
	cat $(res_dir)processed/data/kanta_lab_$(date)_final_fix.csv | exec/add_omop_concepts $(sumstats_res_path)_new_omop_candidates.csv $(res_dir) $(date) $(min_count_for_new_omops)