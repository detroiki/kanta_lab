# Overview

These are various C++ programs for working with the Kanta lab data files.

# Usage

The whole pipeline can be easily run by simply **replacing the paths for the results directory (`res_dir`)** and the **original data directory (`data_dir`)** in the
Makefile. **Additionally, you will need to activate a python environment** with packages: csv, re, sys, time, and collections, for the unit fixing step.

Then run

```
make run_all
```

This will run all the steps in the pipeline and create all the necessary folders and files.

The pipeline can also be run step by step. See the Makefile for the exact commands and steps.

# Pipeline
- [Creating Minimal File](#minimal)
  - [Minimal File Columns](#minimalcolumns)
  - [Usage](#use)
  - [Steps](#steps)
- [Mapping OMOP](#omop)
- [Unit Fixing](#unit)
- [Final Fixing](#final)

  
<a name="minimal">
  
## Creating Minimal File

Reduces the original files to a single file with columns
<a name="minimalcolumns">

### Minimal File Columns

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

## Usage
```
for file_no in 1:10:
  cat finregistry_[file_no].csv.finreg_ID | exec/minimal [res_path] [file_no] [date] [thl_sote_fix_name_map.tsv] [lab_id_map.tsv] [write_reports]
```
### Explanation of arguments
- `res_path`- Path to the directory where all files will be saved. 
- `file_no`- Number of the current file.
- `date`- Current date or other string to make the results from different runs distinguishable.
- `thl_sote_fix_name_map.tsv`- This file is located in the `data/`directory of this repo.
- `lab_id_map.tsv`- This file is located in the `data/`directory of this repo.
- `write_reports`- Whether or not to write problem rows etc. to other files, turn off to conserve space. Can be either `True`or `False`.

## Steps
For a detailed desription of the steps performed see: [Minimal README](https://github.com/detroiki/kanta_lab/tree/v2/code/src/minimal).

<a name="omop"/>

## Mapping OMOP

Maps the OMOP concepts, using both the lab ID and abbreviation map. We can map about 60% of the local lab codes and 87% of the data this way.

The lab ID and abbreviations are mapped to the correct source table, if possible and otherwise hierarchical to the best matching source table. The four tables are LABfi the nation wide table, LABfi_HUS the HUS table, LABfi_TMP the Tampere table, and LABfi_TKU the Turku table. The hierarchy for local lab IDs from non-major hospitals is HUS, TMP, TKU.

### Usage

The program takes in the minimal file created in the previous step and maps the lab measurements to OMOP concepts. The program takes in the following arguments

- `res_path` - The path to the results folder
- `omop_concept_map_path`: The path to the OMOP concept ID map. Mapping from lab IDs and abbreviations to OMOP concept IDs. The delimiter is expected to be "\t". Expects columns: LAB_ID, LAB_SOURCE, LAB_ABBREVIATION, UNIT, OMOP_ID, NAME. The columns names are irrelevant but they need to be in the correct order. LAB_SOURCE is either LABfi, LABfi_HUS, LABfi_TMP, LABfi_TKU.

You can find the map here: TODO
### Resulting file columns

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
