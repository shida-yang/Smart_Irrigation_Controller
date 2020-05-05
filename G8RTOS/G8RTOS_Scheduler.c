/*
 * G8RTOS_Scheduler.c
 */

/*********************************************** Dependencies and Externs *************************************************************/

#include <stdint.h>
#include "msp.h"
#include "G8RTOS_Scheduler.h"

/*
 * G8RTOS_Start exists in asm
 */
extern void G8RTOS_Start();

/* System Core Clock From system_msp432p401r.c */
extern uint32_t SystemCoreClock;

/*
 * Pointer to the currently running Thread Control Block
 */
extern tcb_t * CurrentlyRunningThread;

/*********************************************** Dependencies and Externs *************************************************************/


/*********************************************** Defines ******************************************************************************/

/* Status Register with the Thumb-bit Set */
#define THUMBBIT 0x01000000

/*********************************************** Defines ******************************************************************************/


/*********************************************** Data Structures Used *****************************************************************/

/* Thread Control Blocks
 *	- An array of thread control blocks to hold pertinent information for each thread
 */
static tcb_t threadControlBlocks[MAX_THREADS];

/* Thread Stacks
 *	- An array of arrays that will act as invdividual stacks for each thread
 */
//#pragma DATA_SECTION(threadStacks, "TCB_STACK");
static int32_t threadStacks[MAX_THREADS][STACKSIZE];

/* Periodic Event Threads
 * - An array of periodic events to hold pertinent information for each thread
 */
static ptcb_t Pthread[MAXPTHREADS];

/*********************************************** Data Structures Used *****************************************************************/


/*********************************************** Private Variables ********************************************************************/

/*
 * Current Number of Threads currently in the scheduler
 */
static uint32_t NumberOfThreads;

/*
 * Current Number of Periodic Threads currently in the scheduler
 */
static uint32_t NumberOfPthreads;

/* Thread ID counter */
static uint32_t IDCounter;

/*********************************************** Private Variables ********************************************************************/


/*********************************************** Private Functions ********************************************************************/

/*
 * Initializes the Systick and Systick Interrupt
 * The Systick interrupt will be responsible for starting a context switch between threads
 * Param "numCycles": Number of cycles for each systick interrupt
 */
static void InitSysTick(uint32_t numCycles)
{
	/* Implement this */
    SysTick_Config(numCycles);
    SysTick_enableInterrupt();
}

/*
 * Chooses the next thread to run.
 * Lab 2 Scheduling Algorithm:
 * 	- Simple Round Robin: Choose the next running thread by selecting the currently running thread's next pointer
 * 	- Check for sleeping and blocked threads
 */
void G8RTOS_Scheduler()
{
	/* Implement This */

    uint16_t currentMaxPriority=256;
    tcb_t* tempNextThread=CurrentlyRunningThread->nextTCB;
    uint8_t i;
    for(i=0; i<NumberOfThreads; i++){
        if(tempNextThread->asleep==0 && tempNextThread->blocked==0){
            if(((uint16_t)(tempNextThread->priority))<currentMaxPriority){
                CurrentlyRunningThread=tempNextThread;
                currentMaxPriority=tempNextThread->priority;
            }
        }
        tempNextThread=tempNextThread->nextTCB;
    }
//    do{
//        CurrentlyRunningThread=CurrentlyRunningThread->nextTCB;
//    }while(CurrentlyRunningThread->blocked!=0 || CurrentlyRunningThread->asleep==1);
}


/*
 * SysTick Handler
 * The Systick Handler now will increment the system time,
 * set the PendSV flag to start the scheduler,
 * and be responsible for handling sleeping and periodic threads
 */
void SysTick_Handler()
{
	/* Implement this */
    int i;
    for(i=0; i<NumberOfPthreads; i++){
        if(Pthread[i].execute_time==SystemTime){
            Pthread[i].execute_time+=Pthread[i].period;
            (*(Pthread[i].Handler))();
        }
    }
    SystemTime++;
    tcb_t* ptr=CurrentlyRunningThread;
    for(i=0; i<NumberOfThreads; i++){
        if(ptr->asleep==1 && ptr->sleepCount==SystemTime){
            ptr->asleep=0;
            ptr->sleepCount=0;
        }
        ptr=ptr->nextTCB;
    }

    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

/*********************************************** Private Functions ********************************************************************/


/*********************************************** Public Variables *********************************************************************/

/* Holds the current time for the whole System */
uint32_t SystemTime;

/*********************************************** Public Variables *********************************************************************/


/*********************************************** Public Functions *********************************************************************/

/*
 * Sets variables to an initial state (system time and number of threads)
 * Enables board for highest speed clock and disables watchdog
 */
void G8RTOS_Init()
{
	/* Implement this */
    SysTick->VAL=0;     //set system time to 0
    NumberOfThreads=0;  //set number of threads to 0
    BSP_InitBoard();    //initialize all hardware

    // Relocate vector table to SRAM to use aperiodic events
    uint32_t newVTORTable = 0x20000000;
    memcpy((uint32_t *)newVTORTable, (uint32_t *)SCB->VTOR, 57*4);
    // 57 interrupt vectors to copy
    SCB->VTOR = newVTORTable;
}

/*
 * Starts G8RTOS Scheduler
 * 	- Initializes the Systick
 * 	- Sets Context to first thread
 * Returns: Error Code for starting scheduler. This will only return if the scheduler fails
 */
int G8RTOS_Launch()
{
	/* Implement this */
    /* Implement this */
    if(NumberOfThreads==0){
        return NO_THREADS_SCHEDULED;
    }
    uint8_t i;
    CurrentlyRunningThread=&threadControlBlocks[0];
    for(i=1; i<NumberOfThreads; i++){
        if(threadControlBlocks[i].priority<CurrentlyRunningThread->priority){
            CurrentlyRunningThread=&threadControlBlocks[i];
        }
    }

    InitSysTick(ClockSys_GetSysFreq()/1000);
    __NVIC_SetPriority(PendSV_IRQn, 255);
    __NVIC_SetPriority(SysTick_IRQn, 0);
    G8RTOS_Start();
    return NO_ERROR;
}


/*
 * Adds threads to G8RTOS Scheduler
 * 	- Checks if there are stil available threads to insert to scheduler
 * 	- Initializes the thread control block for the provided thread
 * 	- Initializes the stack for the provided thread to hold a "fake context"
 * 	- Sets stack tcb stack pointer to top of thread stack
 * 	- Sets up the next and previous tcb pointers in a round robin fashion
 * Param "threadToAdd": Void-Void Function to add as preemptable main thread
 * Returns: Error code for adding threads
 */
int G8RTOS_AddThread(void (*threadToAdd)(void), uint8_t priority, char* name)
{
	/* Implement this */
    int32_t primask=StartCriticalSection();
    //no more threads available
    if(NumberOfThreads>=MAX_THREADS){
        EndCriticalSection(primask);
        return THREAD_LIMIT_REACHED;
    }
    uint8_t i;
    for(i=0; i<MAX_THREADS; i++){
        if(threadControlBlocks[i].alive==0){
            break;
        }
    }
    if(i==MAX_THREADS){
        EndCriticalSection(primask);
        return THREADS_INCORRECTLY_ALIVE;
    }
    uint32_t* tempPtr=&threadStacks[i][STACKSIZE-1];

    //initialize PSR (T=1)
    *tempPtr=THUMBBIT;
    tempPtr--;
    //initialize PC
    *tempPtr=threadToAdd;
    tempPtr--;
    //LR
    *tempPtr=0xAAAA5555;
    tempPtr--;
    //initialize R0-R12, R14 to dummy data
    int j;
    for(j=0; j<13; j++){
        *tempPtr=0xAAAA5555;
        tempPtr--;
    }
    //move tempPtr to the last stacked item
    tempPtr++;
    //create new TCP
    //first thread
    if(NumberOfThreads==0){
        tcb_t tempTCB={
                &threadControlBlocks[i],
                &threadControlBlocks[i],
                tempPtr,
                0,       //not blocked
                0,      //not asleep
                0,      //sleep count
                1,      //alive
                priority,//priority
                name,
                (IDCounter<<16)|0
        };
        threadControlBlocks[i]=tempTCB;
    }
    //other threads
    else{
        //find a existing thread
        int j;
        for(j=0; j<MAX_THREADS; j++){
            if(threadControlBlocks[j].alive==1){
                break;
            }
        }
        tcb_t tempTCB={
                &threadControlBlocks[j],
                threadControlBlocks[j].prevTCB,
                tempPtr,
                0,       //not blocked
                0,      //not asleep
                0,      //sleep count
                1,      //alive
                priority,//priority
                name,
                (IDCounter<<16)|i
        };
        threadControlBlocks[i]=tempTCB;
//        threadControlBlocks[i].nextTCB=&threadControlBlocks[0];
//        threadControlBlocks[i].prevTCB=threadControlBlocks[0].prevTCB;
        //prev thread's next thread
        threadControlBlocks[j].prevTCB->nextTCB=&threadControlBlocks[i];
        //next thread's prev thread
        threadControlBlocks[j].prevTCB=&threadControlBlocks[i];
    }
    NumberOfThreads++;
    IDCounter++;
    EndCriticalSection(primask);
    return NO_ERROR;
}


/*
 * Adds periodic threads to G8RTOS Scheduler
 * Function will initialize a periodic event struct to represent event.
 * The struct will be added to a linked list of periodic events
 * Param Pthread To Add: void-void function for P thread handler
 * Param period: period of P thread to add
 * Returns: Error code for adding threads
 */
int G8RTOS_AddPeriodicEvent(void (*PthreadToAdd)(void), uint32_t period)
{
    /* Implement this */
    int32_t primask=StartCriticalSection();

    if(NumberOfPthreads>=MAXPTHREADS){
        EndCriticalSection(primask);
        return THREAD_LIMIT_REACHED;
    }
    //first Pthread
    if(NumberOfPthreads==0){
        Pthread[0].Handler=PthreadToAdd;
        Pthread[0].period=period;
        Pthread[0].execute_time=0;
        Pthread[0].nextPTCB=&Pthread[0];
        Pthread[0].prevPTCB=&Pthread[0];
    }
    //all others
    else{
        Pthread[NumberOfPthreads].Handler=PthreadToAdd;
        Pthread[NumberOfPthreads].period=period;
        Pthread[NumberOfPthreads].execute_time=NumberOfPthreads;
//        int i;
//        for(i=0; i<NumberOfPthreads; i++){
//            //common multiple
//            if(Pthread[i].period%period==0 || period%Pthread[i].period==0){
//                Pthread[NumberOfPthreads]
//            }
//        }
        Pthread[NumberOfPthreads].nextPTCB=&Pthread[0];
        Pthread[NumberOfPthreads].prevPTCB=&Pthread[NumberOfPthreads-1];
        Pthread[NumberOfPthreads-1].nextPTCB=&Pthread[NumberOfPthreads];
        Pthread[0].prevPTCB=&Pthread[NumberOfPthreads];
    }
    NumberOfPthreads++;

    EndCriticalSection(primask);
    return NO_ERROR;
}


/*
 * Puts the current thread into a sleep state.
 *  param durationMS: Duration of sleep time in ms
 */
void sleep(uint32_t durationMS)
{
    /* Implement this */
    CurrentlyRunningThread->sleepCount=SystemTime+durationMS;
    CurrentlyRunningThread->asleep=1;
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

threadId_t G8RTOS_GetThreadId(){
    return CurrentlyRunningThread->threadID;
}

sched_ErrCode_t G8RTOS_KillThread(threadId_t threadId){
    int32_t primask=StartCriticalSection();

    if(NumberOfThreads==1){
        EndCriticalSection(primask);
        return CANNOT_KILL_LAST_THREAD;
    }

    uint8_t i;
    tcb_t* temp_tcb_ptr=CurrentlyRunningThread;
    for(i=0; i<NumberOfThreads; i++){
        if((temp_tcb_ptr->threadID)==threadId){
            break;
        }
        temp_tcb_ptr=temp_tcb_ptr->nextTCB;
    }
    if(i==NumberOfThreads){
        EndCriticalSection(primask);
        return THREAD_DOES_NOT_EXIST;
    }

    temp_tcb_ptr->alive=0;
    temp_tcb_ptr->asleep=0;

    temp_tcb_ptr->prevTCB->nextTCB=temp_tcb_ptr->nextTCB;
    temp_tcb_ptr->nextTCB->prevTCB=temp_tcb_ptr->prevTCB;

    if(temp_tcb_ptr->blocked != 0){
        *(temp_tcb_ptr->blocked)++;
        temp_tcb_ptr->blocked=0;
    }

    NumberOfThreads--;

    EndCriticalSection(primask);

    if(threadId==CurrentlyRunningThread->threadID){
        SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
    }

    return NO_ERROR;
}

sched_ErrCode_t G8RTOS_KillSelf(){
    int32_t primask=StartCriticalSection();

    if(NumberOfThreads==1){
        EndCriticalSection(primask);
        return CANNOT_KILL_LAST_THREAD;
    }

    CurrentlyRunningThread->alive=0;
    CurrentlyRunningThread->asleep=0;

    CurrentlyRunningThread->prevTCB->nextTCB=CurrentlyRunningThread->nextTCB;
    CurrentlyRunningThread->nextTCB->prevTCB=CurrentlyRunningThread->prevTCB;

    if(CurrentlyRunningThread->blocked != 0){
        *(CurrentlyRunningThread->blocked)++;
        CurrentlyRunningThread->blocked=0;
    }

    NumberOfThreads--;

    EndCriticalSection(primask);

    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;

    return NO_ERROR;
}

sched_ErrCode_t G8RTOS_AddAPeriodicEvent(void(*AthreadToAdd)(void), uint8_t priority, IRQn_Type IRQn){
    int32_t primask=StartCriticalSection();

    if(IRQn>PORT6_IRQn || IRQn<PSS_IRQn){
        EndCriticalSection(primask);
        return IRQn_INVALID;
    }
    if(priority>6){
        EndCriticalSection(primask);
        return HWI_PRIORITY_INVALID;
    }
    __NVIC_SetVector(IRQn, AthreadToAdd);
    __NVIC_SetPriority(IRQn, priority);
    NVIC_EnableIRQ(IRQn);

    EndCriticalSection(primask);
    return NO_ERROR;
}

uint32_t getNumberOfThreads(){
    return NumberOfThreads;
}
/*********************************************** Public Functions *********************************************************************/
