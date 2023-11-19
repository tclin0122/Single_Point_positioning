# include <stdio.h>
#include "Rinex2Nav.h"

int main() {
    struct DataGPS navData[PRN_MAX];
    // Replace with the path to your RINEX navigation file
    const char* filePath = "Data/0lov033b.04n";
    int err;

    printf("Start SPP");

    //Read Navigation
    err = rinex2nav(filePath, navData);
    if(err != NULL){
        printf("Nav data read error");
    }
    //Read Observation
    /*
    err = rinex2nav(filePath, navData);
    if(err != NULL){
        printf("Nav data read error");
    }
    */
    //Use RINEX file to do SPP

    return 0;
}
