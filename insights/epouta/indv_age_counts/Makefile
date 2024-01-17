date = v2_2023-12-12
#date = v1_2023-11-21
in_dir = /home/kdetrois/projects/kanta_lab_processing/processed/data/

lab_age:
	/opt/R-4.1.1/bin/Rscript /home/kdetrois/projects/kanta_lab_stats/scripts/indvs_insights/calc_lab_age.R $(date)
  
finregid_map:
	cat $(in_dir)kanta_lab_$(date).tsv | awk '{FS="\t"}{if(NR == 1) print "FINREGISTRYID\tLAB_ID\tLAB_ABBREVIATION\tOMOP_ID\tOMOP_NAME\tLAB_UNIT\tCOUNT"; else{count[$$1"\t"$$4"\t"$$6"\t"$$10"\t"$$11"\t"$$8]++}}END{for(elem in count) print elem"\t"count[elem]}' > /home/kdetrois/projects/kanta_lab_stats/results/finregistry_id_count_$(date).tsv
	
map_and_lab_age:
	make finregid_map
	make lab_age