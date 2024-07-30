#### SETTINGS
personal_dir <- "/media/volume/users/detrokir/"
setwd(personal_dir)
utils_dir <- paste0(personal_dir, "utils/")

source(paste0(utils_dir, "utils_plot.R"))
library(dplyr)
library(lubridate)
library(ggplot2)

sd_connect_dir <- paste0(personal_dir, "sd_connect_data/")
minimal_pheno_file_name <- "minimal_phenotype_2023-08-14.csv"
minimal_pheno_path <- paste0(sd_connect_dir, minimal_pheno_file_name)
out_dir <- "/media/volume/users/detrokir/projects/kanta_lab/insights/results/"
out_table_dir <- paste0(out_dir, "tables/")
out_plot_dir <- paste0(out_dir, "plots/")

date = "2023-09-10"
kanta_file_start <- paste0("kanta_lab_", date, "_")

#### MAIN
lab_finregids <- tibble::as_tibble(data.table::fread(paste0(out_dir, kanta_file_start, "finregistry_id_count.tsv")))
lab_finregids <- unique(lab_finregids$FINREGISTRYID)

pheno_data <- tibble::as_tibble(data.table::fread(minimal_pheno_path))
pheno_data <- dplyr::select(pheno_data, FINREGISTRYID, SEX, DATE_OF_BIRTH, DEATH_DATE) 
pheno_data <- dplyr::mutate(pheno_data, AGE_2020=round(lubridate::time_length(DATE_OF_BIRTH %--% as.Date("2020/01/01"), "years"), 3))
pheno_data <- dplyr::filter(pheno_data, is.na(DEATH_DATE))
pheno_data %>% dplyr::group_by(FINREGISTRYID)

orig_tab = table(pheno_data$AGE_2020)
orig_tib <- tibble::tibble(AGE_2020=names(orig_tab), ALIVE_COUNT=as.numeric(orig_tab)) 
orig_tib

pheno_data <- dplyr::filter(pheno_data, FINREGISTRYID %in% lab_finregids)
gc()
lab_tab <- table(pheno_data$AGE_2020)
lab_tib <- tibble::tibble(AGE_2020=names(lab_tab), LAB_COUNT=as.numeric(lab_tab))
lab_tib

final_tib <- dplyr::left_join(orig_tib, lab_tib, by="AGE_2020")
final_tib
final_tib <- final_tib %>% 
              dplyr::mutate(AGE_2020=round(as.numeric(AGE_2020))) %>% 
              dplyr::group_by(AGE_2020) %>% 
              dplyr::summarise(ALIVE_COUNT=sum(ALIVE_COUNT), LAB_COUNT=sum(LAB_COUNT))
final_tib

data.table::fwrite(final_tib, file=paste0(out_table_dir, kanta_file_start, "age_2020.tsv"), sep="\t")

plt <- ggplot(final_tib, aes(x=AGE_2020, y=LAB_COUNT/ALIVE_COUNT)) +
        geom_col() +
        theme_custom(base_size=18) +
        scale_x_continuous(breaks=seq(0, 85, by=10), limits=c(-1,81)) +
        scale_y_continuous(labels=function(x){paste0(x*100, "%")}, breaks=seq(0,1, by=0.2),limits=c(0,1)) +
        labs(x="Age", y="% of individuals with at least one measurement")
plt
ggsave(plt, 
       file=paste0(out_plot_dir, kanta_file_start, "age_2020.png"),
       device="png",
       dpi=600,
       bg="white",
       width=10,
       height=7)

  