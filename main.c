#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>
#include <sys/stat.h>
#include "csvparser.h"
#define ACTIVITIES_TO_LOAD 100

struct activity {
    struct tm start_time;
    double elapsed_seconds;
    char *name;
    char *location;
};

static struct tm tm_from_date_and_time(char *date, char *time);
void printActivityTimesByName(struct activity activities[],int loaded_activities);
static int csv_to_activity(FILE *csv, struct activity activitys[], int activitiesToLoad);

struct
tm tm_from_date_and_time(char *date, char *time) {
    struct tm time_return = {
        .tm_year = strtoumax(strsep(&date, "-"), NULL, 10),
        .tm_mon = strtoumax(strsep(&date, "-"), NULL, 10),
        .tm_mday = strtoumax(strsep(&date, "-"), NULL, 10),
        .tm_hour = strtoumax(strsep(&time, ":"), NULL, 10),
        .tm_min = strtoumax(strsep(&time, ":"), NULL, 10),
        .tm_sec = 0,
        .tm_isdst = -1
    };
    return time_return;

}

void
printActivityTimesByName(struct activity activities[],int loaded_activities){
    char* acts[loaded_activities];
    unsigned long long times[loaded_activities];
    int act_length = 0;
    int found;
    for(int i = 0; i < loaded_activities; i++){
        found = 0;
        for(int j = 0; j < act_length; j++){
            if(acts[j] == NULL || activities[i].name == NULL){
            fprintf(stderr, "NULL String! Failure!\n");
            exit(EXIT_FAILURE);
            }
            if(!strcmp(acts[j],activities[i].name)){
                times[j]+= activities[i].elapsed_seconds;    
                found = 1;
                break;
            }
        }
        if(!found){
            acts[act_length] = activities[i].name;
            times[act_length] = activities[i].elapsed_seconds;
            act_length += 1;
        }
    }
    unsigned long long total = 0;
    for(int i = 0; i < act_length; i++){
        printf("%s %lld:%lld\n",acts[i],(times[i])/3600,((times[i])%3600)/60);
        total = total + times[i];
    }
    printf("total: %lld:%lld\n",total/3600,(total%3600)/60);
}


int
csv_to_activity(FILE *csv, struct activity activitys[], int activitiesToLoad){

    char * line; //Stores the line currently being read
    char **lineItems; //parsed line into a list of strings
    size_t len = 0;
    ssize_t readline; //The length of string returned
    int loadedActivities = 0; // the number of activities loaded


    while ((( readline = getline(&line, &len, csv)) != -1) && (loadedActivities < activitiesToLoad)){
        struct activity act;
        lineItems = parse_csv(line);
        if(!lineItems || !lineItems[0] || !lineItems[1] || !lineItems[2] || !lineItems[3]){
            fprintf(stderr, "ERROR: couldn't parse line:\n%s\n",line);
            continue;
        }

        act.start_time = tm_from_date_and_time(lineItems[0], lineItems[1]);;
        if(loadedActivities > 0){
            activitys[loadedActivities-1].elapsed_seconds = 
                difftime(mktime(&act.start_time),mktime(&activitys[loadedActivities-1].start_time));
        }
        act.name = lineItems[2];
        act.location = lineItems[3];
        activitys[loadedActivities] = act;
        loadedActivities++;
    }
    if(loadedActivities == 0){
            fprintf(stderr,"ERROR: file is empty\n");
            exit(EXIT_FAILURE);
    }
    free(line);
    return loadedActivities;
}

int
main(int argc, char* argv[]){
    char *time_file;
    int opt;
    while ((opt = getopt(argc, argv, "d:")) != -1) {
        switch (opt) {
            case 'd':
                strcpy(time_file, optarg);
                break;
            default:
                fprintf(stderr, "Usage %s [-d] timetracking.csv\n",argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if(argc-optind == 1 && optind < argc)
        time_file = argv[optind];

    if(!*time_file){
        fprintf(stderr, "Usage %s [-d] timetracking.csv\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE* open_time_file = fopen(time_file, "r");
    if (open_time_file == NULL){
        fprintf(stderr,"ERROR: Could not open file %s\n",argv[1]);
        exit(EXIT_FAILURE);
    }

    struct activity activities[ACTIVITIES_TO_LOAD]; //flashopen_time_file in memory

    int loaded_activities = csv_to_activity(open_time_file, activities, ACTIVITIES_TO_LOAD);
    if(loaded_activities < 1){
            fprintf(stderr,"ERROR: No activities loaded\n");
            exit(EXIT_FAILURE);
    }
    fclose(open_time_file);

    printActivityTimesByName(activities,loaded_activities);
}

