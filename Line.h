//Line.h
#pragma once
#include<QColor>
#include<iostream>
#include<vector>
#include<string>
using namespace std;

class Line {
public:
	Line(int lineId_, const string& lineName_
		, const QColor& lineColor_, bool lineLoop_);
	~Line() {}
	int lineid();
	string linename();
	QColor linecolor();
	bool lineloop();
	bool operator==(const Line& p);
	bool operator!=(const Line& p);
protected:
	int lineId;
	string lineName;
	QColor lineColor;
	bool lineLoop;
};