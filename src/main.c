#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h>
#include <signal.h>

#define MAX_PROCESSES 4

// function to detec is integer x is prime
bool is_prime(int x){
	int i;

	// primos sao sempre maiores ou igual a 2
	if(x < 2) return false;

	// se x for 2 ou 3, x eh primo
	else if(x == 2 || x == 3) return true;

	// se x for divisivel por 2 ou por 3, x nao eh primo
	else if(x % 2 == 0 || x % 3 == 0) return false;

	// todo primo maior que 3 eh da forma 6k +- 1, k inteiro
	i = 5;
	while(i*i <= x){
		if(x % i == 0 || x % (i+2) == 0) return false;
		i = i + 6;
	}

	return true;
}

// main program
int main() {

	unsigned long long int input[100]; // input storage
	int i; // index variable
	char c; // auxiliary char variable 
	int input_size; // stores the size of the input
	int num_processes; // number of necessary processes
	pid_t pid[4]; // stores the PID of each process
	int pipefd[2]; // pipe r/w

	// creates protection and visibility flags
	int protection = PROT_READ | PROT_WRITE;
	int visibility = MAP_SHARED | MAP_ANON;

	// creates shared memory area
	int *prime_count = (int *) mmap(NULL, sizeof(int), protection, visibility, 0, 0);

	// receives a list of integers as input
	i = 0;
	while(c != '\n'){
		scanf("%llu", &input[i]);
		c = getchar();
		i++;
	}

	// stores the total input size
	input_size = i;

	// defines number of necessary processes
	num_processes = MAX_PROCESSES;
	if(input_size < 4) num_processes = input_size;

	// creates pipe
	pipe(pipefd);

	// writes integers in the pipe
	for(i = 0; i < input_size; i++){
		write(pipefd[1], &input[i], sizeof(input[i]));
	}
	
	// close pipe write
	close(pipefd[1]);

	// creates the necessary number of processes
	for(i = 0; i < num_processes; i++){
		
		pid[i] = fork();

		// process function
		if(pid[i] == 0){
			unsigned long long int num;
			
			// close pipe writing
			close(pipefd[1]);

			// reads pipe
			while(read(pipefd[0], &num, sizeof(num)) > 0){

				// if num is prime increase the prime count
				if(is_prime(num)) (*prime_count)++;
			}

			// close pipe reading
			close(pipefd[0]);
				
			// exit process
			exit(0);
		}
	}

	// waits for the end of the processes execution
	for(int k = 0; k < num_processes; k++){
		waitpid(pid[k], NULL, 0);
	}

	// print prime count
	printf("%d\n", (*prime_count));
	return 0;
}
