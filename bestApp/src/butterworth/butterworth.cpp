#include "butterworth.h"

void bilinear(const vec b, vec a,double Ts, vec &bd, vec &ad)
{
    // Zeros are the roots of the numerator
    cx_vec z = roots(b);

    // Poles are the roots of the denominator
    cx_vec p = roots(a);

    unsigned int Np = (static_cast<)unsigned int)(p.size());
    unsigned int Nz = static_cast<unsigned int>(z.size());
    unsigned int Nz_d = (Np > Nz) ? Np : Nz;

    // Calculates poles, zeros and g0 (gain at z=0) of the
    // digital transfer function
    std::complex<double> g0 = b.back();
    cx_vec zd(Nz_d);
    cx_vec pd(Np);
    for (unsigned int i=0; i<Nz; i++) {
        zd[i] = (1.0 + Ts*z[i]/2.0)/(1.0 - Ts*z[i]/2.0);
        g0 = g0*(1.0 - 2.0/(Ts*z[i]));
    }
    for (unsigned int i=0; i<Np; i++) {
        pd[i] = (1.0 + Ts*p[i]/2.0)/(1.0 - Ts*p[i]/2.0);
        g0 = g0/(1.0 - 2.0/(Ts*p[i]));
    }
    if (Np>Nz) {
        for (unsigned int i=Nz; i<Nz_d; i++) {
            zd[i] = -1.0;
        }
    }

    // Zeros are the roots of the numerator
    // Calculate the numerator coefficients from the zeros
    cx_vec bd_cx = {1};
    cx_vec tmp_cx = {1,1};
    for (unsigned int i=0; i<zd.size(); i++) {
        tmp_cx[1] = -zd[i];
        bd_cx = conv(bd_cx,tmp_cx);
    }
    bd_cx = bd_cx*g0;

    // Poles are the roots of the denominator
    // Calculate the denominator coefficients from the poles
    cx_vec ad_cx = {1};
    for (unsigned int i=0; i<pd.size(); i++) {
        tmp_cx[1] = -pd[i];
        ad_cx = conv(ad_cx,tmp_cx);
    }

    // Since we are working with real filters,
    // consider only the real part of the coefficients.
    // The imaginary part should be negligible
    vec bd_tmp(bd_cx.size());
    vec ad_tmp(ad_cx.size());

    for (unsigned int i=0; i<bd_cx.size(); i++) {
        bd_tmp[i] = bd_cx[i].real();
    }
    bd = bd_tmp;
    for (unsigned int i=0; i<ad_cx.size(); i++) {
        ad_tmp[i] = ad_cx[i].real();
    }
    ad = ad_tmp;
}

void butterworthLow(const unsigned int N, const double fc, const double Ts, vec &bd, vec &ad)
{

    // Analog Butterworth Filter
    //
    //                   1
    // H(s) = -------------------------
    //        \prod_{k=1}^{N}(1-s/s_pk)
    //
    // s_pk (k=1..N) are the poles in the s plane

    // Digital cut-off angular frequency
    double wc = 2.0*datum::pi*fc*Ts;

    // Analog cut-off angular frequency with prewarp
    double Wc = 2.0/Ts*tan(wc/2.0);

    // Poles of the Butterworth filter
    cx_vec s_p(N);
    for (unsigned int k=1; k<=N; k++) {
        s_p[k-1] = Wc*exp(std::complex<double>(0,1)*(2.0*k+N-1)*datum::pi/(2.0*N));
    }

    // Coefficients of denominator
    cx_vec tmp_cx = {1,1};
    cx_vec a_cx = {1};
    for (unsigned int i=0; i<s_p.size(); i++) {
        tmp_cx[1] = -s_p[i];
        a_cx = conv(a_cx,tmp_cx);
    }

    // The coefficients are real numbers, therefore discard the imaginary part
    vec a(a_cx.size());
    for (unsigned int i=0; i<a_cx.size(); i++) {
        a[i] = a_cx[i].real();
    }

    //Coefficients of numerator
    vec b = {1};

    // Calculate the digital equivalent
    bilinear(b,a,Ts,bd,ad);
}


void butterworthHigh(const unsigned int N, const double fc, const double Ts, vec &bd, vec &ad)
{

    // Analog Butterworth Filter
    //
    //                  s^N
    // H(s) = ------------------------------
    //        \prod_{k=1}^{N}(1-s/s_pk)
    //
    // s_pk (k=1..N) are the poles in the s plane

    // Digital cut-off angular frequency
    double wc = 2.0*datum::pi*fc*Ts;

    // Analog cut-off angular frequency with prewarp
    double Wc = 2.0/Ts*tan(wc/2.0);

    // Poles of the Butterworth filter
    cx_vec s_p(N);
    for (unsigned int k=1; k<=N; k++) {
        s_p[k-1] = Wc*exp(std::complex<double>(0,1)*(2.0*k+N-1)*datum::pi/(2.0*N));
    }

    // Coefficients of denominator
    cx_vec tmp_cx = {1,1};
    cx_vec a_cx = {1};
    for (unsigned int i=0; i<s_p.size(); i++) {
        tmp_cx[1] = -s_p[i];
        a_cx = conv(a_cx,tmp_cx);
    }

    // The coefficients are real numbers, therefore discard the imaginary part
    vec a(a_cx.size());
    for (unsigned int i=0; i<a_cx.size(); i++) {
        a[i] = a_cx[i].real();
    }



    // Zeros of the Butterworth filter
    // Coefficients of denominator
    // Wc*exp(std::complex<double>(0,1)*(2.0*k+N-1)*datum::pi/(2.0*N))
    vec b(a_cx.size());
    for (unsigned int i=1; i<a_cx.size(); i++) {
        b[i] = 1e-14;
    }
    b[0] = 1.0/pow(Wc,double(N));

    // Calculate the digital equivalent
    bilinear(b,a,Ts,bd,ad);
}


void butterworthBand(const unsigned int Norder, const double fcLow, const double fcHigh, const double Ts, vec &b, vec &a)
{
    // The idea is to start from a low-pass butterworth unitary with fcut 1 rad/s symmetric
    // resizing it with the correct bw and gain (wo) and then move it in frequency
    // in order to place it at fcLow + bw/2

    unsigned int N = Norder;
    // Digital cut-off angular frequency
    vec warped(2);
    warped[0] = fcLow;
    warped[1] = fcHigh;
    double fs = 2.0;
    double TsNorm = 1/fs;

    // transforming frequencies to digital
    for (unsigned int i=0; i<2; i++) {
        warped[i] = 2.0*warped[i]*Ts;
        warped[i] = 2.0/TsNorm*tan(datum::pi*warped[i]*TsNorm);
    }
    double bw = warped[1] - warped[0];
    double wo = sqrt(warped[1] * warped[0]);

    // creating N-th filter with angular cut off freq of 1 rad/s
    // equivalent of python signal.buttap(N) function
    cx_vec z(1);
    cx_vec p(N);
    double k = 1;
    z[0] = std::complex<double>(0.0,0.0);
    vec tmp_lin = linspace(-double(N-1), double(N-1), N);
    for (unsigned int i=0; i<p.size(); i++) {
        p[i] = -bw/2.0*exp(std::complex<double>(0,1)*datum::pi*tmp_lin[i]/(2.0*N));
    }

    // resizing unitary filter
    cx_vec z_bp(N);
    cx_vec p_bp(2*N);
    double k_bp = k*pow(bw, N);

    for (unsigned int i=0; i<N; i++) {
        z_bp[i]   = z[0];
        p_bp[i]   = p[i] + sqrt(p[i]*p[i] - wo*wo);
        p_bp[N+i] = p[i] - sqrt(p[i]*p[i] - wo*wo);
    }

    // applying bilinear transofrmazion to z,p,k
    double fs2 = 2*fs;
    cx_vec z_z(2*N);
    cx_vec p_z(2*N);
    for (unsigned int i=0; i<N; i++) {
        z_z[i] = (fs2 + z_bp[i]) / (fs2 - z_bp[i]);
    }
    // any zeros that were at infinity get moved to the Nyquist freq
    for (unsigned int i=N; i<N*2; i++) {
        z_z[i] = std::complex<double>(-1.0,0.0);
    }
    for (unsigned int i=0; i<N*2; i++) {
        p_z[i] = (fs2 + p_bp[i]) / (fs2 - p_bp[i]);
    }

    cx_vec tmp(2);
    tmp[0] = prod(fs2 - p_bp);
    tmp[1] = prod(fs2 - z_bp);
    tmp[2] = tmp[1] / tmp[0];
    double k_z = k_bp * tmp[2].real();

    // transforming z,p,k to a,b (transfer function coefficients)
    // Coefficients of Numerator
    cx_vec tmp_cx = {1,1};
    cx_vec b_cx = {1};
    for (unsigned int i=0; i<z_z.size(); i++) {
        tmp_cx[1] = -z_z[i];
        b_cx = conv(b_cx,tmp_cx);
    }
    for (unsigned int i=0; i<b_cx.size(); i++)
        b[i] = b_cx[i].real() * k_z;

    // Coefficients of Denominator
    tmp_cx = {1,1};
    cx_vec a_cx = {1};
    for (unsigned int i=0; i<p_z.size(); i++) {
        tmp_cx[1] = -p_z[i];
        a_cx = conv(a_cx,tmp_cx);
    }
    for (unsigned int i=0; i<a_cx.size(); i++)
        a[i] = a_cx[i].real();

}


















