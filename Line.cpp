//Line.cpp
#include"Line.h"
using namespace std;

Line::Line(int lineId_, const string& lineName_, const QColor& lineColor_, bool lineLoop_)
	:lineName(lineName_)
	, lineColor(lineColor_)
	, lineId(lineId_)
	, lineLoop(lineLoop_) {}

int Line::lineid() {
	return lineId;
}

string Line::linename() {
	return lineName;
}

QColor Line::linecolor() {
	return lineColor;
}

bool Line::lineloop() {
	return lineLoop;
}

bool Line::operator==(const Line& p) {
	if (lineId == p.lineId) {
		return true;
	}
	return false;
}

bool Line::operator!=(const Line& p) {
	if (lineId != p.lineId) {
		return false;
	}
	return true;
}