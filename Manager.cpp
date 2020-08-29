#include "Manager.h"
#pragma warning (disable: 4996)	// to avoid error message by using strtok

Manager::Manager(int bpOrder){
	fout.open("log.txt", ios::app);
	avl = new AVLTree(&fout);
	bp = new BpTree(&fout, bpOrder);	
}

Manager::~Manager(){	// destructor
	delete avl;
	delete bp;
	fout.close();
}

void Manager::run(const char* command_txt){
	fout.setf(ios::fixed);
	fout.precision(2);	// set output to second digit below decimal

	ifstream fin;		
	fin.open(command_txt);
	int	load = -1;
		
	if(!fin){	// if the text file does not exist
		cout<<"[ERROR] command file open error!"<<endl;
		return;
	}

	char*	str=NULL;
	char	buf[128]={0};
	char	buf2[128]={0};

	while(fin.getline(buf, 64)){	// read a line (up to 64 characters) from command.txt and store it in a busf
		strcpy(buf2, buf);			// copy a string from a buf to a buf2
		str=strtok(buf, " \n");		// cut and store the string of the buf in str based on the space or '\n'
		

		if(strcmp(str, "LOAD") == 0){				// if command is "LOAD"
			if(load == 1) 	// if tree is already loaded
				printErrorCode(100);	// print error code
			else if((str=strtok(NULL, " \n")) != NULL || !LOAD()) 	// if a string is attached after the LOAD command or if the LOAD function is not executed correctly
				printErrorCode(100);	// print error code

			load = 1;	// load=1 if LOAD has been run once
		}
		else if(strcmp(str, "SEARCH_AVL") == 0){	// if command is "SEARCH_AVL"
			str = strtok(NULL, " \n");

			if (str == NULL) 	// if name is not inputted
				printErrorCode(300);	// print error code
			else if (!avl->Search(str))	//  if Search function doesn't be executed well
				printErrorCode(300);

		}
		else if(strcmp(str, "SEARCH_BP") == 0){		// if command is "SEARCH_BP"
			char *start = strtok(NULL, " \n"); 
			char *end = strtok(NULL, " \n");

			if (start == NULL || end == NULL)	// print an error code if there are no two strings behind the command
				printErrorCode(200);
			else if (!SEARCH_BP(atof(start), atof(end)))	// transfer string start and end into double type and convey to function
				printErrorCode(200);
		}
		else if(strcmp(str, "RANK") == 0){			// if command is "RANK"
			if (strtok(NULL, " \n") != NULL)	// if there is string behind command
				printErrorCode(400);
			if (!RANK())	// if RANK function doesn't executed well, print error code
				printErrorCode(400);
		}
		else if (strcmp(str, "PRINT_AVL") == 0) {		// if command is "PRINT_AVL"
			if (strtok(NULL, " \n") != NULL)	// if there is string behind command
				printErrorCode(600);
			else if (!PRINT_AVL())	// if PRINT_AVL function doesn't be executed, print error code
				printErrorCode(600);
		}
		else if (strcmp(str, "PRINT_BP") == 0) {		// if command is "PRINT_BP"
			if (strtok(NULL, " \n") != NULL)	// if there is string behind command
				printErrorCode(500);
			else if (!PRINT_BP())	// if PRINT_BP function doesn't be executed well, print ErrorCode
				printErrorCode(500);
		}	
		else if (strcmp(str, "EXIT") == 0) {			// if command is "EXIT"
			cout << "========== EXIT =========" << endl;
			cout << "Success" << endl;
			cout << "=========================" << endl;

			fout << "========== EXIT =========" << endl;
			fout << "Success" << endl;
			fout << "=========================" << endl;

			break;
		}
		
		fout<<endl;
	}	
	fin.close();	// close the file
}



bool Manager::LOAD(){	// store data by reading stock_list.txt
	char*	str=NULL;
	char	buf[128]={0};

	ifstream fin;
	fin.open("stock_list.txt");	// open the file
	
	if(!fin){	// if the text file does not exist
		return false;
	}
		
	while(fin.getline(buf, 64)){	// read a line (up to 64 characters) from stock_list.txt and store it in a buf
		StockData* pSto = new StockData();

		// set data of buf to pSto
		str = strtok(buf, " ");
		pSto->setStockID(atoi(str));	// set StockID
		str = strtok(NULL, " ");
		pSto->setName(str);				// set Name
		int open = atoi(strtok(NULL, " "));		// invert string to int
		pSto->setOpening_Price(open);	// set Opening_Price
		int close = atoi(strtok(NULL, " "));	// invert string to int
		pSto->setClosing_Price(close);	// set Closing_Price
		int volume = atoi(strtok(NULL, " "));	// invert string to int
		pSto->setVolume(volume);		// set Volume

		pSto->setEarning(open, close);	// set Earning

		avl->Insert(pSto);		// insert pSto node to AVLTree
		bp->Insert(pSto);		// insert pSto node to BpTree
	}
	
	cout << "======== LOAD ========" << endl
		 << "Success " << endl
		 << "======================" << endl << endl;
	fout << "======== LOAD ========" << endl
		 << "Success " << endl
	 	 << "======================" << endl << endl;
	
	fin.close();

	return true;
}

bool Manager::SEARCH_AVL(char* name){	// find node that has 'name' as 'Name' in AVLTree
	if(!avl->Search(name))	return false;	// if the node does not exist in AVLTree
	else					return true;	// if the node exists
}

bool Manager::SEARCH_BP(double a, double b){
	if(!bp->Search(a, b))			return false;
	else											return true;
}
                                                                                                                                                                                         
bool Manager::RANK(){
	if(!avl->Rank())		return false;	// if vHeap does not exist
	else					return true;
}

bool Manager::PRINT_AVL(){
	if(!avl->Print())	return false;		// if AVLTree does not exist
	else					return true;
}

bool Manager::PRINT_BP(){
	if(!bp->Print())		return false;	// if BpTree does not exist
	else					return true;
}
 
void Manager::printErrorCode(int n){
	cout << "======== ERROR ========" << endl;
	cout << n << endl;
	cout << "=======================" << endl << endl;
	
	fout<< "======== ERROR ========"<< endl;
	fout<< n << endl;
	fout<< "=======================" << endl << endl;
}
