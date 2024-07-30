#date = v2_2023-12-12
date = 2023-09-10

service_provider_map: 
	cat /media/volume/users/detrokir/processed/data/kanta_lab_$(date).tsv | awk '{FS="\t"}{if(NR == 1) print "YEAR\tLAB_SERVICE_PROVIDER\tN_LAB_SERVICE_PROVIDER"; else{count[substr($$2, 1, 4)"\t"$$3]++}}END{for(elem in count) print elem"\t"count[elem]}' > /media/volume/users/detrokir/insights/results/service_provider_year_count_$(date).tsv
	
service_provider_change:
	/opt/R-4.1.1/bin/Rscript /home/kdetrois/projects/kanta_lab_stats/scripts/provider_insights/provider_insights.R $(date)

service_provider_insight:
	make service_provider_map
	make service_provider_change
