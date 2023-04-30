# Data Sources
- `/media/volume/users/detrokir/processed/reports/counts/all_finregids.csv`
	- List of all Finregistry IDs in the first 1-5 Kanta Lab Value files and the number of measurements after removing duplicates and missing values
- `/media/volume/users/detrokir/processed/data/all_minimal_down_100K_prep.csv`
	- Contains information about each lab measurement
# Preprocessing
- All distribution plots were created by col_tabs how many individuals have entries for the value plotted and the filtering out all values with less than 5 individuals. 
	- Using function remove_low_counts at `/media/volume/users/detrokir/R_scripts/utils/utils.R`.

# Smallest N
- ages_all_tall.tsv - 29558
- age_tally_all.png - 29558
- kalpro_distr.png - 5
- li_leuk.png - 5
- measure_count_all.png - 5
- regional_id_tally_100K.png - 5
