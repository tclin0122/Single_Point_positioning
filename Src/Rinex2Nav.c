#include <stdio.h>
#include <stdlib.h>
#include "Rinex2Nav.h"

void replaceDWithE(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        if (str[i] == 'D' || str[i] == 'd') {
            str[i] = 'e';
        }
    }
}

int rinex2nav(const char* filePath, struct DataGPS *navData) {
    FILE* file;
    char line[MAX_LINE_LENGTH];

    int numSats = 0;
    int cnt = 1;

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
        int data11;
        // Extract data as needed
        replaceDWithE(line);
        // Map the data line by line
        switch (cnt)
        {
        case 1:
            if (sscanf(line, "%d%lf%lf%lf%lf%lf%lf%lf%lf%lf", &data11, &data2, &data3, &data4, &data5, &data6, &data7, &data8, &data9, &data10) == 10) {
            // Process the extracted data
            printf("print line = %d\n",cnt);
            printf("print line = %s\n",line);
            // Assign value
            navData[data11].PRN = data11;
            navData[data11].clockBias = data8;
            navData[data11].clockDrift = data9;
            navData[data11].clockDriftRate = data10;
            printf("Data: %d %.19lf %.19lf %.19lf\n", navData[data11].PRN, navData[data11].clockBias, navData[data11].clockDrift, navData[data11].clockDriftRate);
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
            navData[data11].IODE = data1;
            navData[data11].Crs = data2;
            navData[data11].Delta_n = data3;
            navData[data11].Mo = data4;
            printf("Data: %.12lf %.12lf %.12lf %.12lf\n", navData[data11].IODE, navData[data11].Crs, navData[data11].Delta_n, navData[data11].Mo);
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
            navData[data11].Cuc = data1;
            navData[data11].Eccentricity = data2;
            navData[data11].Cus = data3;
            navData[data11].Sqrt_a = data4;
            printf("Data: %.12lf %.12lf %.12lf %.12lf\n", navData[data11].Cuc, navData[data11].Eccentricity, navData[data11].Cus, navData[data11].Sqrt_a);
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
            navData[data11].TOE = data1;
            navData[data11].Cic = data2;
            navData[data11].OMEGA = data3;
            navData[data11].CIS = data4;
            printf("Data: %.12lf %.12lf %.12lf %.12lf\n", navData[data11].TOE, navData[data11].Cic, navData[data11].OMEGA, navData[data11].CIS);
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
            navData[data11].Io = data1;
            navData[data11].Crc = data2;
            navData[data11].Omega = data3;
            navData[data11].Omega_dot = data4;
            printf("Data: %.12lf %.12lf %.12lf %.12lf\n", navData[data11].Io, navData[data11].Crc, navData[data11].Omega, navData[data11].Omega_dot);
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
            navData[data11].IDOT = data1;
            navData[data11].L2_codes_channel = data2;
            navData[data11].GPS_week = data3;
            navData[data11].L2_P_data_flag = data4;
            printf("Data: %.12lf %.12lf %.12lf %.12lf\n", navData[data11].IDOT, navData[data11].L2_codes_channel, navData[data11].GPS_week, navData[data11].L2_P_data_flag);
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
            navData[data11].svAccuracy = data1;
            navData[data11].svHealth = data2;
            navData[data11].TGD = data3;
            navData[data11].IODC = data4;
            printf("Data: %.12lf %.12lf %.12lf %.12lf\n", navData[data11].svAccuracy,  navData[data11].svHealth, navData[data11].TGD, navData[data11].IODC);
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
            navData[data11].transmission_time = data1;
            printf("Data: %.12lf\n", navData[data11].transmission_time);
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
