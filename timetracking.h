#ifndef TIMETRACKER_H
#define TIMETRACKER_H

/* Function Declarations */
struct tm tm_from_date_and_time(char *date, char *time);
int printActivityTimesByName(void);
int csv_to_activity(char filename[]);

#endif
