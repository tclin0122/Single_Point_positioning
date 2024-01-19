#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "SPP.h"

void Cart2Ecip (double X, double Y, double Z, double *lat, double *lon, double *height);
void relative_position (double X, double X_a, double Y, double Y_a, double Z, double Z_a, double lat, double lon, double *zij);
void iono (double Zenth, double TECU, double *d_iono);
void trop (double Zenth, double height, double *d_trop);
int position_correction (double trop_delay[], double iono_delay[], double pseudorange[], double GPS_clk_correction[], double X_r, double Y_r, double Z_r, double X_s[], double Y_s[], double Z_s[], double *RX_x, double *RX_y, double *RX_z);


int spp(struct DataGPS *navData, struct ObsData *obsData, struct ObsHeaderInfo *obsHead, struct ObsSat *satlist) {
    int index = 0;
    double GPS_clk_correction[satlist->GPS_num];
    double X_s[satlist->GPS_num], Y_s[satlist->GPS_num], Z_s[satlist->GPS_num];

    double lat, lon, height, zij, d_iono, d_trop;
    double pseudorange[satlist->GPS_num];
    double Zenith[satlist->GPS_num];
    double iono_delay[satlist->GPS_num];
    double trop_delay[satlist->GPS_num];
    double TECU = 5.3;

    double RX_x, RX_y, RX_z;
    int err_flag;
    printf("Start Single Point Positioning\n");

    //Start the loop
    while (index < satlist->GPS_num) {
        int timediff = 3600; //need to use parsing time!
        double err_tol = pow(10,-20);
        double err_Ek = 1.;
        double err_rel;
        double signal_propagation_time, nominal_transmission_time, GPS_Sat_time_diff, relativistic_correction;
        double sat_clk_correction, system_transmission_time, A, mean_motion, corrected_mean_motion, tk;
        double Mean_anomaly, E0, Ek, a, b, vk, lat_k, lat_correction, radi_correction, inclin_correction;
        double corrected_lat, corrected_radi, corrected_inclin, x_in_orb, y_in_orb, corrected_ascending_node;
        //Compute signal propagation time
        pseudorange[index] = obsData[index].P1;
        signal_propagation_time = pseudorange[index] / c_light;
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
            sat_clk_correction = GPS_Sat_time_diff - navData[satlist->PRN_list[index]].TGD;
            //Compute system transmission time
            system_transmission_time = nominal_transmission_time - sat_clk_correction;
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
                E0 = Ek;
            }
            relativistic_correction = F * navData[satlist->PRN_list[index]].Eccentricity * navData[satlist->PRN_list[index]].Sqrt_a * sin(Ek);
            err_rel -= relativistic_correction;
        }
        GPS_clk_correction[index] = sat_clk_correction;
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
    index = 0;
    /*
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
    while (index < satlist->GPS_num) {
        printf("GPS_clk_correction = %.19lf\n", GPS_clk_correction[index]);
        index++;
    }
    index = 0;
    //Carte2Ellip
    Cart2Ecip(obsHead->approxPosX, obsHead->approxPosY, obsHead->approxPosZ, &lat, &lon, &height);
    //Get Zenith angle and do Trop and Iono
    printf("Start atmosphere effect calculation\n");
    int ind = 0;
    while (ind < satlist->GPS_num) {
        relative_position (X_s[index], obsHead->approxPosX, Y_s[ind], obsHead->approxPosY, Z_s[ind], obsHead->approxPosZ, lat, lon, &zij);
        Zenith[ind] = zij;
        ind++;
        iono (zij, TECU, &d_iono);
        iono_delay[ind] = d_iono;
        trop (zij, height, &d_trop);
        trop_delay[ind] = d_trop;
        //printf("d_t %lf\n", d_trop);
    }
    //Approximate distance
    //void position_correction (double *trop_delay, double *iono_delay, double *pseudorange, double *GPS_clk_correction, double X_r, double Y_r, double Z_r, double *X_s, double *Y_s, double *Z_s, double *RX_x, double *RX_y, double *RX_z);
    printf("Start position correction\n");
    printf("Z_r %lf",obsHead->approxPosZ);
    err_flag = position_correction (trop_delay, iono_delay, pseudorange, GPS_clk_correction, obsHead->approxPosX, obsHead->approxPosY, obsHead->approxPosZ, X_s, Y_s, Z_s, &RX_x, &RX_y, &RX_z);
    if (err_flag != 0) {
        printf("failed SPP\n");
        return 1;
    }
    printf("RX_X %lf\n", RX_x);
    printf("RX_Y %lf\n", RX_y);
    printf("RX_Z %lf\n", RX_z);
    /*
    */

    return 0;
}

void Cart2Ecip (double X, double Y, double Z, double *lat, double *lon, double *height) {
    double a = 6378137;  // semi major axis
    double fe = 1 / 298.257222101;
    double b = a - fe * a; // semi minor axis
    double e2 = (pow(a, 2) - pow(b, 2)) / pow(a, 2);

    double p = sqrt(pow(X, 2) + pow(Y, 2));
    double lat0 = atan2((Z / p) * (pow((1 - e2), -1)), 1) * (180 / M_PI);

    double computedLat = 0;
    double computedLon = 0;
    double h =0;
    while (computedLat != lat0) {
        double N0 = pow(a, 2) / sqrt(pow(a, 2) * pow(cos(lat0 * (M_PI / 180)), 2) + pow(b, 2) * pow(sin(lat0 * (M_PI / 180)), 2));
        h = p / cos(lat0 * (M_PI / 180)) - N0;
        computedLat = atan2((Z / p) * pow(1 - e2 * (N0 / (N0 + h)), -1), 1) * (180 / M_PI);
        lat0 = computedLat;
        computedLon = atan2(Y, X) * (180 / M_PI);
        h = p / cos(lat0 * (M_PI / 180)) - N0;
    }
    //passed the value back
    *lat = computedLat;
    *lon = computedLon;
    *height = h;
}

void relative_position (double X, double X_a, double Y, double Y_a, double Z, double Z_a, double lat, double lon, double *zij) {
    double R[3][3] = {
        {-sin(lat * DEG_2_RADI) * cos(lon * DEG_2_RADI), -sin(lon * DEG_2_RADI), cos(lat * DEG_2_RADI) * cos(lon * DEG_2_RADI)},
        {-sin(lat * DEG_2_RADI) * sin(lon * DEG_2_RADI),  cos(lon * DEG_2_RADI), cos(lat * DEG_2_RADI) * sin(lon * DEG_2_RADI)},
        { cos(lat * DEG_2_RADI),                          0,                     sin(lat * DEG_2_RADI)}
    };
    double transpose_R[3][3];
    double Xij[3], Sij, azimuth_ij, zen_ij;
    // computing the transpose
    for (int row = 0; row < 3; ++row) {
        for (int column = 0; column < 3; ++column) {
            transpose_R[column][row] = R[row][column];
        }
    }
    //Distance diff
    X -= X_a;
    Y -= Y_a;
    Z -= Z_a;
    //Multiplication
    Xij[0] = transpose_R[0][0] * X + transpose_R[0][1] * Y + transpose_R[0][2] * Z;
    Xij[1] = transpose_R[1][0] * X + transpose_R[1][1] * Y + transpose_R[1][2] * Z;
    Xij[2] = transpose_R[2][0] * X + transpose_R[2][1] * Y + transpose_R[2][2] * Z;
    //measurement quantities
    Sij = sqrt(pow(Xij[0],2) + pow(Xij[1],2) + pow(Xij[2],2));
    azimuth_ij = atan2(Xij[1] ,Xij[0]);
    zen_ij = acos(Xij[2] / Sij);
    //passed the value back
    *zij = zen_ij;
}

void iono (double Zenith, double TECU, double *d_iono) {
    double TEC = TECU * 1e16;
    double OF = 1.0 - pow((Re * sin(Zenith) / (Re + hi)), 2);
    OF = 1.0 / sqrt(OF);
    *d_iono = (40.3 / pow(f, 2)) * TEC * OF;
}
//see if need to change to correction model
void trop (double Zenth, double height, double *d_trop) {
    double water_vapor_pressure = 6.108 * RH * exp((17.15 * T - 4684) / (T - 38.45));
    *d_trop = (0.002277 / cos(Zenth)) * (P + (1255 / T + 0.05) * water_vapor_pressure -pow(tan(Zenth),2));
}

int position_correction (double trop_delay[], double iono_delay[], double pseudorange[], double GPS_clk_correction[], double X_r, double Y_r, double Z_r, double X_s[], double Y_s[], double Z_s[], double *RX_x, double *RX_y, double *RX_z) {    
    double xa = 0., ya = 0., za = 0.;
    int num = 12;
    
    printf("num %d\n",num);
    double transmit_time[num], distance_tx_rx[num], L[num], ax[num], ay[num], az[num], A[num][4];
    double ATA[4][4] = {0};
    xa = X_r; ya = Y_r; za = Z_r;
    double V_err = 0;
    double V_err_0 = 0;
    double Err;
    Err = 1;
    for (int index = 0; index < num; ++index) {
        transmit_time[index] = pseudorange[index] / c_light;
    }
    while ( fabs(Err) > ERR_TOLLERANCE) {
        for (int index = 0; index < num; index++) {
            distance_tx_rx[index] = sqrt(pow((X_s[index] - xa + ya * Omeg_dot_earth * transmit_time[index]),2) + pow((Y_s[index] - ya + xa * Omeg_dot_earth * transmit_time[index]),2) + pow((Z_s[index] - za),2));
            L[index] = pseudorange[index] - distance_tx_rx[index] + GPS_clk_correction[index] * c_light - iono_delay[index] - trop_delay[index];
            A[index][0] =  -(X_s[index] - xa) / distance_tx_rx[index];
            A[index][1] =  -(Y_s[index] - ya) / distance_tx_rx[index];
            A[index][2] =  -(Z_s[index] - za) / distance_tx_rx[index];
            A[index][3] = 1.;
        }
        

        double transpose_A[4][num];
        // computing the transpose of A
        for (int row = 0; row < num; ++row) {
            for (int column = 0; column < 4; ++column) {
                transpose_A[column][row] = A[row][column];
            }
        }
        //Multiplication
        for (int row = 0; row < 4; ++row) {
            for (int col= 0; col < 4; ++col) {
                for (int element = 0; element < num; ++element) {
                    ATA[row][col] += transpose_A[row][element] * A[element][col];
                }
            } 
        }
        //inverse of ATA
        double det_A = 0;
        double Qx[4][4];
        det_A = ATA[0][0]*ATA[1][1]*ATA[2][2]*ATA[3][3] + ATA[0][0]*ATA[1][2]*ATA[2][3]*ATA[3][1] + ATA[0][0]*ATA[1][3]*ATA[2][1]*ATA[3][2]
                - ATA[0][0]*ATA[1][3]*ATA[2][2]*ATA[3][1] - ATA[0][0]*ATA[1][2]*ATA[2][1]*ATA[3][3] - ATA[0][0]*ATA[1][1]*ATA[2][3]*ATA[3][2]
                - ATA[0][1]*ATA[1][0]*ATA[2][2]*ATA[3][3] - ATA[0][2]*ATA[1][0]*ATA[2][3]*ATA[3][1] - ATA[0][3]*ATA[1][0]*ATA[2][1]*ATA[3][2]
                + ATA[0][3]*ATA[1][0]*ATA[2][2]*ATA[3][1] + ATA[0][2]*ATA[1][0]*ATA[2][1]*ATA[3][3] + ATA[0][1]*ATA[1][0]*ATA[2][3]*ATA[3][2]
                + ATA[0][1]*ATA[1][2]*ATA[2][0]*ATA[3][3] + ATA[0][2]*ATA[1][3]*ATA[2][0]*ATA[3][1] + ATA[0][3]*ATA[1][1]*ATA[2][0]*ATA[3][2]
                - ATA[0][3]*ATA[1][2]*ATA[2][0]*ATA[3][1] - ATA[0][2]*ATA[1][1]*ATA[2][0]*ATA[3][3] - ATA[0][1]*ATA[1][3]*ATA[2][0]*ATA[3][2]
                - ATA[0][1]*ATA[1][2]*ATA[2][3]*ATA[3][0] - ATA[0][2]*ATA[1][3]*ATA[2][1]*ATA[3][0] - ATA[0][3]*ATA[1][1]*ATA[2][2]*ATA[3][0]
                + ATA[0][3]*ATA[1][2]*ATA[2][1]*ATA[3][0] + ATA[0][2]*ATA[1][1]*ATA[2][3]*ATA[3][0] + ATA[0][1]*ATA[1][3]*ATA[2][2]*ATA[3][0];
        if (det_A == 0) {
            printf("matrix is singular\n");
            printf("print det_A %lf\n", det_A);
            return 1;
        } else if ( (sizeof(Qx)/sizeof(double)) != 4*4 ) {
            printf("matrix size is not correct\n");
            return 1;
        } else {
            printf("det = %lf\n",det_A);
            Qx[0][0] = (ATA[1][1]*ATA[2][2]*ATA[3][3] + ATA[1][2]*ATA[2][3]*ATA[3][1] + ATA[1][3]*ATA[2][1]*ATA[3][2]
                        - ATA[1][3]*ATA[2][2]*ATA[3][1] - ATA[1][2]*ATA[2][1]*ATA[3][3] - ATA[1][1]*ATA[2][3]*ATA[3][2]) / det_A;
            Qx[0][1] = ( - ATA[1][0]*ATA[2][2]*ATA[3][3] - ATA[1][2]*ATA[2][3]*ATA[3][0] - ATA[1][3]*ATA[2][0]*ATA[3][2]
                        + ATA[1][3]*ATA[2][2]*ATA[3][0] + ATA[1][2]*ATA[2][0]*ATA[3][3] + ATA[1][0]*ATA[2][3]*ATA[3][2]) / det_A;
            Qx[0][2] = (ATA[1][0]*ATA[2][1]*ATA[3][3] + ATA[1][1]*ATA[2][3]*ATA[3][0] + ATA[1][3]*ATA[2][0]*ATA[3][1]
                        - ATA[1][3]*ATA[2][1]*ATA[3][0] - ATA[1][1]*ATA[2][0]*ATA[3][3] - ATA[1][0]*ATA[2][3]*ATA[3][1]) / det_A;
            Qx[0][3] = ( - ATA[1][0]*ATA[2][1]*ATA[3][2] - ATA[1][1]*ATA[2][2]*ATA[3][0] - ATA[1][2]*ATA[2][0]*ATA[3][1]
                        + ATA[1][2]*ATA[2][1]*ATA[3][0] + ATA[1][1]*ATA[2][0]*ATA[3][2] + ATA[1][0]*ATA[2][2]*ATA[3][1]) / det_A;
            Qx[1][0] = ( - ATA[0][1]*ATA[2][2]*ATA[3][3] - ATA[0][2]*ATA[2][3]*ATA[3][1] - ATA[0][3]*ATA[2][1]*ATA[3][2]
                        + ATA[0][3]*ATA[2][2]*ATA[3][1] + ATA[0][2]*ATA[2][1]*ATA[3][3] + ATA[0][1]*ATA[2][3]*ATA[3][2]) / det_A;
            Qx[1][1] = (ATA[0][0]*ATA[2][2]*ATA[3][3] + ATA[0][2]*ATA[2][3]*ATA[3][0] + ATA[0][3]*ATA[2][0]*ATA[3][2]
                        - ATA[0][3]*ATA[2][2]*ATA[3][0] - ATA[0][2]*ATA[2][0]*ATA[3][3] - ATA[0][0]*ATA[2][3]*ATA[3][2]) / det_A;
            Qx[1][2] = ( - ATA[0][0]*ATA[2][1]*ATA[3][3] - ATA[0][1]*ATA[2][3]*ATA[3][0] - ATA[0][3]*ATA[2][0]*ATA[3][1]
                        + ATA[0][3]*ATA[2][1]*ATA[3][0] + ATA[0][1]*ATA[2][0]*ATA[3][3] + ATA[0][0]*ATA[2][3]*ATA[3][1]) / det_A;
            Qx[1][3] = (ATA[0][0]*ATA[2][1]*ATA[3][2] + ATA[0][1]*ATA[2][2]*ATA[3][0] + ATA[0][2]*ATA[2][0]*ATA[3][1]
                        - ATA[0][2]*ATA[2][1]*ATA[3][0] - ATA[0][1]*ATA[2][0]*ATA[3][2] - ATA[0][0]*ATA[2][2]*ATA[3][1]) / det_A;
            Qx[2][0] = (ATA[0][1]*ATA[1][2]*ATA[3][3] + ATA[0][2]*ATA[1][3]*ATA[3][1] + ATA[0][3]*ATA[1][1]*ATA[3][2]
                        - ATA[0][3]*ATA[1][2]*ATA[3][1] - ATA[0][2]*ATA[1][1]*ATA[3][3] - ATA[0][1]*ATA[1][3]*ATA[3][2]) / det_A;
            Qx[2][1] = ( - ATA[0][0]*ATA[1][2]*ATA[3][3] - ATA[0][2]*ATA[1][3]*ATA[3][0] - ATA[0][3]*ATA[1][0]*ATA[3][2]
                        + ATA[0][3]*ATA[1][2]*ATA[3][0] + ATA[0][2]*ATA[1][0]*ATA[3][3] + ATA[0][0]*ATA[1][3]*ATA[3][2]) / det_A;
            Qx[2][2] = (ATA[0][0]*ATA[1][1]*ATA[3][3] + ATA[0][1]*ATA[1][3]*ATA[3][0] + ATA[0][3]*ATA[1][0]*ATA[3][1]
                        - ATA[0][3]*ATA[1][1]*ATA[3][0] - ATA[0][1]*ATA[1][0]*ATA[3][3] - ATA[0][0]*ATA[1][3]*ATA[3][1]) / det_A;
            Qx[2][3] = ( - ATA[0][0]*ATA[1][1]*ATA[3][2] - ATA[0][1]*ATA[1][2]*ATA[3][0] - ATA[0][2]*ATA[1][0]*ATA[3][1]
                        + ATA[0][2]*ATA[1][1]*ATA[3][0] + ATA[0][1]*ATA[1][0]*ATA[3][2] + ATA[0][0]*ATA[1][2]*ATA[3][1]) / det_A;
            Qx[3][0] = ( - ATA[0][1]*ATA[1][2]*ATA[2][3] - ATA[0][2]*ATA[1][3]*ATA[2][1] - ATA[0][3]*ATA[1][1]*ATA[2][2]
                        + ATA[0][3]*ATA[1][2]*ATA[2][1] + ATA[0][2]*ATA[1][1]*ATA[2][3] + ATA[0][1]*ATA[1][3]*ATA[2][2]) / det_A;
            Qx[3][1] = (ATA[0][0]*ATA[1][2]*ATA[2][3] + ATA[0][2]*ATA[1][3]*ATA[2][0] + ATA[0][3]*ATA[1][0]*ATA[2][2]
                        - ATA[0][3]*ATA[1][2]*ATA[2][0] - ATA[0][2]*ATA[1][0]*ATA[2][3] - ATA[0][0]*ATA[1][3]*ATA[2][2]) / det_A;
            Qx[3][2] = ( - ATA[0][0]*ATA[1][1]*ATA[2][3] - ATA[0][1]*ATA[1][3]*ATA[2][0] - ATA[0][3]*ATA[1][0]*ATA[2][1]
                        + ATA[0][3]*ATA[1][1]*ATA[2][0] + ATA[0][1]*ATA[1][0]*ATA[2][3] + ATA[0][0]*ATA[1][3]*ATA[2][1]) / det_A;
            Qx[3][3] = (ATA[0][0]*ATA[1][1]*ATA[2][2] + ATA[0][1]*ATA[1][2]*ATA[2][0] + ATA[0][2]*ATA[1][0]*ATA[2][1]
                        - ATA[0][2]*ATA[1][1]*ATA[2][0] - ATA[0][1]*ATA[1][0]*ATA[2][2] - ATA[0][0]*ATA[1][2]*ATA[2][1]) / det_A;
        }

        //Multiplication 
        double X_vector[4],M[4][num];
        int stage_flag = 0;
        for (int row = 0; row < 4; row++) {
            for (int col= 0; col < num; col++) {
                M[row][col] = 0;
                for (int element = 0; element < 4; element++) {
                    M[row][col] += Qx[row][element] * transpose_A[element][col];
                }
            } 
        }
        stage_flag = 1;
        //printf("Size %lu\n", sizeof(M)/sizeof(double));
        //L
        L[0] = 1.558531368529651e+05;
        L[1] = 1.558544505611761e+05;
        L[2] = 1.558555158559211e+05;
        L[3] = 1.558538636308731e+05;
        L[4] = 1.558517675312629e+05;
        L[5] = 1.558537613040965e+05;
        L[6] = 1.558547983146661e+05;
        L[7] = 1.558515947888472e+05;
        L[8] = 1.558522977016945e+05;
        L[9] = 1.558568646658759e+05;
        L[10] = 1.558516207759794e+05;
        L[11] = 1.558532468589780e+05;
        /*
        */
        
        //2-stage Multiplication
        if(stage_flag) {
            for (int row = 0; row < 4; row++) {
                X_vector[row] = 0;
                for (int col= 0; col < num; col++) {
                        X_vector[row] += M[row][col] * L[col];
                }
                printf("\n");
            }
        }
        /*
        */
        //correct!!!
        printf("\n");
        for (int j = 0; j < 4; j++) {
            printf("X = %.9f\t", X_vector[j]);
        }
        printf("\n");
        //Update position
        xa += X_vector[0];
        ya += X_vector[1];
        za += X_vector[2];

        //converge condition
        double V[num];
        for (int row = 0; row < num; ++row) {
            V[row] = 0;
            for (int col= 0; col < 4; ++col) {
                    V[row] += A[row][col] * X_vector[col];
            }
            //printf("V = %lf\n", V[row]);
            //printf("L = %lf\n", L[row]);
            //printf("\n");
            V[row] -= L[row];

            //printf("L = %lf\n", L[row]);
            //printf("V = %lf\n", V[row]);
        }
       
        printf("\n");
        for (int j = 0; j < num; ++j) {
            printf("V = %lf\t", V[j]);
        }
        //

        //Multiplication
        for (int index = 0; index < num; ++index) {
            V_err += V[index] * V[index];
        }
        printf("V_err = %lf\n",V_err);
        //update tolerance
        Err = V_err - V_err_0;
        V_err_0 = V_err;
        printf("ERR = %lf\n", fabs(Err));
        Err = ERR_TOLLERANCE;
    }
    *RX_x = xa;
    *RX_y = ya;
    *RX_z = za;

    return 0;
}
