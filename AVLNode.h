#ifndef _AVLNODE_H_
#define _AVLNODE_H_

#include "StockData.h"

class AVLNode
{
private:
	AVLNode*			pRight;	// points to the right subnode
	AVLNode*			pLeft;	// points to the left subnode
	StockData*		pStockData;	// points to the StockData
	int						mBF;// Balance Factor

public:
	AVLNode(){		// constructor
		pRight = NULL;
		pLeft = NULL;
		pStockData = NULL;
		mBF = 0;
	}
	~AVLNode(){};	// destructor

	// set member variables
	void	setRight(AVLNode* SN)			{ pRight = SN; }
	void	setLeft(AVLNode* SN)			{ pLeft = SN; }
	void	setSD(StockData* pSto)	{ pStockData = pSto; }
	void	setBF(int a)							{ mBF = a; }

	// get member variables
	AVLNode*		getRight()	{ return pRight; }
	AVLNode*		getLeft()	{ return pLeft; }
	StockData*		getSD()		{ return pStockData; }
	int				getBF()		{ return mBF; }
};

#endif
