source("/home/kdetrois/projects/utils/plot_utils.R")
library(dplyr)
library(lubridate)

date = "v2_2023-12-12"
pheno_dir <- "/data/processed_data/minimal_phenotype/"
res_dir <- "/home/kdetrois/projects/kanta_lab_stats/results/"
living_hist_path <- "/data/processed_data/dvv/living_history_2023-06-14.csv"

living <- tibble::as_tibble(data.table::fread(living_hist_path))
print(living)
living <- dplyr::select(living, FINREGISTRYID, START_OF_RESIDENCE, END_OF_RESIDENCE, MUNICIPALITY, MUNICIPALITY_NAME, POST_CODE)
print(living)
living <- dplyr::filter(living, END_OF_RESIDENCE >= as.Date("2019/01/01") | is.na(END_OF_RESIDENCE))
print(living)
living <- dplyr::group_by(living, FINREGISTRYID)
print(living)
living <- dplyr::arrange(living, desc(END_OF_RESIDENCE), desc(START_OF_RESIDENCE))
print(living)
living <- dplyr::slice(living, 1L)
print(living)

finregids <- tibble::as_tibble(data.table::fread(paste0(res_dir, "/indv_age_counts/kanta_lab_", date, "_finregid_counts.tsv")))
print(finregids)
finregids <- dplyr::group_by(finregids, FINREGISTRYID) %>% dplyr::select(FINREGISTRYID, COUNT) %>% dplyr::distinct() %>% dplyr::summarise(COUNT=sum(COUNT))
print(finregids)

krea <- tibble::as_tibble(data.table::fread("/home/kdetrois/projects/biomarker_imputation/data_inspection/data/krea/kanta_lab_v2_2023-12-12_krea_egfr.tsv"))
krea_2019 <- dplyr::filter(
krea <- lubridatedplyr::group_by(krea, FINREGISTRYID) %>% dplyr::summarise(COUNT_KREA=n()) %>% dplyr::distinct()
print(krea)

#writeLines(paste0("Not in living: ", nrow(finregids %>% dplyr::filter(FINREGISTRYID %in% living$FINREGISTRYID))))
#writeLines(paste0("Not in KANTA lab: ", nrow(living %>% dplyr::filter(FINREGISTRYID %in% finregids$FINREGISTRYID))))

print("Joining")
living <- dplyr::left_join(living, finregids, by="FINREGISTRYID")
rm(finregids)
gc()
living <- dplyr::left_join(living, krea, by="FINREGISTRYID")
rm(krea)
gc()
living <- dplyr::group_by(living, MUNICIPALITY_NAME) %>% 
            dplyr::summarise(MUNICIPALITY, MUNICIPALITY_NAME, N_LIVING=n(), N_LAB=sum(!is.na(COUNT)), N_KREA=sum(!is.na(COUNT_KREA))) %>% 
            dplyr::distinct()
living <- dplyr::filter(living, N_LIVING >= 5, N_LAB >= 5)

data.table::fwrite(living, paste0("/data/projects/project_kdetrois/v2_insights/kanta_lab_", date, "_municipality_counts.tsv"), sep="\t")
data.table::fwrite(living, paste0(res_dir, "/indv_age_counts/kanta_lab_", date, "_municipality_counts.tsv"), sep="\t")