library(lubridate)
library(dplyr)
library(tibble)
source("/home/kdetrois/projects/utils/plot_utils.R")
source("/home/kdetrois/projects/utils/print_utils.R")

krea <- tibble::as_tibble(data.table::fread("/home/kdetrois/projects/biomarker_imputation/data_inspection/data/krea/kanta_lab_v2_2023-12-12_krea_egfr.tsv"))
egfr_tib <- dplyr::mutate(krea, eGFR_round=round(GFR)) %>% dplyr::group_by(eGFR_round) %>% dplyr::summarise(N=n()) %>% dplyr::filter(N>=5)
density <- ggplot(egfr_tib, aes(x=eGFR_round, y=N)) +
  geom_line(size=1.5, alpha=0.8) +
  geom_vline(xintercept=60, linetype="dashed") + 
  geom_ribbon(data=subset(egfr_tib, eGFR_round < 60), aes(ymin=0, ymax=N), alpha=0.5, fill=custom_colors_brewer(2)[1]) +
  theme_custom(base_size=18) +
  theme(legend.position="none",
        axis.text.y=element_blank(),
        axis.ticks.y=element_blank()) +
  labs(y="Density", 
       x="eGFR (ml/min/1.73m^2)", 
       title="", 
       subtitle="") 
density

set.seed(29268)

random_indvs <- sample(unique(endpointer$FINREGISTRYID), 6, replace=FALSE)
unique(krea_random$FINREGISTRYID)
krea_random <- dplyr::filter(krea, FINREGISTRYID %in% random_indvs)
# Counting from first measurement for each individual
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
id_colors <- custom_colors_brewer(6)
names(id_colors) <- random_indvs
traj <- ggplot(krea_random, aes(x=BASE_DIST_RAND, y=RAND_GFR, color=as.factor(FINREGISTRYID))) +
  geom_point(size=3) +
  geom_line(alpha=0.6, size=2) +
  scale_color_manual(values=id_colors) +
  geom_hline(yintercept=60, linetype="dashed", alpha=0.5) +
  theme_custom(base_size=18) +
  theme(legend.position="none") +
  scale_x_continuous(breaks=0:8, limits=c(-0.1,8)) +
  scale_y_continuous(breaks=c(0, 30, 60, 90, 120, 150), limits=c(0,150)) +
  labs(y="eGFR (ml/min/1.73m^2)", 
       x="Years after first measurement", 
       #title="Estimated Glomerular Filtration Rate (eGFR)", 
       subtitle="Individual trajectories")
       #caption="Randomized eGFR +/- N(0, 6) and Days +/- N(0, 1)") 
print(traj)

krea <- dplyr::mutate(krea, YEAR=lubridate::year(LAB_DATE_TIME))
indvs_measure <- krea %>% group_by(FINREGISTRYID) %>% summarise(N_MEASURE=n(), YEARS=paste0(unique(YEAR), collapse=","), N_YEAR_MEASURE=length(unique(YEAR)))
years_measured <- indvs_measure %>% dplyr::group_by(N_YEAR_MEASURE) %>% dplyr::summarise(N=n(), has_2022=sum(stringr::str_detect(YEARS, "2022")))

counts <- ggplot(years_measured, aes(x=N_YEAR_MEASURE, y=N)) +
            geom_col() +
            theme_custom(base_size=18) +
            theme(legend.position="none") +
            labs(y="Number of individuals", 
                 x="Number of years with measurements", 
                 title="", 
                 subtitle="", 
                 caption="") +
            scale_y_continuous(labels = function(x) {so_formatter(x)}, n.breaks=7) +
            scale_x_continuous(breaks=0:9)
print(counts)

library(gridExtra)
grid.arrange(density, counts, traj, ncol=2, layout_matrix = cbind(c(1,3), c(2,3)))

krea <- dplyr::group_by(krea, FINREGISTRYID)
n_elems <- krea %>% dplyr::summarise(N=n())
n_elems <- dplyr::group_by(n_elems, N) %>% dplyr::summarise(N_indvs=n()) %>% dplyr::filter(N_indvs >= 5)
counts <- ggplot(n_elems, aes(x=N, y=N_indvs)) +
  geom_col() +
  theme_custom(base_size=18) +
  theme(legend.position="none") +
  labs(y="Number of individuals", 
       x="Number of measurements", 
       title="", 
       subtitle="", 
       caption="") 
print(counts)
