#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MEM_SIZE (1024)

#define WR_DATA _IOW('a', 'a', int32_t*)
#define RD_DATA _IOR('a', 'b', int32_t*)

int main(void)
{
    const int fd = open("/dev/my_device", O_RDWR);
    if (fd < 0) {
        printf("CharDev open failed: %d", fd);
        return fd;
    }

    // system calls
    char rd_buf[MEM_SIZE];
    char wr_buf[MEM_SIZE];
    printf("CharDev Write: ");
    scanf("%s", wr_buf);
    write(fd, wr_buf, strlen(wr_buf)+1);

    read(fd, rd_buf, strlen(rd_buf)+1);
    printf("CharDev Read : %s\n", rd_buf);

    // ioctl
    int32_t num = 0;
    printf("CharDev IOCTL set value: ");
    scanf("%d", &num);
    ioctl(fd, WR_DATA, (int32_t*)&num);

    printf("CharDev IOCTL get value: ");
    ioctl(fd, RD_DATA, (int32_t*)&num);
    printf("%d\n", num);

    return 0;
}
