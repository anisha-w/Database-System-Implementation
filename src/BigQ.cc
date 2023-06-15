#include "BigQ.h"
#include <queue>

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

		if (ceng.Compare(v[i], v[j],&sortorder)<0) {
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

void phaseTwoTMPPS(Pipe &out, OrderMaker &sortorder, File *tmppsFile, int runLength){
	Page *pageBuffer;
	Record rec;
	Record rec2;
	LessThanByOrder comp(&sortorder);
	priority_queue<Page*, std::vector<Page*>, LessThanByOrder> q1(comp);

	//for(int i=1; i<tmppsFile->GetLength();i=i+runLength){ //Anisha Temp fix to figure out final logic 
	for(int i=1; i<tmppsFile->GetLength();i++){ //Anisha : add all pages in queue for now :: add pages in multiples of runlength
		
		pageBuffer = new Page();
		tmppsFile->GetPage(pageBuffer,i-1);
		q1.push(pageBuffer);
		//delete pageBuffer;
	}
	int emptyPage=0; //0 for false 
	pageBuffer = new Page();
	
	while(!q1.empty()){

		//pageBuffer = new Page();
		pageBuffer = q1.top();
		//cout<<"Anisha Page buffer address"<< pageBuffer <<endl; //Anisha : Addresses stay constant for the particular page ; recheck
		q1.pop(); 
		
		emptyPage = pageBuffer->GetFirst(&rec);
		if(emptyPage!=0){
			out.Insert(&rec);
			if(pageBuffer->ReadFirst(&rec2)!=0){
				q1.push(pageBuffer); 
			}
		}
		//delete pageBuffer;
	}

}


BigQ :: BigQ (Pipe &in, Pipe &out, OrderMaker &sortorder, int runlen) {
	// read data from in pipe sort them into runlen pages
	order = sortorder;
	char tpath[100];
	char *dir_loc = "./"; //Anisha temp fix
	sprintf (tpath, "%ssortphaseone.tmp", dir_loc);
	tmppsTempFile.Open(0,(char*)tpath);
	Page p1; //Buffer Page to hold records 
	int pageNotFull=0;
	off_t pageNumber=0;
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
				pageNotFull = p1.Append(recordVector[i]);
				if(pageNotFull==0){
					tmppsTempFile.AddPage(&p1,pageNumber++);
					p1.EmptyItOut();
					pageNotFull = p1.Append(recordVector[i]);
				}
			}
			tmppsTempFile.AddPage(&p1,pageNumber++);
			p1.EmptyItOut(); //ANISHA BREAKTHROUGH 
			recsNo=0;
			recordVector.clear();
		}
		r = new Record();
	}
	delete r;
	if(recsNo>0){
		sortTPMMS(recordVector, sortorder, 0, recsNo-1);
		for (long unsigned int i=0;i<recordVector.size();i++){
			pageNotFull = p1.Append(recordVector[i]);
			if(pageNotFull==0){
				tmppsTempFile.AddPage(&p1,pageNumber++);
				p1.EmptyItOut();
				pageNotFull = p1.Append(recordVector[i]);
			}
			//out.Insert(recordVector[i]);
		}
		tmppsTempFile.AddPage(&p1,pageNumber++);
	}
    // construct priority queue over sorted runs and dump sorted data 
 	// into the out pipe

	tmppsTempFile.Close();

	tmppsTempFile.Open(1,(char*)tpath);
	
	phaseTwoTMPPS(out, sortorder, &tmppsTempFile, runlen);

	remove((char*)tpath); //Anisha check if happening 
    // finally shut down the out pipe
	out.ShutDown ();
}

BigQ::~BigQ () {
}
