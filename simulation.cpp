#include "stdafx.h"
#include "mainwindow.h"


using namespace std;

Simulation::Simulation(MainWindow& window)
{
   w=&window;
   Plane::setWindow(window);
}

void Simulation::initialize(int &end_time, int &queue_limit, double &arrival_rate, double &departure_rate)
{

    departure_rate = w->getDeparture();
    arrival_rate = w->getLanding();
    end_time = w->getTime();
    queue_limit = w->getQueue();
	flight_number = 0;
}

void Simulation::run_idle(int time)
{
    QString qStr=QString("%1: Runway is idle.\n").arg(time);
    if(w->isExactWay()){
        w->appendToDetailExactText(qStr);
    }
    else if(w->isNotExactWay()){
        w->appendToDetailText(qStr);
    }
}

void Simulation::runActivity(Runway &small_airport){

    Random variable(false);
    for (int current_time = 0; current_time < end_time; current_time++) {

        //calculate the plane want to land

        int number_arrivals = variable.poisson(arrival_rate);

        for (int i = 0; i < number_arrivals; i++) {
            Plane current_plane(flight_number++, current_time, arriving);
            if (small_airport.can_land( current_plane) != success)
                current_plane.refuse();
        }

        //calculate the plane want to depart

        int number_departures = variable.poisson(departure_rate);

        for (int j = 0; j < number_departures; j++) {
            Plane current_plane(flight_number++, current_time, departing);
            if (small_airport.can_depart( current_plane) != success) {
                current_plane.refuse();
            }
        }

        //let the plane arrive or depart or nothing to do(only one runway)

        if (w->isTwoRunWay()) {
            Plane landPlane;
            Plane takeoffPlane;
            if(small_airport.activity_two_way(current_time,landPlane,takeoffPlane)==idle) {
                run_idle(current_time);
            }
        }
        else if (w->isTwoPriorWay()) {
            Plane plane_1;
            Plane plane_2;
            if(small_airport.activity_two_way(current_time,plane_1,plane_2)==idle) {
                run_idle(current_time);
            }
        }
        else if (w->isThreeWay()) {
                Plane moving_plane;
                if(small_airport.activity_three(current_time, moving_plane)==idle) {
                    run_idle(current_time);
                }
        }
        else{
                Plane moving_plane;
            switch (small_airport.activity(current_time, moving_plane)) {
            case land:
                moving_plane.land(current_time);
                break;
            case takeoff:
                moving_plane.fly(current_time);
                break;
            case idle:
                run_idle(current_time);
            }
        }

    }
}

void Simulation::runOnQueue(int queue_num, QVector<double>& result) {

    initialize(end_time, queue_limit, arrival_rate, departure_rate);
	queue_limit = queue_num;
	Random variable(false);
    Runway small_airport(*w,queue_limit);

	for (int current_time = 0; current_time < end_time; current_time++) {
		//calculate the plane want to land

		int number_arrivals = variable.poisson(arrival_rate);

		for (int i = 0; i < number_arrivals; i++) {
			Plane current_plane(flight_number++, current_time, arriving);
            if (small_airport.can_land( current_plane) != success)
				current_plane.refuse();
		}

		//calculate the plane want to depart

		int number_departures = variable.poisson(departure_rate);

		for (int j = 0; j < number_departures; j++) {
			Plane current_plane(flight_number++, current_time, departing);
            if (small_airport.can_depart( current_plane) != success)
				current_plane.refuse();
		}

        if (w->isTwoRunWay()) {
            Plane landPlane;
            Plane takeoffPlane;
            if(small_airport.activity_two_way(current_time,landPlane,takeoffPlane)==idle) {
                run_idle(current_time);
            }
        }
        else if (w->isTwoPriorWay()) {
            Plane plane_1;
            Plane plane_2;
            if(small_airport.activity_two_priority(current_time,plane_1,plane_2)==idle) {
                run_idle(current_time);
            }
        }
        else if (w->isThreeWay()) {
                Plane moving_plane;
                if(small_airport.activity_three(current_time, moving_plane)==idle){
                    run_idle(current_time);
                }
        }
        else{
                Plane moving_plane;
            switch (small_airport.activity(current_time, moving_plane)) {
            case land:
                moving_plane.land(current_time);
                break;
            case takeoff:
                moving_plane.fly(current_time);
                break;
            case idle:
                run_idle(current_time);
            }
        }

    }
	result[0] = small_airport.get_landing_rate(end_time);
	result[1] = small_airport.get_takeoff_rate(end_time);
	result[2] = small_airport.get_idle_rate(end_time);
	result[3] = small_airport.get_wait_landing();
	result[4] = small_airport.get_wait_takeoff();

}

void Simulation::runWithFuel() {

    initialize(end_time, queue_limit, arrival_rate, departure_rate);

	Random variable(false);
    Runway small_airport(*w,queue_limit);

	Random variable1(false);

	for (int current_time = 0; current_time < end_time; current_time++) {

		int number_arrivals = variable.poisson(arrival_rate);
        int fuel_num = variable1.uniform(w->getFuelNum());
		for (int i = 0; i < number_arrivals; i++) {
			Plane current_plane(flight_number++, current_time, fuel_num, arriving);
            if (small_airport.can_land( current_plane) != success) {
				current_plane.refuse();
			}
		}

		//calculate the plane want to depart

		int number_departures = variable.poisson(departure_rate);

		for (int j = 0; j < number_departures; j++) {
			Plane current_plane(flight_number++, current_time, departing);
            if (small_airport.can_depart( current_plane) != success) {
				current_plane.refuse();
			}
		}

		//let the plane arrive or depart or nothing to do(only one runway)

		Plane moving_plane;

		typedef Runway_activity(Runway::*f)(int, Plane &);
		f funct = &Runway::activity;
		switch ((small_airport.*funct)(current_time, moving_plane)) {
		case land:
			moving_plane.land(current_time);
			break;
		case takeoff:
			moving_plane.fly(current_time);
			break;
		case idle:
			run_idle(current_time);
		}
        small_airport.reduceFuel();
		if (!small_airport.isSafe()) {
            QString qStr("Danger!!!One plane crashed!!!\n");

            if(w->isExactWay()){
                w->appendToDetailExactText(qStr);
            }
            else if(w->isNotExactWay()){
                w->appendToDetailText(qStr);
            }

			break;
		}

	}

    QString s=small_airport.shut_down( end_time);
    w->appendToText(s);

}

void Simulation::run() {

    initialize(end_time, queue_limit, arrival_rate, departure_rate);

    Runway small_airport(*w,queue_limit);

    runActivity(small_airport);

    QString s=small_airport.shut_down( end_time);

    w->appendToText(s);
}

void Simulation::run(QVector<int>& toland, QVector<int>& todepart) {


    queue_limit=w->getQueue();
    flight_number=0;
    end_time=toland.size();

	Random variable(false);
    Runway small_airport(*w,queue_limit);
\
    if(toland.size()!=todepart.size()){
        QString warn("Error:toland size is not equal to todepart size!");
        w->warning(warn,3000);
        return;
    }

    for (auto curr_land = toland.begin(),curr_depart=todepart.begin(); curr_land != toland.end(); ++curr_land,++curr_depart) {

        int current_time=curr_land - toland.begin();
		//calculate the plane want to land

        int number_arrivals = (*curr_land);

		for (int i = 0; i < number_arrivals; i++) {
            Plane current_plane(flight_number++, current_time, arriving);
            if (small_airport.can_land( current_plane) != success)
				current_plane.refuse();
		}

		//calculate the plane want to depart

        int number_departures = (*curr_depart);

		for (int j = 0; j < number_departures; j++) {
            Plane current_plane(flight_number++, current_time, departing);
            if (small_airport.can_depart( current_plane) != success) {
				current_plane.refuse();
			}
		}

        if (w->isTwoRunWay()) {
            Plane landPlane;
            Plane takeoffPlane;
            if(small_airport.activity_two_way(current_time,landPlane,takeoffPlane)==idle) {
                run_idle(current_time);
            }
        }
        else if (w->isTwoPriorWay()) {
            Plane plane_1;
            Plane plane_2;
            if(small_airport.activity_two_priority(current_time,plane_1,plane_2)==idle) {
                run_idle(current_time);
            }
        }
        else if (w->isThreeWay()) {
                Plane moving_plane;
                if(small_airport.activity_three(current_time,moving_plane)==idle){
                    run_idle(current_time);
                }
        }
        else{
                Plane moving_plane;
                switch (small_airport.activity(current_time, moving_plane)) {
                case land:
                    moving_plane.land(current_time);
                    break;
            case takeoff:
                moving_plane.fly(current_time);
                break;
            case idle:
                run_idle(current_time);
            }
        }

	}

    QString s=small_airport.shut_down(end_time);
    w->appendToExactText(s);
}
