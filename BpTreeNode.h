#ifndef _BpTreeNode_H_
#define _BpTreeNode_H_

#include "StockData.h"

class BpTreeNode{	// 부모 클래스
private:
	BpTreeNode* pParent;
	BpTreeNode* pMostLeftChild;

public:
	BpTreeNode(){	// constructor
		pParent = NULL;
		pMostLeftChild = NULL;
	}

	void setMostLeftChild(BpTreeNode* pN)	{ pMostLeftChild = pN; }
	void setParent(BpTreeNode* pN)				{ pParent = pN; }
	
	BpTreeNode* getParent()						{ return pParent;	}
	BpTreeNode* getMostLeftChild()				{ return pMostLeftChild; }

	//virtual functions
	virtual void setNext(BpTreeNode* pN) {}
	virtual void setPrev(BpTreeNode* pN) {}
	virtual BpTreeNode* getNext() { BpTreeNode* pNext=NULL; return pNext; }
	virtual BpTreeNode* getPrev() { BpTreeNode* pPrev=NULL; return pPrev; }
	virtual void insertDataMap(double avgGrade, vector<pair<int, StockData*> > m) {}
	virtual void insertIndexMap(double n, BpTreeNode* pN){}
	virtual void deleteMap(double n){}
	virtual map <double, BpTreeNode*>* getIndexMap() {	// return the address of IndexMap
		return NULL; 
	}
	virtual map< double, vector<pair<int, StockData*> > > *getDataMap() { // return the address of DataMap
		return NULL; 
	}
};

#endif
