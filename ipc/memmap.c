#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

#define PAGESIZE    (4096)
#define MMAP_PROT   (PROT_READ | PROT_WRITE)
#define MMAP_FLAGS  (MAP_SHARED | MAP_ANONYMOUS)

int main(void)
{
    uint8_t * shared_mem = mmap(NULL, PAGESIZE, MMAP_PROT, MMAP_FLAGS, -1, 0);
    
    if (fork() == 0) {
        ++(*shared_mem);
        printf("child  shared_mem == %u\n", *shared_mem);
    } else {
        ++(*shared_mem);
        printf("parent shared_mem == %u\n", *shared_mem);
        wait(NULL);
    }
}
