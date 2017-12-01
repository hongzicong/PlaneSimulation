#include "stdafx.h"

using namespace std;

MainWindow* Plane::w;

Plane::Plane(int flt, int time, Plane_status status)
{
    flt_num = flt;
    clock_start = time;
    state = status;

    QString qStr=QString("Plane number %1 ready to ").arg(flt);
    if (status == arriving){
        qStr.append(QString("land.\n"));
    }
    else{
        qStr.append(QString("take off.\n"));
    }

    if(w->isExactWay()){
        w->appendToDetailExactText(qStr);
    }
    else if(w->isNotExactWay()){
        w->appendToDetailText(qStr);
    }
}

Plane::Plane(int flt, int time,int fuel ,Plane_status status){
    flt_num = flt;
    clock_start = time;
    state = status;
    fuel_num=fuel;

    QString qStr=QString("Plane number %1 ready to ").arg(flt);
    if (status == arriving){
        qStr.append(QString("land.   It has %1 fuel remained\n").arg(fuel));
    }
    else{
        qStr.append(QString("take off.\n"));
    }

    if(w->isExactWay()){
        w->appendToDetailExactText(qStr);
    }
    else if(w->isNotExactWay()){
        w->appendToDetailText(qStr);
    }

}

Plane::Plane()
{
	flt_num =-1;
	clock_start =-1;
	state = null;
}


void Plane::refuse() const
{
    QString qStr=QString("Plane number %1").arg(flt_num);
	if (state == arriving)
        qStr.append(" directed to another airport\n");
	else
        qStr.append(" told to try to takeoff again later\n");

    if(w->isExactWay()){
        w->appendToDetailExactText(qStr);
    }
    else if(w->isNotExactWay()){
        w->appendToDetailText(qStr);
    }

}


void Plane::land(int time){
    int wait = time-getClockStart();
    QString qStr=QString("%1: Plane number %2 landed after %3 time unit%4 in the takeoff queue.\n").arg(time).arg(getFltNum()).arg(wait).arg((wait == 1) ? "" : "s");
    if(w->isExactWay()){
        w->appendToDetailExactText(qStr);
    }
    else if(w->isNotExactWay()){
        w->appendToDetailText(qStr);
    }
}


void Plane::fly(int time){
    int wait = time-getClockStart();
    QString qStr=QString("%1: Plane number %2 took off after %3 time unit%4 in the takeoff queue.\n").arg(time).arg(getFltNum()).arg(wait).arg((wait == 1) ? "" : "s");
    if(w->isExactWay()){
        w->appendToDetailExactText(qStr);
    }
    else if(w->isNotExactWay()){
        w->appendToDetailText(qStr);
    }
}


void Plane::crash(int time){
    int wait = time-getClockStart();
    QString qStr=QString("%1: Plane number %2 crashed after %3 time unit%4 in the takeoff queue.\n").arg(time).arg(getFltNum()).arg(wait).arg((wait == 1) ? "" : "s");
    if(w->isExactWay()){
        w->appendToDetailExactText(qStr);
    }
    else if(w->isNotExactWay()){
        w->appendToDetailText(qStr);
    }
}


int Plane::started() const
{
	return clock_start;
}

Plane_status Plane::getState() const{
    return state;
}


int Plane::getFuelNum() const{
    return fuel_num;
}

void Plane::setFuelNum(int num){
    fuel_num=num;
}

int Plane::getClockStart(){
    return clock_start;
}

int Plane::getFltNum(){
    return flt_num;
}

void Plane::setWindow(MainWindow &window){
    w=&window;
}
