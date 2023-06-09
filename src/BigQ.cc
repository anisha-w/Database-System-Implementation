#include "BigQ.h"

void sortTPMMSMerge(vector<Record *>& v, OrderMaker &sortorder, int s, int m, int e) {
	
    // temp is used to temporary store the vector obtained by merging
    // elements from [s to m] and [m+1 to e] in v
	vector<Record *> temp;
	Record left;
	Record right;
	ComparisonEngine ceng;

	int i, j;
	i = s;
	j = m + 1;

	while (i <= m && j <= e) {

		if (ceng.Compare(v[i], v[j],&sortorder)>0) {
			temp.push_back(v[i]);
			++i;
		}
		else {
			temp.push_back(v[j]);
			++j;
		}

	}

	while (i <= m) {
		temp.push_back(v[i]);
		++i;
	}

	while (j <= e) {
		temp.push_back(v[j]);
		++j;
	}

	for (int i = s; i <= e; ++i)
		v[i] = temp[i - s];

}

void sortTPMMS(vector<Record *>& v, OrderMaker &sortorder, int low, int high) {
	if (low < high) {
		int m = (low + high) / 2;
		
		sortTPMMS(v, sortorder,low, m);
		sortTPMMS(v, sortorder, m + 1, high);
		sortTPMMSMerge(v, sortorder, low, m, high);
		
	}
}

BigQ :: BigQ (Pipe &in, Pipe &out, OrderMaker &sortorder, int runlen) {
	// read data from in pipe sort them into runlen pages
	//Record *r; //wrong. declaring the pointer doesnt create the object and then in consume function we get segmentation fault because it tries to delete the bits of a object that was never created. 
	Record *r = new Record();

	int totalSize=0;
	int recsNo=0;
	/*while(in.Remove(recordVector[recsNo])==1){
		
		totalSize += recordVector[recsNo]->getBitSize();
		//recordVector.push_back(&r);
		
		if(totalSize>runlen*PAGE_SIZE){
			totalSize=0;
			sortTPMMS(recordVector, 0, recsNo);
			for (int i=0;i<recordVector.max_size();i++)
				out.Insert(recordVector[i]);
			recsNo=0;
			
		}
		if(recsNo == recordVector.capacity()-1){
			recordVector.resize(100);
		}
		recsNo++;
		

	}*/
	while(in.Remove(r)==1){
		totalSize += r->getBitSize();
		recordVector.push_back(r);
		recsNo++;
		//ANISHA : change order so that file doesnt go beyond runlen*pagesize
		if(totalSize>runlen*PAGE_SIZE){
			totalSize=0;
			sortTPMMS(recordVector, sortorder, 0, recsNo-1);
			for (long unsigned int i=0;i<recordVector.size();i++){
				out.Insert(recordVector[i]);
			}
			recsNo=0;
			recordVector.clear();
		}
		r = new Record();
	}
	delete r;
	if(recsNo>0){
		sortTPMMS(recordVector, sortorder, 0, recsNo-1);
		for (long unsigned int i=0;i<recordVector.size();i++){
			out.Insert(recordVector[i]);
		}
	}
    // construct priority queue over sorted runs and dump sorted data 
 	// into the out pipe

    // finally shut down the out pipe
	out.ShutDown ();
}

BigQ::~BigQ () {
}
