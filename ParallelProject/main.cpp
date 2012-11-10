#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "board.h"

using namespace std;

board readHeader(istream& is)
{
	char str[50];
	vector<int> params;
	while(!is.eof() && params.size()<4)
	{	 
		is.getline(str,50);
		params.push_back(atoi(str));
	}
	board b(params[0],params[1],params[2],params[3]);
	//board b(1000,1000,10,10);
	return b;
}

int main(int argc, char* argv[]) { 

	fstream inputfile; 
	char *inputname;                 // input filename
	
	inputname = argv[1];
	inputfile.open(inputname,ios::in);
	string str="";   
	board b = readHeader(inputfile);
	while(!inputfile.eof())
	{	 
		getline(inputfile,str);
		cout << str << endl;
	}

	return 0;
}

