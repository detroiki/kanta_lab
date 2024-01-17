source("/home/kdetrois/projects/utils/plot_utils.R")
library(dplyr)
library(lubridate)

date = "v2_2023-12-12"
out_dir <- "/home/kdetrois/projects/kanta_lab_stats/results/"

age_2020 <- tibble::as_tibble(data.table::fread(file=paste0(out_dir, "kanta_lab_", date, "_age_2020_tabl.tsv"), sep="\t"))

ggplot(age_2022, aes(x=AGE, y=TEST_COUNT/ALIVE_COUNT)) +
  geom_col() +
  theme_custom(base_size=18) +
  scale_x_continuous(breaks=seq(0, 85, by=10), limits=c(-1,81)) +
  scale_y_continuous(labels=function(x){paste0(x*100, "%")}, breaks=seq(0,1, by=0.2),limits=c(0,1)) +
  labs(x="Age", y="% of individuals with at least one measurement")
