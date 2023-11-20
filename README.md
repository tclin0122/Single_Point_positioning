# Single Point positioning
## Intro
This is the algorithm for GNSS single point positioning, it is implemented in C. The file system is structured as 

```bash
├── src
│   ├── main.c
│   ├── Rinex2Nav.c
│   ├── Rinex2Nav.h
│   ├── Rinex2Obs.c
│   ├── Rinex2Obs.h
│   ├── SPP.c
│   └── SPPs.h
├── Data
│   ├── .n file
│   └── .o file
├── Doc
│   └── SPP algorithm.pdf
├── Makefile
└── README.md
```


The data folder includes navigation message, the most important files are the observation file and the navigation file.
The document is from Professor Milan Horemuz at KTH, it describes the detail of how to read and implement the SPP algorithm.
The data is in RINEX_2 format.

## Run the file
Use Makefile to build the program
```
    $ make
```
Run the program
```
    $ ./Main
```
