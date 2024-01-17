date = v2_2023-12-12
in_dir = /home/kdetrois/projects/kanta_lab_processing/processed/data/
out_dir = /home/kdetrois/projects/kanta_lab_stats/results/lab_counts/

create_map:
  cat $(in_dir)kanta_lab_$(date)_new_omop.tsv | awk '{FS="\t";OFS="\t"}{count[$4"\t"$6"\t"$8"\t"$9"\t"$"\t10"]++; count[$4"\t"$6"\t"$8"\t"$9"\t"$"\t10"]++}END{print "LAB_ID\tLAB_ABBREVIATION\tLAB_UNIT\tOMOP_ID\tOMOP_NAME\tCOUNT"; for(elem in count) print elem, count[elem]}' > $(out_dir)kanta_lab_$(date)_lab_id_omop_id_counts.tsv