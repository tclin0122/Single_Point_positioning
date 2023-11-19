# include <stdio.h>
#include "Rinex_reader.h"

int main() {
    struct DataGPS navData[PRN_MAX];
    // Replace with the path to your RINEX navigation file
    const char* filePath = "Data/0lov033b.04n";

    printf("Start SPP");
    
    //Read RINEX file
    rinex_reader(filePath, navData);
    //Use RINEX file to do SPP

    return 0;
}
