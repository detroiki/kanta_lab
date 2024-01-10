source("/home/kdetrois/projects/utils/plot_utils.R")
library(dplyr)
library(lubridate)

args <- commandArgs(trailingOnly = TRUE)
date <- args[1]
date = "v2_2023-12-12"

# All individuals alive and age at the start of 2020
pheno_data <- tibble::as_tibble(data.table::fread("/data/processed_data/minimal_phenotype/minimal_phenotype_2023-08-14.csv"))
pheno_data <- dplyr::select(pheno_data, FINREGISTRYID, SEX, DATE_OF_BIRTH, DEATH_DATE) 
pheno_data <- dplyr::filter(pheno_data, is.na(DEATH_DATE))
pheno_data <- dplyr::mutate(pheno_data, AGE_2022=round(lubridate::time_length(DATE_OF_BIRTH %--% as.Date("2019/12/31"), "years"), 3))

# Individuals alive in each age group
alive_tab <- table(pheno_data$AGE_2022)
alive_tib <- tibble::tibble(AGE=names(alive_tab), ALIVE_COUNT=as.numeric(alive_tab))

# Individuals with lab data
finregids <- tibble::as_tibble(data.table::fread(paste0("/home/kdetrois/projects/kanta_lab_stats/results/finregistry_id_count_", date, ".tsv")))
finregids <- unique(finregids$FINREGISTRYID)
writeLines(paste0("Number of individuals: ", length(finregids)))

# Individuals with lab data in each age group
pheno_data <- dplyr::filter(pheno_data, FINREGISTRYID %in% finregids)
test_tab <- table(pheno_data$AGE_2022)
test_tib <- tibble::tibble(AGE=names(test_tab), TEST_COUNT=as.numeric(test_tab))

# Join and write
tib <- dplyr::left_join(alive_tib, test_tib, by="AGE")
tib <- dplyr::mutate(tib, AGE=round(as.numeric(AGE))) %>% dplyr::group_by(AGE) %>% dplyr::summarise(ALIVE_COUNT=sum(ALIVE_COUNT), TEST_COUNT=sum(TEST_COUNT))
data.table::fwrite(tib, file=paste0("/home/kdetrois/projects/kanta_lab_stats/results/age_2020_table_", date, ".tsv"), sep="\t")