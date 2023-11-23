#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "SPP.h"

void Cart2Ecip (double X, double Y, double Z, double *lat, double *lon);

int spp(struct DataGPS *navData, struct ObsData *obsData, struct ObsHeaderInfo *obsHead, struct ObsSat *satlist) {
    //constant from GPS ICD
    double c = 299792458; //speed of light
    double pi = 3.1415926535898; //Pi
    double mu = 3.986005 * pow(10,14);
    double Omeg_dot_earth = 7.2921151467 * pow(10,-5);
    double F = -4.442807633 * pow(10,-10);
    int timediff = 3600; //need to use parsing time!
    int index = 0;//satlist->GPS_num;
    double err_rel;
    double err_Ek = 1.;
    double err_tol = pow(10,-20);

    double signal_propagation_time, nominal_transmission_time, GPS_Sat_time_diff, relativistic_correction;
    double GPS_clk_correction, system_transmission_time, A, mean_motion, corrected_mean_motion, tk;
    double Mean_anomaly, E0, Ek, a, b, vk, lat_k, lat_correction, radi_correction, inclin_correction;
    double corrected_lat, corrected_radi, corrected_inclin, x_in_orb, y_in_orb, corrected_ascending_node;
    double X_s[satlist->GPS_num], Y_s[satlist->GPS_num], Z_s[satlist->GPS_num];

    double lat, lon;
    printf("Start Single Point Positioning\n");

    //Start the loop
    while (index < satlist->GPS_num) {
        //Compute signal propagation time
        signal_propagation_time = obsData[index].P1 / c;
        //Compute signal transmission time
        nominal_transmission_time = navData[satlist->PRN_list[index]].TOE - signal_propagation_time - timediff;
        //Compute satellite clock correction
        relativistic_correction = 0;
        err_rel = 1;

        while (fabs(err_rel) > err_tol) {
            err_rel = relativistic_correction;
            GPS_Sat_time_diff = navData[satlist->PRN_list[index]].clockBias + navData[satlist->PRN_list[index]].clockDrift 
                            * (nominal_transmission_time - navData[satlist->PRN_list[index]].TOE) + navData[satlist->PRN_list[index]].clockDriftRate
                            * pow((nominal_transmission_time - navData[satlist->PRN_list[index]].TOE),2) + relativistic_correction;
            GPS_clk_correction = GPS_Sat_time_diff - navData[satlist->PRN_list[index]].TGD;
            //Compute system transmission time
            system_transmission_time = nominal_transmission_time - GPS_clk_correction;
            //Compute eccentric anomaly
            A = pow(navData[satlist->PRN_list[index]].Sqrt_a,2);
            mean_motion = sqrt(mu/pow(A,3));
            corrected_mean_motion = mean_motion + navData[satlist->PRN_list[index]].Delta_n;
            tk = system_transmission_time - navData[satlist->PRN_list[index]].TOE;
            Mean_anomaly = navData[satlist->PRN_list[index]].Mo + corrected_mean_motion * tk;
            E0 = Mean_anomaly;
            err_Ek = 1;
            while (fabs(err_Ek) > err_tol) {
                Ek = Mean_anomaly + navData[satlist->PRN_list[index]].Eccentricity * sin(E0);
                err_Ek = Ek - E0;
                //printf("Err = %.19lf\n",err_Ek);
                E0 = Ek;
            }
            relativistic_correction = F * navData[satlist->PRN_list[index]].Eccentricity * navData[satlist->PRN_list[index]].Sqrt_a * sin(Ek);
            err_rel -= relativistic_correction;
        }
        //Compute satellite coordinates Xs, Ys, Zs
        a = (sqrt(1.0 - pow(navData[satlist->PRN_list[index]].Eccentricity,2)) * sin(Ek)) / (1.0 - navData[satlist->PRN_list[index]].Eccentricity * cos(Ek));
        b = (cos(Ek) - navData[satlist->PRN_list[index]].Eccentricity) / (1.0 - navData[satlist->PRN_list[index]].Eccentricity * cos(Ek));
        vk = atan2 ( a , b );
        lat_k = vk + navData[satlist->PRN_list[index]].Omega;
        lat_correction = navData[satlist->PRN_list[index]].Cus * sin(2 * lat_k) + navData[satlist->PRN_list[index]].Cuc * cos(2 * lat_k);
        radi_correction = navData[satlist->PRN_list[index]].Crs * sin(2 * lat_k) + navData[satlist->PRN_list[index]].Crc * cos(2 * lat_k);
        inclin_correction = navData[satlist->PRN_list[index]].Cis * sin(2 * lat_k) +navData[satlist->PRN_list[index]].Cic * cos(2 * lat_k);
        corrected_lat = lat_k + lat_correction;
        corrected_radi = A * (1 - navData[satlist->PRN_list[index]].Eccentricity * cos(Ek)) + radi_correction;
        corrected_inclin = navData[satlist->PRN_list[index]].Io + inclin_correction + navData[satlist->PRN_list[index]].IDOT * tk;
        x_in_orb = corrected_radi * cos(corrected_lat);
        y_in_orb = corrected_radi * sin(corrected_lat);
        corrected_ascending_node = navData[satlist->PRN_list[index]].OMEGA + (navData[satlist->PRN_list[index]].Omega_dot - Omeg_dot_earth) * tk - Omeg_dot_earth * navData[satlist->PRN_list[index]].TOE;

        X_s[index] = x_in_orb * cos(corrected_ascending_node) - y_in_orb * cos(corrected_inclin) * sin(corrected_ascending_node);
        Y_s[index] = x_in_orb * sin(corrected_ascending_node) + y_in_orb * cos(corrected_inclin) * cos(corrected_ascending_node);
        Z_s[index] = y_in_orb * sin(corrected_inclin);
        //update the index
        index++;
    }
    /*
    index = 0;
    // show the satellite coord.
    while (index < satlist->GPS_num) {
        printf("PRN = %d\n", navData[satlist->PRN_list[index]].PRN);
        printf("X_s = %lf\n",X_s[index]);
        printf("Y_s = %lf\n",Y_s[index]);
        printf("Z_s = %lf\n",Z_s[index]);
        //update the index
        index++;
    }
    */
    //Carte2Ellip
    Cart2Ecip(obsHead->approxPosX,obsHead->approxPosY,obsHead->approxPosZ,&lat, &lon);
    printf("lat = %f\n",lat);
    printf("lon = %f\n",lon);
    //Trop delay

    //Iono delay

    //Approximate distance

    
   
 
    return 0;
}

void Cart2Ecip (double X, double Y, double Z, double *lat, double *lon) {
    double a = 6378137;  // semi major axis
    double fe = 1 / 298.257222101;
    double b = a - fe * a; // semi minor axis
    double e2 = (pow(a, 2) - pow(b, 2)) / pow(a, 2);

    double p = sqrt(pow(X, 2) + pow(Y, 2));
    double lat0 = atan2((Z / p) * (pow((1 - e2), -1)), 1) * (180 / M_PI);

    double computedLat = 0;
    double computedLon = 0;
    while (computedLat != lat0) {
        double N0 = pow(a, 2) / sqrt(pow(a, 2) * pow(cos(lat0 * (M_PI / 180)), 2) + pow(b, 2) * pow(sin(lat0 * (M_PI / 180)), 2));
        double h = p / cos(lat0 * (M_PI / 180)) - N0;
        computedLat = atan2((Z / p) * pow(1 - e2 * (N0 / (N0 + h)), -1), 1) * (180 / M_PI);
        lat0 = computedLat;
        computedLon = atan2(Y, X) * (180 / M_PI);
        h = p / cos(lat0 * (M_PI / 180)) - N0;
    }
    //passed the value back
    *lat = computedLat;
    *lon = computedLon;
}


