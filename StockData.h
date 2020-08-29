#ifndef _STUDENTDATA_H_
#define _STUDENTDATA_H_
#pragma warning (disable: 4996)	// to avoid error message by using strtok

#include <iostream>
#include <cstring>
#include <fstream>
#include <map>
#include <math.h>
#include <vector>
#include <algorithm>
#include <deque>
#include <queue>
#include <stack>

using namespace std;

class StockData{
private:
	int			StockID;	
	char		Name[20];
	int Opening_Price;	
	int Closing_Price;		
	int Volume;				
	double	Earning;	

public:
	StockData() {	// constructor
		StockID = 0;
		memset(Name, 0, 20);
		Earning = 0;
		Opening_Price = 0;
		Closing_Price = 0;
		Volume = 0;
	};
	~StockData() {};

	void setStockID(int a) { StockID = a; }
	void setName(char* a) { strcpy(Name, a); }
	void setOpening_Price(int a) { Opening_Price = a; }
	void setClosing_Price(int a) { Closing_Price = a; }
	void setVolume(int a) { Volume = a; }	
	void setEarning(int a, int b)	// calculate and round up the return from the third digit below
	{
		Earning = (b - a) / (double)a;	// formula to get Earning

		if(Earning >= 0)  Earning += 0.005;	// If result is positive
		else   Earning -= 0.005;			// If result is negative
		Earning = (int)(Earning * 100);
		Earning /= 100;
	}

	int		getStockID() { return StockID; }
	char*	getName() { return Name; }
	double	getEarning() { return Earning; }
	int		getOpening_Price() { return Opening_Price; }
	int		getClosing_Price() { return Closing_Price; }
	int		getVolume() { return Volume; }

};

bool compare(pair< pair< double, int>, StockData* > a, pair< pair< double, int>, StockData* > b);

#endif
