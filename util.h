/*
 * File: util.h
 * Author: Afonso Azaruja, 103624 - LEIC-A
 * Description: Macros, structs, prototypes and global variables for proj.c
*/

/*------------------------------------------------------------------------------
                                    MACROS
------------------------------------------------------------------------------*/

/* max size of airport id */
#define MAX_ID 4
/* max size of countries */
#define MAX_COUNTRY 31
/* max size of cities */     
#define MAX_CITY 51
/* max size of string */
#define MAX_STR 1000
/* max size of flight code */
#define MAX_CODE 8

/* max number of airports */
#define MAX_AIRPORT 40
/* max number of flights */
#define MAX_FLIGHTS 30000
/* min number of passengers */
#define MIN_PASS 10
/* max number of passengers */
#define MAX_PASS 100

/* for calculating minutes - calcMinutes function */
#define YEAR 2022
#define MON 1
#define DAY 1
#define MIN_DAY 1440
#define MIN_YEAR 525600

/* for validating input */
#define ERROR -1
#define SUCCESS 1

/* airport errors */
#define ERR_INV_ID "invalid airport ID\n"
#define ERR_TOO_MANY_AIR "too many airports\n"
#define ERR_DUP_ID "duplicate airport\n"
#define ERR_NO_ID "no such airport ID\n"

/* flight errors */
#define ERR_INV_COD "invalid flight code\n"
#define ERR_DUP_FLIGHT "flight already exists\n"
#define ERR_TOO_MANY_FLIGHTS "too many flights\n"
#define ERR_INV_DATE "invalid date\n" /* system date error aswell */
#define ERR_INV_DURATION "invalid duration\n"
#define ERR_INV_CAP "invalid capacity\n"


/*------------------------------------------------------------------------------
                                    STRUCTS
------------------------------------------------------------------------------*/

typedef struct airports {
    char id[MAX_ID];
    char country[MAX_COUNTRY];
    char city[MAX_CITY];
    int n_flights;
}Airports;

typedef struct date {
    int d;
    int m;
    int y;
}Date;

typedef struct time {
    int h;
    int m;
}Time;

typedef struct flights {
    char code[MAX_CODE];
    char id_dep[MAX_ID];
    char id_arr[MAX_ID];
    Date date_dep;
    Date date_arr;
    Time time_dep;
    Time time_arr;
    Time duration;
    int max_pass;
    int min_dep;
    int min_arr;
}Flights; 

typedef struct result {
    int index;
    int value;
}Result;

typedef int Item;

/*------------------------------------------------------------------------------
                                    PROTOTYPES
------------------------------------------------------------------------------*/

/*MAIN FUNCTIONS*/
void addAirport(char input[]);
void listAirports(char input[]);
void addFlight(char input[]);
void listFlights(char input[]);
void changeDate(char input[]);

/*AUXILIRY FUNCTIONS*/
void commandSelec(char input[]);
int retrieveID(char input[], int i);
void retrieveLocation(char input[]);
void retrieveTime(char input[], int i, int j);
int findID(char id[]);
int verifyAirport(char id[]);
int verifyFlight(char code[], char id_dep[], char id_arr[], int day, int month, 
                int year, int hour_d, int min_d, int capacity);
void printAirport(int i);
void printFlight(int i); 
int calcMinutes(int day, int mon, int year, int h, int m);
void arrivalTime(int i);
void bubbleSort(int right);

/*------------------------------------------------------------------------------
                                GLOBAL VARIABLES
------------------------------------------------------------------------------*/

int state = ' '; /* which command is active */

char id[MAX_ID]; /*  */
Airports aux_air;
int v_aux[MAX_FLIGHTS];
Result res_aux;

/* initialization of the number of airports and flights */
int n_air = 0;
int n_flights = 0;

/* initialization of the airports and flights databases and system date */
Airports airport_db[MAX_AIRPORT];
Flights flight_db[MAX_FLIGHTS];
Date sys_date = {1,1,2022};
