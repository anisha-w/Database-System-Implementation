# Database-System-Implementation
A single user database management system in C++, that supports SQL operations like Select, Project, Join and Group By.

## Project Milestones 
Project 1 : Implementing Heap File     
Project 2 : Sort the records     
Project 3 : Implementing Relational Operators      
Project 4 : Query Compilation and Optimization       

## Installation and Execution

### Installation 
#### Step 1 : Download code
Download code files into a folder 

#### Step 2 : Install TPC-H Data files Generator

```sh
mkdir ~/datafilesgit
cd datafilesgit 
git clone https://github.com/electrum/tpch-dbgen.git
make
```
To generate data files (.tlb)
Generate 10MB data
```sh
./dbgen -s 0.01
```
Generate 1GB data
```sh
./dbgen -s 1
```
This will generate 8 *.tbl files containing the data in CSV format with | separator      
Copy these files into the required folder destination [Refer folder structure](#folder-structure)

#### Step 3 : Set path
Open file "test.h" and set path in below lines 
```gcc
const char *dbfile_dir = "./db_files/"; // dir where binary heap files should be stored
const char *tpch_dir ="./data_files/"; // dir where dbgen tpch files (extension *.tbl) can be found
const char *catalog_path = "catalog"; // full path of the catalog file
```

#### Step 4 : Verify Folder structure 
```sh
|-Database-System-Implementation
    |- code files #(.y , .c)    # Downloaded code files 
    |- header_files             # Downloaded code files 
        |- *.h files
    |- src                      # Downloaded code files 
        |- *.cc files
    |- data_files               # (copy t-pch generated files in this folder)
        |- *.tlb files
    |- db_files                 # Create folder 
        |- *.bin files          # (created when tlb files are loaded into the system)
```

### Execution

#### Compile
```sh
make -Bf Makefile
make -Bf MakeFile main
```
#### Run 
```sh
./test.out
```

#### Run all test cases given in testcases.txt
```sh
./testscript.sh
```
## Implementation 

### How is the data stored in the system?
Data imported from tlb file is stored in a DBFile object. The structure of the DB file is given below. The file type is .bin

<div align="center">
<img width="608" alt="image" src="https://github.com/anisha-w/Database-System-Implementation/assets/36306448/85ac779d-72af-4b5e-9e03-8d1d09c0d36e">            
</div>

Each record extract from the tlb file is encoded in binary and stored as the following structure. Each record is of variable size. 
<div align="center">
<img width="701" alt="image" src="https://github.com/anisha-w/Database-System-Implementation/assets/36306448/cbea1070-ee78-426e-a4a1-020af46c49cf">
</div>
