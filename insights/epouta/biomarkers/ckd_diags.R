source("/home/kdetrois/projects/utils/plot_utils.R")
library(dplyr)
library(lubridate)

krea_low <- tibble::as_tibble(data.table::fread("/home/kdetrois/projects/biomarker_imputation/data_inspection/data/krea/kanta_lab_v2_2023-12-05_krea_low_egfr.csv"))
krea_low <- dplyr::mutate(krea_low, X=(GFR<60))
sum(krea_low$X)
krea_low <- dplyr::group_by(krea_low, FINREGISTRYID) %>% dplyr::arrange(LAB_DATE_TIME) %>% dplyr::mutate(LAB_DATE_TIME=lubridate::date(LAB_DATE_TIME))
low_stat <- krea_low %>% dplyr::summarise(X=X, LAB_DATE_TIME=LAB_DATE_TIME, TIME_DIFF=c(0, diff(LAB_DATE_TIME)))
# Marking continuous measurements
low_stat <- low_stat %>% dplyr::mutate(CONT_X=X&lag(X, 1))
# Adding up continuous measurement time intervals
low_stat <- dplyr::mutate(low_stat, CONT_TIME_DIFF=ifelse(CONT_X, lag(TIME_DIFF, 1)+TIME_DIFF, TIME_DIFF))
low_stat[is.na(low_stat)] <- 0
# Looking at low eGFR with time intervals > 90
low_stat <- dplyr::mutate(low_stat, CKD_DATA=CONT_TIME_DIFF > 90 & CONT_X)
low_stat <- dplyr::group_by(low_stat, FINREGISTRYID) %>% dplyr::summarise(CKD_DATA=any(CKD_DATA))
# Getting official diagnoses
endpointer <- tibble::as_tibble(data.table::fread("/data/processed_data/endpointer/R10/densified_first_events_DF10_no_omits_2022-09-20.csv"))
endpointer <- dplyr::filter(endpointer, ENDPOINT == "N14_CHRONKIDNEYDIS") %>% dplyr::mutate(CKD=TRUE) %>% dplyr::select(FINREGISTRYID, CKD)
endpointer <- dplyr::full_join(endpointer, low_stat %>% dplyr::select(FINREGISTRYID, CKD_DATA))

data.table::fwrite(endpointer, "/home/kdetrois/projects/biomarker_imputation/data_inspection/results/ckd_diags.tsv", sep="\t")
