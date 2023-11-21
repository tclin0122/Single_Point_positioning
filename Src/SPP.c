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
    double F = -4.442807633 * pow(10,-10);
    int timediff = 3600; //need to use parsing time!
    int index = 0;
    printf("Start Single Point Positioning\n");
    double signal_propagation_time, nominal_transmission_time, GPS_Sat_time_diff, relativistic_correction;
    double GPS_clk_correction, system_transmission_time, A, mean_motion, corrected_mean_motion, tk;
    double Mean_anomaly, E0, Ek, a, b, vk, lat_k, lat_correction, radi_correction, inclin_correction;
    double corrected_lat, corrected_radi, corrected_inclin;
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
    relativistic_correction = F * navData[satlist->PRN_list[index]].Eccentricity * navData[satlist->PRN_list[index]].Sqrt_a 
                              * sin(Ek);
    printf("%.19lf\n",relativistic_correction); 
    //Compute satellite coordinates Xs, Ys, Zs
    Ek = -1.7688264606399397398236685000349;
    a = (sqrt(1.0 - pow(navData[satlist->PRN_list[index]].Eccentricity,2)) * sin(Ek)) / (1.0 - navData[satlist->PRN_list[index]].Eccentricity * cos(Ek));
    printf("%lf\n",a);
    b = (cos(Ek) - navData[satlist->PRN_list[index]].Eccentricity) / (1.0 - navData[satlist->PRN_list[index]].Eccentricity * cos(Ek));
    printf("%lf\n",b);
    vk = atan2 ( a , b );
    printf("%lf\n",vk);
    lat_k = vk + navData[satlist->PRN_list[index]].Omega;
    printf("%lf\n",lat_k);
    lat_correction = navData[satlist->PRN_list[index]].Cus * sin(2 * lat_k) + navData[satlist->PRN_list[index]].Cuc * cos(2 * lat_k);
    radi_correction = navData[satlist->PRN_list[index]].Crs * sin(2 * lat_k) + navData[satlist->PRN_list[index]].Crc * cos(2 * lat_k);
    inclin_correction = navData[satlist->PRN_list[index]].Cis * sin(2 * lat_k) +navData[satlist->PRN_list[index]].Cic * cos(2 * lat_k);
    corrected_lat = lat_k + lat_correction;
    corrected_radi = A * (1 - navData[satlist->PRN_list[index]].Eccentricity * cos(Ek)) + radi_correction;
    corrected_inclin = navData[satlist->PRN_list[index]].Io + inclin_correction + navData[satlist->PRN_list[index]].IDOT * tk;
    printf("%lf\n",corrected_lat);

    



    return 0;
}
