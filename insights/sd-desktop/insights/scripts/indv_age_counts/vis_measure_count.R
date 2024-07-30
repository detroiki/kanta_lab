source("/home/kdetrois/projects/utils/plot_utils.R")
source("/home/kdetrois/projects/utils/print_utils.R")

source_dir <- "/home/kdetrois/projects/kanta_lab_stats/results/service_providers/"
provider_years <- tibble::as_tibble(data.table::fread(paste0(source_dir, "kanta_lab_", date, "_service_provider_year_count.tsv")))
year_counts <- provider_years %>% group_by(YEAR) %>% summarise(N_LAB_SERVICE_PROVIDER=sum(N_LAB_SERVICE_PROVIDER))
ggplot(year_counts, aes(x=YEAR, y=N_LAB_SERVICE_PROVIDER)) +
  geom_col() +
  theme_custom() +
  scale_x_continuous(breaks=2014:2022) +
  scale_y_continuous(labels=function(x) so_formatter(x), limits=c(0,15e7)) +
  labs(x="Year", y="Total number of measurments")

sum(year_counts$N_LAB_SERVICE_PROVIDER)
