
#include "stdafx.h"

int Random::reseed() {
	seed = seed * multiplier + add_on;
	return seed;
}

Random::Random(bool pseudo) {
	if (pseudo) seed = 1;
	else seed = time(NULL) % INT_MAX;
	multiplier = 2743;
	add_on = 5923;
}


int Random::poisson(double mean) {
	double limit = exp(-mean);
	double product = random_real();
	int count = 0;
	while (product > limit) {
		count++;
		product *= random_real();
	}
	return count;
}

int Random::uniform(int max){
    int result=random_int();
    return result%(max)+1;
}

double Random::random_real() {
	double max = INT_MAX + 1.0;
	double temp = reseed();
	if (temp < 0) {
		temp = temp + max;
	}
	return temp / max;
}

int Random::random_int(){
    int temp=reseed();
    if(temp<0){
        temp=temp+INT_MAX+1;
    }
    return temp;
}
