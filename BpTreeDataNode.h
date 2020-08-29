#ifndef _BPTREEDATANODE_H_
#define _BPTREEDATANODE_H_

#include "BpTreeNode.h"

class BpTreeDataNode : public BpTreeNode
{
private:
	map <double, vector<pair<int, StockData*> > > mapData;	// save Earning, StockID, StockData
	BpTreeNode* pNext;
	BpTreeNode* pPrev;

public:
	BpTreeDataNode(){
		pNext = NULL;
		pPrev = NULL;
	}

	void setNext(BpTreeNode* pN)	{ pNext = pN; }
	void setPrev(BpTreeNode* pN)	{ pPrev = pN; }
	BpTreeNode* getNext()				{ return pNext; }
	BpTreeNode* getPrev()				{ return pPrev; }	

	void insertDataMap(double avgGrade, vector<pair<int, StockData*> > m){	// insert new node to mapData
		mapData.insert(pair< double, vector<pair<int, StockData*>> >(avgGrade, m));		// insert in ascending order according to the size of double variable
	}

	void deleteMap(double n){	// delete all n element of mapIndex
		mapData.erase(n);
	}	

	map< double, vector<pair<int, StockData*> > > *getDataMap(){	// return the address of map
		return &mapData; 
	}
};

#endif
