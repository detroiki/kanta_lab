library(ggplot2)
source("/home/kdetrois/projects/utils/print_utils.R")
source("/home/kdetrois/projects/utils/plot_utils.R")

provider_years <- tibble::as_tibble(data.table::fread(paste0("/home/kdetrois/projects/kanta_lab_stats/results/service_provider_year_count_", date, ".tsv")))
years <- dplyr::group_by(provider_years, YEAR) %>% dplyr::summarise(COUNT=sum(N_LAB_SERVICE_PROVIDER))
years <- dplyr::filter(years, YEAR < 2023)
ggplot2::ggplot(years, aes(x=YEAR, y=COUNT)) +
  geom_col() +
  labs(x="Year", y="", title="Cummulative number of tests") +
  scale_y_continuous(labels=function(x) {so_formatter(x)}, breaks=c(50000000, 100000000, 150000000), limits=c(0,150000000)) +
  scale_x_continuous(breaks=2014:2022) +
  theme_custom(base_size=18) s