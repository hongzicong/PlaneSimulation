#pragma once

#ifndef RANDOM_H
#define RANDOM_H

class Random {
public:
	Random(bool pseudo = true);
	int poisson(double mean);
	double random_real();
    int uniform(int max);
    int random_int();
private:
	int reseed();
	int seed, multiplier, add_on;
};

#endif
