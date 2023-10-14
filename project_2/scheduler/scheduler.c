/* header files */
#include "Procedures_Queue.h"


/* implementation of the scheduling policies, etc. batch(), rr() etc. */
void FCFS(int count, char* test_file) {
	char * str = (char*)malloc(sizeof(char)*100);
	int j = 0;
    
    ProceduresQueue * q = getProceduresQueue();
	Procedure * Pro = malloc(sizeof(Procedure)); 
	FILE *fptr2 = fopen(test_file,"r");
	
	while (fgets(str, 100, fptr2) != NULL) {
		
	    char * path = strtok(str, "\t");
		
		strncpy(Pro->ProName, path, 100);
		strncpy(Pro->ProStatus, "READY", 100);
		Pro->ProPrio = -1;
		Pro->ProExeTime = -1;
		Pro->ProId = -1;
		Pro->ProC = -1;

		add_element(q, *Pro);
		j++;
    }
	fclose(fptr2);
	execFCFS(q, get_wtime(), 1);
}


void SJF(int count, char* test_file) {
	char * str = (char*)malloc(sizeof(char)*100);
	int j = 0;
	
    ProceduresQueue * q = getProceduresQueue();
	Procedure * Pro = malloc(sizeof(Procedure)); 
	
	char ** paths = (char **)malloc(sizeof(char*)*count);
	char ** nums = (char **)malloc(sizeof(char*)*count);
	
	for (int i=0; i<count; i++) {
		paths[i] = (char *)malloc(sizeof(char)*100);
		nums[i] = (char *)malloc(sizeof(char)*100);
	}
	FILE *fptr2 = fopen(test_file,"r");
	
	while (fgets(str, 100, fptr2) != NULL) {
		
	    char * path = strtok(str, "\t");
		strncpy(paths[j], path, 100);
		
		char * useless = strtok(path, "work");
		useless = strtok(NULL, "work");
		char * num = strtok(NULL, "work");
		
		strncpy(nums[j], num, 100);
		j++;
    }
    
    for (int i=0; i<count; i++) {
    
    	char temp_path[100];
    	char temp_num[100];
    	long min_value = 1e10;
    	long index = 0;
    	
    	for (int k=0; k<count; k++) {
    		int thenum = atoi(nums[k]);
    		if ((thenum > 0) && (thenum < min_value)) {
    			index = k;
    			min_value = thenum;
    		}
    	}
    	strncpy(temp_path, paths[index], 100);
    	strncpy(temp_num, nums[index], 100);
    	strncpy(nums[index], "-1", 2);
    			
    	strncpy(Pro->ProName, temp_path, 100);
		strncpy(Pro->ProStatus, "READY", 100);
    	Pro->ProPrio = -1;
		Pro->ProExeTime = -1;
		Pro->ProId = -1;
		Pro->ProC = -1;

		add_element(q, *Pro);
    }
	fclose(fptr2);
	execFCFS(q, get_wtime(), 1);
}


void RR(int count, char* test_file, int quant) {
	char * str = (char*)malloc(sizeof(char)*100);
	int j = 0;
    ProceduresQueue * q = getProceduresQueue();
	Procedure * Pro = malloc(sizeof(Procedure)); 
	FILE *fptr2 = fopen(test_file,"r");
	
	while (fgets(str, 100, fptr2) != NULL) {
		
	    char * path = strtok(str, "\t");
	    strncpy(Pro->ProName, path, 100);
	    
	    char * useless = strtok(path, "work");
		useless = strtok(NULL, "work");
		char * num = strtok(NULL, "work");
		
		strncpy(Pro->ProStatus, "READY", 100);
		Pro->ProPrio = -1;
		Pro->ProExeTime = -1;
		Pro->ProId = -1;		
		Pro->ProC = (int)ceil((atoi(num)*1000)/(double)quant);

		add_element(q, *Pro);
		j++;
    }
	fclose(fptr2);
	execQRR(q, quant, get_wtime(), 1);
}



void PRIO(int count, char* test_file, int quant) {
	int j = 0;
	char * str = (char*)malloc(sizeof(char)*100);

    ProceduresQueue * q = getProceduresQueue();
	Procedure * Pro = malloc(sizeof(Procedure)); 

	char ** paths = (char **)malloc(sizeof(char*)*count);
	char ** nums = (char **)malloc(sizeof(char*)*count);
	char ** prios = (char **)malloc(sizeof(char*)*count);
	
	for (int i=0; i<count; i++) {
		paths[i] = (char *)malloc(sizeof(char)*100);
		nums[i] = (char *)malloc(sizeof(char)*100);
		prios[i] = (char *)malloc(sizeof(char)*100);
	}
	
	FILE *fptr2 = fopen(test_file,"r");
	while (fgets(str, 100, fptr2) != NULL) {
	    char * path = strtok(str, "\t");
	    strncpy(paths[j], path, 100);
	    
	    char * prio = strtok(NULL, "\t");
	    strncpy(prios[j], prio, 100);
	    
	    char * useless = strtok(path, "work");
		useless = strtok(NULL, "work");
		char * num = strtok(NULL, "work");
	
		strncpy(nums[j], num, 100);		
		j++;
    }
    
    for (int i=0; i<count; i++) {
    	char temp_path[100];
    	char temp_num[100];
    	char temp_prio[100];
    	long min_value = 1e10;
    	long index = 0;
    	
    	for (int k=0; k<count; k++) {
    		int thenum = atoi(prios[k]);
    		if ((thenum > 0) && (thenum < min_value)) {
    			index = k;
    			min_value = thenum;
    		}
    	}
    	strncpy(temp_path, paths[index], 100);
    	strncpy(temp_num, nums[index], 100);
    	strncpy(temp_prio, prios[index], 100);
    	strncpy(prios[index], "-1", 2);
    			
    	strncpy(Pro->ProName, temp_path, 100);
		strncpy(Pro->ProStatus, "READY", 100);
    	Pro->ProPrio = atof(temp_prio);
		Pro->ProExeTime = -1;
		Pro->ProId = -1;
		Pro->ProC = (int)ceil((atoi(temp_num)*1000)/(double)quant);

		add_element(q, *Pro);
    }
	fclose(fptr2);

	int prioC = prioNo(q);
	
	ProceduresQueue ** arr = (ProceduresQueue **)malloc(sizeof(ProceduresQueue *)*prioC);
	for (int j=0; j<prioC; j++)
		arr[j] = getProceduresQueue();
	
	int i = -1;
	double last = -1;
	QueueNode * node = q->head;
	Procedure pr;
	while(node != NULL) {
		pr = dequeue(q);
		if (last != pr.ProPrio) 
			i++;
		last = pr.ProPrio;
		add_element(arr[i], pr);
		node = node->next;
	}
	
	double t0 = get_wtime();
	int pr_last = 0;
	
	for (int j=0; j<prioC; j++) {
		if (j==prioC-1) 
			pr_last = 1;
	
		if (isSize(arr[j]) == 1) 
			execFCFS(arr[j], t0, pr_last);
		else 
			execQRR(arr[j], quant, t0, pr_last);
	}
}


int lines_count(char * test_file) {
	FILE *fptr = fopen(test_file,"r");
	if(fptr == NULL) {
		printf("FILE DOES NOT EXIST!!!");
		exit(1);
	}
	
	int count = 0;
	char ch;
	for (ch = getc(fptr); ch != EOF; ch = getc(fptr))
		if (ch == '\n') 
			count = count + 1;
	fclose(fptr);
	return count;
}


int main(int argc, char *argv[]) {
	//make file
	chdir("../work");
	
	int pid = fork();
	if (pid == 0) {
		int garbage = open("/dev/null", O_WRONLY);
    	dup2(garbage, 1);
    	dup2(garbage, 2);
    	close(garbage);
		execl("/usr/bin/make", "make", "-f", "Makefile", NULL);
		exit(1);
	}
	waitpid(-1, NULL, 0);
	chdir("../scheduler");
	
	/* local variables */
	//read arguments
	int count;
	char * input_filename;
	char * policy = argv[1];
	int quant;
	
	/* parse input arguments (policy, quantum (if required), input filename */
	if (argc==4) {
		quant = atof(argv[2]);
		input_filename = argv[3];
	} else if (argc==3) {
		input_filename = argv[2];
	} else {
		printf("Invalid number of parameters!!!\n");
		exit(1);
	}
    printf("\n---> %s <---\n\n", policy);

	
	/* read input file - populate queue */
	/* call selected scheduling policy */
	/* print information and statistics */
	count = lines_count(input_filename);
    if (argc==4) {
		if (strcmp(policy,"RR")==0)
			RR(count, input_filename, quant);
		else if (strcmp(policy,"PRIO")==0)
			PRIO(count, input_filename, quant); 
	} else if (argc==3) {
		if (strcmp(policy,"FCFS")==0 || strcmp(policy,"BATCH")==0)
			FCFS(count, input_filename);
		else if (strcmp(policy,"SJF")==0)
			SJF(count, input_filename);
	}
	
	chdir("../work");
	int garbage = open("/dev/null", O_WRONLY);
    dup2(garbage, 1);
    dup2(garbage, 2);
    close(garbage);
	execl("/usr/bin/make","make","-f","Clean",NULL);
	chdir("../scheduler");
	
  	return 0;
}
