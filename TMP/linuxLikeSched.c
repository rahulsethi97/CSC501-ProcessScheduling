#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lab1.h> //Custom code


extern int ctxsw(int, int, int, int);

int firstEpoch;
int firstEpochGlobal;
int epoch;

int linuxLikeSched(){
        register struct pentry  *optr;  /* pointer to old process entry */
        register struct pentry  *nptr;  /* pointer to new process entry */
	//printReadyQueue();
        optr = &proctab[currpid];
		if(optr -> pstate == PRFREE && q[rdytail].qprev == 0){
			//kprintf("%d\n" , &currpid );
			currpid = 0;
			
	                nptr = &proctab[currpid];
	                nptr->pstate = PRCURR;
			#ifdef  RTCLOCK
	       			 preempt = QUANTUM;              /* reset preemption counter     */
			#endif

	                ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
	                return OK;
		}
        //kprintf("Inside\n");
        if( isOver(preempt)){
                preempt = QUANTUM;

                //If NULL process is running and readyqueue is not empty
                if(currpid == 0 && q[rdyhead].qnext < NPROC){
                        //if all processes have key <= 0
                        if( areAllProcessesOver() ){
                                initiateEpoch();
                                return OK;
                        }else{
                                register struct pentry  *nptr;
                                if (optr->pstate == PRCURR) {
                                        optr->pstate = PRREADY;
                                        insert(currpid,rdyhead,optr->goodness);
                                }

                                nptr = &proctab[ (currpid = getlast(rdytail)) ];
                                nptr->pstate = PRCURR;
				#ifdef  RTCLOCK
       					 preempt = QUANTUM;              /* reset preemption counter     */
				#endif

                                ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
                                return OK;
                        }
                }else{

                //This is the first time after initialising the epoch
			if(firstEpoch == 1)
                        	firstEpoch = 0;
                	else{
                        	epoch = epoch - QUANTUM;
                        	optr -> ctr -= QUANTUM;
				if((optr -> ctr) <= 0)
                                	optr -> goodness = 0;
                        	else
                                	optr -> goodness = optr -> ctr + optr -> pprio;
                	}
		}
			
        }


	if(epoch <= 0){
                //kprintf("c\n");
                initiateEpoch();
                //kprintf("d\n");
                return OK;
        }else{
                /*if(q[rdyhead].qnext >= NPROC && currpid == 0)
                        return OK;*/

                if((optr -> pstate == PRCURR) && (optr -> goodness > q[q[rdytail].qprev].qkey) && (optr -> ctr > 0))
                        return OK;

                preempt = QUANTUM;

                register struct pentry  *nptr;
                if (optr->pstate == PRCURR) {
                        optr->pstate = PRREADY;
                        insert(currpid,rdyhead,optr->goodness);
                }
		int temp = currpid;
		currpid = getlast(rdytail);
		
		if(currpid > NPROC)
			currpid = 0;

                nptr = &proctab[currpid];
                nptr->pstate = PRCURR;

		#ifdef  RTCLOCK
        		preempt = QUANTUM;              /* reset preemption counter     */
		#endif
		

                ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
                return OK;
        }
}
