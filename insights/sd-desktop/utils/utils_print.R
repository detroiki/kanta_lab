so_formatter <- function(num,
                         n_digits=1) {
  dplyr::case_when(
    num < 1e3 ~ as.character(round(num, n_digits)),
    num < 1e6 ~ paste0(as.character(round(num/1e3, n_digits)), "K"),
    num < 1e9 ~ paste0(as.character(round(num/1e6, n_digits)), "M"),
    TRUE ~ "To be implemented..."
  )  
}

mode <- function(num) {
  unique_num <- unique(num)
  unique_num[which.max(tabulate(match(num, unique_num)))]
}

print_value_stats <- function(lab_data,
                              low_ref=0,
                              high_ref=0) {
  writeLines(paste0("Summary of values:"))
  writeLines(paste0("\tNumber of values above reference (", high_ref, "): ", so_formatter(sum(lab_data$LAB_VALUE > high_ref)), " (", 
                    round((sum(lab_data$LAB_VALUE > high_ref) / nrow(lab_data)) * 100, 2), "%)"))
  writeLines(paste0("\tNumber of values below reference (", low_ref, "): ", so_formatter(sum(lab_data$LAB_VALUE < low_ref)), " (", 
                    round((sum(lab_data$LAB_VALUE < low_ref) / nrow(lab_data)) * 100, 2), "%)"))
  print(summary(lab_data$LAB_VALUE))
  writeLines(paste0("Mode: ", mode(lab_data$LAB_VALUE)))
  
  writeLines(paste0("\nSummary of values per individual:"))
  writeLines(paste0("\tNumber of individuals: ", so_formatter(length(unique(lab_data$FINREGISTRYID)))))

  #tally_lab <- lab_data %>% group_by(FINREGISTRYID) %>% tally() %>% ungroup() %>% pull(n)
  #writeLines(paste0("Mode: ", mode(lab_data %>% group_by(FINREGISTRYID) %>% tally() %>% ungroup() %>% pull(n))))
}

