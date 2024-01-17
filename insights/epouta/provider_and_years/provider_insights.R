library(lubridate)
library(dplyr)
source("/home/kdetrois/projects/utils/plot_utils.R")

args <- commandArgs(trailingOnly = TRUE)
date <- args[1]
date <- "v2_2023-12-12"

provider_years <- tibble::as_tibble(data.table::fread(paste0("/home/kdetrois/projects/kanta_lab_stats/results/service_provider_year_count_", date, ".tsv")))
provider_table <- tibble::as_tibble(data.table::fread("/home/kdetrois/projects/kanta_lab_processing/data/thl_sote_map_named.tsv", ))

provider_years <- dplyr::arrange(provider_years, YEAR, desc(N_LAB_SERVICE_PROVIDER)) %>% dplyr::filter(N_LAB_SERVICE_PROVIDER > 100)
provider_years <- dplyr::left_join(provider_years, provider_table %>% dplyr::select(LAB_NAME, OrganizationId, Abbreviation), by=c("LAB_SERVICE_PROVIDER"="LAB_NAME"))
data.table::fwrite(provider_years, paste0("/home/kdetrois/projects/kanta_lab_stats/scripts/provider_insights/service_provider_year_count_g100_", date, ".tsv"), sep="\t")

all_years <- tibble(YEAR=numeric(), LAB_SERVICE_PROVIDER=character(), N_LAB_SERVICE_PROVIDER=numeric())
prev_years <- dplyr::filter(provider_years, YEAR == 2014)

for(year in 2015:2022) {
  crnt_year <- dplyr::filter(provider_years, YEAR == year)
  
  crnt_year <- dplyr::filter(crnt_year, !(LAB_SERVICE_PROVIDER %in% prev_years$LAB_SERVICE_PROVIDER))
  all_years <- dplyr::bind_rows(all_years, crnt_year)
  prev_years <- dplyr::bind_rows(prev_years, crnt_year)
}

all_years <- na.omit(all_years)
all_years %>% group_by(YEAR) %>% summarise(n())
data.table::fwrite(all_years, paste0("/home/kdetrois/projects/kanta_lab_stats/scripts/provider_insights/service_provider_year_count_new_g100_", date, ".tsv"), sep="\t")