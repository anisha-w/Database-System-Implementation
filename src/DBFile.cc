#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "DBFile.h"
#include "Defs.h"

#include <iostream>
#include <cstring>

DBFile::DBFile () {
    currentPageNo=0;
}

//Create file 
int DBFile::Create (const char *f_path, fType f_type, void *startup) {
    switch(f_type){
        case heap: //setFileType("heap");
        	strcpy(fileType,"heap");
            break;
        case sorted: setFileType("sorted"); //ANISHA To do : add sorted order
            break;
        default: return 0;
    }
    char metaDataPath[100];
    strcpy(metaDataPath,(char*)f_path);
    strcat(metaDataPath,"header");
    FILE *metaDataFile = fopen (metaDataPath, "w");

    //fprintf(metaDataFile,fileType,"|\n");
    fprintf(metaDataFile,"heap|\n");
    fclose(metaDataFile);

    f1.Open(0,(char*)f_path);
    cout<<" File created successfully"<<endl;
    return 1; //Sucessfully created
}

//Load records into DBFile 
void DBFile::Load (Schema &f_schema, const char *loadpath) {
    Record r1;
    FILE *tableFile = fopen (loadpath, "r");
    cout<<" Starting to load file"<<endl;
    while (r1.SuckNextRecord (&f_schema, tableFile) == 1) {  
        Add(r1);
    }
    writeBufferToFile(1); //Write last page to buffer
    cout<<" File loaded successfully. Number of data pages in file "<<f1.GetLength()-1<<endl;
}

// Open DBFile 
int DBFile::Open (const char *f_path) {
    char *space = new (std::nothrow) char[100]; //Anisha might have to change
    int len=0;

    char metaDataPath[100];
    strcpy(metaDataPath,(char*)f_path);
    strcat(metaDataPath,"header");
    cout<<"Opening header file "<<(char*)metaDataPath<<endl;
    FILE *metaDataFile = fopen(metaDataPath,"r");
    while (1) {
        int nextChar = getc (metaDataFile);

        if (nextChar == '|')
            break;
        else if (nextChar == EOF) {
            delete [] space;
            cout<<"\nData not found in header file"<<endl;
            return 0;
        }

        space[len] = nextChar;
        len++;
    }
    space[len]=0; //terminate string 

    strcpy(fileType,space);
    //ANISHA TODO : Set order 
    fclose(metaDataFile);
    cout<<"Opening file "<<(char*)f_path<<endl;
    f1.Open(1,(char*)f_path);
    cout<<" DB File opened successfully"<<endl;
    return 1;
}

// Load first page of DBFile into the buffer 
void DBFile::MoveFirst () {
    f1.GetPage(&pageBuffer,0); //Loading first page in buffer
    currentPageNo=0;
}

// Close DBFile 
int DBFile::Close () {
    f1.Close();
    return 1; //True 
}

// Add record to bufferPage. Once bufferpage is full, add it to the file 
void DBFile::Add (Record &rec) {
    bufferNotFull = pageBuffer.Append(&rec);
    if(bufferNotFull==0){//Page filled
        int i=writeBufferToFile(1); 
        bufferNotFull = pageBuffer.Append(&rec);
    } 
}

// Write buffer page to file 
int DBFile::writeBufferToFile(int emptyBuffer){
    f1.AddPage(&pageBuffer,currentPageNo++);
    if(emptyBuffer)
        pageBuffer.EmptyItOut();
    return 1;
}

//Get next record and store pointer in fetchme
int DBFile::GetNext (Record &fetchme) {
    if(pageBuffer.GetFirst(&fetchme)){
        return 1; //Record found
    }
    else //load next file
    {
        currentPageNo++;
        if(currentPageNo < (f1.GetLength()-1))
        {
            f1.GetPage(&pageBuffer,currentPageNo);
            if(pageBuffer.GetFirst(&fetchme))
                return 1; //Record found
            else
                return 0; //Record not found
        }
        else 
            return 0; 
    }
}

//Get next record if it matches the cnf and store pointer in fetchme
int DBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) {
    ComparisonEngine compEng;
   while(GetNext(fetchme)){
       if (compEng.Compare (&fetchme, &literal, &cnf)){
                	return 1;
        }
   }
   return 0; 
}

//Set filetype 
void DBFile::setFileType(char fileTypeName[10]){
	strcpy(fileType, fileTypeName);
}