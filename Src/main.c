# include <stdio.h>
#include "Rinex2Nav.h"
#include "Rinex2Obs.h"

int main() {
    struct DataGPS navData[PRN_MAX];
    // Replace with the path to your RINEX navigation file
    const char* NavPath = "Data/0lov033b.04n";
    const char* ObsPath = "Data/0lov033b.04o";
    int err;

    printf("Start SPP");

    //Read Navigation
    err = rinex2nav(NavPath, navData);
    if(err != 0){
        printf("Nav data read error");
    }
    //Read Observation
    /*
    err = rinex2obs(filePath, navData);
    if(err != NULL){
        printf("Obs data read error");
    }
    */
    //Use RINEX file to do SPP

    return 0;
}
