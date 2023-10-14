/* header files */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <sys/time.h>
#include <signal.h>
#include <math.h>
#include <fcntl.h>

/* global definitions */
/* global variables and data structures */
/* definition and implementation of process descriptor and queue(s) */
int sigadd = 0;

typedef struct Procedure {
	char ProName[100];
	char ProStatus[100];
	double ProPrio;
	double ProExeTime;
	int ProId;
	int ProC;
} Procedure;


typedef struct QueueNode {
	Procedure data;
	struct QueueNode * next;
	struct QueueNode * prev;
} QueueNode;


typedef struct ProceduresQueue {
	struct QueueNode * head;
	struct QueueNode * tail;
	int size;
} ProceduresQueue;


double get_wtime(void) {
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec*1.0e-6;
}


QueueNode * getQueueNode(Procedure data, QueueNode * prev) {
	QueueNode * returnnode = (QueueNode * ) malloc(sizeof(QueueNode));

	if (returnnode == NULL)
		return NULL;
	returnnode->data = data;
	returnnode->next = NULL;
	returnnode->prev = prev;
	return returnnode;
}


int isSize(ProceduresQueue * ref) {
	return ref->size;
}


int isEmpty(ProceduresQueue * ref) {
	if (ref->size == 0)
		return 1;
	else
		return 0;
}


ProceduresQueue * getProceduresQueue() {
	ProceduresQueue * Queue = (ProceduresQueue *)malloc(sizeof(ProceduresQueue));
	
	if (Queue == NULL)
    	return NULL;
	Queue->head = NULL;
	Queue->tail = NULL;
	return Queue;
}


void add_element(ProceduresQueue * ref, Procedure data) {
	QueueNode * node = getQueueNode(data, ref->tail);
	if (ref->head == NULL) {
		ref->head = node;
		ref->size = 1;
	} else {
		ref->tail->next = node;
		ref->size = ref->size + 1;
	}
	ref->tail = node;
}


Procedure dequeue(ProceduresQueue * ref) {
	if (isEmpty(ref) == 1)
		printf("Empty queue!!!\n");
	else {
		Procedure data = ref->head->data;
		QueueNode * temp = ref->head;
		if (ref->head == ref->tail) {
	    	ref->tail = NULL;
			ref->head = NULL;
		} else {
			ref->head = ref->head->next;
			ref->head->prev = NULL;
		}
		ref->size--;
		return data;
	}
}


void excecuteProcedure(char* p, double t0) {
	char * name;
	double t1 = get_wtime();
	
	int pid = fork();
	if (pid==0) {
		char pc[100];
	 	char fname[30];
	 	
		strcpy(pc,p);
		strtok(p, "/");
	    strtok(NULL, "/");
	    name = strtok(NULL, "");
	    
		strcat(fname, "./");
		strcat(fname, name);
		printf("PID %d - CMD: %s\n", getpid(), name);
		execl(pc, fname, NULL);
		exit(1);
	}
    waitpid(-1, NULL, 0);
	double t2 = get_wtime();
	printf("                        Elapsed time = %.3f secs\n", t2-t1);
	printf("                        Workload time = %.3f secs\n", t2-t0);
	
}


void execFCFS(ProceduresQueue * ref, double t0, int print_total) {
	QueueNode * node = ref->head;
	Procedure pr;
	
	while (node != NULL){
		pr = dequeue(ref);
		
		strcpy(pr.ProStatus, "RUNNING");
		excecuteProcedure(pr.ProName, t0);
		strcpy(pr.ProStatus, "EXITED");
		
		node = node->next;
	}
	double t3 = get_wtime();
	if (print_total == 1)
    	printf("WORKLOAD TIME: %.3f seconds\n", t3-t0);
}


int startProcedure(Procedure pr, double t0) {
	char * name;
	char pc[100];
	char fname[30];
	
	strcpy(pc,pr.ProName);
	strtok(pr.ProName, "/");
	strtok(NULL, "/");
	name = strtok(NULL, "");
	strcat(fname, "./");
	strcat(fname, name);
	
	double t1 = get_wtime();

	int ppid = fork();
	
	if (ppid > 0) {
		kill(ppid, SIGSTOP);
		return ppid;
	} else if (ppid == 0) {
		execl(pc, fname, NULL);
		exit(1);
	}
}

/* signal handler(s) */
void s_handler(int sigid) {
	sigadd = 1;
}


void execQRR(ProceduresQueue * ref, double quant, double t0, int print_total) {
	QueueNode * node = ref->head;
	Procedure pr;
	
	struct sigaction sact;
	sact.sa_handler = SIG_IGN;
	sigemptyset(&sact.sa_mask);
	sact.sa_flags = SA_RESTART;
	
	while (node != NULL) { 
		if (isSize(ref) != 0)
			pr = dequeue(ref);

		if (pr.ProId == -1) {
			int pidtemp = startProcedure(pr, t0);
			pr.ProExeTime = 0;
			pr.ProId = pidtemp;

		} else {
		
			pr.ProC -= 1;
			kill(pr.ProId, SIGCONT);
			strcpy(pr.ProStatus, "RUNNING");
						
			if (pr.ProC == 0) {
    			sact.sa_handler = s_handler;
    			sigaction(SIGCHLD, &sact, NULL);
    			double t1 = get_wtime();
				waitpid(pr.ProId, NULL, 0);
				pr.ProExeTime += get_wtime() - t1;
				strcpy(pr.ProStatus, "EXITED");
				
				printf("PID %d - CMD: %s\n", pr.ProId, pr.ProName);
				printf("                        Elapsed time = %.3f secs\n", pr.ProExeTime);
				printf("                        Workload time = %.3f secs\n", get_wtime()-t0);
			
				sact.sa_handler = SIG_IGN;
				sigaction(SIGCHLD, &sact, NULL);
			
			} else if (pr.ProC > 0) {
    			double t_end = get_wtime() + quant/(double)1000;
   				double t = get_wtime();
	  			while (t < t_end) t = get_wtime();

				kill(pr.ProId, SIGSTOP);
				strcpy(pr.ProStatus, "STOPPED");
				pr.ProExeTime += quant/(double)1000;
				
				printf("PID %d - CMD: %s\n", pr.ProId, pr.ProName);
				printf("                        Elapsed time = %.3f secs\n", pr.ProExeTime);
				printf("                        Workload time = %.3f secs\n", get_wtime()-t0);
			}
		}
		
		if (((isSize(ref) == 0) && (pr.ProC == 0)) || (isSize(ref) > 0))
			node = node->next;
		
		if ((strcmp(pr.ProStatus, "EXITED")!=0) && (sigadd == 0) && (isSize(ref)>0))
			add_element(ref, pr);
		else 
			sigadd = 0;
	}
	if (print_total == 1)
    	printf("WORKLOAD TIME: %.3f seconds\n", get_wtime()-t0);
}


int prioNo(ProceduresQueue * ref) {
	Procedure pr;
	int counter = 0;
	double last = -1;
	int size = isSize(ref);
	
	while (size) {
		pr = dequeue(ref);
		
		if (pr.ProPrio != last)
			counter++;
		last = pr.ProPrio;
		
		size -= 1;
		add_element(ref, pr);
	}
	return counter;
}