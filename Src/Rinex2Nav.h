#pragma once

/*
* Rinex2Nav.h
* Read and organize Rinex v2 navigation/ephemeris file 
* Created on: Jun 12, 2018
* Updated on: Jan 21, 2018
*      Author: Aaron Boda
*/

#ifndef RINEX2NAV_H_
#define RINEX2NAV_H_

// Data Structures
struct NavHeaderGPS {
    // Ionospheric alpha and beta constants
    // Use arrays instead of vectors
    double ialpha[4];
    double ibeta[4];
    // Time System correction
    double dUTC[4];
    // Leap Seconds
    int leap;
};

// NOTE: Naming convention based on RINEX Manual
// i.e. Switching between upper/lowercase is deliberate

struct DataGPS {
    int isAvailable;
    int PRN;
    double epochInfo[3];
    double gpsTime;
    double clockBias;
    double clockDrift;
    double clockDriftRate;
    double IODE;
    double Crs;
    double Delta_n;
    double Mo;
    double Cuc;
    double Eccentricity;
    double Cus;
    double Sqrt_a;
    double TOE;
    double Cic;
    double OMEGA;
    double CIS;
    double Io;
    double Crc;
    double Omega;
    double Omega_dot;
    double IDOT;
    double L2_codes_channel;
    double GPS_week;
    double L2_P_data_flag;
    double svAccuracy;
    double svHealth;
    double TGD;
    double IODC;
    double transmission_time;
    double fit_interval;
};

// Function prototypes
void readNav(struct NavHeaderGPS* header, struct DataGPS* navDataGPS, FILE* inputNavfileGPS);
int EpochMatcher(double obsTime, struct DataGPS* NAV);

#endif /* RINEX2NAV_H_ */
