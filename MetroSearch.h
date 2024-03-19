//MetroSearch.h
#pragma once

//对中文字符使用utf-8编码
#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

#include<QtWidgets/QWidget>
#include<QDockWidget>
#include<QPushButton>
#include<QThread>
#include<QTimer>
#include<QLabel>
#include<QColor>
#include<QSize>
#include<QPainter>
#include<QPaintEvent>
#include<QLineEdit>
#include<QComboBox>
#include<QSlider>
#include<QDebug>
#include<QObject>
#include<QToolTip>
#include<QAbstractItemView>
#include<QAbstractItemModel>
#include<QStandardItem>
#include<QStyledItemDelegate>
#include<QMessageBox>
#include"ui_MetroSearch.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include"Python.h"
#include<string>
#include<vector>
#include<stack>
#include<queue>
#include"Line.h"
#include"Station.h"

#define MAXINT 10000000

const double MAXTRANSFERCOST = 4;
const double MAXSITESCOST = 1;

using namespace std;

enum paintType {
    None = 0,
    route,
    map,
    lineMap
};

class MetroSearch : public QWidget {
    Q_OBJECT

public:
    MetroSearch(QWidget* parent = nullptr);
    ~MetroSearch();
    void initCityBox();
    void initHistoryBox();
    void buildNetwork();
    void dijkstraSearch(int startIndex);
    string terminalDirection(int startIndex, int nextIndex);

private:
    Ui::MetroSearchClass ui;
    QComboBox* chooseCity, * chooseStartLine, * chooseStartStation;
    QComboBox* chooseEndLine, * chooseEndStation, * lineBox;
    QComboBox* historyBox;
    QPushButton* searchButton, * exchangeButton, * switchButton;
    QPushButton* lineButton, * updateButton, * historyButton;
    QSlider* transferCostSlider, * sitesCostSlider;
    QLabel* cityIntro, * startLineIntro, * startStationIntro;
    QLabel* endLineIntro, * endStationIntro;
    QLabel* leftTransfer, * rightTransfer;
    QLabel* leftSites, * rightSites;
    QLabel* updateLabel, * historyLabel;
    QLabel* metroIconLabel, * metroIconLabel2, * metroIconLabel3, * cityIcon;
    QFont ft, exchangeFt, sliderFt;
    QTimer timer;
    bool isPaintRoute, isPaintMap;
    int canvasH, canvasW, historyCapacity;

    vector<string> cityListEng, cityList;
    vector<Line*> lineList;
    vector<Station*> stationList;
    vector<QPoint> pointPath;
    QRect paintArea;
    int action, state;
    int stationCount;
    int schemeIndex;
    double transferCost;  //换乘代价
    double sitesCost;  //途径代价
    double redProportion;
    double greenProportion;
    double blueProportion;
    double lightThershold;
    double limitedLat;
    bool isOut;

    //dijstra算法数据成员
    stack<int> scheme;  //最终方案栈
    stack<int> reservedScheme;
    int schemeSize;
    int* schemePath;
    int* updater;  //updater用以记录距离缩短者
    double* dist, * pi;
    double* costDist, * costPi;  //两者为综合代价距离，包含换乘与站台停留时间因素

public slots:
    void initLineBox(int index);
    void initStartStationBox(int index);
    void initEndStationBox(int index);
    void paintEvent(QPaintEvent* event);
    bool event(QEvent* event);
    void searchButtonPaint();
    void switchButtonPaint();
    void lineButtonPaint();
    void updateInfo();
    void transferCostChange(int value);
    void sitesCostChange(int value);
    void exchangeStation();
    void applyHistory();
};

//定义数组求最小函数（返回最小值的下标）
template<class type>
int arrayMin(type* array, int size);