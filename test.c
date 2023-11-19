#include <stdio.h>
#include <stdlib.h>
#include "Rinex_reader.h"


int main() {
    FILE* file;
    char line[MAX_LINE_LENGTH];
    struct DataGPS navData;
    int numSats = 0;
    double rinex_version;
    int cnt=0;

    // Replace with the path to your RINEX navigation file
    const char* filePath = "Data/0lov033b.04n";

    // Read file
    file = fopen(filePath, "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return 1;
    }

    // Skip header lines until "END OF HEADER" is encountered
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strstr(line, "END OF HEADER") != NULL) {
            break;
        }
    }

    // Read data lines
    while (fgets(line, sizeof(line), file) != NULL) {
        // Extract data as needed
        double data1, data2, data3, data4, data5, data6, data7, data8, data9, data10;
        if(cnt == 0){
            sscanf(line, "%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf", &data1, &data2, &data3, &data4, &data5, &data6, &data7, &data8, &data9, &data10);
            printf("Data: %lf %lf %lf\n", data8, data9, data10);
            cnt++;
        }
        else if (cnt >= 0 && cnt <=7) {
            // Process the extracted data
            sscanf(line, "%lf%lf%lf%lf", &data1, &data2, &data3, &data4);
            printf("print line = %d",cnt);
            printf("Data: %lf %lf %lf %lf\n", data1, data2, data3, data4);
            cnt++;
        } else if ( cnt>7 ) {
            sscanf(line, "%lf", &data1);
            cnt=0;
        } else {
            // Handle the case where the line does not match the expected format
            printf("Error reading data from line: %s", line);
        }
    }

    fclose(file);
    return 0;
}
