#ifndef _BPTREEINDEXNODE_H_
#define _BPTREEINDEXNODE_H_

#include "BpTreeNode.h"

class BpTreeIndexNode : public BpTreeNode{
private:
	map <double, BpTreeNode*> mapIndex;	// store Earning, BpTreeIndexNode or BpTreeDataNode
	
public:
	BpTreeIndexNode(){}
	~BpTreeIndexNode(){}
	
	void insertIndexMap(double n, BpTreeNode* pN){	// insert new node to mapIndex
		mapIndex.insert(pair<double, BpTreeNode*>(n, pN));
	}

	void deleteMap(double n){	// delete all n element of mapIndex
		mapIndex.erase(n);
	}

	map <double, BpTreeNode*>* getIndexMap(){ // return the address of map
		return &mapIndex; 
	}
};

#endif
