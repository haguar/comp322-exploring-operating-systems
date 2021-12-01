//Jacob Hagen
//COMP 322 - Fall 2021
//Assignment 5 - Disk Scheduling

#include<stdio.h>
#include<stdlib.h>

int tracks = 0; 
int sequence_size = 0;
int * sequence = NULL;  

void release(){
	if(tracks > 0){
		tracks = 0;
		sequence_size = 0;
		free(sequence);
	}
}

void enter(){
int i, j, track_index;
	release();
	printf("Enter number of concentric track (2 or more): ");
	scanf("%d", &tracks);
	if(tracks <= 1){
		printf("Invalid number of tracks.\n");
		return;
	}
	printf("Enter size of sequence (1-%d): ", tracks - 1);
	scanf("%d", &sequence_size);
	if(sequence_size <= 0 ){
		printf("Invalid size of sequence -too low.\n");
		tracks = 0; 
		return;
	} else if(sequence_size >= tracks){
		printf("Invalid size of sequence -too high.\n");
		tracks = 0;
		return;
	}
	sequence = (int *)malloc(sequence_size * sizeof(int));
	for(i = 0; i < sequence_size; i++){
		track_index = -1;
		printf("Enter track index (1-%d) for sequence index %d: ", tracks -1, i);
		scanf("%d", &track_index);
		if(track_index <= 0){
			printf("Invalid track - too low.\n");
			release();
			return;
		}else if (track_index >= tracks){
			printf("Invalid track - too high.\n");
			release();
			return;
		}
		sequence[i] = track_index;
		for(j = 0; j < i; j++){
			if( sequence [j] == sequence[i]){
				printf("Inavlid track - duplicate.\n");
				release();
				return;
			}
		}
	}
}

void schedule( int algorithm){
	//
	
	
	int i, traversed = 0, current_track = 0;
	//SSTF variables
	int j, * ordered_sequence, * ordered_sequence_delay, average_delay_total, average_delay_size, longest_delay_amount, longest_delay_track;
	if( tracks <= 1){
		printf("Invalid number of tracks.\n");
		return;
	}
	//Construct the additional variables needed for the SSTF algorithm
	if(algorithm == 1){ // SSTF
		ordered_sequence = (int *)malloc(sequence_size * sizeof(int));
		ordered_sequence_delay = (int *)malloc(sequence_size * sizeof(int));
		//Assign default values to both arrays
		for(i = 0; i < sequence_size; i++){
			ordered_sequence[i] = sequence[i];
			ordered_sequence_delay[i] = 0;
		}
		//Organize the ordered sequence array in ascending order
		for(i = 0; i < sequence_size; i++){
			for(j = i + 1; j < sequence_size; j++){
				if(ordered_sequence[i] > ordered_sequence[j]){
					int temporary = ordered_sequence[i];
					ordered_sequence[i] = ordered_sequence[j];
					ordered_sequence[j] = temporary;
				}
			}
		}
		//Compute the delays for the ordered sequence array
		for(i = 1; i < sequence_size; i++){ //iterating ordered_sequence
			for(j = 0; j < i; j++){ //iterating original sequence
				if(ordered_sequence[i] == sequence[j]){
					ordered_sequence_delay[i] = i - j;
					break;
				}
			}
		}
		//Assign default values to the other SSTF variables
		average_delay_total = 0;
		average_delay_size = 0;
		longest_delay_amount = -1;
		longest_delay_track = -1;
		//compute the other SSTF variables
		for(i = 0; i < sequence_size; i++){
			if(ordered_sequence_delay[i] > 0){
				average_delay_total += ordered_sequence_delay[i];
				average_delay_size++;
				if(longest_delay_amount< ordered_sequence_delay[i]){
					longest_delay_amount = ordered_sequence_delay[i];
					longest_delay_track = ordered_sequence[i];
				}
			}
		}
	}
	//Print out the original track sequence
	printf("Sequence of tracks to seek:");
	for (i = 0; i < sequence_size; i++){
		printf(" %d", sequence[i]);
	}
	printf("\n");
	//Print out the traversed track sequence
	printf("Traversed sequence:");
	for(i = 0; i < sequence_size; i++){
		if(algorithm == 0){
			printf(" %d", sequence[i]);
			traversed += abs(current_track - sequence[i]);
			current_track = sequence[i];
		}else if(algorithm == 1){
			printf(" %d", ordered_sequence[i]);
			traversed += abs(current_track - ordered_sequence[i]);
			current_track = ordered_sequence[i];
		}
	}
	printf("\n");
	//Print out the algorithm results
	printf("The number of tracks traverse is: %d\n", traversed);
	//Print out the additional SSTF results
	if(algorithm == 1){//SSTF
		if(average_delay_size > 0){
			printf("The average delay of all tracks processed later is: %.2f\n", (double)average_delay_total / (double)average_delay_size);
			printf("The longest delay experienced by a track is: %d by track %d\n", longest_delay_amount, longest_delay_track);
		}
		free(ordered_sequence);
		free(ordered_sequence_delay);
	}
}

int main(){
	int option = 0;
	while(option !=4){
		option = 4 ;
		printf("Disk Scheduling\n");
		printf("-------------------\n");
		printf("1) Enter parameters\n");
		printf("2) Schedule disk tracks with FIFO\n");
		printf("3) Schedule disk tracks with SSTF\n");
		printf("4) Quit Program and free memory\n");
		printf("\n");
		printf("Enter selection: ");
		scanf("%d", &option);
		printf("\n");
		switch( option ) {
			case 1:
				enter();
				break;
			case 2: 
				schedule(0); //FIFO 
				break;
			case 3: 
				schedule(1);  //SSTF
				break;
			case 4 : 
				printf("Goodbye"); 
				release();
				break;
		default:
			printf("Invalid option, try again.\n");
		}
		printf("\n");
	}
}