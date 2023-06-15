#ifndef BIGQ_H
#define BIGQ_H
#include <pthread.h>
#include <iostream>
#include <vector>
#include "Pipe.h"
#include "File.h"
#include "Record.h"

#include "Comparison.h"
#include "Defs.h"

using namespace std;

struct LessThanByOrder
{
	OrderMaker *order; 
	LessThanByOrder(OrderMaker *order) {
		this->order = order;
	}
	template <typename Page>
	bool operator()( Page* lhs, Page* rhs) const
  	{
		Record r_lhs;
		Record r_rhs;
		lhs->ReadFirst(&r_lhs);
		rhs->ReadFirst(&r_rhs);
	  	ComparisonEngine ceng;
     	return ceng.Compare(&r_lhs, &r_rhs, order) > 0; //ANISHA : CHECK
  	}
};
class BigQ {

private: 
	File tmppsTempFile;
	OrderMaker order;
public:

	vector<Record *> recordVector;
	BigQ (Pipe &in, Pipe &out, OrderMaker &sortorder, int runlen);
	~BigQ ();
};

#endif
