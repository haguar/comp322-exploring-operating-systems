// Jacob Hagen
//COMP 322 - Fall 2021
//Assignment 3 - Banker's Algorithm

#include <stdio.h>
#include <stdlib.h>

int total_processes = 0;
int total_resources = 0;

typedef struct process {
	int * max_requestable;
	int * allocated;
	int * needed;
} process;

process * processes = NULL;

typedef struct resource {
	int total_units;
	int available_units;
} resource;

resource * resources = NULL;

void release(){
	if (total_processes > 0){
		int i;
		for (i = 0; i < total_processes; i++){
			free(processes[i].max_requestable);
			free(processes[i].allocated);
			free(processes[i].needed);
		total_processes = 0;
		total_resources = 0;
		free(processes);
		free(resources);
		}
	}
}

void enter(){
	int i,j;
	release();
	printf("Enter total number of processes: ");
	scanf("%d", &total_processes);
	if(total_processes <= 0){
		printf("Invalid number of processes. \n");
		total_processes = 0;
		return;
	}
	printf("Enter total number of resources: ");
	scanf("%d", &total_resources);
	if(total_resources <= 0){
		printf("Invalid number of resources. \n");
		total_processes = 0;
		total_resources = 0;
		return;
	}
	processes = (process *)malloc(total_processes * sizeof(process));
	resources = (resource *)malloc(total_resources * sizeof(resource));
	for(i = 0; i < total_processes; i++){
		processes[i].max_requestable = (int *)malloc(total_resources * sizeof(int));
		processes[i].allocated = (int *)malloc(total_resources * sizeof(int));
		processes[i].needed = (int *)malloc(total_resources * sizeof(int));
	}
	printf("***\n");
	for(j = 0; j < total_resources; j++){
		int total_units = -1;
		printf("Enter number of total units for resource %d:", j);
		scanf("%d", &total_units);
		if(total_units < 0){
			printf("Invalid number of total units.\n");
			release();
			return;
		}
		resources[j].total_units = total_units;
		resources[j].available_units = total_units;
	}
	printf("***\n");
	for(i = 0; i < total_processes; i++){
		for(j = 0; j < total_resources; j++){
			int max_requestable = -1;
			printf("Enter max number of units that process %d can request from resource %d: ", i, j);
			scanf("%d", &max_requestable);
			if(max_requestable < 0){
				printf("Invalid max number of requestable units - too low.\n");
				release();
				return;
			} else if(max_requestable > resources[j].total_units){
				printf("Invalid max number of requestable units - too high.\n");
				release();
				return;
			}
			processes[i].max_requestable[j] = max_requestable;
			processes[i].needed[j] = max_requestable;
		}
	}
	printf("***\n");
	for(i = 0; i < total_processes; i++){
		for(j = 0; j < total_resources; j++){
			int allocated = -1;
			printf("Enter number of units that process %d is allocated from resource %d: ", i, j);
			scanf("%d", &allocated);
			if(allocated < 0){
				printf("Invalid number of allocated units - too low.\n");
				release();
				return;
			} else if(allocated > processes[i].max_requestable[j]){
				printf("Invalid  number of allocated units - too high.\n");
				release();
				return;
			}
			processes[i].allocated[j] = allocated;
			processes[i].needed[j] -= allocated;
			resources[j].available_units -= allocated;
		}
	}
}

void print_graphs(){
	if(total_processes <= 0) {
		printf("Invalid number of processes.\n");
		return;
	}
	printf("Total units:\n");
	print_array(0);
	printf("\n");
	printf("Available units:\n");
	print_array(1);
	printf("\n");
	printf("Max requestable units:\n");
	print_matrix(0);
	printf("\n");
	printf("Allocated units:\n");
	print_matrix(1);
	printf("\n");
	printf("Needed units:\n");
	print_matrix(2);
}

void print_array(int array_type){
	int j;
	
	for(j = 0; j < total_resources; j++){
		printf("\tr%d", j);
	}
	
	printf("\n");
	
	for(j = 0; j < total_resources; j++){
		switch (array_type) {
			case 0: //total_units
				printf("\t%d", resources[j].total_units);
				break;
			case 1: //available_units
				printf("\t%d", resources[j].available_units);
				break;
			default: //invalid
				printf("\t");
		}
	}
	printf("\n");
}

void print_matrix(int matrix_type){
	
	int i;
	for(i = 0; i < total_processes; i++){
		int j;
		if (i == 0){
			for(j = 0; j < total_resources; j++){
			printf("\tr%d", j);
			}
			printf("\n");
		}
		printf("p%d", i);
		for(j = 0; j < total_resources; j++){
			switch (matrix_type){
				case 0: //max_requestable
					printf("\t%d", processes[i].max_requestable[j]);
					break;
				case 1: //allocated
					printf("\t%d", processes[i].allocated[j]);
					break;
				case 2: //needed
					printf("\t%d", processes[i].needed[j]);
					break;
				default: //invalid
					printf("\t");
			}
		}
		printf("\n");
		
	}
}

int check_arrays(int * first_array, int * second_array){
	//Return 1 if all of first_array values <= all of second_array values
	//return 0 if any of first_array values >any of second_array values
	int j;
	for(j = 0; j < total_resources; j++){
		if(first_array[j] > second_array[j]){
			return 0;
		}
	}
	return 1;
}

void bankers_algorithm(){
	int i, j, completed = 0, * sequenced, * current_units;
	if(total_processes <= 0){
		printf("Invalid number of processes.\n");
		return;
	}
	sequenced = (int *)malloc(total_processes * sizeof(int));
	for(i = 0; i < total_processes; i++){
		sequenced[i] = 0;
	}
	current_units = (int *)malloc(total_resources * sizeof(int));
	for(j = 0; j < total_resources; j++){
		current_units[j] = resources[j].available_units;
	}
	//Run the Bankers's algorithm using a circular for loop
	for (i = 0; completed < total_processes; i = (i + 1) % total_processes){
		//If current process is sequenced, then skip the iteration
		if(sequenced[i] == 1){
			continue;
		}
		//Print the output per line
		printf("Checking if p%d's needs [", i);
		for(j = 0; j < total_resources; j++){
			printf(" %d", processes[i].needed[j]);
		}
		printf("] are <= to available units [");
		for(j = 0; j < total_resources; j++){
			printf(" %d", current_units[j]);
		}
		printf("] - ");
		//Check the arrays to see if the process is skipped or sequenced
		if (check_arrays(processes[i].needed, current_units) == 1){
			//Increase the current available units 
			for(j = 0; j < total_resources; j++){
				current_units[j] += processes[i].allocated[j];
			}
			//signify that the current process is complete
			sequenced[i] = 1;
			completed++;
			printf("p%d safely sequenced", i);
		}else {
			printf("skipping");
		}
		printf("\n");
	}
	
	free(sequenced);
	free(current_units);
}

int main(){
	int option = 0;
	while(option != 4){
		option = 4;
		printf("Banker's Algorithm");
		printf("------------------\n");
		printf("1) Enter parameters:\n");
		printf("2) Print resource and process graphs\n");
		printf("3) Determine a safe sequence\n");
		printf("4) Quit program and free memory\n");
		printf("\n");
		printf("Enter selection: ");
		scanf("%d", &option);
		printf("\n");
		switch (option){
			case 1:
				enter();
				break;
			case 2:
				print_graphs();
				break;
			case 3:
				bankers_algorithm();
				break;
			case 4:
				printf("Goodbye\n");
				release();
				break;
			default:
				printf("Invalid option, try again.\n");
		}
		printf("\n");
	}
	return 0;
}