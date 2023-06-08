# Database-System-Implementation
A single user database management system in C++, that supports SQL operations like Select, Project, Join and Group By

## Installation 

#### Step 1 : Download code
Download code files into a folder 

#### Step 2 : Install Data files 

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

#### Set path


#### Folder structure 


## Execution

```sh
make -Bf Makefile
make -Bf MakeFile main
./test.out
```

