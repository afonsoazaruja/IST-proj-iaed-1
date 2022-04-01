/*
 * File: proj1.c
 * Author: Afonso Azaruja, 103624 - LEIC-A
 * Description: Airport flight management system
*/

#include <stdio.h> 
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "util.h"

int main() {    
    char input[MAX_STR];
    do {
        fgets(input, MAX_STR, stdin);
        commandSelec(input);
    } while (state != 'q');

    return 0;
}

/* adds new airport to airport_db */
void addAirport(char input[]) {
    retrieveID(input, 2);
    if (verifyAirport(id) == ERROR) {
        return;
    }
    strcpy(airport_db[n_air].id ,id); /* if input correct write to db */
    retrieveLocation(input); /* write location details to db */
    airport_db[n_air].n_flights = 0;
    printf("airport %s\n", airport_db[n_air].id);
    n_air++;
}

/* lists the airports id, country and city */
void listAirports(char input[]) {
    int i, j = 2, k, len = strlen(input);
    if (len == 2) { /* list all airports */ 
        bubbleSort(n_air-1);
        for (i = 0; i < n_air; i++) {
            printAirport(i);
        }
    }
    else { /* len/MAX_ID -> iterate for the number of IDs in input */
        for (i = 0; i < len/MAX_ID; i++) {
            retrieveID(input, j);
            k = findID(id);
            if (k == ERROR) { /* if id does not exist */
                printf("%s: %s", id, ERR_NO_ID);
            }
            else {
                printAirport(k);
            }
            j += MAX_ID;
        }
    }
}

/* add new flight to flight_db */
void addFlight(char input[]) {
    int i = 0, j = 2, k = 100, count = 0;
    char code[MAX_CODE], id_dep[MAX_ID], id_arr[MAX_ID];
    int day = 0, month = 0, year = 0; 
    int hour = 0, min = 0, hour_d = 0, min_d = 0, capacity = 0;
    
    if (input[1] == '\n') {
        while (i < n_flights) {    
            printFlight(i);
            i++;
        }
        return;
    }
    
    while(input[j] != ' ') {
        code[i] = input[j];
        j++; i++;
    }
    code[i] = '\0'; j++;

    j = retrieveID(input, j); strcpy(id_dep, id);
    j = retrieveID(input, j); strcpy(id_arr, id);
    retrieveTime(input, j, 10); j = res_aux.index; 
    day = res_aux.value;
    retrieveTime(input, j, 10); j = res_aux.index; 
    month = res_aux.value;
    retrieveTime(input, j, 1000); j = res_aux.index; 
    year = res_aux.value;
    retrieveTime(input, j, 10); j = res_aux.index; 
    hour = res_aux.value;
    retrieveTime(input, j, 10); j = res_aux.index; 
    min = res_aux.value;
    retrieveTime(input, j, 10); j = res_aux.index; 
    hour_d = res_aux.value;
    retrieveTime(input, j, 10); j = res_aux.index; 
    min_d = res_aux.value;
    
    while(input[j] != '\n') {
        count++;
        capacity += (input[j] - '0') * k; k/=10; j++;
    }
    if (count == 2) {
        capacity /= 10;
    }
    
    if (verifyFlight(code, id_dep, id_arr, day, month, year,
                         hour_d, min_d, capacity) == ERROR) {
        return;
    }
    
    strcpy(flight_db[n_flights].code, code);
    strcpy(flight_db[n_flights].id_dep, id_dep);
    strcpy(flight_db[n_flights].id_arr, id_arr);
    flight_db[n_flights].date_dep.d = day;
    flight_db[n_flights].date_dep.m = month;
    flight_db[n_flights].date_dep.y = year;
    flight_db[n_flights].time_dep.h = hour;
    flight_db[n_flights].time_dep.m = min;
    flight_db[n_flights].duration.h = hour_d;
    flight_db[n_flights].duration.m = min_d;
    flight_db[n_flights].min_dep = calcMinutes(day, month, year, hour, min);
    
    arrivalTime(n_flights);
    flight_db[n_flights].min_arr = calcMinutes(flight_db[n_flights].date_arr.d, 
                                            flight_db[n_flights].date_arr.m, 
                                            flight_db[n_flights].date_arr.y, 
                                            flight_db[n_flights].time_arr.h, 
                                            flight_db[n_flights].time_arr.m);

    airport_db[findID(id_dep)].n_flights++; 

    n_flights++;
}

/* list flights ordered by date and time of departure */
void listFlights(char input[]) {
    int i, count = 0;
    
    retrieveID(input, 2); /* id starts at index 2 in input string */

    for (i = 0; i < n_flights; i++) {
        if (state == 'p') {
            /* add integer of the flight_db to a vector */
            if (strcmp(flight_db[i].id_dep, id) == 0) {
                v_aux[count] = i;
                count++;
            }
        }
        if (state == 'c') {
            if (strcmp(flight_db[i].id_arr, id) == 0) {
                v_aux[count] = i;
                count++;
           }
        }
    }
    if (count == 0 && findID(id) == ERROR) {
            printf("%s: %s", id, ERR_NO_ID);
            return;
    }
    bubbleSort(count-1);

    for (i = 0; i < count; i++) {
        printFlight(v_aux[i]);  
    }
}

/* changes system date */
void changeDate(char input[]) {
    int i = 2, d = 0, m = 0, y = 0;
    int min_sys, min_time;
    retrieveTime(input, i, 10); i = res_aux.index;
    d = res_aux.value;
    retrieveTime(input, i, 10); i = res_aux.index;
    m = res_aux.value;
    retrieveTime(input, i, 1000); i = res_aux.index;
    y = res_aux.value;

    min_time = calcMinutes(d, m, y, 00, 00);
    min_sys = calcMinutes(sys_date.d, sys_date.m, sys_date.y, 00, 00);
    
    if (min_time < min_sys || (min_time - min_sys) > MIN_YEAR) {
        printf("%s", ERR_INV_DATE);
        return;
    }
    sys_date.d = d;
    sys_date.m = m;
    sys_date.y = y;
    printf("%02d-%02d-%d\n", sys_date.d, sys_date.m, sys_date.y);
}

/*------------------------------------------------------------------------------
                                AUX FUNCTIONS
------------------------------------------------------------------------------*/

/* selects the command from the user input */
void commandSelec(char input[]) {
    state = input[0];
    switch(state) {
        case 'q':
            break;
        case 'a':
            addAirport(input);
            break;
        case 'l':
            listAirports(input);
            break;
        case 'v':
            addFlight(input);
            break;
        case 'p':
            listFlights(input);
            break;
        case 'c':
            listFlights(input);
            break;
        case 't':
            changeDate(input);
            break;
    }
}

/* retrieves airport id from user input, returns index where ID ends in input */
int retrieveID(char input[], int i) {
    int j = 0;
    while (input[i] != ' ' && input[i] != '\n' && input[i] != '\t') {
        id[j] = input[i];
        i++; j++;
    }
    id[j] = '\0'; 
    i++; /* so index is not in whitespace */
    return i;
}

/* retrieves location details from user input */
void retrieveLocation(char input[]) {
    int j = 0, i = 6;
    while (input[i] == ' ' || input[i] == '\t') {
        i++;
    }
    while (input[i] != ' ' && input[i] != '\t') { /* FIND COUNTRY */
        airport_db[n_air].country[j] = input[i]; i++; j++;
    }
    airport_db[n_air].country[j] = '\0';
    
    while (input[i] == ' ' || input[i] == '\t') {
        i++;
    }
    j = 0;
    while (input[i] != '\n') { /* FIND CITY */
        airport_db[n_air].city[j] = input[i]; i++; j++;
    }
    airport_db[n_air].city[j] = '\0';
}

void retrieveTime(char input[], int i, int j) {
    int value = 0;
    while (isdigit(input[i])) {
        value += (input[i] - '0') * j; j /= 10; i++;
    }
    i++; /* so index is not in whitespace */
    res_aux.index = i;
    res_aux.value = value;
}

/* searchs airport id in db, if it exists return the index position in db */
int findID(char id[]) {
    int i;
    for (i = 0; i < n_air; i++) {
        if(strcmp(airport_db[i].id, id) == 0) {
            return i;
        }
    }
    return ERROR;
}

/* verifies if id is correct and if max airport is reached */
int verifyAirport(char id[]) {
    int i;
    for (i = 0; id[i] != '\0'; i++) {
        if (isupper(id[i]) == 0) {
            printf("%s", ERR_INV_ID);
            return ERROR; 
        }
    }
    if (n_air + 1 > MAX_AIRPORT) {
        printf("%s", ERR_TOO_MANY_AIR);
        return ERROR;
    }
    if (findID(id) != ERROR) { /* findID returns ERROR if no duplicate id */ 
        printf("%s", ERR_DUP_ID);
        return ERROR;
    }
    return SUCCESS;
}

/* verifies if flight code from user input is correct */
int verifyFlight(char code[], char id_dep[], char id_arr[], 
    int day, int month, int year, int hour_d, int min_d,
    int capacity) 
{   
    int i, count_dig = 0, min_flight, min_sys, diff_min;
    /* validates if flight code has 2 uppercase letters */
    for (i = 0; isdigit(code[i]) == 0; i++) {
        if (isupper(code[i]) == 0) {
            printf("%s", ERR_INV_COD);
            return ERROR;
        }
    }
    if (code[2] == '0') {
        printf("%s", ERR_INV_COD);
        return ERROR;
    }
    /* validates if flight code has between 1 and 4 digits */
    for (i = 2; code[2] != '\0' && code[i] != '\0' && count_dig < 5; i++) {
        if (isdigit(code[i]) == 0) {
            printf("%s", ERR_INV_COD);
            return ERROR;
        }
        count_dig++;
    }
    /* validates if there is no flight with same code in the same day */
    for(i = 0; i < n_flights; i++) {
        if (strcmp(flight_db[i].code, code) == 0 
        && (day == flight_db[i].date_dep.d
        && month == flight_db[i].date_dep.m 
        && year == flight_db[i].date_dep.y)) {
            printf("%s", ERR_DUP_FLIGHT);
            return ERROR;
        }
    }
    /* validates if airport id exists */
    if (findID(id_dep) == ERROR) {
        printf("%s: %s", id_dep, ERR_NO_ID);
        return ERROR;
    }
    if (findID(id_arr) == ERROR) {
        printf("%s: %s", id_arr, ERR_NO_ID);
        return ERROR;
    }
    /* validates max number of flights has been reached */
    if (n_flights + 1 > MAX_FLIGHTS) {
        printf("%s", ERR_TOO_MANY_FLIGHTS);
        return ERROR;
    }

    min_flight = calcMinutes(day, month, year, 00, 00);
    min_sys = calcMinutes(sys_date.d, sys_date.m, sys_date.y, 00, 00);
    diff_min = min_flight - min_sys;

    /* if date is in the past or 1 year into the future */
    if (diff_min < 0 || diff_min > MIN_YEAR || year < 2022) {
        printf("%s", ERR_INV_DATE);
        return ERROR;
    }
    /* if flight duration is greater than 12 hours */
    if (hour_d > 12 || (hour_d == 12 && min_d > 0)) {
        printf("%s", ERR_INV_DURATION);
        return ERROR;
    }
    /* if number of passenger is between 10 and 100, including */
    if (capacity < 10 || capacity > 100) {
        printf("%s", ERR_INV_CAP);
        return ERROR;
    }
    return SUCCESS;
}

/* print airport id, country, city and number of flights */
void printAirport(int i) {
    printf("%s %s %s %d\n",
        airport_db[i].id,
        airport_db[i].city,
        airport_db[i].country,
        airport_db[i].n_flights);
}

/* print flight details according to a specific command */
void printFlight(int i) {
    printf("%s ", flight_db[i].code);
    if (state == 'v') { 
        printf("%s %s %02d-%02d-%d %02d:%02d\n",
            flight_db[i].id_dep,
            flight_db[i].id_arr,
            flight_db[i].date_dep.d,
            flight_db[i].date_dep.m,
            flight_db[i].date_dep.y,
            flight_db[i].time_dep.h,
            flight_db[i].time_dep.m);
    }
    else if (state == 'p') {
        printf("%s %02d-%02d-%d %02d:%02d\n",
            flight_db[i].id_arr,
            flight_db[i].date_dep.d,
            flight_db[i].date_dep.m,
            flight_db[i].date_dep.y,
            flight_db[i].time_dep.h,
            flight_db[i].time_dep.m);
    }
    else { /* state == 'c' */
        printf("%s %02d-%02d-%d %02d:%02d\n",
            flight_db[i].id_dep,
            flight_db[i].date_arr.d,
            flight_db[i].date_arr.m,
            flight_db[i].date_arr.y,
            flight_db[i].time_arr.h,
            flight_db[i].time_arr.m);
    }
    
}

/* calculates minutes since 01-01-2022 00:00 to specific date and time after */
int calcMinutes(int day, int mon, int year, int h, int m) {
    int months[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int minutes = 0;
    int i;

    if (year > YEAR) {
        minutes += (year - YEAR) * (MIN_DAY * 365);
    }
    if (mon > MON) {
        for (i = 0; i < mon - 1; i++) {
            minutes += months[i]*MIN_DAY;
        }
    }

    if (day > DAY) {
        minutes += (day - DAY) * MIN_DAY;
    }

    if (h != 0 || m != 0) {
        minutes += m;
        minutes += h * 60;
    }
    return minutes;
}

/* calculates the arrival date and time of flight */
void arrivalTime(int i) {
    int days_of_mon[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int h_res = flight_db[i].time_dep.h + flight_db[i].duration.h; 
    int m_res = flight_db[i].time_dep.m + flight_db[i].duration.m;
    if (m_res >= 60) {
        h_res++;
        flight_db[i].time_arr.m = m_res - 60;
    }
    else {
        flight_db[i].time_arr.m = m_res;
    }
    if (h_res >= 24) {
        flight_db[i].date_arr.d = flight_db[i].date_dep.d + 1;
        flight_db[i].time_arr.h = h_res - 24;
        
        if (flight_db[i].date_arr.d > days_of_mon[flight_db[i].date_dep.m - 1]) {
            flight_db[i].date_arr.d = 1;
            flight_db[i].date_arr.m = flight_db[i].date_dep.m + 1;
        
            if (flight_db[i].date_arr.m == 13) {
                flight_db[i].date_arr.m = 1;
                flight_db[i].date_arr.y = flight_db[i].date_dep.y + 1;
            }
            else{
                flight_db[i].date_arr.y = flight_db[i].date_dep.y;
            }
        }
        else {
            flight_db[i].date_arr.m = flight_db[i].date_dep.m;
            flight_db[i].date_arr.y = flight_db[i].date_dep.y;
        }
    }
    else {
        flight_db[i].time_arr.h = h_res;
        flight_db[i].date_arr.d = flight_db[i].date_dep.d;
        flight_db[i].date_arr.m = flight_db[i].date_dep.m;
        flight_db[i].date_arr.y = flight_db[i].date_dep.y;
    }
}

/* sorting algorithm */
void bubbleSort(int right) {
    int i, j, done, aux;

    for (i = 0; i < right; i++) {
        done = 1;
        for (j = right; j > i; j--) {
            if (state == 'l') {
                if (strcmp(airport_db[j].id, airport_db[j-1].id) < 0) {
                    strcpy(aux_air.id, airport_db[j-1].id);
                    strcpy(aux_air.country, airport_db[j-1].country);
                    strcpy(aux_air.city, airport_db[j-1].city); 

                    strcpy(airport_db[j-1].id, airport_db[j].id);
                    strcpy(airport_db[j-1].country, airport_db[j].country);
                    strcpy(airport_db[j-1].city, airport_db[j].city);
                    
                    strcpy(airport_db[j].id, aux_air.id);
                    strcpy(airport_db[j].country, aux_air.country);
                    strcpy(airport_db[j].city, aux_air.city);
                    done = 0;
                }
            }
            else if (state == 'p') {
                if (flight_db[v_aux[j]].min_dep < flight_db[v_aux[j-1]].min_dep) {
                    aux = v_aux[j-1];
                    v_aux[j-1] = v_aux[j];
                    v_aux[j] = aux;
                    done = 0;
                }
            }
            else {
                if (flight_db[v_aux[j]].min_arr < flight_db[v_aux[j-1]].min_arr) {
                    aux = v_aux[j-1];
                    v_aux[j-1] = v_aux[j];
                    v_aux[j] = aux;
                    done = 0;
                }
            }
        }
        if (done) break;
    }
}
