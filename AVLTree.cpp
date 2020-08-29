#include "AVLTree.h"

AVLTree::AVLTree(ofstream* fout) {	// constructor
	root = NULL;
	make_heap(vHeap.begin(), vHeap.end(), compare);	// sort vHeap as descending order
	this->fout = fout;	// ofstream 
}

AVLTree::~AVLTree(){	// destructor

}


// insert new node
bool AVLTree::Insert(StockData* pSto){
	AVLNode* pNew = new AVLNode;	// make pNew node
	pNew->setSD(pSto);				// set StockData of pNew to pSto
	char	*pName = pSto->getName();

	if(root == NULL){	// if tree is empty
		root = pNew;	// new node is root
		return true;
	}

	
	// Phase 1 : find the direction of inserting
	AVLNode *a = root,			// most recent node with bf = +-1
			*pa = NULL,			// parent of a
			*p = root,			// moves through the tree
			*pp = NULL,			// parent of p
			*rootSub = NULL;	// substitute root

	while (p != NULL) {	// repeat until (p==NULL)
		if (p->getBF() != 0) { a = p; pa = pp; }	// if mBF of p is not 0, move a and pa

		if (strcmp(pName, p->getSD()->getName()) < 0 ) {	// If the name to insert is smaller than the name currently pointed
			pp = p; p = p->getLeft();	// pp and p point each subnode
		}
		else if (strcmp(pName, p->getSD()->getName()) >0) {	// If the name to insert is bigger than the name currently pointed
			pp = p; p = p->getRight();	// pp and p point each subnode
		}
		// Assume no name is the same
	}


	// Phase 2: insert new node into tree first

	// point to the parent node where the pp will be inserted
	// compare the name of the new node with the name of the pp and insert the new node into the left or right subnode of the pp
	if (strcmp(pName, pp->getSD()->getName()) <0) 	// If the name to insert is less than the name to which the pp points, insert it on the left
		pp->setLeft(pNew);
	else				// if name to insert is less than the name to which the pp points, insert to the right
		pp->setRight(pNew);

	


	//Phase 3: determine direction
	int dir;	// represents if new node is inserted to the left or right of node to point a
	AVLNode *ca,	// child of a
			*cb;	// child of b

	if(strcmp(pName, a->getSD()->getName())>0) {	// if new Name is bigger than Name of a
		ca = p = a->getRight(); dir = -1;	// (dir=-1) represents that new node is inserted to the rightside of a 
	}
	else {	// if new Name is smaller than Name of a
		ca = p = a->getLeft(); dir = 1;		// (dir=1) represents that new node is inserted to the leftside of a 
	}



	// Phase 4: set BF
	while (p != pNew) {	// repeat until (p == pNew)
		/* BF of node between node that a points to and new node are all 0.
		   Therefore, compare Name and set BF for nodes between */
		if (strcmp(pName, p->getSD()->getName()) >0) {
			p->setBF(-1); p = p->getRight();
		}
		else {
			p->setBF(1); p = p->getLeft();
		}
	}

	if (a->getBF() == 0 || a->getBF() + dir == 0) {	// when tree still balanced after inserting
		a->setBF(a->getBF() + dir);		// add dir to mBF of a
		return true;
	}
	


	// Phase 5: execute rotation 
	if (dir == 1)	// left imbalance
	{
		if (ca->getBF() == 1) {	// LL rotation
			a->setLeft(ca->getRight());
			ca->setRight(a);
			a->setBF(0); ca->setBF(0);	// reset BF
			rootSub = ca;	// ca is the new root of the subtree
		}

		else {					// LR rotation
			cb = ca->getRight();
			ca->setRight(cb->getLeft());
			a->setLeft(cb->getRight());
			cb->setLeft(ca);
			cb->setRight(a);

			switch (cb->getBF()) {	// execute case statement according to BF of cb and reset BF
			case 0:
				ca->setBF(0); a->setBF(0);
				break;
			case 1:
				a->setBF(-1); ca->setBF(0);
				break;
			case -1:
				ca->setBF(1); a->setBF(0);
				break;
			}
			
			cb->setBF(0); rootSub = cb;	// cb is the new root of the subtree
		}
	}

	else            // right imbalance
	{
		if (ca->getBF() == -1) {	// RR rotation
			a->setRight(ca->getLeft());
			ca->setLeft(a);
			a->setBF(0); ca->setBF(0);	// reset BF
			rootSub = ca;	// ca is the new root of the subtree
		}

		else {						// RL rotation
			cb = ca->getLeft();
			ca->setLeft(cb->getRight());
			a->setRight(cb->getLeft());
			cb->setRight(ca);
			cb->setLeft(a);

			switch (cb->getBF()) {	// execute case statement according to BF of cb and reset BF
			case 0:
				ca->setBF(0); a->setBF(0);
				break;
			case 1:
				ca->setBF(-1); a->setBF(0);
				break;
			case -1:
				a->setBF(1); ca->setBF(0);
				break;
			}

			cb->setBF(0); rootSub = cb;	// cb is the new root of the subtree
		}
	}


	// Phase 6: reset root
	if (pa == NULL)			root = rootSub;
	else if (a == pa->getLeft()) pa->setLeft(rootSub);
	else					pa->setRight(rootSub);
	
	return true;
}

// using inorder traversal, print all information as ascending order of name
bool AVLTree::Print(){
	if (root == NULL)	return false;	// if AVLTree is empty
	
	stack<AVLNode*> S;	 // stack to contain AVLNode

	AVLNode* pCur = root;

	cout << "================== PRINT ===================" << endl;
	*fout << "================== PRINT ===================" << endl;

	// The while statement ends only if pCur points to NULL and stack is empty 
	while (pCur != NULL || !S.empty())
	{
		// move to the leftmost node connected to the current node and push into stack
		while (pCur != NULL) {
			S.push(pCur);	// push the address pCur points to into Stack
			pCur = pCur->getLeft();
		}

		// curNode is NULL, so initialize curNode to the value at the top of the stack
		pCur = S.top();
		S.pop();	// then pop the top of the stack

		printStockInfo(pCur->getSD());	// print the StockData of what pCur points

		// Now we have reached the left node, move to the right node
		pCur = pCur->getRight();
	}

	cout << "===========================================" << endl<<endl;
	*fout << "===========================================" << endl<<endl;

	return true;
}

bool AVLTree::Search(char* name){
	if (root == NULL)	return false;	// if AVLTree is empty


	AVLNode *pCur = root;	// pCur points root of AVLTree

	while (1)
	{
		// if name to find is bigger
		if (strcmp(pCur->getSD()->getName(), name) <0)
		{
			// corresponding name does not exist
			if (pCur->getRight() == NULL)
				return false;

			pCur = pCur->getRight();	// points the right subnode
		}

		// if name to find is smaller
		else if (strcmp(pCur->getSD()->getName(), name) >0)
		{
			// corresponding name does not exist
			if (pCur->getLeft() == NULL)
				return false;

			pCur = pCur->getLeft();		// points the left subnode
		}

		// if find the node with name to find
		else
		{
			cout << "======= SEARCH =======" << endl;
			*fout << "======= SEARCH =======" << endl;
			printStockInfo(pCur->getSD());	// print the StockData of what pCur points
			cout << "======================" << endl << endl;
			*fout << "======================" << endl << endl;

			break;
		}
	}
	// pCur points the AVLNode that has name
	
	StockData* pSD = pCur->getSD(); // store StockData of corresponding node in pSD
	vHeap.push_back( make_pair( make_pair(pSD->getEarning(), pSD->getStockID()), pSD));	// push value into MAX heap
	push_heap(vHeap.begin(), vHeap.end(), compare);	// sort in descending order by Earing in vHeap

	return true;
}

// print items retrieved from AVLTree in descending order of Earing (in ascending order of StockID if Earing is the same)
bool AVLTree::Rank(){
	if(vHeap.empty())		return false;	// if Heap is emtpy

	StockData* pSto;
	
	cout << "================== RANK ===================" << endl;
	*fout << "================== RANK ===================" << endl;

	while(!vHeap.empty()){	// repeat until vHeap is empty
		pSto = vHeap.front().second;		// store StockData of the element nearest vHeap (maximum Heap)
		pop_heap(vHeap.begin(), vHeap.end(), compare);	// move the largest value back and arrange the heap except for the element
		vHeap.pop_back();	// // remove the last element
		printStockInfo(pSto);	//output StockData for the Largest Element
	}
	cout << "===========================================" << endl<<endl;
	*fout << "===========================================" << endl<<endl;

	return true;
}


// print the information of pStockData
void AVLTree::printStockInfo(StockData* pStockData){
	// print in console and log.txt
	cout << pStockData->getStockID() << " " << pStockData->getName() << " " << pStockData->getEarning() << endl;
	*fout << pStockData->getStockID() << " " << pStockData->getName() << " " << pStockData->getEarning() << endl;
	cout << "시가: " << pStockData->getOpening_Price() << endl;
	*fout << "시가: " << pStockData->getOpening_Price() << endl;
	cout << "종가: " << pStockData->getClosing_Price() << endl;
	*fout << "종가: " << pStockData->getClosing_Price() << endl;
	cout << "거래량: " << pStockData->getVolume() << endl;
	*fout << "거래량: " << pStockData->getVolume() << endl;
	cout << "수익률: " << pStockData->getEarning() << endl << endl;
	*fout << "수익률: " << pStockData->getEarning() << endl << endl;
}
