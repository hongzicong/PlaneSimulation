#pragma once

#ifndef PLANE_H
#define PLANE_H

#include "mainwindow.h"

enum Plane_status { null, arriving, departing };

class Plane {
public:
    Plane();// error constructor
    Plane(int flt, int time,int fuel ,Plane_status status);
    Plane(int flt, int time, Plane_status status);	 // correct constructor
	void refuse() const;							 // the plane continue to wait
    void land(int time);						 // the plane land successfully
    void fly(int time);						 // the plane fly successfully
    void crash(int time);
	int started() const;							 // return the time thar the plane enter
    void setFuelNum(int num);

    int getClockStart();
    int getFltNum();
    Plane_status getState() const;
    int getFuelNum() const;

    static MainWindow* w;
    static void setWindow(MainWindow& window);

private:
    int fuel_num;
	int flt_num;								   	 // the flt_num th plane
	int clock_start;								 // the time that the plane enter
    Plane_status state;
};

#endif
