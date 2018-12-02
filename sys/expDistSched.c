/*
 * Author: rsethi3
 */

#include<conf.h>
#include<kernel.h>
#include<proc.h>
#include<q.h>
#include<lab1.h>

extern int ctxsw(int, int, int, int);


int expDistSched(double lambda){
	

        register struct pentry  *optr;  /* pointer to old process entry */
        register struct pentry  *nptr;  /* pointer to new process entry */

        optr = &proctab[currpid];

	if(optr -> pstate == PRFREE && q[rdytail].qprev == 0){
                currpid = 0;

                nptr = &proctab[currpid];
                nptr->pstate = PRCURR;
                ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
                return OK;
        }


        //printReadyQueue();
        int randPrio = (int) expdev(lambda);
        int newProc = eDistNextProc(randPrio);
        //kprintf("%d %d\n" , randPrio , newProc);
        optr = &proctab[currpid];
        if ( (optr -> pstate == PRCURR) && ( ( (q[newProc].qkey > optr->pprio) && (optr->pprio > randPrio)) )){
        //It means if current process would have been in the ready queue it would have been selected
        	#ifdef	RTCLOCK
			preempt = QUANTUM;
		#endif
                return OK;
        }

        if(optr -> pstate == PRCURR){
                optr -> pstate = PRREADY;
                insert(currpid , rdyhead , optr->pprio);
        }

        if(newProc < NPROC)
                dequeue(newProc);
        else{
                newProc = EMPTY;
        }

        currpid = newProc;
        nptr = &proctab[newProc];
        nptr->pstate = PRCURR;          /* mark it currently running    */
#ifdef  RTCLOCK
        preempt = QUANTUM;              /* reset preemption counter     */
#endif
        ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
        return OK;
}
