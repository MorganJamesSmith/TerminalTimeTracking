#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "timetracking.h"

int
main(int argc, char *argv[]) {
	char *time_file = NULL;
	int opt;
	while ((opt = getopt(argc, argv, "d:")) != -1) {
		switch (opt) {
			case 'd':
				time_file = malloc(sizeof(optarg));
				strcpy(time_file, optarg);
				break;
			default:
				fprintf(stderr, "Usage %s [-d] timetracking.csv\n",argv[0]);
				exit(EXIT_FAILURE);
		}
	}

	if (argc-optind == 1 && optind < argc)
		time_file = argv[optind];

	if (!*time_file) {
		fprintf(stderr, "Usage %s [-d] timetracking.csv\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	int loaded_activities = csv_to_activity(time_file);
	if (loaded_activities < 1) {
			fprintf(stderr,"ERROR: No activities loaded\n");
			exit(EXIT_FAILURE);
	}

	printActivityTimesByName();
}
