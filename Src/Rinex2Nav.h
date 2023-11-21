#ifndef RINEX2NAV_H_
#define RINEX2NAV_H_

#define MAX_LINE_LENGTH 80
#define PRN_MAX 32

#include <stdio.h>
#include <string.h>
#include <stdlib.h>  // Include the stdlib.h header for atoi

// Data Structures
struct NavHeaderGPS {
    // Ionospheric alpha and beta constants
    double ialpha;
    double ibeta;
    // Time System correction
    double dUTC;
    // Leap Seconds
    int leap;
};
struct DataGPS {
    //bool isAvailable;
    int PRN;
    double epochInfo; // Toc Time of clock 
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
    double Cis;
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

int rinex2nav(const char* filePath, struct DataGPS *navData);

#endif /* RINEX2NAV_H_ */