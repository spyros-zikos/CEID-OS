// Zikos Spyridon 1084581
// Kyriakoulopoulos Kallinikos 1084583
// Makris Orestis-Antonis 1084516

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <math.h>


double get_wtime(void) {
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec*1.0e-6;
}

double f(double x) {
    return log(x)*sqrt(x);
}

double get_integral(double a, double b, int n) {
    const double dx = (b-a)/n;
    double S = 0;
    
    for (unsigned long i = 0; i < n; i++) {
        double xi = a + (i + 0.5)*dx;
        S += f(xi);
    }
    S *= dx;
    return S;
}

void server(double S) {  
	int key = ftok("ask2.c", 61235);
	int msgid = msgget(key, 0666 | IPC_CREAT);

	msgsnd(msgid, &S, sizeof(double), 0);
    exit(1);
}

double client() {
    int key = ftok("ask2.c", 61235);
    int msgid = msgget(key, 0666);
    
    double k;
    msgrcv(msgid, &k, sizeof(double), 0, 0);
    return k;
}

int main(int argc, char *argv[]) {
    double a = 1.0;
    double b = 4.0;

    unsigned long const n = 1e9;
    int procedures = atoi(argv[1]);
    long int size_of_procedure = ceil(n / procedures);

    int pid;
    double total = 0;
    double t0 = get_wtime();

    for(int i=0; i<procedures; i++) {
        pid = fork();
        if (pid == 0) {
            double S = get_integral(a+((b-a)/procedures)*i, a+((b-a)/procedures)*(i+1), size_of_procedure);
            server(S);
        }
    }

    for(int i=0; i<procedures; i++) 
        if (pid > 0) {
            waitpid(-1, NULL, 0);
            total += client();
        }

    int key = ftok("ask2.c", 61235);
    int msgid = msgget(key, 0666);
    msgctl(msgid, IPC_RMID, NULL);

    double t1 = get_wtime();

    printf("Time=%lf seconds, Result=%.8f\n", t1-t0, total);
    return 0;
}