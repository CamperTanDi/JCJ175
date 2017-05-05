#ifndef	_MATH_H_
#define	_MATH_H_
extern double fabs(double);
extern double floor(double);
extern double ceil(double);
extern double modf(double, double *);
extern double sqrt(double);
extern double atof(const char *);
extern double sin(double);
extern double cos(double);
extern double tan(double);
extern double asin(double);
extern double acos(double);
extern double atan(double);
extern double atan2(double, double);
extern double log(double);
extern double log10(double);
extern double pow(double, double);
extern double exp(double);
extern double sinh(double);
extern double cosh(double);
extern double tanh(double);
extern double eval_poly(double, const double *, int);
extern double frexp(double, int *);
extern double ldexp(double, int);
extern double fmod(double, double);
extern double trunc(double);
extern double round(double);

#if defined(_HTKC_) || defined(_HTFSC_)
extern double __ldexp4__(double, int);
extern double __ldexp3__(double, int);
extern double __frexp4__(double, int *);
extern double __frexp3__(double, int *);
#define	frexp	(sizeof(double)==3?__frexp3__:__frexp4__)
#define	ldexp	(sizeof(double)==3?__ldexp3__:__ldexp4__)
#endif

#if defined(_HTKC_)
#pragma callsizes __ldexp3__ 5,0
#pragma callsizes __ldexp4__ 6,0
#pragma callsizes __frexp4__ 6,1
#pragma callsizes __frexp3__ 5,1
#pragma regsused __ldexp3__ a
#pragma regsused __ldexp4__ a
#pragma regsused __frexp4__ a mp0
#pragma regsused __frexp3__ a mp0
#endif

#if defined(_HTFSC_) 
#pragma callsizes __ldexp3__ 5,0
#pragma callsizes __ldexp4__ 6,0
#pragma callsizes __frexp4__ 6,1
#pragma callsizes __frexp3__ 5,1
#pragma regsused __ldexp3__ wreg
#pragma regsused __ldexp4__ wreg
#pragma regsused __frexp4__ wreg fsr0
#pragma regsused __frexp3__ wreg fsr0
#endif

#ifdef	__MSP430C__
#pragma inline(fabs)
#endif
// Microchip equivalent definitions 
#define	M_E		2.7182818284590452354
#define	M_LOG2E		1.4426950408889634074
#define	M_LOG10E	0.43429448190325182765
#define	M_LN2		0.69314718055994530942
#define	M_LN10		2.30258509299404568402
#define	M_PI		3.14159265358979323846
#define	M_PI_2		1.57079632679489661923
#define	M_PI_4		0.78539816339744830962
#define	M_1_PI		0.31830988618379067154
#define	M_2_PI		0.63661977236758134308
#define	M_2_SQRTPI	1.12837916709551257390
#define	M_SQRT2		1.41421356237309504880
#define	M_SQRT1_2	0.70710678118654752440
#endif	/* _MATH_H_ */

