#include "helpers.h"


helpers::helpers(void)
{
}


helpers::~helpers(void)
{
}

board helpers::readHeader(istream& is)
{
	char str[50];
	vector<int> params;
	while(is.good() && params.size()<=4)
	{	 
		is.getline(str,50);
		params.push_back(atoi(str));
	}
	board b(params[0],params[1],params[2],params[3],params[4]);
	//board b(1000,1000,10,10);
	return b;
}

fstream helpers::openFile(char *filename)
{
	fstream inputfile; 
	inputfile.open(filename,ios::in);
	return inputfile;
}