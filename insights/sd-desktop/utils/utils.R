load_pckgs <- function() {
  base_path <- "/media/volume/users/detrokir/"
  source(paste0(base_path, "R_scripts/utils/utils_plotting.R"))
  source(paste0(base_path, "R_scripts/utils/utils_print.R"))
  library(tidyverse)
  library(data.table)
  library(ggplot2)
  #install.packages("/media/volume/users/detrokir/R_pckgs/latex2exp_0.5.0.tar.gz", repos=NULL, lib="/home/detrokir/lib/")
  #library(feather, lib="/home/detrokir/lib/")
  #library(latex2exp, lib="/home/detrokir/lib/")
  library(cowplot)
  library(ggridges)
  library(plotrix)
  library(lubridate)
}
pct_format <- scales::label_percent(accuracy=0.1)

get_n_alive <- function() {
  birth_data <- data.table::fread("/media/volume/users/detrokir/data/minimal_phenotype_20221216.csv")
  birth_data <- birth_data %>% dplyr::filter(is.na(death_date))
  n_alive <- nrow(birth_data)
  return(n_alive)
}

make_tab_tib <- function(data, col) {
  tab <- table(data[,get(col)], useNA="always")
  tib <- tibble::tibble(NAME=names(tab), COUNT=as.numeric(tab)) %>% 
          dplyr::arrange(desc(COUNT)) %>% 
          dplyr::mutate(PERCENTAGE=pct_format(COUNT/sum(COUNT)))
  colnames(tib)[1] <- col
  return(tib)
}

get_down_data <- function() {
  lab_data <- readr::read_delim("/media/volume/users/detrokir/processed/data/all_minimal_down_100K.csv")
  
  lab_data$NAME <- stringr::str_to_lower(lab_data$NAME)
  lab_data$NAME <- stringr::str_replace_all(lab_data$NAME, " ", "")
  
  lab_data$NAME <- stringr::str_remove_all(lab_data$NAME, "\\*") 
  lab_data$NAME <- stringr::str_remove_all(lab_data$NAME, "\\#") 
  
  lab_data <- dplyr::mutate(lab_data, ABBREVIATION=ifelse(ID_SOURCE == 0, NAME, ABBREVIATION))
  lab_data <- dplyr::select(lab_data, -NAME)

  return(lab_data)
}

add_birth_data <- function(lab_data,
                           birth_data=NULL,
                           add_lab_age=TRUE) {
  if(is.null(birth_data))
    birth_data <- data.table::fread("/media/volume/users/detrokir/data/minimal_phenotype_20221216.csv")
  birth_data <- dplyr::select(birth_data, FINREGISTRYID, date_of_birth, sex)
  colnames(birth_data) <- stringr::str_to_upper(colnames(birth_data))
  lab_data <- dplyr::left_join(lab_data, birth_data, by="FINREGISTRYID")
  lab_data$DATE_OF_BIRTH <- as.Date(lab_data$DATE_OF_BIRTH, "%d-%m-%Y")
  if(add_lab_age)
    lab_data <- dplyr::mutate(lab_data, LAB_AGE=lubridate::time_length(lab_data$DATE_OF_BIRTH %--% lab_data$LAB_DATE_TIME, "years"))
  
  return(lab_data)
}

add_birth_data_2021_age <- function(lab_data,
                           birth_data=NULL,
                           add_lab_age=TRUE) {
  if(is.null(birth_data))
    birth_data <- data.table::fread("/media/volume/users/detrokir/data/minimal_phenotype_20221216.csv")
  birth_data <- dplyr::select(birth_data, FINREGISTRYID, date_of_birth, sex)
  colnames(birth_data) <- stringr::str_to_upper(colnames(birth_data))
  lab_data <- dplyr::left_join(lab_data, birth_data, by="FINREGISTRYID")
  lab_data$DATE_OF_BIRTH <- as.Date(lab_data$DATE_OF_BIRTH, "%d-%m-%Y")
  if(add_lab_age)
    lab_data <- dplyr::mutate(lab_data, LAB_AGE=lubridate::time_length(lab_data$DATE_OF_BIRTH %--% as.Date("2021/01/01"), "years"))
  
  return(lab_data)
}

get_lab_value_data <- function(lab_data,
                               lab_value,
                               perfect_match=FALSE) {
  if(!perfect_match) {
    lab_data <- dplyr::filter(lab_data, 
                              (stringr::str_detect(NAME, lab_value) | stringr::str_detect(ABBREVIATION, lab_value)))
  } else {
    lab_data <- dplyr::filter(lab_data, ABBREVIATION == lab_value)
                              #((ID_SOURCE == 0) & (NAME == lab_value)) | 
                                #((ID_SOURCE == 1) & (ABBREVIATION == lab_value)))
  }
  return(lab_data)
}

get_lab_value_data_clean <- function(lab_data,
                                     lab_value,
                                     unit_transforms,
                                     unit_pull) {
  lab_data <- get_lab_value_data(lab_data, lab_value, perfect_match=TRUE) 
  for(unit_orig in names(unit_transforms)) {
    lab_data <- dplyr::mutate(lab_data, UNIT=stringr::str_replace_all(UNIT, unit_orig, unit_transforms[unit_orig]))
  }
  lab_data <- dplyr::filter(lab_data, stringr::str_detect(UNIT, unit_pull))
}

process_date_strs <- function(date_strs) {
  date_strs <- as.character(date_strs)
  dates <- as.Date(date_strs, format="%Y-%m-%d")
  print(sum(is.na(dates)))
  dates[is.na(dates)] <- as.Date(date_strs[is.na(dates)], format="%Y%m%d")
  return(dates)
}

remove_low_counts <- function(lab_data, goal_cols) {
  legit_vals <- group_by(lab_data, get(goal_cols), .add=TRUE) %>% 
                  distinct(FINREGISTRYID, .keep_all = TRUE) %>% 
                  tally() %>% 
                  filter(n>=5) %>% 
                  pull("get(goal_cols)")
  lab_data <- dplyr::filter(lab_data, get(goal_cols) %in% legit_vals)
  return(lab_data)
}

get_concept_names <- function() {
  concept_names <- readr::read_delim("/media/volume/users/jgraciat/data/thl_hus_tku_tmp_lab_codes.csv", col_types="c")
  concept_names <- dplyr::filter(concept_names, !is.na(concept_id_omop_grouped))
  # Careful a sinlge name can now be mapping to multiple concept ids
  concept_names <- tidyr::separate_rows(concept_names, concept_id_omop_grouped)
}

get_concept_map <- function() {
  concept_map <- readr::read_delim("/media/volume/users/detrokir/data/omop_map.tsv", col_types="c")
  concept_map <- concept_map %>% dplyr::rename(LAB_ID=id, ID_SOURCE=source, LAB_ABBREVIATION=abbreviation, OMOP_UNIT=unit, OMOP_ID=concept_id_omop_grouped, OMOP_NAME=concept_name)
 # concept_map <- dplyr::filter(concept_map, !is.na(concept_id))
}

map_concept_names <- function(summary_counts, type="right") {
  concept_map <- get_concept_map()
  if(type=="right")
    summary_counts <- dplyr::right_join(summary_counts, 
                                        dplyr::select(concept_map, concept_id, concept_name), 
                                         by=c("concept_id_omop_grouped"="concept_id"))
  if(type=="left")
    summary_counts <- dplyr::left_join(summary_counts, 
                                        dplyr::select(concept_map, concept_id, concept_name), 
                                        by=c("concept_id_omop_grouped"="concept_id"))
  return(summary_counts)
}
map_omop <- function(lab_value_counts,
                     by="ABBREVIATION") {
  concept_names <- get_concept_names() 
  if(by == "ABBREVIATION") {
    lab_value_counts <- dplyr::left_join(lab_value_counts, 
                                         dplyr::select(concept_names, abbreviation, concept_id_omop_grouped), 
                                         by=c("ABBREVIATION"="abbreviation"))
  } else {
    lab_value_counts <- dplyr::left_join(lab_value_counts, 
                                         dplyr::select(concept_names, id, abbreviation, concept_id_omop_grouped, source), 
                                         by=c("LAB_ID"="id"))
    lab_value_counts <- dplyr::filter(lab_value_counts, source == SOURCE) %>% dplyr::select(-source)
    lab_value_counts <- dplyr::rename(lab_value_counts, "ABBREVIATION"="abbreviation")
    
  }
  concept_map <- get_concept_map()
  lab_value_counts <- dplyr::mutate(lab_value_counts, 
                                    concept_id_omop_grouped=as.character(concept_id_omop_grouped))
  lab_value_counts <- dplyr::left_join(lab_value_counts, 
                                       dplyr::select(concept_map, concept_id, concept_name),
                                       by=c("concept_id_omop_grouped"="concept_id"))
  lab_value_counts <- na.omit(lab_value_counts)
  lab_value_counts <- dplyr::rename(lab_value_counts, "OMOP_ID"="concept_id_omop_grouped")
  lab_value_counts <- dplyr::rename(lab_value_counts, "OMOP_CONCEPT"="concept_name")
  return(lab_value_counts)
}



