#ifndef SIMULATION_H
#define SIMULATION_H

#include "mainwindow.h"

class Simulation
{
public:
    Simulation(MainWindow& w);
	int end_time;
	int queue_limit;//the maximum size of the queue
	int flight_number;
	double arrival_rate, departure_rate;

    void initialize(int &end_time, int &queue_limit, double &arrival_rate, double &departure_rate);
	void run_idle(int time);
    void run();
    void runOnQueue(int queue_num, QVector<double>& result);
    void runWithFuel();
    void run(QVector<int>& toland, QVector<int>& todepart);
    void runActivity(Runway &small_airport);
    MainWindow* w;

};

#endif // SIMULATION_H
