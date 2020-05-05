#include "msp.h"
#include "BSP.h"

/**
 * main.c
 */
void main(void)
{
    BSP_InitBoard();
    getRainData(0);
    while(1);
}
