# Table of Contents
- [Run](#run)
- [Important Steps](#important)
   -  [Duplicates](#dup)
   -  [Removed Lines](#rem)
- [Output](#out)
   - [Columns `LAB_ID`, `LAB_ID_SOURCE`, and `LAB_ABBREVIATION`](#special)
- [Expected Columns](#expect)


<a name="run">
 
# Usage

```c
cat finregistry_[file_no].csv.finreg_ID | exec/minimal [res_path] [file_no] [date] [data/thl_sote_fix_name_map.tsv] [data/lab_id_map.tsv] [write_reports]
```

The input arguments need be appended in the correct order

* `res_path`: Path to results directory
* `file`: File number
* `date`: Date of file
* `thl_sote_path`: Path to THL SOTE organisations name map `data/thl_sote_organisations.tsv`
* `thl_abbrv_path`: Path to official abbreviations map `data/thl_lab_id_abbrv_map.tsv`
* `write_reports`: Wheter to write report or not, either "True" or "False"
  
 <a name="important">
 
 # Important Steps

 <a name="dup">
  
 ## Duplicates 

 Duplicate lines are removed. A duplicate is defined if all of the following data is the same: 
 1. Finregistry ID
 2. Date and time
 3. Service provider organization
 3. Laboratory test name ID
 4. Laboratory test name abbreviation
 5. Test result value
 6. Test result unit

<a name="rem">

 ## Removed lines

 Removed lines are directly written to a file located at 
 `<res_dir>/problem_rows/problem_rows_file_<file_no>_<date>.tsv`, if the `write_reports` argument is set to `True`.
 
 Lines are removed if they have:
 * Hetu roots that are not `1.2.246.21` (they are manually assigned hetus).
 * A measurement status of `K`, `W`, `X`, or `I` (unfinished, wrong, no result, sample in the lab waiting for result).
 
 Lines with no information about the lab value and abnormality or
 a missing Lab ID are removed and if the `write_reports` argument is set to True,
 written to a file located at
 `<res_dir>/problem_rows/missing_data_rows_file_<file_no>_<date>.tsv`,

<a name="out">

 ## Output

 The final files have the following columns:
  1. `FINREGISTRYID` - Pseudoanonimized IDs
  2. `LAB_DATE_TIME` - Date and time of lab measurement
  3. `LAB_SERVICE_PROVIDER` - Service provider string based on OID mapped to city
  4. `LAB_ID` - Regional or local lab ID
  5. `LAB_ID_SOURCE` - Source of lab ID 0: local 1: national
  6. `LAB_ABBREVIATION` - Laboratory abbreviation from data (local) or mapped using the THL map (national)
  7. `LAB_VALUE` - The value of the laboratory measurement
  8. `LAB_UNIT` - The unit from the file
  9. `LAB_ABNORMALITY` - The abnormality of the measurement i.e. high, low, positive, negative. A lot of missingness
  10. `MEASUREMENT_STAUTS`- The measurement status, i.e. C - corrected results or F - final result. See [Koodistopalvelu - AR/LABRA - Tutkimusvastauksien tulkintakoodit 1997](https://koodistopalvelu.kanta.fi/codeserver/pages/publication-view-page.xhtml?distributionKey=2637&versionKey=321&returnLink=fromVersionPublicationList).
  11. `REFERENCE_VALUE_TEXT`- The reference values for the measurement in text form.
  12. `DATA_SYSTEM` - Data system used to store the information.
  13. `DATA_SYSTEM_VERSION` - Version of the data system used.

 <a name="special">

 ### Columns `LAB_ID`, `LAB_ID_SOURCE`, and `LAB_ABBREVIATION`

 The columns for lab ID, lab ID source, and lab abbreviation are based on
 the following original columns:
* `labooratoriotutkimusoid` 
    * THL - lab ID source: 0
    * lab abbreviation from THL abbreviation map in `data/thl_lab_id_abbrv_map.tsv`
* `paikallinentutkimusnimikeid`
    * Local - lab ID source: 1
    * lab abbreviation from column `paikallinentutkimusnimike`

<a name="expect">

# Expected columns
 1. `laboratoriotutkimusoid` - Laboratory test OID
 2. `asiakirjaoid` - Document OID
 3. `merkintaoid` - Note OID
 4. `entryoid` - Entry OID
 5. `potilashenkilotunnus` - Finregistry ID
 6. `palvelutapahtumatunnus` - Service event ID
 7. `tutkimuksennaytelaatu` - Measurement
 8. `tutkimuksentekotapa` - Test method
 9. `potilassyntymaaika_pvm` - Patient birth date
 10. `potilas_sukupuoli` - Patient sex
 11. `labooratoriotutkimusoid` - Laboratory test OID
 12. `tutkimusaika` - Test time
 13. `alkuperainenasiakirjaoid` - Original document OID
 14. `asiakirjaversio` - Document version
 15. `rekisterinpitaja_organisaatio_h` - Registry controller organisation ID
 16. `rekisterinpitaja_h` -  Registry organisation ID
 17. `asiakirjavalistilapk` - Document status
 18. `marittelykokoelmaoid` - Collection OID
 19. `tietojarjestelanimi` - Data system name
 20. `tietojarjestelavalmistaja` - Data system manufacturer
 21. `tietojarjestelaversio` - Data system version
 22. `asiajirjaluontiaika` - Document creation time
 23. `pal_alkuperainenasiakirjaoid` - Original document OID
 24. `pal_asiakirjaversio` - Document version
 25. `pal_ariakirjaoid` - Document OID
 26. `pal_asiakirjaversio` - Document version
 27. `rekisterinpitaja_organisaatio` - Registry controller organisation ID
 28. `rekisterinpitaja` -  Registry organisation ID
 29. `palveluntuottaja_organisaatio` - Service provider organisation ID
 30. `palveluisanta_organisaatio` - Service host organisation ID
 31. `hetu_root` - Finregistry ID root
 32. `paikallinentutkimusnimike` - Local test name
 33. `paikallinentutkimusnimikeid` - Local test name ID
 34. `tutkimuskoodistonjarjestelmaid` - Test code system ID
 35. `tutkimuksenvastauksentila` - Test result status
 36. `tutkimustulosarvo` - Test result value
 37. `tutkimustulosyksikkö` - Test result unit
 38. `tuloksenpoikkeavuus` - Test result abnormality
 39. `tuloksenvalmistumisaika` - Test result time
 40. `viitearvoryhma` - Reference value group
 41. `viitevalialkuarvo` - Reference value lower limit
 42. `viitevalialkuarvoyksikko` - Reference value lower limit unit
 43. `viitevaliloppuarvo` - Reference value upper limit
 44. `viitevaliloppuarvoyksikko` - Reference value upper limit unit
 45. `viitearvoteksti` - Reference value text
 46. `erikoisalalyhenne` - Speciality abbreviation
 
