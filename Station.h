//Station.h
#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<cmath>
#include"Line.h"
#define PI 3.1415926535
using namespace std;

struct Adjacent {
	int adjacentId;
	double spacing;
	Adjacent* next;
};

class Station
	:public Line {
public:
	Station(int stationId_, const Line& line_, const string& stationName_
		, double lon_, double lat_);
	~Station();
	int stationid();
	string stationname();
	double longitude();
	double latitude();
	Adjacent* adjacent();
	void addAdjacent(const Station& nextStation);
	friend double distance(const Station& station1_, const Station& station2_);
protected:
	int stationId;
	double stationLon, stationLat;
	string stationName;
	Adjacent* next;
};