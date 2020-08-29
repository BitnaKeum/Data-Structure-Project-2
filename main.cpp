#include "Manager.h"


int main(void){
	cout.setf(ios::fixed);
	cout.precision(2);	// Set to output to second digit below decimal point

	int bpOrder = 3;

	Manager ds(bpOrder);
	ds.run("command.txt");

	return 0;
}


bool compare( pair< pair< double, int>, StockData* > a, pair< pair< double, int>, StockData* > b ){
	if( a.first.first == b.first.first){	// if Earning of a and b is same, compare StockID
		return a.first.second > b.first.second;	
	}else{		// if Earning is not same, compare Earning
		return a.first.first < b.first.first;
	}
}
