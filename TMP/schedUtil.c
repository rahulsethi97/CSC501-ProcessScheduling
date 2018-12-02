#include <lab1.h>
#include <conf.h>
#include <kernel.h>
#include <stdio.h>
#include <q.h>
#include <proc.h>

int sched_class; //0 - default , 1 - EXPDISTSCHED and 2 - LINUXSCHED
int firstEpoch;
int epoch;
int firstEpochGlobal;

void setschedclass(int flag){
	sched_class = flag;
}

int getschedclass(){
	return sched_class;
}

int eDistNextProc(int randPrio){
	
	if(randPrio > q[q[rdytail].qprev].qkey){
		return q[rdytail].qprev;
	}

	int retProc = q[rdyhead].qnext;
	
	while((randPrio > q[retProc].qkey) && (retProc < NPROC)){
		retProc = q[retProc].qnext;
	}

	if(retProc < NPROC)
		return retProc;
	else
		return 0; // null process
}

void printReadyQueue(){
	int i = q[rdyhead].qnext;
	struct pentry *procEntry;
 
	while( i < NPROC ){

		procEntry = &proctab[i];
		kprintf("%d , %d, %s | " , i , procEntry -> pprio , procEntry -> pname);
		i = q[i].qnext;
	}
	kprintf("\n");
}

int areAllProcessesOver(){
	if(q[q[rdytail].qprev].qkey <= 0)
		return 1;
	return 0;
}

void initiateEpoch(){
	epoch = 0;
	int nPtr = q[rdyhead].qnext;
	//printReadyQueue();
	
	int size = 0;
	int queue[NPROC];
	
	//Its made to avoid infinite loop
	while (nPtr < NPROC && proctab[nPtr].pstate == PRREADY){
		queue[size++] = nPtr;
		dequeue(nPtr);
		nPtr = q[nPtr].qnext;
	}	
	int i=0;
	for(i = 0; i< size ; i++){
		//kprintf("%d\n" , nPtr);
		register struct	pentry	*next;
		next = &proctab[queue[i]];	
		if(next -> newprio != 0 && next -> newprio != next -> pprio)
			next -> pprio = next -> newprio;

		if (next -> ctr < 0)
			next -> ctr = 0; 
		
		next -> qTime = ((next -> ctr)/2) + next -> pprio;
		next -> ctr = next -> qTime;
		next -> goodness = next -> ctr + next -> pprio;
		epoch += next -> qTime;
		insert(queue[i],rdyhead,next -> goodness);	
	}
	firstEpochGlobal++;
	firstEpoch = 1;
}

int isOver(int a){
	if( a <= 0)
		return 1;
	return 0;
}
