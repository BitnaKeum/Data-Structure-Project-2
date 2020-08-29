#include "BpTree.h"

BpTree::BpTree(ofstream* fout, int order = 3){	// constructor of BpTree
	root = NULL;
	this->order = order;
	this->fout = fout;
}

BpTree::~BpTree(){	// destructor

}

// insert BpTreeNode into BpTree
void BpTree::Insert(StockData* pSto){
	double key = pSto->getEarning();	// key = Earning
	vector<pair<int, StockData*> > value;			// value = map< id, stock data* >
	value.push_back( make_pair(pSto->getStockID(), pSto) );	


	if(root == NULL){ // empty B+ tree
		BpTreeNode* pDataNode = new BpTreeDataNode;	
		pDataNode->insertDataMap(key, value);	// set key and value in Map of newly created node
		root = pDataNode;	// set root
		return;
	}
	

	BpTreeNode* pDataNode = searchDataNode(key);	// search data node with key
	
	map<double, vector<pair<int, StockData*>> > *m = pDataNode->getDataMap();	// DataMap of pDataNode
	map<double, vector<pair<int, StockData*>>>::iterator itData;

	itData = m->find(key);
	if (itData == m->end()) {	// If there is no duplicate Earning within the map
		pDataNode->insertDataMap(key, value);	// new values are sorted by key value and inserted into DataNode
	}
	else {	// If there is a duplicate Earning
		itData->second.push_back(make_pair( value.begin()->first, value.begin()->second ));	//Push the value in value at the end of the vData vector

		if (itData->second[0].first > itData->second[1].first)	// If the stockID of the existing value is larger
			itData->second[0].swap(itData->second[1]);		// rearrange in ascending order using swap function
	}



	if(exceedDataNode(pDataNode))// when the split should occur by inserting the DataNode
		splitDataNode(pDataNode);
}


//Return the position where the entered value will be inserted
BpTreeNode* BpTree::searchDataNode(double n){

	BpTreeNode* pCur = root;
	map<double, BpTreeNode*>::iterator itIndex;

	while(pCur->getMostLeftChild() != NULL){  // repeat until sub node doesn't exist
		itIndex = pCur->getIndexMap()->begin();	// itIndex represents the first value of current node

		if(n < itIndex->first){ // move to subnode if n less than the value itIndex represents
			pCur = pCur->getMostLeftChild();
		}else{	// n is bigger than the value itIndex represents
			while(true){
				itIndex++;	// increase the value to represent a greater value
				if(itIndex == pCur->getIndexMap()->end() || n < itIndex->first){// If no value is found in current level
					itIndex--;
					pCur = itIndex->second;	// move to the sub level
					break;
				}
			}
		}
	}

	return pCur;
}


//Split data when exceeds the maximum number of data that node can has
void BpTree::splitDataNode(BpTreeNode* pDataNode){
	int splitPosition_data = ceil((order-1)/2.0) + 1;	// ceil indicates where the split occurs as a function of the rounding

	map<double, vector<pair<int, StockData*>>>::iterator itData;	// iterator about DataNode
	map<double, BpTreeNode*>::iterator itIndex;

	
	BpTreeNode* leftDataNode = new BpTreeDataNode();	// node to be split left in pDataNode
	BpTreeNode* upDataNode	 = new BpTreeIndexNode();	// node to be split up in pDataNode

	itData = pDataNode->getDataMap()->begin();	// itData represents the first element in DataMap in pDataNode	
	leftDataNode->insertDataMap(itData->first, itData->second);	// store first element in leftDataNode
	pDataNode->deleteMap(itData->first);		// delete first element of pDataNode
	itData = pDataNode->getDataMap()->begin();	// reset the initiator since the first element of pDataNode was deleted
	upDataNode->insertIndexMap(itData->first, pDataNode);	// store first element of pDataNode after deletion in upDataNode


	if (pDataNode->getParent() == NULL)	// if there is no parent node
	{
		root = upDataNode;	// upDataNode becomes IndexNode and Parent Node
		upDataNode->setMostLeftChild(leftDataNode);	// set MostLeftChild of upDataNode to leftDataNode
		leftDataNode->setParent(upDataNode);	// parent node of leftDataNode and pDataNode becomes upDataNode
		pDataNode->setParent(upDataNode);

		// set pNext, pPrev of the DataNode
		leftDataNode->setNext(pDataNode);
		pDataNode->setPrev(leftDataNode);
	}
	else if (pDataNode->getParent()->getIndexMap()->size() == 1)	// If the parent node has one element
	{
		if (pDataNode->getParent()->getMostLeftChild() == pDataNode)	// If pDataNode is to the left of the parent node
		{
			itIndex = upDataNode->getIndexMap()->begin();
			pDataNode->getParent()->insertIndexMap(itIndex->first, itIndex->second);	//Insert upDataNode information in the map of the parent node

			pDataNode->getParent()->setMostLeftChild(leftDataNode);	// set MostLeftChild of parent node
			leftDataNode->setParent(pDataNode->getParent());	// set parent of leftDataNode
			itIndex = pDataNode->getParent()->getIndexMap()->begin();	// itIndex points to the first element of the parent node
			itIndex->second = pDataNode;

			// set pNext, pPrev of the DataNode
			if (pDataNode->getPrev()) {
				pDataNode->getPrev()->setNext(leftDataNode);
				leftDataNode->setPrev(pDataNode->getPrev());
			}
			leftDataNode->setNext(pDataNode);
			pDataNode->setPrev(leftDataNode);
		}
		else if (pDataNode->getParent()->getMostLeftChild() != pDataNode)	// If pDataNode is to the right of the parent node
		{
			itIndex = upDataNode->getIndexMap()->begin();
			pDataNode->getParent()->insertIndexMap(itIndex->first, itIndex->second);	//Insert upDataNode information in the map of the parent node

			itIndex = pDataNode->getParent()->getIndexMap()->begin();	// itIndex points to the first element of the parent node
			itIndex->second = leftDataNode;
			++itIndex;
			itIndex->second = pDataNode;	// second element of parent node points to pDataNode
			leftDataNode->setParent(pDataNode->getParent());	 // set parent of leftDataNode
	
			// set pNext, pPrev of the DataNode
			pDataNode->getPrev()->setNext(leftDataNode);
			leftDataNode->setPrev(pDataNode->getPrev());
			leftDataNode->setNext(pDataNode);
			pDataNode->setPrev(leftDataNode);
		}
		
		

	}
	else  // If the parent node has two elements
	{
		itIndex = pDataNode->getParent()->getIndexMap()->begin();
		itIndex++;	// itIndex represents the second element of parent node

		if (itIndex->second == pDataNode)	//If the pDataNode is connected to the second element of the parent node
		{
			itIndex = upDataNode->getIndexMap()->begin();
			pDataNode->getParent()->insertIndexMap(itIndex->first, itIndex->second);//Insert upDataNode information in the map of the parent node	

			itIndex = pDataNode->getParent()->getIndexMap()->end();
			--itIndex;	// itIndex represents the last element in parent node
			itIndex->second = pDataNode;
			--itIndex;
			itIndex->second = leftDataNode;		// The second element of the parent node points the leftDataNode
			leftDataNode->setParent(pDataNode->getParent());	// set parent node of leftDataNode
			
			// set pNext, pPrev of the DataNode
			pDataNode->getPrev()->setNext(leftDataNode);
			leftDataNode->setPrev(pDataNode->getPrev());
			leftDataNode->setNext(pDataNode);
			pDataNode->setPrev(leftDataNode);
			

			if (exceedIndexNode(pDataNode->getParent()))	// As a result of DataNode split, IndexNode split is needed 
				splitIndexNode(pDataNode->getParent());
		}

		else if ((--itIndex)->second == pDataNode)// If the pDataNode is connected to the second of the first element of the parent node
		{
			itIndex = upDataNode->getIndexMap()->begin();	// itIndex represents upDataNode
			pDataNode->getParent()->insertIndexMap(itIndex->first, itIndex->second);	//Insert upDataNode information in the map of the parent node

			itIndex = pDataNode->getParent()->getIndexMap()->begin();	 // itIndex represents the first element in parent node
			itIndex->second = leftDataNode; //The first element of the parent node indicates the leftDataNode
			leftDataNode->setParent(pDataNode->getParent());

			++itIndex;	// itIndex represents the middle element in parent node
			itIndex->second = pDataNode;	//The second element of the parent node indicates the pDataNode

			// set pNext, pPrev of the DataNode
			pDataNode->getPrev()->setNext(leftDataNode);
			leftDataNode->setPrev(pDataNode->getPrev());
			leftDataNode->setNext(pDataNode);
			pDataNode->setPrev(leftDataNode);

			if (exceedIndexNode(pDataNode->getParent())) // As a result of DataNode split, IndexNode split is needed 
				splitIndexNode(pDataNode->getParent());
		}

		else   // If pDataNode is the MostLeftChild of the parent node
		{
			itIndex = upDataNode->getIndexMap()->begin();	// itIndex represents upDataNode
			pDataNode->getParent()->insertIndexMap(itIndex->first, itIndex->second);	//Insert upDataNode information in the map of the parent node

			pDataNode->getParent()->setMostLeftChild(leftDataNode);	// MostLeftChild of parent node is leftDataNode
			leftDataNode->setParent(pDataNode->getParent());	// set parent of leftDataNode
		
			itIndex = pDataNode->getParent()->getIndexMap()->begin();	 // itIndex represents the first element in parent node
			itIndex->second = pDataNode; 


			// set pNext, pPrev of the DataNode
			if (pDataNode->getPrev()) {
				pDataNode->getPrev()->setNext(leftDataNode);	// connect to another unchanged DataNode
				leftDataNode->setPrev(pDataNode->getPrev());
			}
			leftDataNode->setNext(pDataNode);
			pDataNode->setPrev(leftDataNode);

			if (exceedIndexNode(pDataNode->getParent())) // As a result of DataNode split, IndexNode split is needed 
				splitIndexNode(pDataNode->getParent());
		}

	}

}


//Split data when exceeds the maximum number of data that node can has
void BpTree::splitIndexNode(BpTreeNode* pIndexNode){
	int splitPosition_index = ceil(order/2.0);

	map<double, BpTreeNode*>::iterator itIndex;

	BpTreeNode* leftIndexNode = new BpTreeIndexNode();	// node to be split left in pIndexNode
	BpTreeNode* upIndexNode = new BpTreeIndexNode();	// node to be split up in pIndexNode


	itIndex = pIndexNode->getIndexMap()->begin();	// itIndex represents the first element in DataIndex in pIndexNode	




	leftIndexNode->insertIndexMap(itIndex->first, itIndex->second);	// store first element in leftIndexNode
	leftIndexNode->setMostLeftChild(pIndexNode->getMostLeftChild());	// MostLeftChild of leftIndexNode is MostLeftChild of pIndexNode
	leftIndexNode->getMostLeftChild()->setParent(leftIndexNode);	// set the parent of the MostLeftChild node connected to the leftIndexNode
		
	pIndexNode->deleteMap(itIndex->first);		// delete first element of pIndexNode
	itIndex = pIndexNode->getIndexMap()->begin();	// reset the iterator since the first element of pIndexNode was deleted

	upIndexNode->insertIndexMap(itIndex->first, pIndexNode);	// save first element of pIndexNode after deletion in upIndexNode
	
	pIndexNode->setMostLeftChild(itIndex->second);	// set MostLeftChild in pIndexNode
	pIndexNode->deleteMap(itIndex->first);		// delete first element of pIndexNode

	itIndex = leftIndexNode->getIndexMap()->begin();
	itIndex->second->setParent(leftIndexNode);	// set the parent of the second element of the leftIndexNode




	if (pIndexNode->getParent())	// pIndexNode has parent node
	{
		itIndex = upIndexNode->getIndexMap()->begin();	// itIndex represents upIndexNode
		pIndexNode->getParent()->insertIndexMap(itIndex->first, itIndex->second);	// insert upIndexNode into parent node of pIndexNode

		// pIndexNode의 parent node는 leftIndexNode의 parent node
		itIndex = pIndexNode->getParent()->getIndexMap()->begin();	// itIndex represents first element of parent node of pIndexNode
		
																	
		if (pIndexNode->getParent()->getMostLeftChild() == pIndexNode)	// If pIndexNOde is connected to the left side of the root
		{
			itIndex->second = pIndexNode;
			pIndexNode->getParent()->setMostLeftChild(leftIndexNode);
		}
		else
			itIndex->second = leftIndexNode;

		leftIndexNode->setParent(pIndexNode->getParent());

		if (exceedIndexNode(pIndexNode->getParent()))	// If a split in IndexNode needs to occur again in parent IndexNode
			splitIndexNode(pIndexNode->getParent());	// implement splitIndexNode function about parent node
	}
	else // pIndexNode has no parent node
	{
		leftIndexNode->setParent(upIndexNode);	// set parent of leftIndexNode
		upIndexNode->setMostLeftChild(leftIndexNode);	// MostLeftChild of upIndexNode is leftIndexNode
		pIndexNode->setParent(upIndexNode);		// set parent of pIndexNode to upIndexNode
		root = upIndexNode;		// reset root
	}

}



//Return True if exceeds the maximum number of data that DataNode can has
bool BpTree::exceedDataNode(BpTreeNode* pDataNode){
	map <double, vector<pair<int, StockData*> > > *m = pDataNode->getDataMap();

	if(m->size() > order-1)			return true;
	else							return false;
}

//Return True if exceeds the maximum number of data that IndexNode can has
bool BpTree::exceedIndexNode(BpTreeNode* pIndexNode){
	map <double, BpTreeNode*>* m = pIndexNode->getIndexMap();
	
	if(m->size() > order-1)			return true;
	else							return false;
}


// print all data in BpTree in descending order of earning
bool BpTree::Print(){
	if (root == NULL)	return false;	// if BpTree is empty

	cout << "================== PRINT ===================" << endl;
	*fout << "================== PRINT ===================" << endl;

	BpTreeNode* pCur = root;	// pCur points root
	while (pCur->getMostLeftChild() != NULL)
		pCur = pCur->getMostLeftChild();	
	// pCur finally points the first DataNode
	
	BpTreeDataNode* pData = (BpTreeDataNode*)pCur;
	while (pData->getNext() != NULL) {
		pCur = pData->getNext();	
		pData = (BpTreeDataNode*)pCur;
	}	// pCur finally points the last DataNode
	
	map<double, vector<pair<int, StockData*>>>::iterator itData;
	vector<pair<int, StockData*>> temp;	// contain the data stored in vector

	while (pCur != NULL) // repeat until (pCur == NULL)
	{
		itData = pCur->getDataMap()->end();	
		--itData;	// itData represents the last index in current DataNode

		while(1) {
			 temp = itData->second; // temp contains the vector of the element itData points to

			for (int i = temp.size() - 1; i >= 0; i--)	// repeat as size of temp
				printStockInfo(temp[i].second);
				// if temp has multiple information, output in reverse order of temp since it should be printed as descending order of StockID

			if(itData == pCur->getDataMap()->begin())	//Once you have printed out all the information in the current DataNode, break the loop
				break;
			
			--itData;	// reduce itData if there is information not already in the DataNode
		} //Prints all information currently in DataNode

		pCur = pData->getPrev();	// pCur points the previous DataNode
		pData = (BpTreeDataNode*)pCur;	// pData points the same node with pCur
	}	// print all data from last node to first node of DataNode

	cout << "===========================================" << endl << endl;
	*fout << "===========================================" << endl << endl;

	return true;
}

// Search for values that has Earning between a and b
bool BpTree::Search(double a, double b)
{
	if (root == NULL)	return false;	// if BpTree is empty

	BpTreeNode* pCur = root;	// pCur points root
	while (pCur->getMostLeftChild() != NULL)
		pCur = pCur->getMostLeftChild();
	// pCur finally points the first DataNode

	stack<StockData*> S;	// stack that stores StockData

	map<double, vector<pair<int, StockData*>>>::iterator itData;
	vector<pair<int, StockData* >> temp;

	while (pCur != NULL) {	// repeat until (pCur == NULL)
		itData = pCur->getDataMap()->begin();	// itData represents the first index of the current DataNode

		while (itData != pCur->getDataMap()->end())// after itData points to all elements of the Map
		{
			temp = itData->second;	// temp is vector of itData

			if (itData->first >= a && itData->first <= b)  { // if the value that itData represents is in corresponding range
				// If the vector size is greater than 1, the Earing is the same and there are stored vector values.
				for (int i = 0; i < itData->second.size(); i++)	// repeat as vector size
					S.push(temp[i].second);	 // store StockData in Stack
			}
			itData++;
		}

		pCur = pCur->getNext();	// pCur points the next DataNode
	}

	if (S.empty())	return false;	// return false since the value of the range does not exist


	cout << "======= SEARCH =======" << endl;
	*fout << "======= SEARCH =======" << endl;

	while (!S.empty()) {  // repeat until stack is empty
		printStockInfo(S.top());	// print StockData of S
		S.pop();	// pop the frontmost element
	}

	cout << "======================" << endl << endl;
	*fout << "======================" << endl << endl;


	return true;
}

void BpTree::printStockInfo(StockData* pStockData){
	// print all data in console and log.txt
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
