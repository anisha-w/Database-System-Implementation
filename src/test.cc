#include <iostream>
#include <pthread.h>

#include "DBFile.h"
#include "test.h"
#include "BigQ.h"


using namespace std;


//Helper functions
void *producer (void *arg) {

	Pipe *myPipe = (Pipe *) arg;

	Record temp;
	int counter = 0;

	DBFile dbfile;
	dbfile.Open (rel->path ());
	cout << " producer: opened DBFile " << rel->path () << endl;
	dbfile.MoveFirst ();

	while (dbfile.GetNext (temp) == 1) {
		counter += 1;
		if (counter%100000 == 0) {
			 cerr << " producer: " << counter << endl;	
		}
		myPipe->Insert (&temp);
	}

	dbfile.Close ();
	myPipe->ShutDown ();

	cout << " producer: inserted " << counter << " recs into the pipe\n";
	return nullptr;
}

void *consumer (void *arg) {
	
	testutil *t = (testutil *) arg;

	ComparisonEngine ceng;

	DBFile dbfile;
	char outfile[100];

	if (t->write) {
		sprintf (outfile, "%s.bigq", rel->path ());
		dbfile.Create (outfile, heap, NULL);
	}

	int err = 0;
	int i = 0;

	Record rec[2];
	Record *last = NULL, *prev = NULL;

	while (t->pipe->Remove (&rec[i%2])) {
		prev = last;
		last = &rec[i%2];

		if (prev && last) {
			if (ceng.Compare (prev, last, t->order) >= 1) {
				err++;
			}
			if (t->write) {
				dbfile.Add (*prev);
			}
		}
		if (t->print) {
			last->Print (rel->schema ());
		}
		i++;
	}

	cout << " consumer: removed " << i << " recs from the pipe\n";

	if (t->write) {
		if (last) {
			dbfile.Add (*last);
		}
		cerr << " consumer: recs removed written out as heap DBFile at " << outfile << endl;
		dbfile.Close ();
	}
	cerr << " consumer: " << (i - err) << " recs out of " << i << " recs in sorted order \n";
	if (err) {
		cerr << " consumer: " <<  err << " recs failed sorted order test \n" << endl;
	}
	return nullptr;
}

// Menu test function 
// load from a tpch file
void test1 () {

	DBFile dbfile;
	cout << " DBFile will be created at " << rel->path () << endl;
	dbfile.Create (rel->path(), heap, NULL);

	char tbl_path[100]; // construct path of the tpch flat text file
	sprintf (tbl_path, "%s%s.tbl", tpch_dir, rel->name()); 
	cout << " tpch file will be loaded from " << tbl_path << endl;

	dbfile.Load (*(rel->schema ()), tbl_path);
	dbfile.Close ();
}

// sequential scan of a DBfile 
void test2 () {

	cout << " Scan all records for : " << rel->name() << "\n";
	DBFile dbfile;
	dbfile.Open (rel->path());
	dbfile.MoveFirst ();

	Record temp;

	cout << "\n ** Records **\n";
	int counter = 0;
	while (dbfile.GetNext (temp) == 1) {
		counter += 1;
		temp.Print (rel->schema());
		if (counter % 10000 == 0) {
			cout << counter << "\n";
		}
	}
	cout << "\n Total scanned " << counter << " recs \n\n";
	dbfile.Close ();
}

// scan of a DBfile and apply a filter predicate
void test3 () {

	cout <<" Filter with CNF for : " << rel->name() << "\n";

	CNF cnf; 
	Record literal;
	rel->get_cnf (cnf, literal);

	DBFile dbfile;
	dbfile.Open (rel->path());
	dbfile.MoveFirst ();

	Record temp;

	cout << "\n ** Records **\n";
	int counter = 0;
	while (dbfile.GetNext (temp, cnf, literal) == 1) {
		counter += 1;
		temp.Print (rel->schema());
		if (counter % 10000 == 0) {
			cout << counter << "\n";
		}
	}
	cout << "\n Total selected " << counter << " recs \n\n";
	dbfile.Close ();
}

void test4 (int option, int runlen) {

	// sort order for records
	OrderMaker sortorder;
	rel->get_sort_order (sortorder);

	int buffsz = 100; // pipe cache size
	Pipe input (buffsz);
	Pipe output (buffsz);

	// thread to dump data into the input pipe (for BigQ's consumption)
	pthread_t thread1;
	pthread_create (&thread1, NULL, producer, (void *)&input);

	// thread to read sorted data from output pipe (dumped by BigQ)
	pthread_t thread2;
	// sortorder.Print(); // CHECK
	testutil tutil = {&output, &sortorder, false, false};
	if (option == 5) {
		tutil.print = true;
	}
	else if (option == 6) {
		tutil.write = true;
	}
	pthread_create (&thread2, NULL, consumer, (void *)&tutil);

	BigQ bq (input, output, sortorder, runlen);

	pthread_join (thread1, NULL);
	pthread_join (thread2, NULL);
}

int main () {

	setup ();

	void (*test) ();  
	relation *rel_ptr[] = {n, r, c, p, ps, o, li, s};
	void (*test_ptr[]) () = {&test1, &test2, &test3};  

	int tindx = 0;
	while (tindx < 1 || tindx > 6) {
		cout << " Select test: \n";
		cout << " \t 1. Load file in heap format \n";
		cout << " \t 2. Scan \n";
		cout << " \t 3. Scan & filter \n";
		cout << " \t 4. Sort db\n";
		cout << " \t 5. Sort + display \n";
		cout << " \t 6. Sort + write \n\t";
		cin >> tindx;
	}
	cout << " Starting test : "<< tindx << endl ; 

	int findx = 0;
	while (findx < 1 || findx > 8) {
		cout << "\n select table: \n";
		cout << "\t 1. nation \n";
		cout << "\t 2. region \n";
		cout << "\t 3. customer \n";
		cout << "\t 4. part \n";
		cout << "\t 5. partsupp \n";
		cout << "\t 6. orders \n";
		cout << "\t 7. lineitem \n";
		cout << "\t 8. supplier \n";
		cin >> findx;
	}
	cout << " Table Selected: "<< findx << endl ; 

	rel = rel_ptr [findx - 1];
	if(tindx <=3){
		test = test_ptr [tindx - 1];
		test ();
	}
	else{
		int runlen;
		cout << "\t\n specify runlength:\t ";
		cin >> runlen;
		cout << " Runlength : "<< runlen << endl ; 
		test4 (tindx, runlen);
	}
	cleanup ();
}
