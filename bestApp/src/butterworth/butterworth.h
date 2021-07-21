#ifndef BUTTERWORTH_H
#define BUTTERWORTH_H

#include <armadillo>

using namespace arma;

void bilinear(const vec b, vec a,double Ts, vec &bd, vec &ad);

void butterworthLow(const quint32 N, const double fc, const double Ts, vec &bd, vec &ad);

void butterworthHigh(const quint32 N, const double fc, const double Ts, vec &bd, vec &ad);

void butterworthBand(const quint32 Norder, const double fcLow, const double fcHigh, const double Ts, vec &b, vec &a);

#endif // BUTTERWORTH_H
