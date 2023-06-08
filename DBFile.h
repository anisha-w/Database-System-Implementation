#ifndef DBFILE_H
#define DBFILE_H

#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"

typedef enum {heap, sorted, tree} fType;

// stub DBFile header..replace it with your own DBFile.h 

class DBFile {
private:
	File f1;
	Page pageBuffer;
	
	int bufferNotFull=1; //0 for false( full), 1 for true(not full) 
	off_t currentPageNo; //Page number of the page that is in buffer

public:
	DBFile (); 

	//Create a file
	int Create (const char *fpath, fType file_type, void *startup);
	int Open (const char *fpath);
	int Close ();

	void Load (Schema &myschema, const char *loadpath);

	void MoveFirst ();
	void Add (Record &addme);
	int GetNext (Record &fetchme);
	int GetNext (Record &fetchme, CNF &cnf, Record &literal);

	//When buffer Page is filled. write it to file. 
	int writeBufferToFile(int emptyBuffer);

};
#endif
