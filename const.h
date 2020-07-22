#pragma once
#define NUM_THREADS 4
//定义pi的值
#define pi 3.1415926

#define l 60
#define m 60
#define h 10

double dt = 0.005;
double mass0 = 0.0005;
double const_Ks = 0.4;
double const_Kd = 0.0002;
double const_L_0 = 0.01;

#define Wire 1
#define Solid 2

GLint visual_style = Wire;