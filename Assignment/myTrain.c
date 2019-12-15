//
// Created by Rui Mu on 5/11/19.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "WGraph.h"
#include "stack.h"

#define STATION_NAME_SIZE 32
#define INFINITY 9999
#define DONE "done"
#define TIME_LENGTH 5

/**
 * read stations for numbers
 * @param list save to list
 * @param numberOfStations number of stations
 */
// void readStationsToList(LinkedList list, int numberOfStations) {
//    char stationNames[STATION_NAME_SIZE];
//    // for each and get station name
//    for (int i = 0; i < numberOfStations; i++) {
// get station name
//        scanf("%s\n", stationNames);
// save to list
//        add(list, stationNames);
//    }
// }

void readStationsToArray(char **stationNames, int numberOfStations) {
    char stationName[STATION_NAME_SIZE];
    // for each and get station name
    for (int i = 0; i < numberOfStations; i++) {
        // get station name
        scanf("%s", stationName);
        stationNames[i] = malloc(STATION_NAME_SIZE * sizeof(char));
        strcpy(stationNames[i], stationName);
        stationNames[i][strlen(stationName)] = '\0';
    }
}


/**
 * read the number of lines and line tables
 * @param stationNames
 * @param numberOfStations
 */
void
readTrainTimeTables(char **stationNames, TimeTables lineTimeTables, int numberOfStations, int *numberOfTrains) {

    int numberOfStops;
    char stationName[STATION_NAME_SIZE];
    char arriveTime[TIME_LENGTH];

    printf("Enter the number of trains: ");
    scanf("%d", numberOfTrains);

    // input more than 0 trains
    if (*numberOfTrains > 0) {
        // create time tables
        // TimeTables *lineTimeTables = malloc(*numberOfTrains * sizeof(TimeTables));
        // check trains
        for (int i = 0; i < *numberOfTrains; i++) {
            printf("Enter the number of stops: ");
            scanf("%d", &numberOfStops);
            // create line time table
            // TimeTables lineTimeTable = createTimeTables();
            // get all the time tables
            for (int j = 0; j < numberOfStops; j++) {
                // get station name
                scanf("%s", arriveTime);
                scanf("%s", stationName);
                // printf("arriveTime: %s,stationName :%s,numberOfStops:%d\n", arriveTime, stationName, numberOfStops);
                for (int k = 0; k < numberOfStations; k++) {
                    // get the target station name
                    if (strcmp(stationName, stationNames[k]) == 0) {
                        addTimeTable(lineTimeTables, i, arriveTime, k, stationName);
                        break;
                    }
                }

            }


            // get all the lines
            // lineTimeTables[i] = lineTimeTable;
        }
    }

}

/**
 *
 * @param trainGraph
 * @param from
 * @param to
 * @return
 */
int checkConnected(Graph graph, int from, int to) {

    int result = 0;
    if (adjacent(graph, from, to) == 0) {
        // 该车辆都没有进入的车辆
        if (degreeOfIn(graph, from) == 0 && degreeOfIn(graph, to) == 0) {
            return result;
        }
        // 该车站都没有出去的车辆
        if (degreeOfOut(graph, from) == 0 && degreeOfOut(graph, to) == 0) {
            return result;
        }

        return 1;
    }
    return result;
}

/**
 * construct the graph from line time tables
 * @param trainGraph
 * @param lineTimeTables
 */
void createTrainsGraph(Graph trainGraph, TimeTables lineTimeTables, int arrivedByTime) {

    StationSchedule first = lineTimeTables->header;
    // set hour
    char hour[3];

    // set the same line graph
    if (first != NULL) {
        StationSchedule second = first->next;
        int vertex = 0;

        while (second != NULL) {
            if (first->lineNumber == second->lineNumber) {
                Edge edge;
                edge.v = vertex;
                edge.w = vertex + 1;
                // start time
                strncpy(hour, first->time, 2);
                hour[2] = '\0';
                edge.startHour = atoi(hour);
                edge.startMin = atoi(first->time + 2);
                // end time
                strncpy(hour, second->time, 2);
                hour[2] = '\0';
                edge.endHour = atoi(hour);
                edge.endMin = atoi(second->time + 2);
                // duration
                edge.duration = (edge.endHour * 60 + edge.endMin) - (edge.startHour * 60 + edge.startMin);

                printf("direction line:\n");
                printf("  from station: time: %s, name: %s, line: %d, start %d\n", first->time,
                       first->stationName, first->lineNumber, vertex);
                printf("  to station: time: %s, name: %s, line: %d, end %d\n",
                       second->time, second->stationName,
                       second->lineNumber, vertex + 1);

                if ((edge.endHour * 60 + edge.endMin) <= arrivedByTime) {
                    insertEdge(trainGraph, edge);
                }
            }
            // get next
            vertex++;
            first = second;
            second = second->next;
        }
    }

    // 再次计算
    first = lineTimeTables->header;
    Vertex v = 0;
    while (first != NULL) {
        // create edge
        StationSchedule second = lineTimeTables->header;
        // compare and create the edge
        Vertex w = 0;
        while (second != NULL) {
            //  相同的车站，不同的路线,就要设置换乘
            if (first->stationIndex == second->stationIndex && first->lineNumber != second->lineNumber) {

                Edge edge;
                edge.v = v;
                edge.w = w;
                // start time
                strncpy(hour, first->time, 2);
                hour[2] = '\0';
                edge.startHour = atoi(hour);
                edge.startMin = atoi(first->time + 2);
                // end time
                strncpy(hour, second->time, 2);
                hour[2] = '\0';
                edge.endHour = atoi(hour);
                edge.endMin = atoi(second->time + 2);
                // duration
                edge.duration = (edge.endHour * 60 + edge.endMin) - (edge.startHour * 60 + edge.startMin);
                if (edge.duration > 0) {
                    // check 是不是有换乘
                    // check相同的车站是否可以换乘
                    if (checkConnected(trainGraph, v, w) == 1) {
                        edge.v = v;
                        edge.w = w;
                        printf("01 transfer line:\n");
                        printf("  from station: time: %s, name: %s, line: %d, start %d\n", first->time,
                               first->stationName, first->lineNumber, v);
                        printf("  to station: time: %s, name: %s, line: %d, end %d\n",
                               second->time, second->stationName,
                               second->lineNumber, w);
                        if ((edge.endHour * 60 + edge.endMin) <= arrivedByTime) {
                            // insert edge
                            insertEdge(trainGraph, edge);
                        }
                    }
                } else {
                    // 变成正的
                    edge.duration = 0 - edge.duration;
                    // check相同的车站是否可以换乘
                    if (checkConnected(trainGraph, v, w) == 1) {
                        edge.v = w;
                        edge.w = v;
                        printf("02 transfer line:\n");
                        printf("  from station: time: %s, name: %s, line: %d, start %d\n", second->time,
                               second->stationName, second->lineNumber, w);
                        printf("  to station: time: %s, name: %s, line: %d, end %d\n",
                               first->time, first->stationName,
                               first->lineNumber, v);
                        if ((edge.endHour * 60 + edge.endMin) <= arrivedByTime) {
                            // insert edge
                            insertEdge(trainGraph, edge);
                        }
                    }
                }

            }
            // next
            second = second->next;
            w++;
        }
        v++;
        // get the next
        first = first->next;
    }


}

void dijikstra(Graph graph, int pred[], int numberOfVertices, int start) {

    int **cost = malloc(numberOfVertices * sizeof(int *));
    int distance[numberOfVertices];
    int visited[numberOfVertices], count, minDistance, nextNode = 0, i, j;
    // sef default
    for (i = 0; i < numberOfVertices; i++) {
        pred[i] = -1;
    }

    // create init dijistra state
    for (i = 0; i < numberOfVertices; i++) {
        // allocate every list
        cost[i] = malloc(numberOfVertices * sizeof(int));
        for (j = 0; j < numberOfVertices; j++) {
            if (adjacent(graph, i, j)) {
                cost[i][j] = getGraphCost(graph, i, j);
            } else {
                cost[i][j] = INFINITY;
            }
        }
    }



    for (i = 0; i < numberOfVertices; i++) {
        distance[i] = cost[start][i];
        pred[i] = start;
        visited[i] = 0;
    }
    distance[start] = 0;
    visited[start] = 1;
    // pred[start] = start;
    count = 0;
    while (count < numberOfVertices - 1) {
        minDistance = INFINITY;
        for (i = 0; i < numberOfVertices; i++)
            if (distance[i] < minDistance && !visited[i]) {
                minDistance = distance[i];
                nextNode = i;
            }
        visited[nextNode] = 1;
        for (i = 0; i < numberOfVertices; i++)
            if (!visited[i])
                if (minDistance + cost[nextNode][i] < distance[i]) {
                    distance[i] = minDistance + cost[nextNode][i];
                    pred[i] = nextNode;
                }
        count++;
    }

    // free cost
    for (i = 0; i < numberOfVertices; i++) {
        free(cost[i]);
    }
    free(cost);

    // print graph
    printf("Graph trains edges\n");
    for (i = 0; i < numberOfVertices; i++) {
        for (j = 0; j < numberOfVertices; j++) {
            if (adjacent(graph, i, j) == 1) {
                printf("from %d to %d, cost: %d\n", i, j, getGraphCost(graph, i, j));
            }
        }
    }

    printf("parent cost:\n");
    for (i = 0; i < numberOfVertices; i++) {
        printf("current: %d,parent: %d\n", i, pred[i]);
    }
}


/**
 * print search next path
 * @param fromStationVertex
 * @param toStationVertex
 * @param lineTimeTables
 */
void printSearchPath(int fromStationVertex, int toStationVertex, TimeTables lineTimeTables) {
    // 获取图的顶点
    StationSchedule first = lineTimeTables->header, fromStation = NULL, toStation = NULL;
    // printf("printSearchPath: fromStationVertex: %d,toStationVertex: %d\n",fromStationVertex,toStationVertex);
    int index = 0;
    while (first != NULL) {
        if (index == fromStationVertex) {
            fromStation = first;
        }
        if (index == toStationVertex) {
            toStation = first;
        }

        index++;
        first = first->next;
    }

    if (fromStation != NULL && toStation != NULL) {
        if (fromStation->lineNumber == toStation->lineNumber) {
            printf("%s %s\n", toStation->time, toStation->stationName);
        } else {
            printf("Change at %s\n", toStation->stationName);
            // printf("%s %s\n", toStation->time, toStation->stationName);
            // printf("%s %s to %s %s\n", fromStation->time, fromStation->stationName, toStation->time,
            // toStation->stationName);
            printf("%s %s\n", toStation->time,
                   toStation->stationName);
        }
    }

    // check 如果找不到就只输出一个
    if (fromStation == NULL && toStation != NULL) {
        printf("%s %s\n", toStation->time, toStation->stationName);
    }
    // check 如果找不到就只输出一个
    if (fromStation != NULL && toStation == NULL) {
        printf("%s %s\n", fromStation->time, fromStation->stationName);
    }
}

void addSearchEdge(Graph graph, TimeTables lineTimeTables, int fromStationIndex, int toStationIndex,
                   int numberOfTimeTables) {
    // 获取图的顶点
    StationSchedule first = lineTimeTables->header;
    int fromStationVertex = 0, toStationVertex = 0;
    while (first != NULL) {
        if (first->stationIndex == fromStationIndex) {
            Edge edge;
            edge.v = numberOfTimeTables;
            edge.w = fromStationVertex;
            edge.duration = 0;
            printf("01 add edge from start %d to %d \n", numberOfTimeTables, fromStationVertex);
            insertEdge(graph, edge);
        }
        if (first->stationIndex == toStationIndex) {
            Edge edge;
            edge.v = toStationVertex;
            edge.w = numberOfTimeTables + 1;
            edge.duration = 0;
            printf("02 add edge from start %d to %d \n", toStationVertex, numberOfTimeTables + 1);
            insertEdge(graph, edge);
        }
        fromStationVertex++;
        toStationVertex++;
        first = first->next;
    }
}

void removeSearchEdge(Graph graph, TimeTables lineTimeTables, int fromStationIndex, int toStationIndex,
                      int numberOfTimeTables) {
    // 获取图的顶点
    StationSchedule first = lineTimeTables->header;
    int fromStationVertex = 0, toStationVertex = 0;
    while (first != NULL) {
        if (first->stationIndex == fromStationIndex) {
            Edge edge;
            edge.v = numberOfTimeTables;
            edge.w = fromStationVertex;
            edge.duration = 0;
            if (adjacent(graph, edge.v, edge.w)) {
                printf("01 remove edge from start %d to %d \n", numberOfTimeTables, fromStationVertex);
                removeEdge(graph, edge);
            }
        }
        if (first->stationIndex == toStationIndex) {
            Edge edge;
            edge.v = toStationVertex;
            edge.w = numberOfTimeTables + 1;
            edge.duration = 0;
            if (adjacent(graph, edge.v, edge.w)) {
                printf("02 remove edge from start %d to %d \n", toStationVertex, numberOfTimeTables + 1);
                removeEdge(graph, edge);
            }
        }
        fromStationVertex++;
        toStationVertex++;
        first = first->next;
    }
}

/**
 * 搜索图形
 * @param trainGraph
 * @param stationNames
 */
void searchTrainArrived(TimeTables lineTimeTables, char **stationNames, int numberOfStations) {
    Stack stack = newStack();

    int numberOfTimeTables = lineTimeTables->length;

    // set hour
    char hour[3];
    int fromStationIndex = -1, toStationIndex = -1;
    char stationName[STATION_NAME_SIZE];
    char arriveBy[TIME_LENGTH];
    int arriveByTime;

    while (1) {
        // get from
        printf("From: ");
        scanf("%s", stationName);

        *(stationName + strlen(stationName)) = '\0';
        // 比较字符串相等
        if (strcmp(stationName, DONE) == 0) {
            printf("Thank you for using myTrain.\n");
            break;
        }

        // 寻找对应的站名
        for (int i = 0; i < numberOfStations; i++) {
            if (strcmp(stationName, stationNames[i]) == 0) {
                fromStationIndex = i;
                break;
            }
        }

        // 获取到达的车站
        printf("To: ");
        scanf("%s", stationName);
        *(stationName + strlen(stationName)) = '\0';
        // 获取到达车站的索引
        // 寻找对应的站名
        for (int i = 0; i < numberOfStations; i++) {
            if (strcmp(stationName, stationNames[i]) == 0) {
                toStationIndex = i;
                break;
            }
        }

        // 获取到达的车站
        printf("Arrive by: ");
        scanf("%s", arriveBy);
        *(arriveBy + strlen(arriveBy)) = '\0';

        strncpy(hour, arriveBy, 2);
        hour[2] = '\0';
        arriveByTime = atoi(hour) * 60 + atoi(arriveBy + 2);
        // print time tables
        printTimeTables(lineTimeTables);
        // construct the graph
        // 多出两个顶点用来计算用来计算距离
        Graph trainGraph = newGraph(numberOfTimeTables + 2);

        // read and construct graph
        createTrainsGraph(trainGraph, lineTimeTables, arriveByTime);


        int *paths = malloc((numberOfTimeTables + 2) * sizeof(int));
        // check 都寻找到站名
        if (fromStationIndex > -1 && toStationIndex > -1) {
            // add search edge
            addSearchEdge(trainGraph, lineTimeTables, fromStationIndex, toStationIndex, numberOfTimeTables);

            // print graph
            printf("Graph trains edges\n");
            for (int i = 0; i < numberOfTimeTables + 2; i++) {
                for (int j = 0; j < numberOfTimeTables + 2; j++) {
                    if (adjacent(trainGraph, i, j) == 1) {
                        printf("from %d to %d, cost: %d\n", i, j, getGraphCost(trainGraph, i, j));
                    }
                }
            }

            // search engine
            dijikstra(trainGraph, paths, numberOfTimeTables + 2, numberOfTimeTables);

            // 找到了path
            if (paths[numberOfTimeTables + 1] != -1) {

                StackPush(stack, numberOfTimeTables + 1);
                int parent = paths[numberOfTimeTables + 1];
                while (parent < numberOfTimeTables && paths[parent] != -1 &&
                       parent != numberOfTimeTables) {
                    StackPush(stack, parent);
                    parent = paths[parent];
                }

                // 开始打印输出
                printf("========search result ======\n");
                int firstVertex = numberOfTimeTables, secondVertex;
                while (!StackIsEmpty(stack)) {
                    secondVertex = StackPop(stack);
                    if (secondVertex > numberOfTimeTables - 1) {
                        break;
                    }
                    printSearchPath(firstVertex, secondVertex, lineTimeTables);
                    firstVertex = secondVertex;
                }
            } else {
                printf("No connection found.");
            }
            // remove search edge
            removeSearchEdge(trainGraph, lineTimeTables, fromStationIndex, toStationIndex, numberOfTimeTables);
        }

        free(paths);
        freeGraph(trainGraph);
    }

    // 清空stack
    dropStack(stack);
}

int main(int argc, char *argv[]) {

    // 定义变量
    int numberOfStations, numberOfTrains = 0;
    printf("Enter the number of stations: ");
    scanf("%d", &numberOfStations);

    char **stationNames;
    // stationNames = malloc(numberOfStations * STATION_NAME_SIZE * sizeof(char *));
    stationNames = malloc(numberOfStations * sizeof(char *));
    if (stationNames != NULL) {
        // read to array
        readStationsToArray(stationNames, numberOfStations);

        TimeTables lineTimeTables = createTimeTables();

        // read line trains:
        readTrainTimeTables(stationNames, lineTimeTables, numberOfStations, &numberOfTrains);
        // search engine
        searchTrainArrived(lineTimeTables, stationNames, numberOfStations);
        // release
        freeTimeTables(lineTimeTables);

        // free station names
        for (int i = 0; i < numberOfStations; i++) {
            free(stationNames[i]);
        }
        // free
        free(stationNames);
    }
}


