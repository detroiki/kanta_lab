library(lubridate)
library(dplyr)
library(tibble)
source("/home/kdetrois/projects/utils/plot_utils.R")

date <- "v2_2023-12-12"

krea <- tibble::as_tibble(data.table::fread(paste0("/home/kdetrois/projects/biomarker_imputation/data_inspection/data/krea/kanta_lab_", date, "_krea.tsv")))
krea <- dplyr::filter(krea, LAB_UNIT == "µmol/l")
pheno_data <- tibble::as_tibble(data.table::fread("/data/processed_data/minimal_phenotype/minimal_phenotype_2023-08-14.csv"))
pheno_data <- dplyr::select(pheno_data, FINREGISTRYID, SEX, DATE_OF_BIRTH, DEATH_DATE) 
krea <- dplyr::left_join(krea, pheno_data %>% dplyr::select(FINREGISTRYID, DATE_OF_BIRTH, SEX))
krea <- dplyr::mutate(krea, LAB_AGE=lubridate::time_length(DATE_OF_BIRTH %--% LAB_DATE_TIME, "years"))
rm(pheno_data)
gc()
krea <- dplyr::ungroup(krea) %>% dplyr::mutate(krea, GFR=case_when(
  krea$SEX == 0 & krea$LAB_VALUE <= 80 ~ 141*((krea$LAB_VALUE/79.6)**(-0.411))*((0.993)**krea$LAB_AGE),
  krea$SEX == 0 & krea$LAB_VALUE > 80 ~ 141*((krea$LAB_VALUE/79.6)**(-1.209))*((0.993)**krea$LAB_AGE),
  krea$SEX == 1 & krea$LAB_VALUE <= 62 ~ 144*((krea$LAB_VALUE/61.9)**(-0.329))*((0.993)**krea$LAB_AGE),
  krea$SEX == 1 & krea$LAB_VALUE > 62 ~ 144*((krea$LAB_VALUE/61.9)**(-1.209))*((0.993)**krea$LAB_AGE),
))
data.table::fwrite(krea, paste0("/home/kdetrois/projects/biomarker_imputation/data_inspection/data/krea/kanta_lab_", date, "_krea_egfr.tsv"), sep="\t")
