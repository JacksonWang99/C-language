//
// Created by Rui.Mu on 2019-05-22.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "list.h"

/**
 * create a new iterator of int;
 * @return the collections
 */
TimeTables createTimeTables() {

    // request memories
    TimeTables it = malloc(sizeof(struct TimeTablesRep));
    if (it != NULL) {
        // set all the elements to null;
        it->header = NULL;
        it->tail = NULL;
        it->cursor = NULL;
        it->length = 0;
    }
    return it;
}

/**
 * reset the iterator list
 * @param it list
 */
void resetTimeTable(TimeTables it) {

    // check collections is null
    if (it != NULL) {
        // reset all the value to default
        it->cursor = it->header;
    }

}

void deleteTimeTable(TimeTables it) {

    if (it != NULL) {
        if (it->tail != NULL) {

            if (it->tail->previous == NULL) {

                free(it->tail);
                it->header = NULL;
                it->tail = NULL;
                it->cursor = NULL;
            } else {
                StationSchedule previous = it->tail->previous;
                previous->next = NULL;
                free(it->tail);
                it->tail = previous;
            }

            it->length--;


        }
    }

}

/**
 * create aa new node
 * @param data data
 * @return return new node point
 */
StationSchedule createStationSchedule(int lineNumber, char *time, int stationIndex, char *stationName) {
    // define new node
    StationSchedule newNode = malloc(sizeof(struct station));
    // no memories
    if (newNode != NULL) {
        // request the address for the data
        newNode->time = malloc((strlen(time) + 1) * sizeof(char));
        strcpy(newNode->time, time);
        *(newNode->time + strlen(time)) = '\0';
        newNode->lineNumber = lineNumber;
        newNode->stationIndex = stationIndex;
        newNode->stationName = malloc((strlen(stationName) + 1) * sizeof(char));
        strcpy(newNode->stationName, stationName);
        *(newNode->stationName + strlen(stationName)) = '\0';
        // set all the value to default
        newNode->next = NULL;
        newNode->previous = NULL;
    }
    // return new node
    return newNode;

}


/**
 * add a new element
 * @param it list
 * @param v value
 * @return 1:is true 0 is false
 */
int addTimeTable(TimeTables it, int lineNumber, char *time, int stationIndex, char *stationName) {

    // result
    int result = (it != NULL);

    if (result) {
        // create new node
        StationSchedule newNode = createStationSchedule(lineNumber, time, stationIndex, stationName);
        // check new node and it whether NULL
        result = (newNode != NULL);
        if (result) {
            if (it->header == NULL) {
                it->tail = it->header = newNode;
            } else {

                newNode->previous = it->tail;

                it->tail->next = newNode;

                it->tail = it->tail->next;
            }
            // add length
            it->length++;
            // reset cursor
            it->cursor = it->header;
        }
    }

    // return value
    return result;
}


/**
 * check has the next value
 * @param it list
 * @return 1 has next 0 no next
 */
int hasNextTimeTable(TimeTables it) {
    int result = 0;
    if (it != NULL) {
        // next value
        result = (it->cursor != NULL);
    }
    // return directly
    return result;
}

/**
 * get next element
 * @param it
 * @return return the next data
 */
char *nextTimeTable(TimeTables it) {
    // define result
    char *result = NULL;
    if (hasNextTimeTable(it)) {
        result = it->cursor->time;
        // adjust the position and values
        it->cursor = it->cursor->next;
    }

    return result;
}

/**
 * free memories
 * @param it
 */
void freeTimeTables(TimeTables it) {
    //  check value
    if (it != NULL) {
        // define variables
        StationSchedule temp = NULL, n = it->header;
        // check and free
        while (n != NULL) {
            temp = n;
            n = n->next;
            free(temp->time);
            free(temp->stationName);
            free(temp);
        }
        // set all to NULL;
        it->header = NULL;
        it->tail = NULL;
        it->cursor = NULL;

        free(it);
    }
}


void printTimeTables(TimeTables it) {
    if (it != NULL) {
        printf("Print timeTables\n");
        printf("Total timetables is : %d \n",it->length);
        StationSchedule temp = it->header;
        int index = 0;
        while (temp != NULL) {
            printf("index: %d: line number: %d, station name: %s, time: %s===\n", index,temp->lineNumber,
                    temp->stationName,
                    temp->time);
            temp = temp->next;
            index++;
        }
    }
}

int lengthTimeTables(TimeTables it) {
    if (it != NULL) {
        return it->length;
    }

    return 0;
}
