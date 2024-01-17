library(tibble)
library(lubridate)
library(dplyr)
source("/home/kdetrois/projects/utils/print_utils.R")

base_path <- "/home/kdetrois/projects/kanta_lab_processing/processed/sumstats/"
lab_counts_path <- "/home/kdetrois/projects/kanta_lab_stats/results/lab_counts/"
date <- "v2_2023-12-12"
out_path <- "/data/projects/project_kdetrois/"

###### Getting lab IDs and abbreviations for at least 5 individuals
# Mapping after adding new OMOPs
lab_data <- tibble::as_tibble(data.table::fread(paste0(lab_counts_path, "kanta_lab_", date, "lab_id_omop_id_counts.tsv"), sep="\t"))
# Previous data contains indv counts
lab_counts <- tibble::as_tibble(data.table::fread(paste0(base_path, "kanta_lab_", date, "_lab_counts.csv")))
# Joining
lab_counts <- dplyr::rename(lab_counts, LAB_ABBREVIATION=LAB_ABBRV)
lab_data <- dplyr::left_join(lab_data, lab_counts %>% dplyr::select(-OMOP_ID, -OMOP_NAME), by=c("LAB_ID", "LAB_ABBREVIATION", "LAB_UNIT"))
# Filtring for at least 5 individuals
lab_data <- dplyr::filter(lab_data, INDV_COUNT >= 5)
lab_data <- dplyr::rename(lab_data, LAB_INDV_COUNT=INDV_COUNT)

omop_sumstats <- tibble::as_tibble(data.table::fread(paste0(base_path, "kanta_lab_", date, "_omop_sumstats.tsv")))
omop_sumstats %>% dplyr::group_by(OMOP_ID, LAB_UNIT)
omop_sumstats$LAB_UNIT[omop_sumstats$LAB_UNIT == ""] <- NA
omop_sumstats$OMOP_ID <- as.character(omop_sumstats$OMOP_ID)
omop_sumstats <- dplyr::left_join(omop_sumstats, lab_data %>% dplyr::select(-COUNT), by=c("LAB_UNIT", "OMOP_ID", "OMOP_NAME"))

omop_sumstats <- dplyr::group_by(omop_sumstats, OMOP_ID, OMOP_NAME, LAB_UNIT) %>% 
                  dplyr::summarise(MEAN=MEAN,
                                   MEDIAN=MEDIAN,
                                   SD=SD,
                                   N_ELEMS=N_ELEMS, N_INDVS=N_INDVS,
                                   FIRST_QUANTILE=FIRST_QUANTILE,
                                   THIRD_QUANTILE=THIRD_QUANTILE,
                                   MIN=MIN,
                                   MAX=MAX,
                                   LAB_INFO=paste0(LAB_ID, "_", LAB_ABBREVIATION, "_", LAB_INDV_COUNT, collapse=",")) %>%
                  dplyr::distinct()

omop_sumstats %>% dplyr::group_by(OMOP_ID, LAB_UNIT)
orig_n <- sum(omop_sumstats$N_ELEMS)
orig_indv <- sum(omop_sumstats$N_INDVS)
omop_sumstats <- dplyr::filter(omop_sumstats, N_INDVS >= 100)
writeLines(paste0("Filtering for at least 100 individuals per OMOP ID reduces number of entries from ", orig_n, " to ", sum(omop_sumstats$N_ELEMS), " (", round(orig_n-sum(omop_sumstats$N_ELEMS), 2), " less)"))
data.table::fwrite(omop_sumstats %>% dplyr::select(-FIRST_QUANTILE, -THIRD_QUANTILE, -MIN, -MAX), paste0(out_path, "v2_insights/kanta_lab_", date, "_omop_sumstats_min100indvs.tsv"), sep="\t")
data.table::fwrite(omop_sumstats, paste0(base_path, "kanta_lab_", date, "_omop_sumstats_min100indvs.tsv"), sep="\t")

# This removes binary information from lab  values where we have non-binary information
for(omop_id in unique(omop_sumstats$OMOP_ID)) {
  sub_data <- omop_sumstats %>% dplyr::filter(OMOP_ID == omop_id)
  sub_data <- dplyr::filter(sub_data, LAB_UNIT != "binary" | is.na(LAB_UNIT))
  if(nrow(sub_data) > 0) {
    omop_sumstats <- dplyr::filter(omop_sumstats, 
                                      (OMOP_ID != omop_id) | 
                                      (OMOP_ID == omop_id) & ((LAB_UNIT != "binary") | (is.na(LAB_UNIT))))
    } 
}

data.table::fwrite(omop_sumstats, paste0(out_path, "omop_sumstats/kanta_lab_", date, "_omop_sumstats_processed.tsv"), sep="\t")
data.table::fwrite(omop_sumstats, paste0(base_path, "kanta_lab_", date, "_omop_sumstats_processed.tsv"), sep="\t")

unmapped <- lab_data %>% dplyr::filter(is.na(OMOP_ID)) %>% dplyr::select(-OMOP_ID, -OMOP_NAME)
unmapped <- unmapped %>% dplyr::arrange(desc(LAB_COUNT))
unmapped <- dplyr::filter(unmapped, LAB_INDV_COUNT >= 100)
data.table::fwrite(unmapped %>% dplyr::arrange(desc(LAB_COUNT)), paste0("kanta_lab_", date, "_lab_id_unmapped.tsv"), sep="\t")
data.table::fwrite(unmapped %>% dplyr::arrange(desc(LAB_COUNT)), paste0(out_path, "v2_insights/", "kanta_lab_", date, "_lab_id_unmapped.tsv"), sep="\t")
paste0(out_path, "omop_sumstats/lab_id_unmapped_counts.tsv")

