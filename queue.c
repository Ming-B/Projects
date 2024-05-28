#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#define BURST_LOWER 1
#define BURST_UPPER 5
#define MAX_TIME 50
#define PROC_CREATE_CHANCE 0.30


struct queue {
	struct node* head;
	struct node* tail;
};

struct node {
 	struct proc* process;
 	struct node* next;	
};

struct proc {
  	int pid;	
   int arrival;
   int finish;
  	int burst;
};


void init_queue(struct queue* q);
bool enqueue(struct queue* q, struct proc* p);
struct proc* dequeue(struct queue* q);
void print_queue(struct queue* q);
bool enqueue_random_proc(struct queue* q, int* pidgen, int time);
float rand_float();
int rand_burst();


int main() {	
	srand(time(NULL));		  

	struct proc* cpu;

	int clock = 0;	
	int pid = 0;   //pid generator
	int completed = 0;   //number of terminated processes
	int total_turnover = 0;  //total_turnover time of all terminated processes

	struct queue* q = malloc(sizeof(struct queue));
	init_queue(q);

	struct proc p1 = { .pid = pid++, .arrival = 0, .burst = rand_burst() };
	struct proc p2 = { .pid = pid++, .arrival = 0, .burst = rand_burst() };

	enqueue(q, &p1); 
	enqueue(q, &p2);
   
	while(clock < MAX_TIME){
		//load front of queue onto the cpu	
		cpu = dequeue(q);

		
		if(cpu != NULL){
			//let process execute entirely	
			for(int i=0; i<(cpu->burst); i++){
   			clock++;
   			
				//if threshold exceeded, add new process into ready queue	
   			if(rand_float() < PROC_CREATE_CHANCE){ 
   				enqueue_random_proc(q, &pid, clock);
   			}
		   }
		
			//set finish time and update turnover metrics	
			completed++;
			cpu->finish = clock;
			total_turnover += (clock - cpu->arrival);

			printf("Terminated process\n");
      	printf("PID: %d    Arrival: %d    Burst: %d    Finish: %d\n", cpu->pid, cpu->arrival, cpu->burst, cpu->finish);
      	printf("Current Time: %d    Processes Executed: %d    Total Turnover: %d\n\n\n", clock, completed, total_turnover);

		} else {
			clock++; 
			if(rand_float() < PROC_CREATE_CHANCE){
				enqueue_random_proc(q, &pid, clock);
			}	  
		}		  

	}

	printf("Average Turnover Time: %f\n", (double)total_turnover / completed);	
}

//adds new process into ready queue
bool enqueue_random_proc(struct queue* q, int* pidgen, int time){
	struct proc* temp = malloc(sizeof(struct proc));
	temp->pid = (*pidgen)++;
	temp->arrival = time;
	temp->burst = rand_burst();

	return enqueue(q, temp);
}


float rand_float(){
	return (float) rand() / RAND_MAX;
}

int rand_burst(){
	return (rand() % (BURST_UPPER - BURST_LOWER + 1)) + BURST_LOWER;
}


void init_queue(struct queue* q){
	q->head = NULL;
	q->tail = NULL;
}


bool enqueue(struct queue* q, struct proc* p){
	if(p == NULL){
		printf("Can't enqueue null process\n");
		return false;
	} else if(q == NULL){
		printf("Can't operate on null queue\n");
		return false;
	}		  
		 
   //creation of new node	
	struct node* newnode = malloc(sizeof(struct node));
	if(newnode == NULL){
		printf("Malloc failed\n");
		return false;
	}

	newnode->process = p;
	newnode->next = NULL;

	//make current tail point to this new node...check first if there was a tail to begin with
	if(q->tail != NULL) {
		q->tail->next = newnode;
	}
	q->tail = newnode;

	//case where node being enqueued into an empty list
	if(q->head == NULL) {
		q->head = newnode;
	}

	return true;
}

struct proc* dequeue(struct queue* q){
	if(q->head == NULL){	
		return NULL;
	}

	//store the contents of current head to return...so free() can be called	
	struct node* temp = q->head;
	struct proc* result = temp->process;

	//move head forward
	q->head = q->head->next;
	if(q->head == NULL){
		q->tail = NULL;  //when queue contains no elements after dequeue
	}

	free(temp);
	return result;
}

void print_queue(struct queue* q){
	struct node* trav = q->head;
	while(trav != NULL){
		printf("%d -> ", trav->process->pid);
		trav = trav->next;
	}
	printf("NULL\n");
}

