//
// Created by Rui.Mu on 2019-05-22.
//

#include <stdio.h>

typedef struct TimeTablesRep *TimeTables;

// the definition of element linked list
struct station {
    // the next node
    struct station *next;
    struct station *previous;
    // the address keep the value
    // line number
    int lineNumber;
    // time
    char *time;
    // station index
    int stationIndex;
    // station name
    char *stationName;
};

typedef struct station *StationSchedule;

// the structure of list
struct TimeTablesRep {
    // header reset
    StationSchedule header;
    StationSchedule tail;
    // the current cursor value
    StationSchedule cursor;
    // TimeTables Length
    int length;
};

TimeTables createTimeTables();

/**
 * add time tables
 * @param it TimeTables
 * @param data
 * @param lineNumber
 * @return
 */
int addTimeTable(TimeTables it, int lineNumber, char *time,int stationIndex,char *stationName);

int hasNextTimeTable(TimeTables it);

char *nextTimeTable(TimeTables it);

void resetTimeTable(TimeTables it);

void deleteTimeTable(TimeTables it);

void freeTimeTables(TimeTables it);

void printTimeTables(TimeTables it);

int lengthTimeTables(TimeTables it);

