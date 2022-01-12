#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/io.h>

#define BASEPORT (0x378)  /* /dev/lp1 */

int main(void)
{
    // access port
    ioperm(BASEPORT, 3, 1);
    perror("ioperm open ");

    // read write
    const unsigned char val_out = 23;
    outb(val_out, BASEPORT);
    printf("\n\twrite = %d\n", val_out);
    
    usleep(100000);

    // read value
    const unsigned char val_in = inb(BASEPORT+1);
    printf("\n\tread  = %d\n", val_in);

    // close port
    ioperm(BASEPORT, 3, 1);
    perror("ioperm close");

    return 0;
}
