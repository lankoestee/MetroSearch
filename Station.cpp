//Station.cpp
#include"Station.h"
using namespace std;

Station::Station(int stationId_, const Line& line_, const string& stationName_
	, double lon_, double lat_)
	:Line(line_)
	, stationName(stationName_)
	, stationId(stationId_)
	, next(NULL)
	, stationLon(lon_)
	, stationLat(lat_) {}

Station::~Station() {  //销毁邻接表
	Adjacent* p, * q = next;
	while (q) {
		p = q->next;
		delete q;
		q = p;
	}
}

int Station::stationid() {
	return stationId;
}

string Station::stationname() {
	return stationName;
}

double Station::longitude() {
	return stationLon;
}

double Station::latitude() {
	return stationLat;
}

void Station::addAdjacent(const Station& nextStation) {
	Adjacent* temp = new Adjacent;
	temp->adjacentId = nextStation.stationId;
	if (stationLon == nextStation.stationLon && stationLat == nextStation.stationLat) {
		temp->spacing = 0;
	}
	else {
		temp->spacing = distance(*this, nextStation);
	}
	temp->next = NULL;
	if (!next) {
		next = temp;
		return;
	}
	Adjacent* p = next;
	while (p->next) {
		p = p->next;
	}
	p->next = temp;
}

Adjacent* Station::adjacent() {
	return next;
}

double distance(const Station& station1_, const Station& station2_) {
	double lon1 = station1_.stationLon * PI / 180, lat1 = station1_.stationLat * PI / 180
		, lon2 = station2_.stationLon * PI / 180, lat2 = station2_.stationLat * PI / 180;
	return abs(6372 * (acos(cos(lat1) * cos(lat2) * cos(lon1 - lon2)
		+ sin(lat1) * sin(lat2))));
}