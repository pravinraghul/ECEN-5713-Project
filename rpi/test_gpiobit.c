#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#define GPIOBIT_IOC_MAGIC 0x15
#define GPIOBIT_CLKPIN _IOWR(GPIOBIT_IOC_MAGIC, 1, unsigned long)
#define GPIOBIT_DATAPIN _IOWR(GPIOBIT_IOC_MAGIC, 2, unsigned long)

int main (int argc, char *argv[]) {
    int fd, ret, pin, value;
    
    if (argc != 3) {
        printf("Usage: test-gpiobit <1|2> <value>\n");
        printf("1 - clock pin\n");
        printf("2 - data pin\n");
        return 1;
    }

    pin = atoi(argv[1]);
    if (pin != 1 && pin != 2) {
        printf("invalid pin number\n");
        return 1;
    }
    value = atoi(argv[2]);
    fd = open("/dev/gpiobit", O_RDWR);
    if (fd < 0) {
        printf("failed to open\n");
    }

    pin = (pin==1) ? GPIOBIT_CLKPIN: GPIOBIT_DATAPIN;
    printf("pin: %d\n", pin);
    ret = ioctl(fd, pin, (int*) &value);
    if (ret != 0) {
        printf("ioctl failed\n");
        return 1;
    }
    printf("ioctl successful\n");
    close(fd);
    return 0;
}