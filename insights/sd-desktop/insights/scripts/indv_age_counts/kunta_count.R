#### SETTINGS
personal_dir <- "/media/volume/users/detrokir/"
setwd(personal_dir)
utils_dir <- paste0(personal_dir, "utils/")

source(paste0(utils_dir, "utils_plot.R"))
library(dplyr)
library(lubridate)
library(ggplot2)

sd_connect_dir <- paste0(personal_dir, "sd_connect_data/")
living_hist_file_name <- "living_history_2023-06-14.csv"
living_hist_path <- paste0(sd_connect_dir, living_hist_file_name)
out_dir <- "/media/volume/users/detrokir/projects/kanta_lab/insights/results/"
out_table_dir <- paste0(out_dir, "tables/")
out_plot_dir <- paste0(out_dir, "plots/")

date = "2023-09-10"
kanta_file_start <- paste0("kanta_lab_", date, "_")

#### MAIN
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
gc()

finregids <- tibble::as_tibble(data.table::fread(paste0(out_table_dir, kanta_file_start, "finregistry_id_count.tsv")))
print(finregids)
finregids <- dplyr::group_by(finregids, FINREGISTRYID) %>% 
                dplyr::filter(lubridate::year(LAB_DATE_TIME) == 2019) %>%
                dplyr::select(FINREGISTRYID, COUNT) %>% 
                dplyr::distinct() %>% 
                dplyr::summarise(COUNT=sum(COUNT))
print(finregids)

#krea <- tibble::as_tibble(data.table::fread("/home/kdetrois/projects/biomarker_imputation/data_inspection/data/krea/kanta_lab_v2_2023-12-12_krea_egfr.tsv"))
#krea_2019 <- dplyr::filter(
#krea <- lubridatedplyr::group_by(krea, FINREGISTRYID) %>% dplyr::summarise(COUNT_KREA=n()) %>% dplyr::distinct()
#print(krea)

#writeLines(paste0("Not in living: ", nrow(finregids %>% dplyr::filter(FINREGISTRYID %in% living$FINREGISTRYID))))
#writeLines(paste0("Not in KANTA lab: ", nrow(living %>% dplyr::filter(FINREGISTRYID %in% finregids$FINREGISTRYID))))

print("Joining")
living <- dplyr::left_join(living, finregids, by="FINREGISTRYID")
rm(finregids)
gc()
living <- dplyr::left_join(living, krea, by="FINREGISTRYID")
rm(krea)
gc()
kunta <- dplyr::group_by(living, MUNICIPALITY_NAME) %>% 
              dplyr::summarise(MUNICIPALITY, MUNICIPALITY_NAME, N_LIVING=n(), N_LAB=sum(!is.na(COUNT)), N_KREA=sum(!is.na(COUNT_KREA))) %>% 
              dplyr::distinct()
kunta <- dplyr::filter(kunta, N_LIVING >= 5, N_LAB >= 5)

data.table::fwrite(kunta, paste0(out_dir, kanta_file_start, kunta_counts.tsv"), sep="\t")
