# Database-System-Implementation
A single user database management system in C++, that supports SQL operations like Select, Project, Join and Group By.

## Project Milestones 
Project 1 : Implementing Heap File     
Project 2 : Sort the records     
Project 3 : Implementing Relational Operators      
Project 4 : Query Compilation and Optimization       

## Installation 

#### Step 1 : Download code
Download code files into a folder 

#### Step 2 : Install TPC-H Data files 

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

#### Set path
Open file "test.cc" and set path in below lines 
```gcc
const char *dbfile_dir = "./db_files/"; // dir where binary heap files should be stored
const char *tpch_dir ="./data_files/"; // dir where dbgen tpch files (extension *.tbl) can be found
const char *catalog_path = "catalog"; // full path of the catalog file
```

#### Folder structure 
```sh
|-Database-System-Implementation
    |- Parser files #(.y , .c)
    |- header_files
        |- *.h files
    |- src
        |- *.cc files
    |- data_files #(copy t-pch generated files in this folder)
        |- *.tlb files
    |- db_files
        |- *.bin files #(created when tlb files are loaded into the system)
```

## Execution

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
