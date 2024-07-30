source("/home/kdetrois/projects/utils/plot_utils.R")

krea_low <- tibble::as_tibble(data.table::fread("/home/kdetrois/projects/biomarker_imputation/data_inspection/data/krea/kanta_lab_v2_2023-12-05_krea_low_egfr.csv", nrows=100))
krea_low <- dplyr::mutate(krea_low, X=(GFR<60))
krea_low <- dplyr::group_by(krea_low, FINREGISTRYID) %>% dplyr::arrange(LAB_DATE_TIME) %>% dplyr::mutate(LAB_DATE_TIME=date(LAB_DATE_TIME))
low_stat <- krea_low %>% dplyr::summarise(X=X, LAB_DATE_TIME=LAB_DATE_TIME, TIME_DIFF=c(0, diff(LAB_DATE_TIME)))
# Marking continuous measurements
low_stat <- low_stat %>% dplyr::mutate(CONT_X=X&lag(X, 1))
# Adding up continuous measurement time intervals
low_stat <- dplyr::mutate(low_stat, CONT_TIME_DIFF=ifelse(CONT_X, lag(TIME_DIFF, 1)+TIME_DIFF, TIME_DIFF))
low_stat[is.na(low_stat)] <- 0
# Looking at low eGFR with time intervals > 90
low_stat <- dplyr::mutate(low_stat, CKD_DATA=CONT_TIME_DIFF > 90 & CONT_X)
#381,909 CKDS
endpointer <- tibble::as_tibble(data.table::fread("/data/processed_data/endpointer/R10/densified_first_events_DF10_no_omits_2022-09-20.csv", nrows=5000000))
endpointer <- dplyr::filter(endpointer, ENDPOINT == "N14_CHRONKIDNEYDIS") %>% dplyr::mutate(CKD=TRUE) %>% dplyr::select(FINREGISTRYID, CKD)
endpointer <- filter(endpointer, FINREGISTRYID %in% unique(krea$FINREGISTRYID))
endpointer <- dplyr::left_join(endpointer, low_stat %>% dplyr::select(FINREGISTRYID, CKD_DATA))
data.table::endpointer("/home/kdetrois/projects/biomarker_imputation/data_inspection/results/ckd_diags.tsv", sep="\t")

## Plot the trajectories
set.seed(1232)
random_indvs <- sample(unique(krea_low$FINREGISTRYID), 6, replace=FALSE)
krea_random <- dplyr::filter(krea_low, FINREGISTRYID %in% random_indvs)
krea_random <- krea_random %>%
  dplyr::group_by(FINREGISTRYID) %>%
  dplyr::mutate(BASE_DATE=min(LAB_DATE_TIME)) %>% 
  dplyr::mutate(BASE_DISTANCE=lubridate::time_length(BASE_DATE %--% LAB_DATE_TIME, "days"))

# Adding random variables to days
rand_days <- floor(rnorm(n=nrow(krea_random), mean=0, sd=0.1))
krea_random <-  dplyr::ungroup(krea_random) %>% 
  dplyr::mutate(krea_random, BASE_DIST_RAND=((krea_random$BASE_DISTANCE+rand_days)/365.25))
# Adding random variables to value
rand_value <- rnorm(nrow(krea_random), mean=0, sd=6)
krea_random <- dplyr::mutate(krea_random, RAND_VALUE=(krea_random$LAB_VALUE + rand_value),
                             krea_random, RAND_GFR=abs(krea_random$GFR + rand_value))

#### Hand picking colors for the individuals
id_colors <- as.vector(custom_colors_brewer(6))

#### Plotting trajectories #########
ggplot(krea_random, aes(x=BASE_DIST_RAND, y=RAND_GFR, color=as.factor(FINREGISTRYID))) +
  geom_point(size=3) +
  geom_line(alpha=0.5, size=2) +
  scale_color_manual(values=id_colors) +
  geom_hline(yintercept=60, linetype="dashed", alpha=0.5) +
  theme_custom(base_size=18) +
  theme(legend.position="none") +
  scale_x_continuous(breaks=0:8, limits=c(-0.1,8)) +
  scale_y_continuous(breaks=c(0, 30, 60, 90, 120, 150), limits=c(0,150)) +
  labs(y="eGFR (ml/min/1.73m^2)", 
       x="Years After First Measurement for Individual", 
       title="Estimated Glomerular Filtration Rate (eGFR)", 
       subtitle="Individual Trajectories", 
       caption="Randomized eGFR +/- N(0, 6) and Days +/- N(0, 1)") 
#ggsave(filename="/home/kdetrois/projects/biomarker_imputation/data_inspection/results/low_egfr_traj.pdf", width=1000, height=700)
