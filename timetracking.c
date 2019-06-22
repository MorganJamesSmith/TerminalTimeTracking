#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>
#include <stdio.h>
#include "csvparser.h"
#include "timetracking.h"

#define ACTIVITIES_TO_LOAD 100

struct Activity {
	struct tm start_time;
	double elapsed_seconds;
	char *name;
	char *location;
};

/* Variables */
struct Activity activities[ACTIVITIES_TO_LOAD];
int loadedactivities = 0;

/* Functions */
struct tm
tm_from_date_and_time(char *date, char *time)
{
	char datecp[sizeof(*date)];
	strcpy(datecp, date);

	int year = strtoumax(strtok(datecp, "-"), NULL, 10);

	int month = strtoumax(strtok(NULL, "-"), NULL, 10) - 1;
	if(month > 11 || month < 0)
		printf("Bad month\n");

	int day = strtoumax(strtok(NULL, "-"), NULL, 10);
	if(day > 31 || day < 1)
		printf("Bad day\n");

	char timecp[sizeof(*time)];
	strcpy(timecp, time);

	int hour = strtoumax(strtok(timecp, ":"), NULL, 10);
	if(hour > 23 || hour < 0)
		printf("Bad day\n");

	int min = strtoumax(strtok(NULL, ":"), NULL, 10);
	if(min > 59 || min < 0)
		printf("Bad min\n");

	struct tm time_return = {
		.tm_year = year,
		.tm_mon = month,
		.tm_mday = day,
		.tm_hour = hour,
		.tm_min = min,
		.tm_sec = 0,
		.tm_isdst = -1
	};
	return time_return;
}


int
printActivityTimesByName(void)
{
	char *acts[loadedactivities];
	unsigned long long times[loadedactivities];
	int act_length = 0;
	int found;
	for (int i = 0; i < loadedactivities; i++) {
		found = 0;
		for (int j = 0; j < act_length; j++) {
			if (acts[j] == NULL || activities[i].name == NULL) {
			fprintf(stderr, "NULL String! Failure!\n");
			return -1;
			}
			if (!strcmp(acts[j],activities[i].name)) {
				times[j]+= activities[i].elapsed_seconds;
				found = 1;
				break;
			}
		}
		if (!found) {
			acts[act_length] = activities[i].name;
			times[act_length] = activities[i].elapsed_seconds;
			act_length += 1;
		}
	}
	unsigned long long total = 0;
	for(int i = 0; i < act_length; i++) {
		printf("%s %lld:%lld\n",acts[i],(times[i])/3600,((times[i])%3600)/60);
		total = total + times[i];
	}
	printf("total: %lld:%lld\n",total/3600,(total%3600)/60);
	return act_length;
}


int
csv_to_activity(char filename[])
{
	FILE *csv = fopen(filename, "r");
	if (csv == NULL) {
		return(1);
	}

	char *line; //Stores the line currently being read
	char **lineItems; //parsed line into a list of strings
	size_t len = 0;
	ssize_t readline; //The length of string returned


	while ((( readline = getline(&line, &len, csv)) != -1) && (loadedactivities < ACTIVITIES_TO_LOAD)) {
		struct Activity act;
		lineItems = parse_csv(line);
		if (!lineItems || !lineItems[0] || !lineItems[1] || !lineItems[2] || !lineItems[3]) {
			fprintf(stderr, "ERROR: couldn't parse line:\n%s\n",line);
			continue;
		}

		act.start_time = tm_from_date_and_time(lineItems[0], lineItems[1]);;
		if (loadedactivities > 0) {
			activities[loadedactivities-1].elapsed_seconds =
				difftime(mktime(&act.start_time),mktime(&activities[loadedactivities-1].start_time));
		}
		act.name = lineItems[2];
		act.location = lineItems[3];
		activities[loadedactivities] = act;
		loadedactivities++;
	}
	if (loadedactivities == 0) {
			fprintf(stderr,"ERROR: file is empty\n");
			return 1;
	}
	free(line);

	fclose(csv);
	return loadedactivities;
}


