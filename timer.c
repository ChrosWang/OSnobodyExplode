//Bomb disposal-the timer
//Chros Wang
//Ideas from the game Keep talking and nobody explode. 
//Sample run: auto run, you don't need to run this
//Date: 05/12/2020
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct ipcmsg {
    // The first field must be of type "long", as required by SystemV IPC.
    long msgtype;    // IPC message type (1 = register, 2 = report, 3 = reset, etc.)
    int eventid;     // the event type ID
    //char data[0];    // other data (zero or more bytes)
};
// NOTE: the data array is declared here as an array of length zero. In the code
// below, the actual size may be zero (if there is no data for the message) but
// it will often be somze be some larger size.

// A "message" is everything in the above struct.
// For a message carrying n bytes of "other data", the total message size is:
#define MSG_SIZE sizeof(struct ipcmsg)

// The "payload" is everything in the above struct except the required first
// long integer.
// For a message carrying n bytes of "other data", the "payload sizeC" is:
#define MSG_PAYLOAD_SIZE sizeof(struct ipcmsg) - sizeof(long)
int send_a_message(int msgtype, int eventid, int q) {
    struct ipcmsg *m = (struct ipcmsg *)malloc(MSG_SIZE);
        m->msgtype = msgtype; // 1 means "register"
        m->eventid = eventid;
        if (msgsnd(q, m, MSG_PAYLOAD_SIZE, 0) < 0) {
            //perror("msgsnd");
            //printf("Can't send IPC message.\n");
            exit(0);
            return -1;
        }
        free(m);
        return 0;
}


int main(int argc, char *argv[]) {
	struct timespec t_start;
	clock_gettime(CLOCK_REALTIME, &t_start);
	struct timespec t_now;
    //struct timespec t_end; 
	int total = atoi(argv[1]); 
	int elapse = 0; 
	int q = atoi(argv[2]); 
    while(total - elapse > 0){
    	usleep(1000000);

    	clock_gettime(CLOCK_REALTIME, &t_now);
    	int cur_seconds = t_now.tv_sec;
    	int born_seconds = t_start.tv_sec;
    	elapse = cur_seconds - born_seconds;
    	if (elapse%10 == 0 || total - elapse < 6) {
    		send_a_message(5, total-elapse, q);
    	}

    }
    send_a_message(5, -1, q);
}
