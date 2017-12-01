#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stdafx.h"
#include <QDebug>

#define MAX_RANGE 1
#define MIN_RANGE 0

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QButtonGroup* allButton=new QButtonGroup(this);
    allButton->addButton(ui->checkBox);
    allButton->addButton(ui->checkBox_2);
    allButton->addButton(ui->checkBox_3);
    allButton->addButton(ui->checkBox_4);
    allButton->addButton(ui->checkBox_5);
    ui->textEdit_3->hide();
    ui->textEdit_4->hide();

    QObject::connect(ui->radioButton,&QRadioButton::clicked,this,&MainWindow::turnText);
    QObject::connect(ui->begin,&QPushButton::clicked,this,&MainWindow::slotPrintText);
    QObject::connect(ui->begin,&QPushButton::clicked,this,&MainWindow::slotPlot);
    QObject::connect(ui->begin,&QPushButton::clicked,this,&MainWindow::slotPrintTextExact);
    QObject::connect(ui->max_queue,static_cast<void (QSpinBox:: *)(int)>(&QSpinBox::valueChanged),ui->max_queue_2,&QSpinBox::setValue);
    QObject::connect(ui->max_queue_2,static_cast<void (QSpinBox:: *)(int)>(&QSpinBox::valueChanged),ui->max_queue,&QSpinBox::setValue);
    QObject::connect(ui->l_doubleSpinBox,static_cast<void (QDoubleSpinBox:: *)(double)>(&QDoubleSpinBox::valueChanged),ui->l_doubleSpinBox_2,&QDoubleSpinBox::setValue);
    QObject::connect(ui->l_doubleSpinBox_2,static_cast<void (QDoubleSpinBox:: *)(double)>(&QDoubleSpinBox::valueChanged),ui->l_doubleSpinBox,&QDoubleSpinBox::setValue);
    QObject::connect(ui->d_doubleSpinBox,static_cast<void (QDoubleSpinBox:: *)(double)>(&QDoubleSpinBox::valueChanged),ui->d_doubleSpinBox_2,&QDoubleSpinBox::setValue);
    QObject::connect(ui->d_doubleSpinBox_2,static_cast<void (QDoubleSpinBox:: *)(double)>(&QDoubleSpinBox::valueChanged),ui->d_doubleSpinBox,&QDoubleSpinBox::setValue);
    QObject::connect(ui->unit_time,static_cast<void (QSpinBox:: *)(int)>(&QSpinBox::valueChanged),ui->unit_time_2,&QSpinBox::setValue);
    QObject::connect(ui->unit_time_2,static_cast<void (QSpinBox:: *)(int)>(&QSpinBox::valueChanged),ui->unit_time,&QSpinBox::setValue);
    QObject::connect(ui->checkBox_4,&QCheckBox::stateChanged,this,&MainWindow::updateFuelNum);

    ui->d_doubleSpinBox->setRange(MIN_RANGE,MAX_RANGE);
    ui->d_doubleSpinBox->setSingleStep(0.01);

    ui->l_doubleSpinBox->setRange(MIN_RANGE,MAX_RANGE);
    ui->l_doubleSpinBox->setSingleStep(0.01);

    ui->plot->yAxis->setRange(0,1);
    ui->plot->yAxis->setLabel("rate");
    ui->plot->xAxis->setLabel("queue limit");

    ui->plot1->yAxis->setRange(0,1000);
    ui->plot1->yAxis->setLabel("number");
    ui->plot1->xAxis->setLabel("queue limit");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::turnText(){
    if(ui->radioButton->isChecked()){
        ui->textEdit_3->show();
        ui->textEdit_4->show();
        ui->textEdit->hide();
        ui->textEdit_2->hide();
    }
    else{
        ui->textEdit_2->show();
        ui->textEdit->show();
        ui->textEdit_3->hide();
        ui->textEdit_4->hide();
    }
}

double MainWindow::getDeparture(){
    return ui->d_doubleSpinBox->value();
}

double MainWindow::getLanding(){
    return ui->l_doubleSpinBox->value();
}

int MainWindow::getQueue(){
    return ui->max_queue->value();
}

int MainWindow::getTime(){
    return ui->unit_time->value();
}

void MainWindow::slotPrintText(){
    if(isNotExactWay()){
        ui->textEdit->clear();
        ui->textEdit_3->clear();
        Simulation s(*this);
        if(getLanding()+getDeparture()>1.0){
            QString warn("Safety Warning: This airport will become saturated.");
            warning(warn,3000);
        }
        if(isNewOneWay()){
            if(getFuelNum()==0){
                QString warn("Error: Fuel num is incorrect");
                warning(warn,3000);
                return;
            }
            s.runWithFuel();
        }
        else{
            s.run();
        }
    }
}

void MainWindow::slotPrintTextExact(){
    if(isExactWay()){
        ui->textEdit_2->clear();
        ui->textEdit_4->clear();
        Simulation s(*this);
        QVector<int> toland;
        QVector<int> todepart;
        if(isNewOneWay()){
            QString warn("Error: Not support stub runing with fuel");
            warning(warn,3000);
        }
        else{
            getLandVector(toland);
            getDepartVector(todepart);
            s.run(toland,todepart);
        }
    }
}

int MainWindow::getMinQueue(){
    return ui->min_range_queue->value();
}

int MainWindow::getMaxQueue(){
    return ui->max_range_queue->value();
}


void MainWindow::slotPlot(){
    if(ui->tabWidget->currentIndex()==1){
        Simulation s(*this);
        int maxRange=getMaxQueue();
        int minRange=getMinQueue();

        if(getLanding()+getDeparture()>1.0){
            ui->statusBar->showMessage("Safety Warning: This airport will become saturated.",3000);
        }

        QVector<double> x(maxRange-minRange+1);
        QVector<double> y0(maxRange-minRange+1);
        QVector<double> y1(maxRange-minRange+1);
        QVector<double> y2(maxRange-minRange+1);
        QVector<double> y3(maxRange-minRange+1);
        QVector<double> y4(maxRange-minRange+1);
        QVector<double> temp(5);

        for(int i=0,queue_num=minRange;i<maxRange-minRange+1;++i,++queue_num){
            x[i]=queue_num;
            s.runOnQueue(queue_num,temp);
            y0[i]=temp[0];
            y1[i]=temp[1];
            y2[i]=temp[2];
            y3[i]=temp[3];
            y4[i]=temp[4];
        }

        ui->plot->clearGraphs();
        ui->plot1->clearGraphs();

        ui->plot->legend->setVisible(true);
        QFont legendFont = font();
        legendFont.setPointSize(9);
        ui->plot->legend->setFont(legendFont);
        ui->plot->legend->setBrush(QBrush(QColor(255,255,255,230)));
        ui->plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);

        ui->plot1->legend->setVisible(true);
        ui->plot1->legend->setFont(legendFont);
        ui->plot1->legend->setBrush(QBrush(QColor(255,255,255,230)));
        ui->plot1->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);

        ui->plot->addGraph();
        ui->plot->addGraph();
        ui->plot->addGraph();

        ui->plot1->addGraph();
        ui->plot1->addGraph();

        ui->plot->graph(0)->setData(x,y0);
        ui->plot->graph(0)->setPen(QPen(Qt::gray));
        ui->plot->graph(0)->setLineStyle(QCPGraph::lsLine);
        ui->plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
        ui->plot->graph(0)->setName("average landing rate");

        ui->plot->graph(1)->setData(x,y1);
        ui->plot->graph(1)->setPen(QPen(Qt::red));
        ui->plot->graph(1)->setLineStyle(QCPGraph::lsLine);
        ui->plot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 3));
        ui->plot->graph(1)->setName("average takeoff rate");

        ui->plot->graph(2)->setData(x,y2);
        ui->plot->graph(2)->setPen(QPen(Qt::black));
        ui->plot->graph(2)->setLineStyle(QCPGraph::lsLine);
        ui->plot->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, 3));
        ui->plot->graph(2)->setName("Percentage of time runway idle");

        ui->plot1->graph(0)->setData(x,y3);
        ui->plot1->graph(0)->setPen(QPen(Qt::black));
        ui->plot1->graph(0)->setLineStyle(QCPGraph::lsLine);
        ui->plot1->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, 3));
        ui->plot1->graph(0)->setName("wait landing");

        ui->plot1->graph(1)->setData(x,y4);
        ui->plot1->graph(1)->setPen(QPen(Qt::gray));
        ui->plot1->graph(1)->setLineStyle(QCPGraph::lsLine);
        ui->plot1->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 3));
        ui->plot1->graph(1)->setName("wait take off");

        ui->plot->xAxis->setRange(minRange,maxRange);
        ui->plot1->xAxis->setRange(minRange,maxRange);
        ui->plot1->yAxis->setRange(0,10);

        ui->plot->replot();
        ui->plot1->replot();
        ui->plot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
        ui->plot1->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
    }
}

bool MainWindow::isTwoRunWay(){
    return ui->checkBox->isChecked();
}

bool MainWindow::isTwoPriorWay(){
    return ui->checkBox_2->isChecked();
}

bool MainWindow::isThreeWay(){
    return ui->checkBox_3->isChecked();
}

bool MainWindow::isNewOneWay(){
    return ui->checkBox_4->isChecked();
}

void MainWindow::warning(QString &warn,int time){
    ui->statusBar->showMessage(warn,time);
}

void MainWindow::updateFuelNum(int state){
    if(state==Qt::Checked){
        layout=new QHBoxLayout;
        label=new QLabel;
        spinBox=new QSpinBox;
        QFont f("微软雅黑",10);

        spinBox->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        label->setText(QString("   fuel num    "));
        label->setFont(f);
        layout->setContentsMargins(5,0,5,0);
        layout->addWidget(label);
        layout->addWidget(spinBox);
        ui->verticalLayout->addLayout(layout);
    }
    else{
        delete(spinBox);
        delete(label);
        delete(layout);
    }
}

int MainWindow::getFuelNum(){
    return spinBox->value();
}


void MainWindow::getLandVector(QVector<int> &toland){
    QString qStr=ui->landText->toPlainText();
    QStringList list=qStr.split("\n");
    for(int i=0;i<list.size();++i){
        toland.append(list[i].toInt());
    }
}

void MainWindow::getDepartVector(QVector<int> &todepart){
    QString qStr=ui->takeoffVec->toPlainText();
    QStringList list=qStr.split("\n");
    for(int i=0;i<list.size();++i){
        todepart.append(list[i].toInt());
    }
}

void MainWindow::appendToDetailExactText(QString &text){
    ui->textEdit_4->append(text);
}

void MainWindow::appendToDetailText(QString &text){
    ui->textEdit_3->append(text);
}

void MainWindow::appendToText(QString &text){
    ui->textEdit->append(text);
}

void MainWindow::appendToExactText(QString &text){
    ui->textEdit_2->append(text);
}

bool MainWindow::isExactWay(){
    return ui->tabWidget->currentIndex()==2;
}

bool MainWindow::isNotExactWay(){
    return ui->tabWidget->currentIndex()==0;
}
