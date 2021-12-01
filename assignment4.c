//Jacob Hagen
//COMP 322 - Fall 2021
//Assignment 4 - Memory allocation algorithms

#include <stdio.h>
#include <stdlib.h>

int physical_size = 0;
int algorithm = -1;
//-1 - Invalid
//0 - First fit
//1 - Best fit
int free_index = -1;

typedef struct block {
	int start;
	int size;
} block;

block * list;

void release() {
	if(physical_size > 0) {
		physical_size = 0;
		algorithm = -1;
		free_index = -1;
		free(list);
	}
}

void enter() {
	int i;
	release();
	printf("Enter size of physical memory: ");
	scanf("%d", &physical_size);
	if(physical_size <= 0){
		printf("Invalid physical memory size.\n");
		return;
	}
	printf("Enter hole-fitting algorithm (0=first fit, 1=best fit): ");
	scanf("%d", &algorithm);
	if(algorithm != 0 && algorithm != 1){
		printf("Invalid algorithm selection.\n");
		physical_size = 0;
		return;
	}
	list = (block *)malloc(physical_size * sizeof(block));
	for(i = 0; i < physical_size; i++){
		list[i].start = -1;
		list[i].size = -1;
	}
	free_index = 0;
}

void print_list(){
	int i;
	printf("\n");
	printf("Index\tStart\tOpening\n");
	printf("-----------------------\n");
	for(i = 0; i < free_index; i++){
		int end = list[i].start + list[i].size;
		printf("%d\t%d\t%d\n", i, list[i].start, end);
	}
	printf("Size: %d\n", physical_size);
}

void allocate(){
	int i, size = 0, open_location = 0, open_size, selection = -1, start, smallest_size = physical_size +1;
	if(physical_size <= 0){
		printf("Invalid physical memory size.\n");
		return;
	} else if(free_index == physical_size){
		printf("Memory is full. Deallocate first.\n");
		return;
	}
	printf("Enter block size: ");
	scanf("%d", &size);
	if(size <= 0){
		printf("Invalid block size.\n");
		return;
	}
	//Check the openings between each block, starting with memory address 0
	//and block 0's starting address
	for (i = 0; i < free_index; i++){
		open_size = list[i].start - open_location;
		if(size <= open_size){
			if(algorithm == 0 && selection == -1){
				selection = i;
				start = open_location;
			}else if(algorithm == 1 && smallest_size > open_size){
				selection = i;
				start = open_location;
				smallest_size = open_size;
			}
		}
		open_location = list[i].start + list[i].size;
	}
	//check the last opening between the last block's ending addres and the physical memory size
	open_size = physical_size - open_location;
	if(size <= open_size){
		if(algorithm == 0 && selection == -1){
				selection = free_index;
				start = open_location;
		}else if(algorithm == 1 && smallest_size > open_size){
				selection = free_index;
				start = open_location;
		}
	}
	//perform the insertion at the selected index
	if(selection >= 0){
		for(i = free_index; i > selection; i--){
			list[i].start = list[i - 1].start;
			list[i].size = list[i - 1].size;
		}
		list[selection].start = start;
		list[selection].size = size;
		free_index++;
		int end = list[selection].start + list[selection].size;
		printf("New block inserted, starting at %d and ending before %d\n", list[selection].start, end);
		
	}else {
		printf("Unable to insert new block.\n");
		return;
	}
	//print the list after the operation
	print_list();
}

void deallocate(){
	int i, index = -1;
	if(physical_size <= 0){
		printf("Invalid physical memory size.\n");
		return;
	} else if(free_index == 0) {
		printf("Memory is empty. Allocate first.\n");
		return;
	}
	printf("Enter block index: ");
	scanf("%d", &index);
	if(index < 0){
		printf("Invalid block index.\n");
		return;
	} else if(index >= free_index){
		printf("Invalid block index - too high.\n");
		return;
	}
	//perform the deletion at the selected index (opposite order of insertion)
	free_index--;
	for(i = index; i < free_index; i++){
		list[i].start = list[i + 1].start;
		list[i].size = list[i + 1].size;
	}
	list[free_index].start = -1;
	list[free_index].size = -1;
	printf("Block successfully deallocated. \n");
	//print the list after the operation
	print_list();
}

void defragment(){
	int i;
	if(physical_size <= 0){
		printf("Invalid physical memory size.\n");
		return;
	} else if(free_index == 0){
		printf("Memory is empty. Allocate first.\n");
		return;
	}
	//change every block's starting address into the previous block's ending address
	list[0].start = 0;
	for(i = 1; i < free_index; i++){
		list[i].start = list[i -1].start + list[i - 1].size;
	}
	printf("Memory successfully defragmented.\n");
	//print the list after the operation
	print_list();
}

int main() {
	int option = 0;
	while(option != 5){
		option = 5;
		printf("Memory Allocation\n");
		printf("-----------------\n");
		printf("1) Enter parameters\n");
		printf("2) Allocate memory for block\n");
		printf("3) Deallocate memory for block\n");
		printf("4) Defragment memory\n");
		printf("5) Quit program and free memory\n");
		printf("\n");
		printf("Enter selection: ");
		scanf("%d", &option);
		printf("\n");
		switch (option) {
			case 1:
				enter();
				break;
			case 2:
				allocate();
				break;
			case 3:
				deallocate();
				break;
			case 4:
				defragment();
				break;
			case 5:
				printf("Goodbye\n");
				release();
				break;
			default:
				printf("Invalid option, try again.\n");
		}
		printf("\n");
	}
}