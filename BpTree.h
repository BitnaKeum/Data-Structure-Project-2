#ifndef _BPTREE_H_
#define _BPTREE_H_

#include "BpTreeIndexNode.h"
#include "BpTreeDataNode.h"

class BpTree{
private:
	BpTreeNode*	root;
	int			order;	
	ofstream*	fout;

public:
	BpTree(ofstream* fout, int order);	// constructor	
	~BpTree();	// destructor

	void	Insert(StockData* pStockData);
	bool	exceedDataNode(BpTreeNode* pDataNode);
	bool	exceedIndexNode(BpTreeNode* pIndexNode);
	void	splitDataNode(BpTreeNode* pDataNode);
	void	splitIndexNode(BpTreeNode* pIndexNode);
	BpTreeNode*	searchDataNode(double n);

	bool	Print(); // PRINT_BP
	bool	Search(double a, double b); // SEARCH_BP	

	void	printStockInfo(StockData* pStockData);
};

#endif
