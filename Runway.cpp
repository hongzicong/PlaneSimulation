
#include "stdafx.h"
#include "mainwindow.h"

using namespace std;

Runway::Runway(MainWindow &window,int limit) {
    w=&window;
	queue_limit = limit;
	num_land_requests = num_takeoff_requests = 0;
	num_landings = num_takeoffs = 0;
	num_land_refused = num_takeoff_refused = 0;
	num_land_accepted = num_takeoff_accepted = 0;
	land_wait = takeoff_wait = idle_time = 0;
}


Error_code Runway::can_land(const Plane &current) {
    Error_code result;
    if(w->isTwoPriorWay()){
        if(landing.size()<queue_limit){
            result=landing.append(current);
        }
        else if(takeoff.size()<queue_limit){
            result=takeoff.append(current);
        }
        else{
            result=fail;
        }

    }
    else if(w->isThreeWay()){
        if(thirdway.size()==0){
            result=thirdway.append(current);
        }
        if(landing.size()<queue_limit){
            result=landing.append(current);
        }
        else if(thirdway.size()<queue_limit){
            result=thirdway.append(current);
        }
        else{
            result=fail;
        }
    }
    else if(w->isNewOneWay()){
        if(landing.size()<queue_limit){
            if(current.getFuelNum()<landing.size()+1){
                result=landing.appendToTop(current);
            }
            else{
                result=landing.append(current);
            }
        }
        else{
            result=fail;
        }
    }
    else{
        if (landing.size() < queue_limit)
            result = landing.append(current);
        else{
            result = fail;
        }
    }
    num_land_requests++;
    if(result!=success){
        num_land_refused++;
    }
    else{
        num_land_accepted++;
    }
    return result;
}


Error_code Runway::can_depart(const Plane &current) {
	Error_code result;
    if(w->isTwoPriorWay()){
        if(takeoff.size()<queue_limit){
            result=takeoff.append(current);
        }
        else if(landing.size()==0){
            result=landing.append(current);
        }
        else{
            result=fail;
        }
    }
    else if(w->isThreeWay()){
        if(thirdway.size()==0){
            result=thirdway.append(current);
        }
        else if(takeoff.size()<queue_limit){
            result=takeoff.append(current);
        }
        else if(landing.size()==0&&thirdway.size()<queue_limit){
            result=thirdway.append(current);
        }
        else{
            result=fail;
        }
    }
    else{
        if (takeoff.size() < queue_limit)
            result = takeoff.append(current);	//result is success
        else
            result = fail;						//result is fail
    }

    num_takeoff_requests++;
    if (result != success)
        num_takeoff_refused++;
    else
        num_takeoff_accepted++;

	return result;
}


Runway_activity Runway::activity(int time, Plane &moving) {
    Runway_activity in_progress=noidle;
	//landing has high priority
	if (!landing.empty()) {
		landing.retrieve(moving);
		land_wait += time-moving.started();
		num_landings++;
		in_progress = land;
		landing.serve();
	}
	else if (!takeoff.empty()) {
		takeoff.retrieve(moving);
		takeoff_wait += time-moving.started();
		num_takeoffs++;
		in_progress =(Runway_activity)2;
		takeoff.serve();
	}
	else {
		idle_time++;
		in_progress = idle;
	}
	return in_progress;
}

Runway_activity Runway::activity_two_way(int time,Plane &land_plane,Plane &takeoff_plane){
    Runway_activity in_progress=noidle;
    if(landing.empty()&&takeoff.empty()){
        idle_time++;
        return idle;
    }
    if(!landing.empty()){
        landing.retrieve(land_plane);
        land_wait+=time-land_plane.started();
        num_landings++;
        landPlane(time,land_plane);
        landing.serve();
    }
    if (!takeoff.empty()) {
            takeoff.retrieve(takeoff_plane);
            takeoff_wait += time-takeoff_plane.started();
            num_takeoffs++;
            flyPlane(time,takeoff_plane);
            takeoff.serve();
    }
    return in_progress;
}

Runway_activity Runway::activity_two_priority(int time,Plane &plane_1,Plane &plane_2){
    Runway_activity in_progress=noidle;
    if(landing.empty()&&takeoff.empty()){
        idle_time++;
        return idle;
    }
    if(!landing.empty()){
        landing.retrieve(plane_1);
        if(plane_1.getState()==arriving){
            land_wait+=time-plane_1.started();
            num_landings++;
            landPlane(time,plane_1);
        }
        else{
            takeoff_wait += time-plane_1.started();
            num_takeoffs++;
            flyPlane(time,plane_1);
        }
        landing.serve();
    }
    if (!takeoff.empty()) {
            takeoff.retrieve(plane_2);
            if(plane_2.getState()==land){
                land_wait+=time-plane_2.started();
                num_landings++;
                landPlane(time,plane_2);
            }
            else{
                takeoff_wait += time-plane_2.started();
                num_takeoffs++;
                flyPlane(time,plane_2);
            }
            takeoff.serve();
    }
    return in_progress;
}

void Runway::flyPlane(int time,Plane &moving){
    int wait = time-moving.getClockStart();
    QString qStr=QString("%1: Plane number %2 took off after %3 time unit%4 in the takeoff queue.\n").arg(time).arg(moving.getFltNum()).arg(wait).arg((wait == 1) ? "" : "s");
    if(w->isExactWay()){
        w->appendToDetailExactText(qStr);
    }
    else if(w->isNotExactWay()){
        w->appendToDetailText(qStr);
    }
}

void Runway::landPlane(int time,Plane &moving){
    int wait = time-moving.getClockStart();
    QString qStr=QString("%1: Plane number %2 landed after %3 time unit%4 in the takeoff queue.\n").arg(time).arg(moving.getFltNum()).arg(wait).arg((wait == 1) ? "" : "s");
    if(w->isExactWay()){
        w->appendToDetailExactText(qStr);
    }
    else if(w->isNotExactWay()){
        w->appendToDetailText(qStr);
    }
}

Runway_activity Runway::activity_three(int time,Plane &moving){
    Runway_activity in_progress=noidle;
    if(landing.empty()&&takeoff.empty()&&thirdway.empty()){
        idle_time++;
        return idle;
    }
    if(!landing.empty()){
        landing.retrieve(moving);
        if(moving.getState()==arriving){
            land_wait+=time-moving.started();
            num_landings++;
            landPlane(time,moving);
        }
        else{
            takeoff_wait += time-moving.started();
            num_takeoffs++;
            flyPlane(time,moving);
        }
        landing.serve();
    }
    if (!takeoff.empty()) {
        takeoff.retrieve(moving);
        if(moving.getState()==arriving){
            land_wait+=time-moving.started();
            num_landings++;
            landPlane(time,moving);
        }
        else{
            takeoff_wait += time-moving.started();
            num_takeoffs++;
            flyPlane(time,moving);
        }
        takeoff.serve();
    }
    if(!thirdway.empty()){
        thirdway.retrieve(moving);
        if(moving.getState()==arriving){
            land_wait+=time-moving.started();
            num_landings++;
            landPlane(time,moving);
        }
        else{
            takeoff_wait += time-moving.started();
            num_takeoffs++;
            flyPlane(time,moving);
        }
        thirdway.serve();
    }
    return in_progress;
}

QString Runway::shut_down(int time) const {
    QString m1=QString("Simulation has concluded after %1 time units.\n\n").arg(time);
    m1.append(QString("Total number of planes processed %1\n\n").arg(num_land_requests+num_takeoff_requests));
    m1.append(QString("Total number of planes asking to land %1\n\n").arg(num_land_requests));
    m1.append(QString("Total number of planes asking to take off %1\n\n").arg(num_takeoff_requests));
    m1.append(QString("Total number of planes accepted for landing %1\n\n").arg(num_land_accepted));
    m1.append(QString("Total number of planes accepted for takeoff %1\n\n").arg(num_takeoff_accepted));
    m1.append(QString("Total number of planes refused for landing %1\n\n").arg(num_land_refused));
    m1.append(QString("Total number of planes refused for takeoff %1\n\n").arg(num_takeoff_refused));
    m1.append(QString("Total number of planes that landed %1\n\n").arg(num_landings));
    m1.append(QString("Total number of planes that took off %1\n\n").arg(num_takeoffs));
    m1.append(QString("Total number of planes left in landing queue %1\n\n").arg(landing.size()));
    m1.append(QString("Total number of planes left in takeoff queue %1\n\n").arg(takeoff.size()));
    m1.append(QString("Percentage of time runway idle %1\n\n").arg(get_idle_rate(time)));
    m1.append(QString("Average wait in landing queue %1 time units\n\n").arg(get_wait_landing()));
    m1.append(QString("Average wait in takeoff queue %1 time units\n\n").arg(get_wait_takeoff()));
    m1.append(QString("Average observed rate of planes wanting to land %1 per time unit\n\n").arg(get_landing_rate(time)));
    m1.append(QString("Average observed rate of planes wanting to take off %1 per time unit\n\n").arg(get_takeoff_rate(time)));
    return m1;
}

double Runway::get_idle_rate(int time) const{
    return (double)idle_time/time;
}

double Runway::get_wait_landing()const{
    return (double)land_wait/(double)num_landings;
}

double Runway::get_wait_takeoff() const{
    return (double)takeoff_wait/(double)num_takeoffs;
}

double Runway::get_landing_rate(int time) const{
    return (double)num_land_requests/time;
}

double Runway::get_takeoff_rate(int time) const{
    return (double)num_takeoff_requests/time;
}

int Runway::getLandingSize() const{
    return landing.size();
}

bool Runway::isSafe() const{
    Node *node=landing.front;
    while(node!=landing.rear){
        if(node->entry.getFuelNum()<=0){
            return false;
        }
        node=node->next;
    }
    return true;
}

void Runway::reduceFuel(){
    Node *node=landing.front;
    while(node!=landing.rear){
        node->entry.setFuelNum(node->entry.getFuelNum()-1);
        node=node->next;
    }
}
