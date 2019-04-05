/*
 * eventTest.cpp
 *
 *  Created on: 25-Feb-2019
 *      Author: VivekK4
 */

// WORKING solution for getting touch location

/* 	TOP LEFT CORNER
	0000000 8118 187f 6a81 8118 1800 00b7 0009 2900
	0000010 ff00 0109 2900 ff00 0109 0900 0001 0109
	0000020 0900 0001 0109 1900 00ff ff19 1900 00ff
	0000030 0008 3800 fcfa 0009 1900 00ff 0109 2900
	0000040 ff00 0109 2900 ff00 ff19 0800 0000 fc38
	0000050 09fe 0000 0009 1901 00ff 0008 2800 ff02
	0000060 0009 0800 0000 0128 09fd 0000 ff19 1900
	0000070 00ff 0008 3800 fbfe 0009 0900 0100 0009
	0000080 0801 0000 0608 0903 0000 ff19 1900 00ff
	0000090 ff19 1900 00ff 0008 0800 0100 0009 1900
	00000a0 00ff 0029 08ff 0000 1408 090b 0000 ff19
	00000b0 0800 0000 fb38 09f2 0000 0009 1901 00ff
	00000c0 0009 0901 0001 0009 0801 0000 0d08 090a
	00000d0 0000 ff19 0900 0100 0109 0900 0101 0109
	00000e0 0900 0100 0008 3800 f6fb 0009 1900 00ff
	00000f0 0008 2800 ff05 0009 0800 0000 0028 09fe
	0000100 0000 ff19 1900 00ff 0009 0801 0000 0308
	0000110 0903 0000 ff19 0900 0100 0009 0801 0000
	0000120 fa38 09f9 0000 ff19 1900 00ff 0008 0800
	0000130 0106 0009 1900 00ff ff19 0801 0000 fe38
	0000140 09fc 0000 ff19 1900 00ff 0129 08ff 0000
*/


/* 	BOTTOM RIGHT CORNER
	0000000 d518 0917 0000 ff19 2900 ff00 0009 0901
	0000010 0100 0008 1800 05fc 0009 0900 0001 0109
	0000020 0900 0101 0209 0901 0101 0109 0900 0100
	0000030 0109 0900 0101 0109 0900 0001 0209 0901
	0000040 0001 0109 0901 0102 0109 0900 0102 0209
	0000050 0901 0001 0008 0800 0607 0009 0900 0100
	0000060 0008 0800 0102 0009 1900 00ff 0009 0801
	0000070 0000 fd38 09fb 0000 ff19 3900 ffff 0029
	0000080 19ff 00ff ff39 39ff fffe ff19 1900 00ff
	0000090 ff19 1900 01ff ff19 1900 01ff ff19 0900
	00000a0 0001 0109 0901 0001 0109 0901 0101 0009
	00000b0 0901 0001 0109 0900 0100 0109 0900 0100
	00000c0 0109 0800 0000 0808 0902 0000 ff19 0900
	00000d0 0100 ff19 0800 0000 0228 09ff 0000 ff19
	00000e0 1900 01ff 0009 1901 01ff 0008 0800 0405
	00000f0 0009 0900 0100 0009 1901 00ff 0029 19ff
	0000100 00ff ff19 2900 ff00 0109 0900 0001 0009
	0000110 0901 0001 0029 19ff 00ff 0008 2800 fe02
	0000120 0009 0900 0001 0008 3800 fbf8 0009 1900
	0000130 00ff 0109 0900 0101 0109 0800 0000 0608
	0000140 0903 0000 0009 1901 00ff 0008 2800 ff01
	0000150 0009 1900 00ff 0009 1901 00ff 0008 0800
	0000160 0004 0009 1900 00ff 0008 0800 0002 0009
	0000170 0900 0101 0109 0900 0101 0109 0900 0100
	0000180 0109 0900 0100 0009 1901 01ff 0009 1901
	0000190 00ff 0009 1901 00fe 0008 3800 fcfa 0009
	00001a0 0900 0001 0009 0901 0100 0109 0900 0001
	00001b0 0109 0900 0001 0009 0901 0001 ff19 2900
	00001c0 ff00 0109 0900 0001 0109 0900 0001 0009
	00001d0 1901 00ff ff19 1900 00ff ff19 0900 0100
	00001e0 0109 2900 ff00 0109 2900 ff01 0009 1901
	00001f0 00ff ff19 1900 00ff ff19 1900 00ff 0109
	0000200 0900 0001 0109 0900 0100 ff19 1900 01ff
	0000210 0008 1800 11e8 0009 1900 00ff 0029 19ff
	0000220 00ff 0029 19ff 00ff 0029 19ff 00ff 0029
	0000230 29ff ff00 0029 29ff ff00 0029 29ff fe01
	0000240 0029 29ff ff00 0029 29fe fe00 0029 29ff
	0000250 ff00 ff19 3900 ffff fe39 19ff 00ff fe19
	0000260 1900 00ff fd39 19ff 00ff fd39 39ff fefe
	0000270 0029 39ff feff ff39 29fe ff01 0029 09fe
	0000280 0001 0129 09ff 0001 0309 0900 0001 0209
	0000290 0901 0202 0109 0901 0101 0009 0903 0200
	00002a0 0009 1901 01fe fe19 1901 00ff ff39 39ff
	00002b0 fffe ff39 39ff fdff ff39 29fc fe00 0029
	00002c0 29fc fc00 0129 29ff fe01 0029 29ff ff02
	00002d0 0109 0900 0101 0009 0901 0200 0009 1901
	00002e0 01ff ff19 3900 ffff ff39 39ff fdfe ff39
	00002f0 39fe fcff ff39 39fd ffff 0029 29fe ff00
	0000300 ff19 0900 0200 0009 1902 02ff fe19 1901
	0000310 00ff fe39 39ff feff ff39 39ff fcfd fe39
	0000320 29fc fe00 ff39 29fc fc00 0029 29ff ff02
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>

#define EVENT_DEVICE    "/dev/input/event0"
#define EVENT_TYPE      EV_ABS
#define EVENT_CODE_X    ABS_X
#define EVENT_CODE_Y    ABS_Y

/**
 * function to send touch events
 * @param	[int]	event file descriptor eg. /dev/input/event0
 * @param	[int]	X axis
 * @param	[int]	Y axis
 */
void sendTouchEvent(int &, int , int );
/**
 * function to initialize event pointer
 * @param	[int]	input to file descriptor of event[out]
 * @param	[char *]	name of device to which we are reading
 */
int initEventFD(int &, char *);
/* TODO: Close fd on SIGINT (Ctrl-C), if it's open */
int __main(int argc, char *argv[])
{
    struct input_event ev;
    int fd;
    char name[256] = "Unknown";

    if ((getuid ()) != 0) {
        fprintf(stderr, "You are not root! This may not work...\n");
//        return EXIT_SUCCESS;
    }

    /* Open Device */
    fd = open(EVENT_DEVICE, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "%s is not a vaild device\n", EVENT_DEVICE);
        return EXIT_FAILURE;
    }

    /* Print Device Name */
    ioctl(fd, EVIOCGNAME(sizeof(name)), name);
    printf("Reading from:\n");
    printf("device file = %s\n", EVENT_DEVICE);
    printf("device name = %s\n", name);

    for (;;) {
        const size_t ev_size = sizeof(struct input_event);
        ssize_t size;

        /* TODO: use select() */

        size = read(fd, &ev, ev_size);
        if (size < ev_size) {
            fprintf(stderr, "Error size when reading\n");
            goto err;
        }

        printf("Event Code = %d\n", ev.code);
//        if (ev.type == EVENT_TYPE && (ev.code == EVENT_CODE_X
//                      || ev.code == EVENT_CODE_Y)) {
//            /* TODO: convert value to pixels */
//            printf("%s = %d\n", ev.code == EVENT_CODE_X ? "X" : "Y",
//                    ev.value);
//        }
    }

    return EXIT_SUCCESS;

err:
    close(fd);
    return EXIT_FAILURE;
}



void sendTouchEvents(int &fd, int X, int Y)
{
	struct input_event ev;
	ev.type =  EV_ABS;
	ev.code = ABS_X;
	ev.value = X;

	//write X axis to file
	//write(fd, ev, sizeof(struct input_event));


}

int initEventFD(int &fd, char *name )
{
	fd = open(EVENT_DEVICE, O_RDWR);
	if (fd == -1) {
		fprintf(stderr, "%s is not a vaild device\n", EVENT_DEVICE);
		return EXIT_FAILURE;
	}

	// Set name of device
	ioctl(fd, EVIOCGNAME(sizeof(name)), name);
	// init completed return success
	return EXIT_SUCCESS;
}
