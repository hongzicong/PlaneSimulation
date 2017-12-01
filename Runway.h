#pragma once

#include "Plane.h"
#include "Extended_queue.h"
#include "mainwindow.h"

#ifndef RUNWAY_H
#define RUNWAY_H

enum Runway_activity { idle, land, takeoff,both,noidle};

class Runway {
public:
    Runway(MainWindow &window,int limit);
    Error_code can_land(const Plane &current);//if number is less than maximum size ,append to landing
    Error_code can_depart(const Plane &current);
	Runway_activity activity(int time, Plane &moving);
    Runway_activity activity_two_way(int time,Plane &landPlane,Plane &takeoffPlane);
    Runway_activity activity_two_priority(int time,Plane &plane_1,Plane &plane_2);
    Runway_activity activity_three(int time,Plane &moving);
    QString shut_down( int time) const;
    double get_landing_rate(int time) const;
    double get_takeoff_rate(int time) const;
    double get_idle_rate(int time) const;
    double get_wait_landing()const;
    double get_wait_takeoff()const;
    int getLandingSize() const;
    bool isSafe() const;
    void reduceFuel();
    void flyPlane(int time,Plane &moving);
    void landPlane(int time,Plane &moving);

private:
    MainWindow *w;
    Extended_queue landing;
	Extended_queue takeoff;
    Extended_queue thirdway;
	int queue_limit;
	int num_land_requests; // number of planes asking to land
	int num_takeoff_requests; // number of planes asking to take off
	int num_landings; // number of planes that have landed
	int num_takeoffs; // number of planes that have taken off
	int num_land_accepted; // number of planes queued to land
	int num_takeoff_accepted; // number of planes queued to take off
	int num_land_refused; // number of landing planes refused
	int num_takeoff_refused; // number of departing planes refused
	int land_wait; // total time of planes waiting to land
	int takeoff_wait; // total time of planes waiting to take off
	int idle_time; // total time runway is idle
};

#endif
