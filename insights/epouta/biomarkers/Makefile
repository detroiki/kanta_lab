date = v2_2023-12-12

run_both:
	make extract_krea
	make run_insights

run_insights: /opt/R-4.1.1/bin/Rscript /home/kdetrois/projects/biomarker_imputation/data_inspection/scripts/krea_time_sumstats.R
	/opt/R-4.1.1/bin/Rscript /home/kdetrois/projects/biomarker_imputation/data_inspection/scripts/krea_time_sumstats.R
	
extract_krea:
	cat /home/kdetrois/projects/kanta_lab_processing/processed/data/kanta_lab_$(date)_new_omop.tsv | awk '{FS="\t";OFS="\t"}{if($$9 == 3020564 || NR== 1) print;}' > /home/kdetrois/projects/biomarker_imputation/data_inspection/data/krea/kanta_lab_$(date)_krea.tsv
	
extract_t4:
	cat /home/kdetrois/projects/kanta_lab_processing/processed/data/kanta_lab_v2_2023-12-05_omop_new.csv | awk '{FPAT = "([^,]*)|(\"[^\"]+\")"}{if($$9 == 3020564 || NR== 1) print;}' > /home/kdetrois/projects/biomarker_imputation/data_inspection/data/krea/kanta_lab_v2_2023-12-05_krea_n.csv