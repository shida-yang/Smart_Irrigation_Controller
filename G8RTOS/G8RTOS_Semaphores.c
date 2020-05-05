/*
 * G8RTOS_Semaphores.c
 */

/*********************************************** Dependencies and Externs *************************************************************/

#include <stdint.h>
#include "msp.h"
#include "G8RTOS_Semaphores.h"
#include "G8RTOS_CriticalSection.h"
#include "G8RTOS.h"

/*********************************************** Dependencies and Externs *************************************************************/


/*********************************************** Public Functions *********************************************************************/

/*
 * Initializes a semaphore to a given value
 * Param "s": Pointer to semaphore
 * Param "value": Value to initialize semaphore to
 * THIS IS A CRITICAL SECTION
 */
void G8RTOS_InitSemaphore(semaphore_t *s, int32_t value)
{
	/* Implement this */
    int32_t flags=StartCriticalSection();
    *s=value;
    EndCriticalSection(flags);
}

/*
 * No longer waits for semaphore
 *  - Decrements semaphore
 *  - Blocks thread is sempahore is unavalible
 * Param "s": Pointer to semaphore to wait on
 * THIS IS A CRITICAL SECTION
 */
void G8RTOS_WaitSemaphore(semaphore_t *s)
{
	/* Implement this */
    int32_t flags=StartCriticalSection();
    (*s)--;
    if((*s)<0){
        CurrentlyRunningThread->blocked=s;
        EndCriticalSection(flags);
        SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
    }
    else{
        EndCriticalSection(flags);
    }
}

/*
 * Signals the completion of the usage of a semaphore
 *  - Increments the semaphore value by 1
 *  - Unblocks any threads waiting on that semaphore
 * Param "s": Pointer to semaphore to be signaled
 * THIS IS A CRITICAL SECTION
 */
void G8RTOS_SignalSemaphore(semaphore_t *s)
{
	/* Implement this */
    int32_t flags=StartCriticalSection();
    (*s)++;
    if((*s)<=0){
        tcb_t* tcb_ptr=CurrentlyRunningThread->nextTCB;
        while(tcb_ptr->blocked != s){
            tcb_ptr=tcb_ptr->nextTCB;
        }
        tcb_ptr->blocked=0;
    }
    EndCriticalSection(flags);
}

/*********************************************** Public Functions *********************************************************************/


