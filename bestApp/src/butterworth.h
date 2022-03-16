#ifndef BUTTERWORTH_H
#define BUTTERWORTH_H

#include <armadillo>

using namespace arma;

void bilinear(const vec b, vec a,double Ts, vec &bd, vec &ad);

void butterworthLow(const unsigned int N, const double fc, const double Ts, vec &bd, vec &ad);

void butterworthHigh(const unsigned int N, const double fc, const double Ts, vec &bd, vec &ad);

void butterworthBand(const unsigned int Norder, const double fcLow, const double fcHigh, const double Ts, vec &b, vec &a);

#endif // BUTTERWORTH_H
