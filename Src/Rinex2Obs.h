#ifndef RINEX2OBS_H_
#define RINEX2OBS_H_

#define MAX_LINE_LENGTH 80
#define PRN_MAX 32

#include <stdio.h>
#include <string.h>
#include <stdlib.h>  // Include the stdlib.h header for atoi

struct ObsHeaderInfo {
    	double rinexType;
		double approxPosX;
        double approxPosY;
        double approxPosZ;
		// double antDeltaHEN;
		// double firstObsTime;
		// double lastObsTime;
		// char obsTypes;
};

struct ObsSat {
    	double PRN_list;
};

struct ObsData {
    	double C1;
		double L1;
		double L2;
		double P1;
		double P2;
        double D1;
        double D2;
};



int rinex2obs(const char* filePath, struct ObsData *obsData, struct ObsHeaderInfo *obsHead);


#endif /* RINEX2OBS_H_ */