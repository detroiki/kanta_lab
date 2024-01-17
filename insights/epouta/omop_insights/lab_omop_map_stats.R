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
lab_data <- tibble::as_tibble(data.table::fread(paste0(lab_counts_path, "kanta_lab_", date, "_lab_id_omop_id_counts.tsv"), sep="\t"))
# Previous data contains indv counts
lab_data_prev <- tibble::as_tibble(data.table::fread(paste0(base_path, "kanta_lab_", date, "_lab_counts.csv")))
# Joining
lab_data_prev <- dplyr::rename(lab_data_prev, LAB_ABBREVIATION=LAB_ABBRV)
lab_data <- dplyr::left_join(lab_data, lab_data_prev %>% dplyr::select(-OMOP_ID, -OMOP_NAME), by=c("LAB_ID", "LAB_ABBREVIATION", "LAB_UNIT"))
# Filtring for at least 5 individuals
lab_data <- dplyr::filter(lab_data, INDV_COUNT >= 5)
lab_data <- dplyr::rename(lab_data, LAB_INDV_COUNT=INDV_COUNT)

####### Stats
# Original mapping
writeLines(paste0("Originally mapped OMOP rows: " , sum(lab_data_prev %>% filter(!is.na(OMOP_ID)) %>% pull(LAB_COUNT))))
writeLines(paste0("Makes ", round(sum(lab_data_prev %>% filter(!is.na(OMOP_ID)) %>% pull(LAB_COUNT)) / sum(lab_counts$LAB_COUNT)*100, 1), "% of rows"))
writeLines("Top unmapped Lab IDs and abbreviations: ")
lab_data_prev %>% filter(is.na(OMOP_ID)) %>% group_by(LAB_ID, LAB_ABBREVIATION) %>% summarise(sum=sum(LAB_COUNT)) %>% arrange(desc(sum))

# Final mapping
writeLines(paste0("Final mapped OMOP rows: " , sum(lab_data %>% filter(!is.na(OMOP_ID)) %>% pull(LAB_COUNT))))
writeLines(paste0("Makes ", round(sum(lab_data %>% filter(!is.na(OMOP_ID)) %>% pull(LAB_COUNT)) / sum(lab_counts$LAB_COUNT)*100, 1), "% of rows"))
writeLines("Top unmapped Lab IDs and abbreviations: ")
lab_data %>% filter(is.na(OMOP_ID)) %>% group_by(LAB_ID, LAB_ABBREVIATION) %>% summarise(sum=sum(LAB_COUNT)) %>% arrange(desc(sum))

# Final with at least 50 individuals
lab_data_g100 <- dplyr::filter(lab_data, LAB_INDV_COUNT >= 100)
writeLines(paste0("Final mapped OMOP rows: " , sum(lab_data_g100 %>% filter(!is.na(OMOP_ID)) %>% pull(LAB_COUNT))))
writeLines(paste0("Removes " , sum(lab_data %>% filter(!is.na(OMOP_ID)) %>% pull(LAB_COUNT)) - sum(lab_data_g100 %>% filter(!is.na(OMOP_ID)) %>% pull(LAB_COUNT)), " rows."))
writeLines(paste0("Makes ", round(sum(lab_data_g100 %>% filter(!is.na(OMOP_ID)) %>% pull(LAB_COUNT)) / sum(lab_counts$LAB_COUNT)*100, 1), "% of rows"))
writeLines("Top unmapped Lab IDs and abbreviations: ")
lab_data_g100 %>% filter(is.na(OMOP_ID)) %>% group_by(LAB_ID, LAB_ABBREVIATION) %>% summarise(sum=sum(LAB_COUNT)) %>% arrange(desc(sum))

# Lab IDs mapped
n_lab_ids_mapped <- length(dplyr::filter(lab_data_g100, !is.na(OMOP_ID)) %>% dplyr::pull(LAB_ID) %>% unique())
n_lab_ids <- length(dplyr::pull(lab_data_g100, LAB_ID) %>% unique())
writeLines(paste0(n_lab_ids_mapped, " Lab IDs mapped of ", n_lab_ids, " total lab IDs (", round(n_lab_ids_mapped / n_lab_ids, 3)*100, "%)"))

n_lab_ids_mapped <- length(dplyr::filter(lab_data, !is.na(OMOP_ID)) %>% dplyr::pull(LAB_ID) %>% unique())
n_lab_ids <- length(dplyr::pull(lab_data, LAB_ID) %>% unique())
writeLines(paste0(n_lab_ids_mapped, " Lab IDs mapped of ", n_lab_ids, " total lab IDs (", round(n_lab_ids_mapped / n_lab_ids, 3)*100, "%)"))
