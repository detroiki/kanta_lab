run_all:
	make create_minimal
	make concat_files
	make compress_files
	make map_omop
	python3.9 /media/volume/users/detrokir/python_scripts/unit_fixing.py
	make final_fixing
	make top_omop
	make final_fixing_omop

create_minimal: 
	for file_no in 1 2 3 4 5 6 7 8 9 10 ; do \
		echo "Reading file $$file_no" ; \
		zstdcat "/media/volume/users/vllorens/data/kanta_lab_20233001/stage0/finregistry_$$file_no.csv.finreg_IDs.zst" | head -n 1000 | exec/minimal /media/volume/users/detrokir/ $$file_no "/media/volume/users/detrokir/data/thl_sote_fix_name_map.tsv" /media/volume/users/detrokir/data/lab_id_min.csv ; \
	done 

concat_files:
	cat /media/volume/users/detrokir/processed/data/minimal_file_1.csv | awk -F ";" -f exec/concat_files_head.awk > /media/volume/users/detrokir/processed/data/kanta_lab_minimal.csv ; \
	for file_no in 2 3 4 5 6 7 8 9 10 ; do \
		cat "/media/volume/users/detrokir/processed/data/minimal_file_$$file_no.csv" | awk -F ";" -f exec/concat_files.awk >> /media/volume/users/detrokir/processed/data/kanta_lab_minimal.csv ; \
	done

compress_files:
	for file_no in 1 2 3 4 5 6 7 8 9 10 ; do \
		zstd -v -T24 --ultra -22 --rm "/media/volume/users/detrokir/processed/data/minimal_file_$$file_no.csv" ; \
		zstd -v -T24 --ultra -22 --rm "/media/volume/users/detrokir/processed/reports/problem_rows/duplines_$$file_no.csv" ; \
		zstd -v -T24 --ultra -22 --rm "/media/volume/users/detrokir/processed/reports/problem_rows/problem_rows_file_$$file_no.csv" ; \
	done
	zstd -v -T24 --ultra -22 --rm /media/volume/users/detrokir/processed/data/all_minimal.csv

map_omop:
	cat /media/volume/users/detrokir/processed/data/kanta_lab_minimal.csv | exec/map_omop /media/volume/users/detrokir/ /media/volume/users/detrokir/data/omop_map.tsv

final_fixing:
	cat /media/volume/users/detrokir/processed/data/kanta_lab_minimal_fixed_units.csv | exec/final_fixing /media/volume/users/detrokir/

top_omop:
	exec/top_lab_data /media/volume/users/detrokir/processed/data/kanta_lab_minimal_final.csv /media/volume/users/detrokir/

final_fixing_omop:
	exec/final_fixing_omop /media/volume/users/detrokir/processed/data/kanta_lab_minimal_final.csv /media/volume/users/detrokir/


	
