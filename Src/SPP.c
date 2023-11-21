#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "SPP.h"

int spp(struct DataGPS *navData, struct ObsData *obsData, struct ObsHeaderInfo *obsHead, struct ObsSat *satlist) {
    //constant from GPS ICD
    double c = 299792458; //speed of light
    double pi = 3.1415926535898; //Pi
    double mu = 3.986005 * pow(10,14);
    double Omeg_dot_earth = 7.2921151467 * pow(10,-5);
    int timediff = 3600; //need to use parsing time!
    int index = 0;
    printf("Start Single Point Positioning\n");
    double signal_propagation_time, nominal_transmission_time, GPS_Sat_time_diff, relativistic_correction;
    double GPS_clk_correction, system_transmission_time, A, mean_motion, corrected_mean_motion, tk;
    double Mean_anomaly, E0, Ek;
    //PRN
    printf("%d\n",satlist->PRN_list[index]);
    //Compute signal propagation time
    signal_propagation_time = obsData[1].P1 / c;
    printf("show %f\n",signal_propagation_time);
    //Compute signal transmission time
    nominal_transmission_time = navData[satlist->PRN_list[1]].TOE - signal_propagation_time - timediff;
    printf("show %f\n",nominal_transmission_time);
    //Compute satellite clock correction
    relativistic_correction = 0;
    GPS_Sat_time_diff = navData[satlist->PRN_list[index]].clockBias + navData[satlist->PRN_list[index]].clockDrift 
                        * (nominal_transmission_time - navData[satlist->PRN_list[index]].TOE) + navData[satlist->PRN_list[index]].clockDriftRate
                        * pow((nominal_transmission_time - navData[satlist->PRN_list[index]].TOE),2) + relativistic_correction;
    printf("%.19lf\n",GPS_Sat_time_diff);
    GPS_clk_correction = GPS_Sat_time_diff - navData[satlist->PRN_list[index]].TGD;
    printf("%.19lf\n",navData[satlist->PRN_list[index]].clockBias);
    printf("%.19lf\n",GPS_clk_correction);
    //Compute system transmission time
    system_transmission_time = nominal_transmission_time - GPS_clk_correction;
    printf("%.19lf\n",system_transmission_time);
    //Compute eccentric anomaly
    A = pow(navData[satlist->PRN_list[index]].Sqrt_a,2);
    printf("A = %.19lf\n",A);
    mean_motion = sqrt(mu/pow(A,3));
    printf("mean_motion = %.19lf\n",mean_motion);
    corrected_mean_motion = mean_motion + navData[satlist->PRN_list[index]].Delta_n;
    printf("corrected_mean_motion = %.19lf\n",corrected_mean_motion);
    tk = system_transmission_time - navData[satlist->PRN_list[index]].TOE;
    printf("tk = %.19lf\n",tk);
    Mean_anomaly = navData[satlist->PRN_list[index]].Mo + corrected_mean_motion * tk;
    printf("Mean_anomaly = %.19lf\n",Mean_anomaly);
    E0 = Mean_anomaly;
    for (int i=1;i<2;i++) {
        Ek = Mean_anomaly + navData[satlist->PRN_list[index]].Eccentricity * E0;
        E0 = Ek;
    }
    printf("Ek = %.19lf\n",Ek);
    


    return 0;
}
