# Data Sources
- `/media/volume/users/detrokir/processed/reports/counts/all_finregids.csv`
	- List of all Finregistry IDs in the first 1-5 Kanta Lab Value files and the number of measurements after removing duplicates and missing values
- `/media/volume/users/detrokir/processed/data/all_minimal_down_100K_prep.csv`
	- Contains information about each lab measurement
# Preprocessing
- All distribution plots were created by col_tabs how many individuals have entries for the value plotted and the filtering out all values with less than 5 individuals. 
	- Using function remove_low_counts at `/media/volume/users/detrokir/R_scripts/utils/utils.R`.
	- In case of GFR this is all based on rounded values
- In the case of stratified plots the same was done for each strata
- The individual trajectories have been randomized, the individuals are randomly picked and for each day and value I added a random number drawn from a normal distribution denoted in the caption.

# Smallest N
- gfr_distr.png - 5
- gfr_diag_comp.png - 5
- pkrea_distr_age_strata.png - 5
- ldl_distr.png - 5
- ldl_diag_comp.png - 5
- pkrea_indv_trajectory.png - 1 (with randomization)
- ldl_indv_trajectory.png - 1 (with randomization)
