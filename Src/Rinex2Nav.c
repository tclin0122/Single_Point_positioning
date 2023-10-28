#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Define data structures for C
struct NavHeaderGPS {
    double ialpha[4];
    double ibeta[4];
    double dUTC[4];
    int leap;
};

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

// Function to split and organize navigation parameters
void rinex2NavDataSplitter(char* line, double* data, int max_data_count) {
    int i = 0;
    char* token = strtok(line, " ");
    while (token != NULL && i < max_data_count) {
        if (strstr(token, "D") != NULL) {
            data[i] = atof(replaceChars(token, 'D', 'e'));
        } else {
            data[i] = atof(token);
        }
        token = strtok(NULL, " ");
        i++;
    }
}

int EpochMatcher(double obsTime, struct DataGPS* NAV, int NAV_size) {
    double diff = 1000000;
    int index = 0;
    for (int i = 0; i < NAV_size; i++) {
        double new_diff = fabs(obsTime - NAV[i].gpsTime);
        if (new_diff < diff) {
            diff = new_diff;
            index = i;
        }
    }
    return index;
}

DataGPS epochNavOrganizer(char** block, int block_size) {
    int prn = atoi(block[0]);
    double epochInfo[3];
    char line[256];
    line[0] = '\0';
    double parameters[29];
    int parameters_size = 0;

    for (int i = 1; i < block_size; i++) {
        strcat(line, block[i]);
    }

    rinex2NavDataSplitter(line + 3, parameters, 29);

    DataGPS GPS;
    GPS.isAvailable = 1;
    GPS.PRN = prn;
    memcpy(GPS.epochInfo, epochInfo, 3 * sizeof(double));
    GPS.gpsTime = gpsTime(epochInfo);
    GPS.clockBias = parameters[0];
    GPS.clockDrift = parameters[1];
    GPS.clockDriftRate = parameters[2];
    GPS.IODE = parameters[3];
    GPS.Crs = parameters[4];
    GPS.Delta_n = parameters[5];
    GPS.Mo = parameters[6];
    GPS.Cuc = parameters[7];
    GPS.Eccentricity = parameters[8];
    GPS.Cus = parameters[9];
    GPS.Sqrt_a = parameters[10];
    GPS.TOE = parameters[11];
    GPS.Cic = parameters[12];
    GPS.OMEGA = parameters[13];
    GPS.CIS = parameters[14];
    GPS.Io = parameters[15];
    GPS.Crc = parameters[16];
    GPS.Omega = parameters[17];
    GPS.Omega_dot = parameters[18];
    GPS.IDOT = parameters[19];
    GPS.L2_codes_channel = parameters[20];
    GPS.GPS_week = parameters[21];
    GPS.L2_P_data_flag = parameters[22];
    GPS.svAccuracy = parameters[23];
    GPS.svHealth = parameters[24];
    GPS.TGD = parameters[25];
    GPS.IODC = parameters[26];
    if (parameters_size > 27) {
        GPS.transmission_time = parameters[27];
    }
    if (parameters_size > 28) {
        GPS.fit_interval = parameters[28];
    }
    return GPS;
}

void readNav(FILE* infile, NavHeaderGPS* header, DataGPS* navDataGPS, int* navDataGPS_size) {
    const char* sTokenIALPHA = "ION ALPHA";
    const char* sTokenIBETA = "ION BETA";
    const char*
