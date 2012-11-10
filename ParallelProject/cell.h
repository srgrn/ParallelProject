#pragma once
class cell
{
public:
	cell(int x,int y,int len);
	~cell(void);
private:
	int topx;
	int topy;
	int length;
};

