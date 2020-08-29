#ifndef _AVLTREE_H_
#define _AVLTREE_H_

#include "AVLNode.h"

class AVLTree{
private:
	AVLNode*	root;
	vector < pair< pair<double, int>, StockData* > > vHeap;	// Heap to be used in Search function
	ofstream*	fout;

public:
	AVLTree(ofstream* fout);
	~AVLTree();
	
	bool		Insert(StockData* pStu);
	bool		Print();
	bool		Search(char* name);
	bool		Rank();

	void		printStockInfo(StockData* pStockData);
};

#endif
