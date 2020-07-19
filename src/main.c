#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#define MAX_PROCESSES 4

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

int main() {

	int input[100];
	int i, input_size, num_processes, prime_count;
	char c;
	pid_t pid[4];

	// receives a list of integers as input
	i = 0;
	while(c != '\n'){
		scanf("%d", &input[i]);
		c = getchar();
		i++;
	}

	// stores the total input size
	input_size = i;
	printf("input size: %d\n", input_size);

	// defines the number of necessary processes
	if(input_size > MAX_PROCESSES) num_processes = MAX_PROCESSES;
	else num_processes = input_size;
	printf("number of processes: %d\n", num_processes);

	// creates processes
	for(i = 0; i < num_processes; i++){
		pid[i] = fork();

		if(pid[i] == 0){
			printf("estou no processo %d\n", i);
			exit(0);
		}
	}

	printf("estou no processo pai\n");

	for(i = 0; i < input_size; i++){
		printf("input %d esperando o processo %d\n", i, i % MAX_PROCESSES);
		waitpid(pid[i % MAX_PROCESSES], NULL, 0);
		printf("sai do processo %d\n", i % MAX_PROCESSES);
	}

	printf("finalizando o processo pai\n");
	return 0;
}
