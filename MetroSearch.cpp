//MetroSearch.cpp
#include "MetroSearch.h"

//定义数组求最小函数（返回最小值的下标）
template<class type>
int arrayMin(type* array, int size) {
    int minSubcript;
    type min = MAXINT;
    for (int i = 0; i < size; i++) {
        if (array[i] == -1) {
            continue;
        }
        if (array[i] < min) {
            minSubcript = i;
            min = array[i];
        }
    }
    return minSubcript;
}

//——————————MetroSearch构造函数——————————
MetroSearch::MetroSearch(QWidget* parent)
    : QWidget(parent)
    , canvasH(760)
    , canvasW(620)
    , paintArea(920, 50, 660, 800)
    , transferCost(MAXTRANSFERCOST / 2)
    , sitesCost(MAXSITESCOST / 2)
    , redProportion(0.2125)
    , greenProportion(0.7154)
    , blueProportion(0.0721)
    , lightThershold(128)
    , isPaintRoute(false)
    , isPaintMap(false)
    , limitedLat(0.06)
    , action(paintType::None)
    , state(paintType::None)
    , isOut(false)
    , historyCapacity(20)
{
    //ui.setupUi(this);  //不对图形化编程头进行引用
    setWindowTitle("地铁怎么坐");
    resize(1600, 900);
    this->setMouseTracking(true);
    this->setStyleSheet("background-color:white;");
    this->setWindowIcon(QIcon(QPixmap(QString::fromStdString(":/MetroSearch/figure/metroIcon.png"))));

    //字体大小及风格统一
    ft.setFamily("微软雅黑");
    ft.setPointSize(10);
    ft.setBold(true);
    exchangeFt.setFamily("Arial");
    exchangeFt.setPointSize(12);
    exchangeFt.setBold(true);
    exchangeFt.setLetterSpacing(QFont::PercentageSpacing, 75);
    sliderFt.setFamily("SimHei");
    sliderFt.setPointSize(10);
    sliderFt.setBold(true);

    //初始界面部件设计
    cityIntro = new QLabel(QString::fromUtf8("城市"), this);
    cityIntro->setFont(ft);
    cityIntro->setGeometry(250, 50, 60, 50);
    cityIntro->setAlignment(Qt::AlignVCenter);

    chooseCity = new QComboBox(this);
    chooseCity->setGeometry(310, 50, 150, 50);
    chooseCity->setFont(ft);

    startLineIntro = new QLabel(QString::fromUtf8("起点线路"), this);
    startLineIntro->setAlignment(Qt::AlignVCenter);
    startLineIntro->setFont(ft);
    startLineIntro->setGeometry(80, 140, 100, 35);

    chooseStartLine = new QComboBox(this);
    chooseStartLine->setGeometry(80, 175, 290, 50);
    chooseStartLine->setFont(ft);
    chooseStartLine->setStyleSheet(QString("QComboBox{selection-background-color:white;selection-color:black;}"));

    startStationIntro = new QLabel(QString::fromUtf8("起点站"), this);
    startStationIntro->setAlignment(Qt::AlignVCenter);
    startStationIntro->setFont(ft);
    startStationIntro->setGeometry(385, 140, 100, 35);

    chooseStartStation = new QComboBox(this);
    chooseStartStation->setGeometry(385, 175, 290, 50);
    chooseStartStation->setFont(ft);

    endLineIntro = new QLabel(QString::fromUtf8("终点线路"), this);
    endLineIntro->setAlignment(Qt::AlignVCenter);
    endLineIntro->setFont(ft);
    endLineIntro->setGeometry(80, 240, 100, 35);

    chooseEndLine = new QComboBox(this);
    chooseEndLine->setFont(ft);
    chooseEndLine->setGeometry(80, 275, 290, 50);
    chooseEndLine->setStyleSheet(QString("selection-background-color:white;selection-color:black;"));

    endStationIntro = new QLabel(QString::fromUtf8("终点站"), this);
    endStationIntro->setAlignment(Qt::AlignVCenter);
    endStationIntro->setFont(ft);
    endStationIntro->setGeometry(385, 240, 100, 35);

    chooseEndStation = new QComboBox(this);
    chooseEndStation->setFont(ft);
    chooseEndStation->setGeometry(385, 275, 290, 50);

    searchButton = new QPushButton(QString::fromUtf8("查询"), this);
    searchButton->setGeometry(690, 175, 100, 150);
    searchButton->setFont(ft);
    searchButton->setStyleSheet(QString("QPushButton{background-color:#2b4490;color:white;border-radius:10px;font-size:16pt;}QPushButton:pressed{border-style:solid;border-width:5px;border-color:#102b6a;}"));
    
    transferCostSlider = new QSlider(this);
    transferCostSlider->setOrientation(Qt::Horizontal);
    transferCostSlider->setRange(0, 100);
    transferCostSlider->setGeometry(130, 375, 500, 30);
    transferCostSlider->setValue(50);

    sitesCostSlider = new QSlider(this);
    sitesCostSlider->setOrientation(Qt::Horizontal);
    sitesCostSlider->setRange(0, 100);
    sitesCostSlider->setGeometry(130, 450, 500, 30);
    sitesCostSlider->setValue(50);

    exchangeButton = new QPushButton(QString::fromUtf8("↑↓"), this);
    exchangeButton->setFont(exchangeFt);
    exchangeButton->setGeometry(15, 225, 50, 50);
    exchangeButton->setStyleSheet(QString("text-align:center;padding-right:4px;padding-bottom:6px;"));

    leftTransfer = new QLabel(QString("距离<br/>优先"), this);
    leftTransfer->setAlignment(Qt::AlignHCenter);
    leftTransfer->setFont(sliderFt);
    leftTransfer->setGeometry(70, 367, 60, 60);

    rightTransfer = new QLabel(QString("换乘少<br/>优先"), this);
    rightTransfer->setAlignment(Qt::AlignHCenter);
    rightTransfer->setFont(sliderFt);
    rightTransfer->setGeometry(630, 367, 80, 60);

    leftSites = new QLabel(QString("距离<br/>优先"), this);
    leftSites->setAlignment(Qt::AlignHCenter);
    leftSites->setFont(sliderFt);
    leftSites->setGeometry(70, 442, 60, 60);

    rightSites = new QLabel(QString("站点少<br/>优先"), this);
    rightSites->setAlignment(Qt::AlignHCenter);
    rightSites->setFont(sliderFt);
    rightSites->setGeometry(630, 442, 80, 60);

    switchButton = new QPushButton(QString("切换地图显示模式"), this);
    switchButton->setFont(ft);
    switchButton->setGeometry(550, 540, 200, 40);

    lineButton = new QPushButton("显示线路", this);
    lineButton->setFont(ft);
    lineButton->setGeometry(370, 540, 100, 40);

    lineBox = new QComboBox(this);
    lineBox->setFont(ft);
    lineBox->setStyleSheet(QString("selection-background-color:white;selection-color:black;"));
    lineBox->setGeometry(70, 540, 300, 40);

    updateButton = new QPushButton(QString("更新"), this);
    updateButton->setFont(ft);
    updateButton->setGeometry(220, 740, 100, 40);

    historyBox = new QComboBox(this);
    historyBox->setFont(ft);
    historyBox->setGeometry(220, 640, 400, 40);

    historyButton = new QPushButton(QString("应用"), this);
    historyButton->setFont(ft);
    historyButton->setGeometry(630, 640, 80, 40);

    updateLabel = new QLabel(QString::fromStdString("全线网更新"), this);
    updateLabel->setFont(ft);
    updateLabel->setAlignment(Qt::AlignCenter);
    updateLabel->setGeometry(70, 740, 150, 40);

    historyLabel = new QLabel(QString::fromStdString("历史记录"), this);
    historyLabel->setFont(ft);
    historyLabel->setAlignment(Qt::AlignCenter);
    historyLabel->setGeometry(70, 640, 150, 40);

    metroIconLabel = new QLabel(this);
    metroIconLabel->setPixmap(QPixmap::fromImage(
        QImage(QString(":/MetroSearch/figure/metroIcon.png"))));
    metroIconLabel->setGeometry(600, 0, 150, 150);
    metroIconLabel->setScaledContents(true);

    metroIconLabel2 = new QLabel(this);
    metroIconLabel2->setPixmap(QPixmap::fromImage(
        QImage(QString(":/MetroSearch/figure/metroIcon2.png"))));
    metroIconLabel2->setGeometry(15, 175, 50, 50);
    metroIconLabel2->setScaledContents(true);

    metroIconLabel3 = new QLabel(this);
    metroIconLabel3->setPixmap(QPixmap::fromImage(
        QImage(QString(":/MetroSearch/figure/metroIcon3.png"))));
    metroIconLabel3->setGeometry(15, 275, 50, 50);
    metroIconLabel3->setScaledContents(true);

    cityIcon = new QLabel(this);
    cityIcon->setPixmap(QPixmap::fromImage(
        QImage(QString(":/MetroSearch/figure/cityIcon.png"))));
    cityIcon->setGeometry(190, 50, 50, 50);
    cityIcon->setScaledContents(true);


    //初始化城市复选框
    initCityBox();

    //初始化线路复选框
    initLineBox(0);

    //信号及槽函数
    connect(chooseCity,
        SIGNAL(currentIndexChanged(int)),
        this,
        SLOT(initLineBox(int)));
    connect(chooseStartLine,
        SIGNAL(currentIndexChanged(int)),
        this,
        SLOT(initStartStationBox(int)));
    connect(chooseEndLine,
        SIGNAL(currentIndexChanged(int)),
        this,
        SLOT(initEndStationBox(int)));
    connect(searchButton,
        &QPushButton::clicked,
        this,
        &MetroSearch::searchButtonPaint);
    connect(transferCostSlider,
        SIGNAL(valueChanged(int)),
        this,
        SLOT(transferCostChange(int)));
    connect(sitesCostSlider,
        SIGNAL(valueChanged(int)),
        this,
        SLOT(sitesCostChange(int)));
    connect(exchangeButton,
        &QPushButton::clicked,
        this,
        &MetroSearch::exchangeStation);
    connect(switchButton,
        &QPushButton::clicked,
        this,
        &MetroSearch::switchButtonPaint);
    connect(lineButton,
        &QPushButton::clicked,
        this,
        &MetroSearch::lineButtonPaint);
    connect(updateButton,
        &QPushButton::clicked,
        this,
        &MetroSearch::updateInfo);
    connect(historyButton,
        &QPushButton::clicked,
        this,
        &MetroSearch::applyHistory);
}

//——————————MetroSearch析构函数——————————
MetroSearch::~MetroSearch() {
    delete chooseCity, chooseStartLine, chooseEndLine, chooseStartStation
        , chooseEndStation, lineBox, searchButton, switchButton, exchangeButton
        , lineButton, transferCostSlider, sitesCostSlider, cityIntro
        , startLineIntro, startStationIntro, endLineIntro, endStationIntro
        , leftTransfer, rightTransfer, leftSites, rightSites
        , updateButton, metroIconLabel, metroIconLabel2, metroIconLabel3
        , historyButton, updateLabel, historyLabel, historyBox;
    lineList.clear();
    stationList.clear();
    cityListEng.clear();
    pointPath.clear();
    while (!scheme.empty()) {
        scheme.pop();
    }
    while (!reservedScheme.empty()) {
        reservedScheme.pop();
    }
}

//——————————初始化城市复选框——————————
void MetroSearch::initCityBox() {
    ifstream inFile("./city/cityIndex.csv", ios::in);
    string lineStr;
    bool first = true;
    while (getline(inFile, lineStr)) {
        // 存成二维表结构
        if (first) {
            first = false;
            continue;
        }
        stringstream ss(lineStr);
        string str;
        vector<string> lineArray;

        // 按照逗号分隔
        while (getline(ss, str, ',')) {
            lineArray.push_back(str);
        }
        chooseCity->addItem(QString::fromStdString(lineArray[0]));
        cityList.push_back(lineArray[0]);
        cityListEng.push_back(lineArray[1]);
    }
    getchar();
    inFile.close();
    QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(chooseCity->view()->model());
    if (model) {
        for (int i = 0; i < model->rowCount(); i++) {
            model->item(i)->setTextAlignment(Qt::AlignCenter);
            model->item(i)->setSizeHint(QSize(270, 35));
        }
    }
    initHistoryBox();
}

//——————————初始化线路复选框——————————
void MetroSearch::initLineBox(int cityIndex) {
    chooseStartLine->blockSignals(true);  //为防止删除和添加选项操作触发信号函数，故在此进行信号屏蔽
    chooseEndLine->blockSignals(true);
    lineBox->blockSignals(true);
    chooseStartLine->clear();  //清除遗留数据
    chooseEndLine->clear();
    lineBox->clear();
    for (int i = 0; i < stationList.size(); i++) {
        delete stationList[i];
    }
    stationList.clear();
    for (int i = 0; i < lineList.size(); i++) {
        delete lineList[i];
    }
    lineList.clear();
    string prefix("./city/"), suffix(".csv");
    string fileName = prefix + cityListEng[cityIndex] + suffix;
    ifstream inFile(fileName, ios::in);
    string lineStr;
    bool first = true;
    int stationI = 0, lineI = 0;
    Line* tempLinePtr = NULL;

    //逐行读取数据
    while (getline(inFile, lineStr)) {
        if (first) {
            first = false;
            continue;
        }
        stringstream ss(lineStr);
        string str;
        vector<string> lineArray;
        while (getline(ss, str, ',')) {
            lineArray.push_back(str);
        }

        //增添新的线路，同时为lineList添加新对象
        if (!tempLinePtr || tempLinePtr->linename() != lineArray[0]) {
            QColor color(lineArray[2].c_str());  //提取颜色信息
            Line* tempLine;
            if (lineArray[6] == "1") {
                tempLine = new Line(lineI, lineArray[0], color, true);  //使用指针防止对象在当次循环结束后销毁
            }
            else {
                tempLine = new Line(lineI, lineArray[0], color, false);
            }
            tempLinePtr = tempLine;
            lineList.push_back(tempLinePtr);

            double light = redProportion * color.red() + greenProportion
                * color.green() + blueProportion * color.blue();  //将颜色转换为亮度
            chooseStartLine->addItem(QString::fromStdString(lineArray[0]));  //添加选项
            chooseStartLine->setItemData(lineI, color, Qt::BackgroundRole);  //设置选项背景颜色（线路色）
            chooseEndLine->addItem(QString::fromStdString(lineArray[0]));
            chooseEndLine->setItemData(lineI, color, Qt::BackgroundRole);
            lineBox->addItem(QString::fromStdString(lineArray[0]));
            lineBox->setItemData(lineI, color, Qt::BackgroundRole);
            //根据亮度阈值判断背景色偏黑或偏白，并设置字体为黑白反色
            if (light < lightThershold) {
                chooseStartLine->setItemData(lineI, QColor(Qt::white), Qt::ForegroundRole);
                chooseEndLine->setItemData(lineI, QColor(Qt::white), Qt::ForegroundRole);
                lineBox->setItemData(lineI, QColor(Qt::white), Qt::ForegroundRole);
            }
            else {
                chooseStartLine->setItemData(lineI, QColor(Qt::black), Qt::ForegroundRole);
                chooseEndLine->setItemData(lineI, QColor(Qt::black), Qt::ForegroundRole);
                lineBox->setItemData(lineI, QColor(Qt::black), Qt::ForegroundRole);
            }

            lineI++;
        }
        Station* tempStation = new Station(stationI++, *tempLinePtr, lineArray[3], stod(lineArray[4]), stod(lineArray[5]));  //使用指针创建对象防止单次循环后被销毁
        stationList.push_back(tempStation);
    }
    getchar();
    inFile.close();

    //复选框文本居中设置
    QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(chooseStartLine->view()->model());
    if (model) {
        for (int i = 0; i < model->rowCount(); i++) {
            model->item(i)->setTextAlignment(Qt::AlignCenter);
            model->item(i)->setSizeHint(QSize(270, 35));
        }
    }
    model = dynamic_cast<QStandardItemModel*>(chooseEndLine->view()->model());
    if (model) {
        for (int i = 0; i < model->rowCount(); i++) {
            model->item(i)->setTextAlignment(Qt::AlignCenter);
            model->item(i)->setSizeHint(QSize(270, 35));
        }
    }
    model = dynamic_cast<QStandardItemModel*>(lineBox->view()->model());
    if (model) {
        for (int i = 0; i < model->rowCount(); i++) {
            model->item(i)->setTextAlignment(Qt::AlignCenter);
            model->item(i)->setSizeHint(QSize(270, 35));
        }
    }
    chooseStartLine->blockSignals(false);
    chooseEndLine->blockSignals(false);
    lineBox->blockSignals(false);

    //组建地铁网络
    buildNetwork();

    //初始化站点复选框
    initStartStationBox(0);
    initEndStationBox(0);
}

//——————————初始化历史记录复选框——————————
void MetroSearch::initHistoryBox() {
    historyBox->clear();

    //读取历史记录文件
    ifstream inFile("history.csv", ios::in);
    string rowStr;
    vector<vector<string>> dataArray;
    while (getline(inFile, rowStr)) {
        stringstream ss(rowStr);
        string str;
        vector<string> rowArray;

        while (getline(ss, str, ',')) {
            rowArray.push_back(str);
        }
        dataArray.push_back(rowArray);
    }
    getchar();
    inFile.close();


    for (int i = 1; i < historyCapacity && i < dataArray.size(); i++) {
        string addStr = dataArray[i][0] + " | " + dataArray[i][2] + "->"
            + dataArray[i][4];
        historyBox->addItem(QString::fromStdString(addStr));
    }
    QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(historyBox->view()->model());
    if (model) {
        for (int i = 0; i < model->rowCount(); i++) {
            model->item(i)->setTextAlignment(Qt::AlignCenter);
            model->item(i)->setSizeHint(QSize(270, 35));
        }
    }
}

//——————————使用邻接表组建地铁网络——————————
void MetroSearch::buildNetwork() {
    stationCount = stationList.size();

    //连接同线邻站
    for (int i = 0; i < stationCount; i++) {
        if (i != 0 && stationList[i]->lineid() == stationList[i - 1]->lineid()) {
            stationList[i]->addAdjacent(*stationList[i - 1]);
            stationList[i - 1]->addAdjacent(*stationList[i]);
        }

        //将环线首尾相连
        if (stationList[i]->lineloop() && (i == 0
            || stationList[i]->lineid() != stationList[i - 1]->lineid())) {
            for (int j = i; j <= stationCount; j++) {
                if (j == stationCount
                    || stationList[j]->lineid() != stationList[i]->lineid()) {
                    stationList[i]->addAdjacent(*stationList[j - 1]);
                    stationList[j - 1]->addAdjacent(*stationList[i]);
                    break;
                }
            }
        }


        //连接不同线换乘站
        for (int j = 0; j < stationCount; j++) {
            if (i != j && stationList[i]->stationname() == stationList[j]->stationname()
                && stationList[i]->latitude() == stationList[j]->latitude()
                && stationList[i]->longitude() == stationList[j]->longitude()) {
                stationList[i]->addAdjacent(*stationList[j]);
            }
        }
    }
}

//——————————初始化起始站点复选框——————————
void MetroSearch::initStartStationBox(int index) {

    //清除复选框
    chooseStartStation->clear();

    //添加新项目
    for (int i = 0; i < stationCount; i++) {
        if (stationList[i]->lineid() == index) {
            chooseStartStation->addItem(QString::fromStdString(stationList[i]->stationname()));
        }
    }

    //使复选框项目居中
    QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(chooseStartStation->view()->model());
    if (model) {
        for (int i = 0; i < model->rowCount(); i++) {
            model->item(i)->setTextAlignment(Qt::AlignCenter);
            model->item(i)->setSizeHint(QSize(270, 35));
        }
    }
}

//——————————初始化终到站点复选框——————————
void MetroSearch::initEndStationBox(int index) {

    //清除复选框
    chooseEndStation->clear();

    //添加新项目
    for (int i = 0; i < stationCount; i++) {
        if (stationList[i]->lineid() == index) {
            chooseEndStation->addItem(QString::fromStdString(stationList[i]->stationname()));
        }
    }

    //使复选框项目居中
    QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(chooseEndStation->view()->model());
    if (model) {
        for (int i = 0; i < model->rowCount(); i++) {
            model->item(i)->setTextAlignment(Qt::AlignCenter);
            model->item(i)->setSizeHint(QSize(270, 35));
        }
    }
}

//——————————改变换乘代价——————————
void MetroSearch::transferCostChange(int value) {
    transferCost = value / 100.0 * MAXTRANSFERCOST;
}

//——————————改变途径代价——————————
void MetroSearch::sitesCostChange(int value) {
    sitesCost = value / 100.0 * MAXSITESCOST;
}

//——————————查询按钮响应槽函数——————————
void MetroSearch::searchButtonPaint() {

    //将该路线添加进入历史记录中
    string cityName = chooseCity->currentText().toStdString(),
        startName = chooseStartStation->currentText().toStdString(),
        endName = chooseEndStation->currentText().toStdString(),
        startLine = chooseStartLine->currentText().toStdString(),
        endLine = chooseEndLine->currentText().toStdString();

    //开始读取文件
    ifstream inFile("history.csv", ios::in);
    string rowStr;
    vector<vector<string>> dataArray;
    while (getline(inFile, rowStr)) {
        // 存成二维表结构
        stringstream ss(rowStr);
        string str;
        vector<string> rowArray;

        // 按照逗号分隔
        while (getline(ss, str, ',')) {
            rowArray.push_back(str);
        }
        dataArray.push_back(rowArray);
    }
    getchar();
    inFile.close();
    if (cityName == dataArray[1][0] && startLine == dataArray[1][1] && startName == dataArray[1][2]
        && endLine == dataArray[1][3] && endName == dataArray[1][4]) {

        //记录相同，无需重复记录
        //更新事件，使得paintEvent立即进入route绘图模式
        action = paintType::route;
        this->update();
        return;
    }
    ofstream outFile("history.csv", std::ios::out | std::ios::trunc);
    outFile << dataArray[0][0] << "," << dataArray[0][1] << "," << dataArray[0][2]
        << "," << dataArray[0][3] << "," << dataArray[0][4] << endl;
    outFile << cityName << "," << startLine << "," << startName << ","
        << endLine << "," << endName << endl;
    for (int i = 1; i < historyCapacity && i < dataArray.size(); i++) {
        outFile << dataArray[i][0] << "," << dataArray[i][1] << "," << dataArray[i][2]
            << "," << dataArray[i][3] << "," << dataArray[i][4] << endl;
    }
    outFile.close();
    
    //修改历史记录复选框
    historyBox->clear();
    string addString = cityName + " | " + startName + "->" + endName;
    historyBox->addItem(QString::fromStdString(addString));
    for (int i = 1; i < historyCapacity && i < dataArray.size(); i++) {
        addString = dataArray[i][0] + " | " + dataArray[i][2] + "->" + dataArray[i][4];
        historyBox->addItem(QString::fromStdString(addString));
    }

    //使复选框选项居中
    QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(historyBox->view()->model());
    if (model) {
        for (int i = 0; i < model->rowCount(); i++) {
            model->item(i)->setTextAlignment(Qt::AlignCenter);
            model->item(i)->setSizeHint(QSize(270, 35));
        }
    }

    //更新事件，使得paintEvent立即进入route绘图模式
    action = paintType::route;
    this->update();
}

//——————————切换按钮相应槽函数——————————
void MetroSearch::switchButtonPaint() {
    //更新事件，使得paintEvent立即进入map或route绘图模式
    if (state == paintType::route) {
        action = paintType::map;
    }
    else if (state == paintType::map) {
        action = paintType::route;
    }
    this->update();
}

//——————————线路查询按钮响应槽函数——————————
void MetroSearch::lineButtonPaint() {
    //更新事件，使得paintEvent立即进入lineMap绘图模式
    action = paintType::lineMap;
    this->update();
}

//——————————更新按钮响应槽函数——————————
void MetroSearch::updateInfo() {
    /*QMessageBox updateMessage(this);
    updateMessage.setBaseSize(300, 100);
    updateMessage.setFont(sliderFt);
    updateMessage.setWindowTitle(QString("地铁怎么坐"));
    updateMessage.setText(QString("线网更新中……"));
    updateMessage.show();*/
    QMessageBox::information(this,
        QString::fromStdString("地铁怎么坐"),
        QString::fromStdString("全线网更新中，点击OK以继续更新，点击后请耐心等待"));
    Py_Initialize();
    char szFile[] = "getInfo.py";
    FILE* fp = fopen(szFile, "r");
    if (fp && PyRun_SimpleFile(fp, szFile) != 0)
    {
        fclose(fp);
        qDebug() << "PyRun_SimpleFile" << szFile << "failed!";
    }
    Py_Finalize();
    QMessageBox::information(this,
        QString::fromStdString("地铁怎么做"),
        QString::fromStdString("全线网更新完成，您可以继续操作"));
    /*updateMessage.close();*/
}

//——————————历史记录应用按钮响应槽函数——————————
void MetroSearch::applyHistory() {

    //获取当前历史记录索引
    int index = historyBox->currentIndex();

    //开始读取文件部分
    ifstream inFile("history.csv", ios::in);
    string rowStr;

    //建立二维vector矩阵以表示csv文件内容
    vector<vector<string>> dataArray;

    //除去标题栏
    getline(inFile, rowStr);
    while (getline(inFile, rowStr)) {
        stringstream ss(rowStr);
        string str;
        vector<string> rowArray;

        //以逗号分隔
        while (getline(ss, str, ',')) {
            rowArray.push_back(str);
        }
        dataArray.push_back(rowArray);
    }
    getchar();
    inFile.close();

    //按照dataArray内容更改对应起点终点内容
    for (int i = 0; i < cityList.size(); i++) {
        if (cityList[i] == dataArray[index][0]) {
            chooseCity->setCurrentIndex(i);

            //用两个Ready的和保证在遍历完全后及时退出
            bool startReady = false, endReady = false;
            for (int j = 0; j < stationList.size(); j++) {

                //根据dataArray内容更改起点站复选框索引
                if (stationList[j]->linename() == dataArray[index][1]
                    && stationList[j]->stationname() == dataArray[index][2]) {
                    chooseStartLine->setCurrentIndex(stationList[j]->lineid());
                    chooseStartStation->setCurrentText(
                        QString::fromStdString(stationList[j]->stationname()));
                    startReady = true;
                }

                //根据dataArray内容更改终点站复选框索引
                if (stationList[j]->linename() == dataArray[index][3]
                    && stationList[j]->stationname() == dataArray[index][4]) {
                    chooseEndLine->setCurrentIndex(stationList[j]->lineid());
                    chooseEndStation->setCurrentText(
                        QString::fromStdString(stationList[j]->stationname()));
                    endReady = true;
                }
                if (startReady && endReady) {
                    break;
                }
            }

            //及时退出，避免无效运算
            break;
        }
    }
}

//——————————改变起点终点——————————
void MetroSearch::exchangeStation() {
    int slIndex = chooseStartLine->currentIndex()
        , ssIndex = chooseStartStation->currentIndex()
        , elIndex = chooseEndLine->currentIndex()
        , esIndex = chooseEndStation->currentIndex();
    chooseStartLine->setCurrentIndex(elIndex);
    chooseEndLine->setCurrentIndex(slIndex);
    chooseStartStation->setCurrentIndex(esIndex);
    chooseEndStation->setCurrentIndex(ssIndex);
    QRect temp = metroIconLabel2->geometry();
    metroIconLabel2->setGeometry(metroIconLabel3->geometry());
    metroIconLabel3->setGeometry(temp);
}

//——————————重写paintEvent函数——————————
void MetroSearch::paintEvent(QPaintEvent* event) {

    //route绘图模式
    if (action == paintType::route) {
        qDebug() << "route";

        if (isPaintRoute) {
            //清空所有数组和栈
            delete[] pi;
            delete[] dist;
            delete[] costPi;
            delete[] costDist;
            delete[] updater;
            while (!scheme.empty()) {
                scheme.pop();
            }
            while (!reservedScheme.empty()) {
                reservedScheme.pop();
            }
        }
        isPaintRoute = true;

        //赋予dijkstra算法所需数组空间
        pi = new double[stationCount];
        dist = new double[stationCount];
        costPi = new double[stationCount];
        costDist = new double[stationCount];
        updater = new int[stationCount];

        //清空画布
        QPainter painter(this);
        painter.eraseRect(paintArea);

        //收集复选框信息
        int startLineIndex = chooseStartLine->currentIndex()
            , endLineIndex = chooseEndLine->currentIndex()
            , startStationIndex = chooseStartStation->currentIndex()
            , endStationIndex = chooseEndStation->currentIndex();
        string startLineText = chooseStartLine->currentText().toStdString()
            , endLineText = chooseEndLine->currentText().toStdString()
            , startStationText = chooseStartStation->currentText().toStdString()
            , endStationText = chooseEndStation->currentText().toStdString();
        schemeIndex = 0;

        //通过遍历确定起点站和终点站在stationList中的位置
        for (int i = 0; i < stationCount; i++) {
            if (stationList[i]->stationname() == startStationText
                && stationList[i]->lineid() == startLineIndex) {
                startStationIndex = i;
            }
            if (stationList[i]->stationname() == endStationText
                && stationList[i]->lineid() == endLineIndex) {
                endStationIndex = i;
            }
        }

        //运行dijkstra算法，得到各个点的优化路径
        dijkstraSearch(startStationIndex);

        //——————————导出方案栈——————————
        //方案栈可能出现的几种情况
        //1、在起点阶段记录了一次换乘，仅换乘而只在两条线路中乘坐了一条，则忽略这次换乘
        //2、在终点阶段记录了一次换乘，仅换乘而只在两条线路中乘坐了一条，则忽略这次换乘
        //3、由于换乘代价的引入，除起点终点外，中途不存在无效换乘
        bool isBreak = false;
        int temp = endStationIndex;
        int last = -1;

        //若updater仍保留初始化值，则认为两站点脱网
        if (updater[endStationIndex] == -2) {
            isOut = true;
            scheme.push(endStationIndex);
            scheme.push(startStationIndex);
            reservedScheme = scheme;

            painter.translate(920, 50);
            painter.setFont(QFont("SimHei", 10));
            int pointY = 0, textY = 22;

            //绘制起点站圆点
            QBrush brush;
            brush.setColor(stationList[scheme.top()]->linecolor());
            brush.setStyle(Qt::SolidPattern);
            painter.setBrush(brush);
            painter.setPen(Qt::NoPen);
            painter.drawEllipse(0, pointY, 30, 30);

            //绘制起点站名称
            painter.setPen(QColor(Qt::black));
            painter.drawText(QPoint(60, textY), QString::fromStdString(startStationText));

            //绘制警告
            textY += 50;
            painter.drawText(QPoint(60, textY), QString::fromStdString("两站点脱网，无法通过既有线路换乘"));

            //绘制终点站圆点
            scheme.pop();
            pointY += 100;
            brush.setColor(stationList[scheme.top()]->linecolor());
            brush.setStyle(Qt::SolidPattern);
            painter.setBrush(brush);
            painter.setPen(Qt::NoPen);
            painter.drawEllipse(0, pointY, 30, 30);

            //绘制终点站名称
            textY += 50;
            painter.setPen(QColor(Qt::black));
            painter.drawText(QPoint(60, textY), QString::fromStdString(endStationText));
        }
        else {
            isOut = false;
            while (updater[temp] != -1) {

                //除去重复终点站
                if (last == endStationIndex && stationList[last]->stationname()
                    == stationList[temp]->stationname()) {
                    scheme.pop();
                }

                //一般途径站压栈
                scheme.push(temp);
                last = temp;
                temp = updater[temp];

                //除去重复起点站
                if (updater[temp] == -1 && stationList[temp]->stationname()
                    == stationList[scheme.top()]->stationname()) {
                    isBreak = true;
                    break;
                }
            }
            if (!isBreak) {
                scheme.push(temp);
            }
            reservedScheme = scheme;

            //进行换乘可视化的展示以及文本展示
            int lastId = -1, llastId;
            int pointY = 0, textY = 22;
            int stops = 0;
            bool isFirst = true;
            painter.translate(920, 50);
            painter.setFont(QFont("SimHei", 10));
            QFontMetrics fontMetrics(exchangeFt);

            while (!scheme.empty()) {  //不可使用scheme.size()，因其会随循环次数而变

                //绘制起点站
                if (isFirst) {

                    //绘制起点站圆点
                    QBrush brush;
                    brush.setColor(stationList[scheme.top()]->linecolor());
                    brush.setStyle(Qt::SolidPattern);
                    painter.setBrush(brush);
                    painter.setPen(Qt::NoPen);
                    painter.drawEllipse(0, pointY, 30, 30);

                    //绘制起点站名称
                    painter.setPen(QColor(Qt::black));
                    painter.drawText(QPoint(60, textY), QString::fromStdString(startStationText));
                    isFirst = false;
                }

                //绘制换乘站
                else if (stationList[lastId]->lineid() != stationList[scheme.top()]
                    ->lineid()) {
                    int nowId = scheme.top();
                    QPen pen;
                    QBrush brush;

                    //绘制自上一起点站原点下来的线段
                    pen.setColor(stationList[lastId]->linecolor());
                    pen.setWidth(5);
                    painter.setPen(pen);
                    painter.drawLine(QPoint(15, pointY + 5), QPoint(15, pointY + 120));

                    //绘制在同一线路内的乘坐方案
                    //包括线路名称，终点站方向，乘坐站数
                    string terminal = terminalDirection(llastId, lastId);
                    brush.setColor(stationList[lastId]->linecolor());
                    brush.setStyle(Qt::SolidPattern);

                    //计算线路背景颜色以及字体颜色
                    QColor foreColor = stationList[lastId]->linecolor();
                    double light = redProportion * foreColor.red() + greenProportion
                        * foreColor.green() + blueProportion * foreColor.blue();
                    if (light < lightThershold) {
                        foreColor = QColor(Qt::white);
                    }
                    else {
                        foreColor = QColor(Qt::black);
                    }

                    //计算字符串在当前字体下的像素宽度
                    int pixelWidth = fontMetrics.boundingRect(
                        QString::fromStdString(stationList[lastId]->linename())).width();
                    
                    //绘制线路名称背景框
                    painter.drawRect(QRect(QPoint(60, textY + 25),
                        QPoint(pixelWidth + 100, textY + 55)));
                    pen.setColor(foreColor);
                    painter.setPen(pen);
                    painter.drawText(
                        QRect(QPoint(60, textY + 25), QPoint(pixelWidth + 100, textY + 55))
                        , Qt::AlignCenter
                        , QString::fromStdString(stationList[lastId]->linename()));

                    //绘制同一线路内的乘坐方案
                    pen.setColor(Qt::black);
                    painter.setPen(pen);
                    string text = "往" + terminal + "方向，乘坐" + to_string(stops - 1) + "站";
                    painter.drawText(QPoint(pixelWidth + 115, textY + 47), QString::fromStdString(text));

                    ////根据线路名字的长短，画不同的长度背景色方框
                    //if (stationList[lastId]->linename().size() <= 12) {
                    //    painter.drawRect(QRect(QPoint(60, textY + 25), QPoint(130, textY + 55)));
                    //    pen.setColor(foreColor);
                    //    painter.setPen(pen);
                    //    painter.drawText(QRect(QPoint(60, textY + 25), QPoint(130, textY + 55))
                    //        , Qt::AlignCenter
                    //        , QString::fromStdString(stationList[lastId]->linename()));

                    //    //绘制同一线路内的乘坐方案
                    //    pen.setColor(Qt::black);
                    //    painter.setPen(pen);
                    //    string text = "往" + terminal + "方向，乘坐" + to_string(stops - 1) + "站";
                    //    painter.drawText(QPoint(145, textY + 47), QString::fromStdString(text));
                    //}
                    //else if (stationList[lastId]->linename().size() <= 20) {
                    //    painter.drawRect(QRect(QPoint(60, textY + 25), QPoint(200, textY + 55)));
                    //    pen.setColor(foreColor);
                    //    painter.setPen(pen);
                    //    painter.drawText(QRect(QPoint(60, textY + 25), QPoint(200, textY + 55))
                    //        , Qt::AlignCenter
                    //        , QString::fromStdString(stationList[lastId]->linename()));
                    //    pen.setColor(Qt::black);
                    //    painter.setPen(pen);
                    //    string text = "往" + terminal + "方向，乘坐" + to_string(stops) + "站";
                    //    painter.drawText(QPoint(215, textY + 47), QString::fromStdString(text));
                    //}
                    //else {
                    //    painter.drawRect(QRect(QPoint(60, textY + 25), QPoint(350, textY + 55)));
                    //    pen.setColor(foreColor);
                    //    painter.setPen(pen);
                    //    painter.drawText(QRect(QPoint(60, textY + 25), QPoint(350, textY + 55))
                    //        , Qt::AlignCenter
                    //        , QString::fromStdString(stationList[lastId]->linename()));
                    //    pen.setColor(Qt::black);
                    //    painter.setPen(pen);
                    //    string text = "往" + terminal + "方向，乘坐" + to_string(stops) + "站";
                    //    painter.drawText(QPoint(365, textY + 47), QString::fromStdString(text));
                    //}
                    stops = 0;

                    //绘制上一线路换乘站圆点
                    pointY += 100;
                    brush.setColor(stationList[lastId]->linecolor());
                    brush.setStyle(Qt::SolidPattern);
                    painter.setPen(Qt::NoPen);
                    painter.setBrush(brush);
                    painter.drawEllipse(0, pointY, 30, 30);

                    //绘制上一线路换乘站名称
                    textY += 100;
                    painter.setPen(QColor(Qt::black));
                    painter.drawText(QPoint(60, textY)
                        , QString::fromStdString(stationList[nowId]->stationname()));

                    //绘制换乘虚线
                    pen.setColor(Qt::black);
                    pen.setStyle(Qt::DashLine);
                    pen.setWidth(1);
                    painter.setPen(pen);
                    painter.drawLine(QPoint(15, pointY + 35), QPoint(15, pointY + 55));

                    //绘制这一线路换乘圆点
                    pointY += 60;
                    brush.setColor(stationList[nowId]->linecolor());
                    brush.setStyle(Qt::SolidPattern);
                    painter.setBrush(brush);
                    painter.setPen(Qt::NoPen);
                    painter.drawEllipse(0, pointY, 30, 30);

                    //绘制这一线路换乘站名称
                    textY += 60;
                    painter.setPen(QColor(Qt::black));
                    painter.drawText(QPoint(60, textY)
                        , QString::fromStdString(stationList[nowId]->stationname()));
                }

                //绘制终点站
                else if (scheme.size() == 1) {
                    int nowId = scheme.top();
                    QPen pen;
                    QBrush brush;

                    //绘制终点站前连接线段
                    pen.setColor(stationList[nowId]->linecolor());
                    pen.setWidth(5);
                    painter.setPen(pen);
                    painter.drawLine(QPoint(15, pointY + 5), QPoint(15, pointY + 120 - 5));

                    //绘制终点站名称
                    pointY += 100;
                    brush.setColor(stationList[nowId]->linecolor());
                    brush.setStyle(Qt::SolidPattern);
                    painter.setBrush(brush);
                    painter.drawEllipse(0, pointY, 30, 30);

                    //绘制最后一条线路的乘坐指南
                    string terminal;
                    if (lastId < nowId) {
                        terminal = terminalDirection(nowId, nowId + 1);
                    }
                    else {
                        terminal = terminalDirection(nowId, nowId - 1);
                    }
                    brush.setColor(stationList[lastId]->linecolor());
                    brush.setStyle(Qt::SolidPattern);
                    QColor foreColor = stationList[nowId]->linecolor();
                    double light = redProportion * foreColor.red() + greenProportion
                        * foreColor.green() + blueProportion * foreColor.blue();
                    if (light < lightThershold) {
                        foreColor = QColor(Qt::white);
                    }
                    else {
                        foreColor = QColor(Qt::black);
                    }

                    //计算字符串在当前字体下的宽度
                    int pixelWidth = fontMetrics.boundingRect(
                        QString::fromStdString(stationList[lastId]->linename())).width();

                    //绘制线路名称背景框
                    painter.drawRect(QRect(QPoint(60, textY + 25),
                        QPoint(pixelWidth + 100, textY + 55)));
                    pen.setColor(foreColor);
                    painter.setPen(pen);
                    painter.drawText(
                        QRect(QPoint(60, textY + 25), QPoint(pixelWidth + 100, textY + 55))
                        , Qt::AlignCenter
                        , QString::fromStdString(stationList[lastId]->linename()));

                    //绘制同一线路内的乘坐方案
                    pen.setColor(Qt::black);
                    painter.setPen(pen);
                    string text = "往" + terminal + "方向，乘坐" + to_string(stops - 1) + "站";
                    painter.drawText(QPoint(pixelWidth + 115, textY + 47), QString::fromStdString(text));


                    /*if (stationList[lastId]->linename().size() <= 12) {
                        painter.drawRect(QRect(QPoint(60, textY + 25), QPoint(130, textY + 55)));
                        pen.setColor(foreColor);
                        painter.setPen(pen);
                        painter.drawText(QRect(QPoint(60, textY + 25), QPoint(130, textY + 55))
                            , Qt::AlignCenter
                            , QString::fromStdString(stationList[lastId]->linename()));
                        pen.setColor(Qt::black);
                        painter.setPen(pen);
                        string text = "往" + terminal + "方向，乘坐" + to_string(stops) + "站";
                        painter.drawText(QPoint(145, textY + 47), QString::fromStdString(text));
                    }
                    else if (stationList[lastId]->linename().size() <= 20) {
                        painter.drawRect(QRect(QPoint(60, textY + 25), QPoint(200, textY + 55)));
                        pen.setColor(foreColor);
                        painter.setPen(pen);
                        painter.drawText(QRect(QPoint(60, textY + 25), QPoint(200, textY + 55))
                            , Qt::AlignCenter
                            , QString::fromStdString(stationList[lastId]->linename()));
                        pen.setColor(Qt::black);
                        painter.setPen(pen);
                        string text = "往" + terminal + "方向，乘坐" + to_string(stops) + "站";
                        painter.drawText(QPoint(215, textY + 47), QString::fromStdString(text));
                    }
                    else {
                        painter.drawRect(QRect(QPoint(60, textY + 25), QPoint(270, textY + 55)));
                        pen.setColor(foreColor);
                        painter.setPen(pen);
                        painter.drawText(QRect(QPoint(60, textY + 25), QPoint(270, textY + 55))
                            , Qt::AlignCenter
                            , QString::fromStdString(stationList[lastId]->linename()));
                        pen.setColor(Qt::black);
                        painter.setPen(pen);
                        string text = "往" + terminal + "方向，乘坐" + to_string(stops) + "站";
                        painter.drawText(QPoint(285, textY + 47), QString::fromStdString(text));
                    }*/
                    stops = 0;

                    textY += 100;
                    painter.setPen(QColor(Qt::black));
                    painter.drawText(QPoint(60, textY)
                        , QString::fromStdString(stationList[nowId]->stationname()));
                }
                stops++;
                llastId = lastId;
                lastId = scheme.top();
                scheme.pop();
            }
        }

        //改变切换按钮文字
        switchButton->setText(QString::fromStdString("切换地图显示模式"));

        state = paintType::route;
        action = paintType::None;

    }

    //map绘图模式
    else if (action == paintType::map) {
        qDebug() << "map";

        if (isPaintMap) {
            delete[] schemePath;
            pointPath.clear();
        }
        isPaintMap = true;
        QPainter painter(this);
        painter.eraseRect(paintArea);
        painter.translate(940, 70);
        schemeSize = reservedScheme.size();
        double leftLon = MAXINT, rightLon = 0, topLat = 0, bottomLat = MAXINT;
        schemePath = new int[schemeSize];
        for (int i = 0; i < schemeSize; i++) {
            schemePath[i] = reservedScheme.top();
            reservedScheme.pop();
        }

        //进行地理坐标系画图范围框定
        //暂未考虑纬度偏大时对墨卡托投影的影响
        for (int i = 0; i < schemeSize; i++) {
            if (stationList[schemePath[i]]->longitude() < leftLon) {
                leftLon = stationList[schemePath[i]]->longitude();
            }
            if (stationList[schemePath[i]]->longitude() > rightLon) {
                rightLon = stationList[schemePath[i]]->longitude();
            }
            if (stationList[schemePath[i]]->latitude() > topLat) {
                topLat = stationList[schemePath[i]]->latitude();
            }
            if (stationList[schemePath[i]]->latitude() < bottomLat) {
                bottomLat = stationList[schemePath[i]]->latitude();
            }
        }
        double diffLon = rightLon - leftLon, diffLat = topLat - bottomLat
            , middleLon = (rightLon + leftLon) / 2
            , middleLat = (topLat + bottomLat) / 2;
        if (diffLon * canvasH < diffLat * canvasW) {
            leftLon = middleLon - diffLat * canvasW / canvasH / 2;
            rightLon = middleLon + diffLat * canvasW / canvasH / 2;
        }
        else {
            topLat = middleLat + diffLon * canvasH / canvasW / 2;
            bottomLat = middleLat - diffLon * canvasH / canvasW / 2;
        }
        if (diffLat < limitedLat) {
            rightLon += limitedLat * canvasW / canvasW / 2;
            leftLon -= limitedLat * canvasW / canvasW / 2;
            topLat += limitedLat / 2;
            bottomLat -= limitedLat / 2;
        }
        diffLon = rightLon - leftLon;
        diffLat = topLat - bottomLat;

        //绘制灰色边缘线路
        QPoint lastPoint, point;
        QPen pen;
        pen.setWidth(3);
        pen.setColor(QColor(Qt::gray));
        for (int i = 0; i < stationCount; i++) {
            if (i != 0) {
                lastPoint = point;
            }
            point.setX(canvasW * (stationList[i]->longitude()
                - leftLon) / (rightLon - leftLon));
            point.setY(canvasH * ((topLat
                - stationList[i]->latitude()) / (topLat - bottomLat)));
            if (i && stationList[i]->lineid() == stationList[i - 1]->lineid()) {
                painter.setPen(pen);
                painter.drawLine(lastPoint, point);
            }
            if (stationList[i]->lineloop() && (i == stationCount - 1
                || stationList[i]->lineid() != stationList[i + 1]->lineid())) {
                for (int j = i; j >= 0; j--) {
                    if (j == 0
                        || stationList[j]->lineid() != stationList[j - 1]->lineid()) {
                        QPoint beginPoint;
                        beginPoint.setX(canvasW * (stationList[j]->longitude()
                            - leftLon) / (rightLon - leftLon));
                        beginPoint.setY(canvasH * ((topLat
                            - stationList[j]->latitude()) / (topLat - bottomLat)));
                        painter.drawLine(beginPoint, point);
                        break;
                    }
                }
            }
            painter.drawEllipse(point, 2, 2);
        }

        //彩色绘画规划路线
        QBrush brush;
        for (int i = 0; i < schemeSize; i++) {
            if (i != 0) {
                lastPoint = point;
            }
            if (schemeSize == 1) {
                point.setX(canvasW / 2);
                point.setY(canvasH / 2);
            }
            else if (leftLon == rightLon) {
                point.setX(canvasW / 2);
                point.setY(canvasH * ((topLat
                    - stationList[schemePath[i]]->latitude()) / (topLat - bottomLat)));
            }
            else if (topLat == bottomLat) {
                point.setX(canvasW * (stationList[schemePath[i]]->longitude()
                    - leftLon) / (rightLon - leftLon));
                point.setY(canvasH / 2);
            }
            else {
                point.setX(canvasW * (stationList[schemePath[i]]->longitude()
                    - leftLon) / (rightLon - leftLon));
                point.setY(canvasH * ((topLat
                    - stationList[schemePath[i]]->latitude()) / (topLat - bottomLat)));
            }
            if (i != 0 && isOut == false) {
                pen.setColor(stationList[schemePath[i - 1]]->linecolor());
                pen.setWidth(6);
                painter.setPen(pen);
                painter.drawLine(lastPoint, point);
            }
            else if (i != 0 && isOut == true) {
                pen.setColor(QColor(Qt::gray));
                pen.setStyle(Qt::DashLine);
                pen.setWidth(4);
                painter.setPen(pen);
                painter.drawLine(lastPoint, point);
            }
            painter.setPen(Qt::NoPen);
            brush.setColor(stationList[schemePath[i]]->linecolor());
            brush.setStyle(Qt::SolidPattern);
            painter.setBrush(brush);
            painter.drawEllipse(point, 7, 7);

            pointPath.push_back(point);
        }

        brush.setColor(Qt::white);
        painter.setBrush(brush);
        for (const auto& poi : pointPath) {
            painter.drawEllipse(poi, 4, 4);
        }

        //彻底擦除画布外画面
        int maxErase = 10000;
        canvasH += 20;
        canvasW += 20;
        painter.eraseRect(-20, canvasH, -maxErase, -maxErase);
        painter.eraseRect(canvasW, canvasH, -maxErase, maxErase);
        painter.eraseRect(canvasW, -20, maxErase, -maxErase);
        painter.eraseRect(-20, -20, maxErase, -maxErase);
        canvasH -= 20;
        canvasW -= 20;

        //改变切换按钮文字
        switchButton->setText(QString::fromStdString("切换路径显示模式"));

        //标明当前状态
        state = paintType::map;
        action = paintType::None;
    }

    //lineMap绘图模式
    else if (action == paintType::lineMap) {
        int lineIndex = lineBox->currentIndex();
        //利用firstId记录第一个站点
        int firstId = -1;
        for (int i = 0; i < stationCount; i++) {
            if (stationList[i]->lineid() == lineIndex) {
                if (firstId == -1) {
                    firstId = i;
                }
                reservedScheme.push(i);
            }
        }

        //若为环线，则首尾相连
        if (stationList[firstId]->lineloop()) {
            reservedScheme.push(firstId);
        }

        //委托map绘图模式进行绘图
        action = paintType::map;
        this->update();
    }
}

//——————————重写event函数——————————
bool MetroSearch::event(QEvent* event) {

    //判断是否为鼠标事件
    if (state == paintType::map && event->type() == QEvent::MouseMove) {
        QMouseEvent* mouse = dynamic_cast<QMouseEvent*>(event);
        int mouseX = mouse->x() - 940, mouseY = mouse->y() - 70;

        //判断是否在画图区域范围内，避免多次进入无效循环
        if (mouseX > 0 && mouseX < canvasW && mouseY>0 && mouseY < canvasH) {
            bool inPoint = false;
            for (int i = 0; i < schemeSize; i++) {
                if (mouseX<pointPath[i].x() + 3 && mouseX>pointPath[i].x() - 3
                    && mouseY<pointPath[i].y() + 3 && mouseY>pointPath[i].y() - 3) {
                    inPoint = true;

                    //为站点提供鼠标放置悬停窗口
                    vector<int> inLine;
                    inLine.push_back(stationList[schemePath[i]]->lineid());
                    string tipText(stationList[schemePath[i]]->stationname());

                    //寻找可换乘线路
                    Adjacent* p = stationList[schemePath[i]]->adjacent();
                    while (p) {
                        bool willAdd = true;
                        for (int j = 0; j < inLine.size(); j++) {
                            if (inLine[j] == stationList[p->adjacentId]->lineid()) {
                                willAdd = false;
                                break;
                            }
                        }
                        if (willAdd) {
                            inLine.push_back(stationList[p->adjacentId]->lineid());
                        }
                        p = p->next;
                    }
                    for (int j = 0; j < inLine.size(); j++) {
                        tipText += "<br/>";
                        tipText += lineList[inLine[j]]->linename();
                    }

                    //作为ToolTip鼠标悬浮输出
                    QToolTip::showText(QCursor::pos()
                        , QString::fromStdString(tipText));
                    inLine.clear();
                }
            }
            if (!inPoint) {
                QToolTip::hideText();
            }
        }
    }
    return QWidget::event(event);
}

//——————————基于DIJKSTRA算法的路径搜索——————————
/*仅依据起点站计算所有站点到起点站的最佳路线
* 未进行堆优化及斐波那契堆优化，时间复杂度现为n^2，可以进一步提高
* 包含了换乘代价和途径代价
* 用updater记录何站引起该站距离缩短*/
void MetroSearch::dijkstraSearch(int startIndex) {

    //进行数组初始化
    pi[startIndex] = -1;
    dist[startIndex] = 0;
    costPi[startIndex] = -1;
    costDist[startIndex] = 0;
    for (int i = 0; i < stationCount; i++) {
        if (i != startIndex) {
            pi[i] = MAXINT;
            costPi[i] = MAXINT;
            updater[i] = -2;
        }
    }
    updater[startIndex] = -1;
    int shortestId = startIndex;

    //进行全部站点的遍历
    for (int i = 0; i < 2 * stationCount; i++) {
        if (i == 0) {
            Adjacent* p = stationList[startIndex]->adjacent();
            while (p) {
                pi[p->adjacentId] = p->spacing;
                costPi[p->adjacentId] = p->spacing + sitesCost;
                updater[p->adjacentId] = startIndex;
                p = p->next;
            }
            continue;
        }
        shortestId = arrayMin(costPi, stationCount);
        Adjacent* p = stationList[shortestId]->adjacent();
        dist[shortestId] = pi[shortestId];
        costDist[shortestId] = costPi[shortestId];
        pi[shortestId] = -1;
        costPi[shortestId] = -1;

        //遍历该站点的所有邻接站点
        while (p) {
            if (p->spacing == 0) {
                if (costDist[shortestId] + sitesCost + transferCost
                    < costPi[p->adjacentId]) {
                    pi[p->adjacentId] = p->spacing + dist[shortestId];
                    costPi[p->adjacentId] = p->spacing + costDist[shortestId]
                        + sitesCost + transferCost;
                    updater[p->adjacentId] = shortestId;
                }
            }
            else if (costDist[shortestId] + sitesCost < costPi[p->adjacentId]) {
                pi[p->adjacentId] = p->spacing + dist[shortestId];
                costPi[p->adjacentId] = p->spacing + costDist[shortestId] + sitesCost;
                updater[p->adjacentId] = shortestId;
            }
            p = p->next;
        }
    }
}

//——————————确定终点站方向——————————
string MetroSearch::terminalDirection(int startIndex, int nextIndex) {

    //环线则令下一站为终点站方向
    if (stationList[startIndex]->lineloop()) {
        return stationList[nextIndex]->stationname();
    }

    //根据startIndex和nextIndex的大小判断方向
    if (startIndex < nextIndex) {
        for (int i = startIndex; i < stationCount; i++) {
            if (i == stationCount - 1
                || stationList[i]->lineid() != stationList[i + 1]->lineid()) {
                return stationList[i]->stationname();
            }
        }
    }
    else {
        for (int i = startIndex; i >= 0; i--) {
            if (i == 0 || stationList[i]->lineid() != stationList[i - 1]->lineid()) {
                return stationList[i]->stationname();
            }
        }
    }
}