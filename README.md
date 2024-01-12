# Table of Contents

- [Using the Final Data](#final_cols)
  - [Columns](#final_cols)
  - [Important Notes](#final_notes)
    - [Service Providers](#provider)
    - [Lab IDs and abbreviations](#ids)
    - [Units](#units)
  - [How to Best Use the Data](#best_use)
  - [Known Bugs](#bugs)
- [This Repo](#repo)
  - [Usage](#repo)
  - [Processing Pipeline](#pipe)



# Using the Final Data

The current processed version of the kanta lab  v2 - 2023-12-12 has the following 11 columns

<a name="final_cols">

## Columns

| #   | Column Name | Easy Description | General Notes | Technical Notes |
| --- | --- | --- | --- | --- |
| 1   | `FINREGISTRYID` | Pseudoanonimized IDs |     |     |
| 2   | `LAB_DATE_TIME` | Date and time of lab measurement |     |     |
| 3   | `LAB_SERVICE_PROVIDER` | Service provider | This is NOT the lab performing the test but the service provider that ordered the test see Section [Important Notes](#final_notes) for more details. | The original data contains uses OIDs  ([OID-yksilöintitunnukset](https://thl.fi/aiheet/tiedonhallinta-sosiaali-ja-terveysalalla/ohjeet-ja-soveltaminen/koodistopalvelun-ohjeet/oid-yksilointitunnukset)). These were mapped to a readable string based on the city where the service provider is registered i.e. *HUS is mapped to Helsinki_1301*. The map is at [upload/data/thl\_sote\_map_named.tsv](https://github.com/detroiki/kanta_lab/blob/main/upload/data/thl_sote_map_named.tsv) and based on [THL - SOTE-organisaatiorekisteri 2008](https://koodistopalvelu.kanta.fi/codeserver/pages/classification-view-page.xhtml?classificationKey=421&versionKey=501). |
| 4   | `LAB_ID` | National (THL) or local lab ID of the measurement |     |     |
| 5   | `LAB_ID_SOURCE` | Source of the lab ID | 0: local and 1: national (THL) |     |
| 6   | `LAB_ABBREVIATION` | Laboratory abbreviation of the measurement from the data (local) or mapped using the THL map (national) |     | The map for the national (THL) IDs is at  [upload/data/thl\_lab\_id\_abbrv\_map.tsv](https://github.com/detroiki/kanta_lab/blob/main/upload/data/thl_lab_id_abbrv_map.tsv),  and was downloaded from [Kuntaliitto - Laboratoriotutkimusnimikkeistö](https://koodistopalvelu.kanta.fi/codeserver/pages/classification-view-page.xhtml?classificationKey=88&versionKey=120) |
| 7   | `LAB_VALUE` | The value of the laboratory measurement |     |     |
| 8   | `LAB_UNIT` | The unit of the labroatroy measurement from the data |     |     |
| 9   | `OMOP_ID` | OMOP concept ID of the lab measurement | For more details about the lab measurements see Sections \[Mapping to OMOP\](#omop)[](#) | Mapped using the lab ID and the lab abbreviation (not the unit) at [upload/data/omop\_concept\_map.tsv](https://github.com/detroiki/kanta_lab/blob/main/upload/data/omop_concept_map.tsv) |
| 10  | `OMOP_NAME` | Name of the OMOP concept |     |     |
| 11  | `LAB_ABNORMALITY` | Abnormality of the lab measurement | Describes whether the test is result is normal or abnormal i.e. too high or low low based on the laboratories reference values. This is **not** a quality control variable but to state it simply and inaccurately denotes whether the patient is healthy or not. See [AR/LABRA - Poikkeustilanneviestit](https://91.202.112.142/codeserver/pages/publication-view-page.xhtml?distributionKey=10329&versionKey=324&returnLink=fromVersionPublicationList) for the abbreviations meanings. | The column contains a lot of missingness. |
| 12  | `MEASUREMENT_STATUS` | The measurement status | The final data contains only `C` \- corrected results or `F` \- final result | See [Koodistopalvelu - AR/LABRA - Tutkimusvastauksien tulkintakoodit 1997](https://koodistopalvelu.kanta.fi/codeserver/pages/publication-view-page.xhtml?distributionKey=2637&versionKey=321&returnLink=fromVersionPublicationList) |
| 13  | `REFERENCE_VALUE_TEXT` | The reference values for the measurement in text form | This can be used to define the lab abnormality with better coverage using regex expressions (-to be implemented for the whole data). |     |

<a name="final_notes">

## Important notes

<a name="provider">
  
### Service Providers:

We **do not have any information on the actual lab performing the tests**, instead we can only know which service provider has ordered the tests. However, there are not too many labs and service providers in Finland so this column could likely still be used for some quality control purposes.

<a name="ids">

### Lab IDs and abbreviations

Some labs use the national lab IDs. However, **each lab can use their own unique lab ID and abbreviation** to describe the lab measurement. Additionally, different lab tests can sometimes be considered equivalent, i.e. plasma and serum based measurements.

- For this reason **it is recommended to use the OMOP IDs to find all measurements**, unless you have expert opinions on which lab IDs and abbreviations to use.
- **At this time we do not know all local lab IDs**, some of the lab IDs learned from the data have been added to the mapping based on their lab abbreviations. See Section \[\](). Overall this increases the OMOP mapping to about 93% of the data.

<a name="units">

### Units:

The units in the final data are from the data and **do not necessarily map to the assigned OMOP concepts**. Each lab measurement typically has 1-2 most common units. The rest of the data might be either very rarely used units or typos.

**Special units** have been added for some inconsistencies in the data.

- **The unit `binary`** denotes test results that contain not lab value and but only 0: normal and 1: abnormal. These are either binary tests, such as covid tests or measurements where the lab value is missing and we only know whether the result was normal or abnormal.
- **The unit `ordered`** denotes tests that are titles where multiple tests are ordered together, i.e. 2475 b-pvk+tkd - Short blood count panel - Blood.

<a name="best_use">

### How to Best Use the Data
For the reasons behind each step see the previous section.

***Use Case: Single measurement of interest***

1.  Find the OMOP concept ID for the measurement
    - see [athena.ohdsi.org](https://athena.ohdsi.org/search-terms/start), ask me (kira.detrois at helsinki.fi) for the summary statistics or simply use the data
2.  Filter the data based on the OMOP ID
3.  Filter the data for the top (1-2) most commonly used units
    - Either remove the rest of the data or fix obvious typos
4.  Have fun! :smiley:

***Use Case: Multiple Measurements i.e. for ML purposes***
- TODO

<a name="bugs">

### Known Bugs

- The label ordered is actually currently not in use. This has been fixed for future versions.

<a name="repo">


# This Repo

This repo contains various C++ programs (and one python file) for processing the original Kanta lab files.

## Usage

The whole pipeline can be easily run by simply **replacing the paths for the results directory (`res_dir`)** and the **original data directory (`data_dir`)** in the
Makefile. **Additionally, you will need to activate a python environment with packages: csv, re, sys, time, and collections, for the unit fixing step.**

Then run

```
make run_all
```

This will run all the steps in the pipeline and create all the necessary folders and files.

The pipeline can also be run step by step. See the Makefile for the exact commands and steps.

<a name="pipe">

## Pipeline
- [Creating Minimal File](#minimal)
- [Mapping OMOP](#omop)
- [Unit Fixing](#unit)
- [Final Fixing](#final)

  
<a name="minimal">
  
### Creating Minimal File

Reduces the original files to a single file with columns
<a name="minimalcolumns">

#### Minimal File Columns

### Usage
```
for file_no in 1:10:
  cat finregistry_[file_no].csv.finreg_ID | exec/minimal [res_path] [file_no] [date] [thl_sote_fix_name_map.tsv] [lab_id_map.tsv] [write_reports]
```
#### Explanation of arguments
- `res_path`- Path to the directory where all files will be saved. 
- `file_no`- Number of the current file.
- `date`- Current date or other string to make the results from different runs distinguishable.
- `thl_sote_fix_name_map.tsv`- This file is located in the `data/`directory of this repo.
- `lab_id_map.tsv`- This file is located in the `data/`directory of this repo.
- `write_reports`- Whether or not to write problem rows etc. to other files, turn off to conserve space. Can be either `True`or `False`.
### Current Steps

#### Reading Files
1. Reads in duplicate lines from all previous files 
```c
std::unordered_map<std::string, int> all_dup_lines; # Counts number of times the duplicate lines appear in the current file
```
2. Reads in the [THL SOTE Organisation](https://thl.fi/fi/web/tiedonhallinta-sosiaali-ja-terveysalalla/ohjeet-ja-soveltaminen/koodistopalvelun-ohjeet/sote-organisaatio-rekisteri) map 
  - Created by mapping each organisations OID to shorter strings which are based on the the city 
  -` Example: 1.2.246.10.1739.4173.10.1 mapped to Helsinki_1301 is HUS
```c
std::unordered_map<std::string, std::string> thl_sote_map;
```
3. Reads in the official [THL Koodistopalvelu regional lab IDs map to abbreviations](https://koodistopalvelu.kanta.fi/codeserver/pages/classification-view-page.xhtml?classificationKey=88&versionKey=120)
  - Example: 
  
  ![image](https://user-images.githubusercontent.com/56593546/235346446-813e5adb-0199-4c15-ac6d-4b6585ff90d0.png)
  
```c
    std::unordered_map<std::string, std::string> thl_abbrv_map;
``` 

#### For each line from std::cin
##### Creates a new file with columns
  0. `FINREGISTRYID` - Pseudoanonimized IDs
  1. `LAB_DATE_TIME` - Date and time of lab measurement
  2. `LAB_SERVICE_PROVIDER` - Service provider string based on OID mapped to city
  3. `LAB_ID` - Regional or local lab ID
  4. `LAB_ID_SOURCE` - Source of lab ID 0: local 1: regional
  5. `LAB_ABBREVIATION` - Laboratory abbreviation from data (local) or mapped using the THL map (regional)
  6. `LAB_VALUE` - The value of the laboratory measurement
  7. `LAB_UNIT` - The unit from the file
  8. `LAB_ABNORMALITY` - The abnormality of the measurement i.e. high, low, positive, negative. A lot of missingness

#### Steps
1. Reads in line a splits, using ";" - See function `read_correct_lines`
    - Checks if the line has 25 columns
    - If not tries to concatenate with next line to see if it was an early line break
    - If not checks if the next line is well defined 
    - Writes badly defined lines to a file `[res_path]/processed/reports/problem_rows/problem_rows_file_[file_no].csv`
2. Turns all NA markers to actual NAs in the data 
    - `Puuttuu`, `""`, `THYJÄ`, `_`, `-1` (except in value column
3. Columns directly copied from the data:
```c
std::string finregistry_id = final_line_vec[1];
std::string date_time = final_line_vec[11];
std::string lab_value = final_line_vec[19];
std::string lab_unit = final_line_vec[20];
std::string lab_abnormality = final_line_vec[22];
```    
4. Lab IDs, depending on data
  - If we have both the code and name -> local lab ID, source = 0
  - If we have only the code -> regional lab ID, source = 1
5. Getting abbreviations for regional lab ID, using `thl_abbrv_map`
6. Getting THL SOTE organisation string, using ``thl_sote_map`
7. Creating duplicate line string 
```c
std::vector<std::string> dup_vec = {finregistry_id, date_time, service_provider_name, lab_id, lab_abbrv, lab_value, lab_unit};
std::string dup_line = concat_string(dup_vec, std::string("")); 
```
8. Adding only non-duplicate lines to the file
9. Adding line to duplicate line map `all_dup_lines`
10. Adding lines with neither lab value, nor lab abnormality information to a file `[res_path]/processed/reports/problem_rows/missing_data_rows_file_[file_no].csv
## Important
- Currently the process takes a lot of space before compression (over 100GB)
  - It is in principal possible to skip all the error file creations if necessary to save some space. However, this still needs to be implemented

<a name="omop"/>

### Mapping OMOP

Maps the OMOP concepts, using both the lab ID and abbreviation map. We can map about 60% of the local lab codes and 87% of the data this way.

The lab ID and abbreviations are mapped to the correct source table, if possible and otherwise hierarchical to the best matching source table. The four tables are LABfi the nation wide table, LABfi_HUS the HUS table, LABfi_TMP the Tampere table, and LABfi_TKU the Turku table. The hierarchy for local lab IDs from non-major hospitals is HUS, TMP, TKU.

#### Usage

The program takes in the minimal file created in the previous step and maps the lab measurements to OMOP concepts. The program takes in the following arguments

- `res_path` - The path to the results folder
- `omop_concept_map_path`: The path to the OMOP concept ID map. Mapping from lab IDs and abbreviations to OMOP concept IDs. The delimiter is expected to be "\t". Expects columns: LAB_ID, LAB_SOURCE, LAB_ABBREVIATION, UNIT, OMOP_ID, NAME. The columns names are irrelevant but they need to be in the correct order. LAB_SOURCE is either LABfi, LABfi_HUS, LABfi_TMP, LABfi_TKU.

You can find the map here: TODO
#### Resulting file columns

Adds columns 

10. `OMOP_ID` - The OMOP ID of the lab measurement
11. `OMOP_NAME` - The OMOP name of the lab measurement

to the [Minimal File Columns](#minimalcolumns).

### Current Steps

1. Reads in the OMOP concept maps. The OMOP concept ID map has separate maps for each lab source LABfi, LABfi_HUS, LABfi_TMP, LABfi_TKU

```c
    // OMOP source -> lab ID + abbreviation -> OMOP ID
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> omop_concept_map;
    // OMOP ID -> OMOP name
    std::unordered_map<std::string, std::string> omop_names;

```

1. Reads in the minimal data from stdin. The delimiter is expected to be ";". The column names are irrelevant but they need to be in the correct order as described in section [Minimal File Columns](#minimalcolumns).
2. Gets the service provider source of the current measurement. So either LABfi, LABfi_HUS, LABfi_TMP, or LABfi_TKU, depending on the location of the service provider.
3. Gets the lab ID and abbreviation of the current measurement.
4. Maps the lab ID and abbreviation to OMOP concept ID and name, using the `omop_concept_map`.

<a name="unit">

## Unit Fixing

This is implemented in pyhton, using regex. Fixes include:

- Turning empty units to NAs
- Fixing and unifying all `10e5`, `10^9` etc. to one form `e5`, `e9` etc.
- Fixing and unifying all forms of `(y|μ)ks(ikkö)` to `u`
- Fixing and unifying all forms of `kopio(t), sol(y|μ|u), kpl, pisteet` to `count`.
- Fixing and unifying all `n(ä)kö(ke)k(enttä)`to `field`
- Fixing and unifying all `gulost` and `gstool` to `g`, since the fact that it is `g stool`is clear from the measurement.
- Fixing and unfiying all `(til)osuus` to `osuus`. 
- Fixing and unifying all `tiiteri` to `titer`.
- Fixing and unifying all `mosm/kg` to `mosm_kgh2o`.
- Fixing and unifying all `inrarvo` to `inr`.
- Fixing and unifying all `ml/min/(173m2)` to `ml/min/173m2`.
- Fixing all missing or wrong `/`, this can include i.e. `7`.
- Fixing typos, such as `mot` instead of `mol` and `mmmol` instead of `mmol`.
- Changing `umol` to `μmol` and `ug/l` to `μg/l`.
- Making `+` and `pos` to `A` and `-` and `neg(at)` to `N`.
- Making `o/oo` to `promille`.
- Making `/d`, and `/vrk` to `/24h`.
- Removing information such as the degree and ph, i.e. `mg/l/37c` simply becomes `mg/l`. These are typically already unified in the concepts and only irregularly denoted in the unit.
- Removing information `/100leuk`and `/24h` for the same reason as above.
- Making power changes for `ku/l`to `u/ml`, `pg/ml` to `ng/l`, `μg/l` to `ng/ml`, `μg/ml` to `mg/l`.

For all regex commands see: [code/src/unit_fixing.py](https://github.com/detroiki/kanta_lab/blob/main/code/src/unit_fixing.py)

<a name="final">

## Final Fixing

 This program performs final fixes to the data. It is run after the data has been processed
 and the OMOP IDs have been added. The fixes are:
 - Fixing percentages that are in osuus (fraction) format into % format
 - Fixing abnormality abbreviations to be consistent. This means replacing < with L, > with H, 
    POS with A and NEG with N. If the abbreviation is not one of these, it is replaced with NA.
 - Removing lines where the measurement year is before 2014
 - Removing lines where the lab value is not a number. Makes illegal units that are numbers NA. 
 - Removing values from title lines (Making them NAs) and turning the lab unit to ordered. 
   These are lines where often there is random information in the lab value column. 
 - Moving lab abnormality information to the lab value column if the lab value is NA. These are 
   marked with binary in the lab unit column.
 - Adding units to INR and pH measurements because they often lack their unit and it is very clear what it shoul be.
