#include <stdio.h>
#include <stdlib.h>
#include "Rinex_reader.h"

void replaceDWithE(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        if (str[i] == 'D' || str[i] == 'd') {
            str[i] = 'e';
        }
    }
}

int main() {
    FILE* file;
    char line[MAX_LINE_LENGTH];
    struct DataGPS navData;
    int numSats = 0;
    double rinex_version;
    int cnt=1;

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
        double data1, data2, data3, data4;
        double data5, data6, data7, data8, data9, data10;
        // Extract data as needed
        replaceDWithE(line);
        // Map the data line by line
        switch (cnt)
        {
        case 1:
            if (sscanf(line, "%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf", &data1, &data2, &data3, &data4, &data5, &data6, &data7, &data8, &data9, &data10) == 10) {
            // Process the extracted data
            printf("print line = %d\n",cnt);
            printf("print line = %s\n",line);
            // Assign value
            navData.PRN = data1;
            navData.clockBias = data8;
            navData.clockDrift = data9;
            navData.clockDriftRate = data10;
            printf("Data: %d %.19lf %.19lf %.19lf\n", navData.PRN, navData.clockBias, navData.clockDrift, navData.clockDriftRate);
            cnt++;
            }
            /*
            } else {
                // Handle the case where the line does not match the expected format
                printf("Error reading data from line: %s", line);
            }
            */
            break;
        case 2:
            if (sscanf(line, "%lf%lf%lf%lf", &data1, &data2, &data3, &data4) == 4) {
            // Process the extracted data
            printf("print line = %d\n",cnt);
            printf("print line = %s\n",line);
            // Assign value
            navData.IODE = data1;
            navData.Crs = data2;
            navData.Delta_n = data3;
            navData.Mo = data4;
            printf("Data: %.12lf %.12lf %.12lf %.12lf\n", navData.IODE, navData.Crs, navData.Delta_n, navData.Mo);
            cnt++;
            }
            /*
            } else {
                // Handle the case where the line does not match the expected format
                printf("Error reading data from line: %s", line);
            }
            */
            break;
        case 3:
            if (sscanf(line, "%lf%lf%lf%lf", &data1, &data2, &data3, &data4) == 4) {
            // Process the extracted data
            printf("print line = %d\n",cnt);
            printf("print line = %s\n",line);
            // Assign value
            navData.Cuc = data1;
            navData.Eccentricity = data2;
            navData.Cus = data3;
            navData.Sqrt_a = data4;
            printf("Data: %.12lf %.12lf %.12lf %.12lf\n", navData.Cuc, navData.Eccentricity, navData.Cus, navData.Sqrt_a);
            cnt++;
            }
            /*
            } else {
                // Handle the case where the line does not match the expected format
                printf("Error reading data from line: %s", line);
            }
            */
            break;
        case 4:
            if (sscanf(line, "%lf%lf%lf%lf", &data1, &data2, &data3, &data4) == 4) {
            // Process the extracted data
            printf("print line = %d\n",cnt);
            printf("print line = %s\n",line);
            // Assign value
            navData.TOE = data1;
            navData.Cic = data2;
            navData.OMEGA = data3;
            navData.CIS = data4;
            printf("Data: %.12lf %.12lf %.12lf %.12lf\n", navData.TOE, navData.Cic, navData.OMEGA, navData.CIS);
            cnt++;
            }
            /*
            } else {
                // Handle the case where the line does not match the expected format
                printf("Error reading data from line: %s", line);
            }
            */
            break;
        case 5:
            if (sscanf(line, "%lf%lf%lf%lf", &data1, &data2, &data3, &data4) == 4) {
            // Process the extracted data
            printf("print line = %d\n",cnt);
            printf("print line = %s\n",line);
            // Assign value
            navData.Io = data1;
            navData.Crc = data2;
            navData.Omega = data3;
            navData.Omega_dot = data4;
            printf("Data: %.12lf %.12lf %.12lf %.12lf\n", navData.Io, navData.Crc, navData.Omega, navData.Omega_dot);
            cnt++;
            }
            /*
            } else {
                // Handle the case where the line does not match the expected format
                printf("Error reading data from line: %s", line);
            }
            */
            break;
        case 6:
            if (sscanf(line, "%lf%lf%lf%lf", &data1, &data2, &data3, &data4) == 4) {
            // Process the extracted data
            printf("print line = %d\n",cnt);
            printf("print line = %s\n",line);
            // Assign value
            navData.IDOT = data1;
            navData.L2_codes_channel = data2;
            navData.GPS_week = data3;
            navData.L2_P_data_flag = data4;
            printf("Data: %.12lf %.12lf %.12lf %.12lf\n", navData.IDOT, navData.L2_codes_channel, navData.GPS_week, navData.L2_P_data_flag);
            cnt++;
            }
            /*
            } else {
                // Handle the case where the line does not match the expected format
                printf("Error reading data from line: %s", line);
            }
            */
            break;
        case 7:
            if (sscanf(line, "%lf%lf%lf%lf", &data1, &data2, &data3, &data4) == 4) {
            // Process the extracted data
            printf("print line = %d\n",cnt);
            printf("print line = %s\n",line);
            // Assign value
            navData.svAccuracy = data1;
            navData.svHealth = data2;
            navData.TGD = data3;
            navData.IODC = data4;
            printf("Data: %.12lf %.12lf %.12lf %.12lf\n", navData.svAccuracy,  navData.svHealth, navData.TGD, navData.IODC);
            cnt++;
            }
            /*
            } else {
                // Handle the case where the line does not match the expected format
                printf("Error reading data from line: %s", line);
            }
            */
            break;

        case 8:
            if (sscanf(line, "%lf", &data1) == 1) {
            // Process the extracted data
            printf("print line = %d\n",cnt);
            printf("print line = %s\n",line);
            // Assign value
            navData.transmission_time = data1;
            printf("Data: %.12lf\n", navData.transmission_time);
            cnt = 1;
            }
            /*
            } else {
                // Handle the case where the line does not match the expected format
                printf("Error reading data from line: %s", line);
            }
            */
            break;
        default:
            break;
        }

    }
    // Close the file
    fclose(file);
    return 0;
}


