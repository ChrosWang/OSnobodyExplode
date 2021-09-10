//Bomb disposal-the client
//Chros Wang
//Ideas from the game Keep talking and nobody explode. 
//Sample run: ./client1 1201
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
#include <stdbool.h>

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

char *readline(int fd) {
    char *s = malloc(256); // assumes line will be 255 characters or less
    int n = 0;
    while (1) {
        int r = read(fd, s + n, 1);
        if (r == 0) {
            printf("Line didn't end, but there is no more data available in file %d\n", fd);
            s[n] = '\0'; // add the terminating zero and return the string
            return s;
        } else if (r < 0) {
            printf("There was a problem reading from file %d: error number %d\n", fd, n);
            s[n] = '\0'; // add the terminating zero and return the string
            return s;
        } else if (s[n] == '\n') {
            s[n] = '\0'; // add the terminating zero and return the string
            return s;
        } else if (n == 255) {
            printf("Already read 255 characters, but line didn't end yet in file %d\n", fd);
            s[n] = '\0'; // add the terminating zero and return the string
            return s;
        } else {
            n++;
        }
    }
}



int send_a_message(int msgtype, int eventid, int q) {
    struct ipcmsg *m = (struct ipcmsg *)malloc(MSG_SIZE);
        m->msgtype = msgtype; // 1 means "register"
        m->eventid = eventid;
        if (msgsnd(q, m, MSG_PAYLOAD_SIZE, 0) < 0) {
            perror("msgsnd");
            printf("Can't send IPC message.\n");
            return -1;
        }
        free(m);
        return 0;
}

int main(int argc, char **argv)
{
    setbuf(stdout, NULL);

    //char* userID = initialize();
    char *ans = NULL;
    char *newline = NULL; 
  	if (argc != 2) {
        printf("Enter the same game room number when executing.\n");
        printf("usage: %s <room_number>\n", argv[0]);
        exit(1);
    }
    key_t key = atoi(argv[1]);

    // Open the mailbox queue, but don't create one if it doesn't exist yet.
    int q = msgget(key, 0);
    if (q == -1) {
        perror("msgget");
        printf("Can't open IPC mailbox queue.\n");
        exit(1);
    }
    printf("Opened game room number %d.\n", key);
   	bool game_start = false;
    while(1){
        printf("Next action: ");
       // printf("%p\n", ans);
        if (ans!= NULL)
        	free(ans); 
        ans = NULL;
        free(newline);
        newline = NULL;
        ans = readline(0);
        char* command = NULL; 
        char* arg = NULL;
        //user name 

        if (strcmp(ans, "help") == 0) {
        	send_a_message(6, 1, q);
        	continue; 
        }
        if (strcmp(ans, "check") == 0) {
        	send_a_message(6, 1, q);
        	continue; 
        }
        if (strcmp(ans, "press") == 0) {
        	send_a_message(6, 1, q);
        	continue; 
        }
        if (strcmp(ans, "cut") == 0) {
        	send_a_message(6, 1, q);
        	continue; 
        }
        if (strcmp(ans, "start") == 0) {
        	send_a_message(6, 1, q);
        	continue; 
        }
        command = strsep(&ans, " "); 
		arg = strsep(&ans, "\n"); 
		if (strcmp(command, "start") == 0) {
        	if (game_start) {
        	continue;
        	}
        	game_start = true;
        	//printf("here\n");
        	if (strcmp(arg, "random") == 0) 
        		send_a_message(4, 1, q);
        	else if(strcmp(arg, "1") == 0)
        		send_a_message(4, 2, q);
        	else if(strcmp(arg, "2") == 0)
        		send_a_message(4, 3, q);
        	else if(strcmp(arg, "3") == 0)
        		send_a_message(4, 4, q);

        	int cpid = fork(); 
    		if (cpid == 0) {
    			char mbox[10];
    			sprintf(mbox, "%d", q);
        		int ecode = execlp("./timer", "./timer", "300", mbox, NULL);
        		//free(mbox);
        		if (ecode == -1) {
            		printf("Failed to execute timer\n");
            		return -1;
        		}
    		}
        	continue; 
        }
		if (strcmp (command, "check") == 0) {
        	if (strcmp(arg, "serial number") == 0) {
            	send_a_message(1, 1, q);
            	continue;
       		} else if (strcmp(arg, "button") == 0 || strcmp(arg, "buttons") == 0) {
       			send_a_message(1, 2, q);
       			continue;
       		}
       		else if(strcmp(arg, "wire") == 0 || strcmp(arg, "wires") == 0) {
       			send_a_message(1, 3, q);
       			continue;
       		}
       		else if(strcmp(arg, "battery") == 0 || strcmp(arg, "batteries") == 0) {
       			send_a_message(1, 4, q);
       			continue;
       		} else {
       			send_a_message(1, 99, q);
       			continue;
       		}
    	}
    	if (strcmp (command, "press") == 0) {
        	if (strcmp(arg, "one") == 0 || strcmp(arg, "1") == 0) {
            	send_a_message(2, 1, q);
            	continue;
       		}
       		if (strcmp(arg, "two") == 0 || strcmp(arg, "2") == 0) {
            	send_a_message(2, 2, q);
            	continue;
       		}
       		if (strcmp(arg, "three") == 0 || strcmp(arg, "3") == 0) {
            	send_a_message(2, 3, q);
            	continue;
       		}
       		if (strcmp(arg, "four") == 0 || strcmp(arg, "4") == 0) {
            	send_a_message(2, 4, q);
            	continue;
       		}
       		if (strcmp(arg, "five") == 0 || strcmp(arg, "5") == 0) {
            	send_a_message(2, 5, q);
            	continue;
       		}
       		if (strcmp(arg, "six") == 0 || strcmp(arg, "6") == 0) {
            	send_a_message(2, 6, q);
            	continue;
       		}
       		if (strcmp(arg, "seven") == 0 || strcmp(arg, "7") == 0) {
            	send_a_message(2, 7, q);
            	continue;
       		}
       		if (strcmp(arg, "eight") == 0 || strcmp(arg, "8") == 0) {
            	send_a_message(2, 8, q);
            	continue;
       		}
       		if (strcmp(arg, "nine") == 0 || strcmp(arg, "9") == 0) {
            	send_a_message(2, 9, q);
            	continue;
       		}
       		if (strcmp(arg, "red") == 0) {
            	send_a_message(2, 11, q);
            	continue;
       		}
       		if (strcmp(arg, "blue") == 0) {
            	send_a_message(2, 12, q);
            	continue;
       		}
       		if (strcmp(arg, "yellow") == 0) {
            	send_a_message(2, 13, q);
            	continue;
       		}
       		if (strcmp(arg, "green") == 0) {
            	send_a_message(2, 14, q);
            	continue;
       		}
       		if (strcmp(arg, "magenta") == 0) {
            	send_a_message(2, 15, q);
            	continue;
       		}
       		if (strcmp(arg, "cyan") == 0) {
            	send_a_message(2, 16, q);
            	continue;
       		}
    	}
    	if (strcmp (command, "cut") == 0) {
        	if (strcmp(arg, "red") == 0) {
            	send_a_message(3, 1, q);
            	continue;
       		}
       		if (strcmp(arg, "blue") == 0) {
            	send_a_message(3, 2, q);
            	continue;
       		}
       		if (strcmp(arg, "yellow") == 0) {
            	send_a_message(3, 3, q);
            	continue;
       		}
       		if (strcmp(arg, "green") == 0) {
            	send_a_message(3, 4, q);
            	continue;
       		}
       		if (strcmp(arg, "magenta") == 0) {
            	send_a_message(3, 5, q);
            	continue;
       		}
       		if (strcmp(arg, "cyan") == 0) {
            	send_a_message(3, 6, q);
            	continue;
       		}
    	} 
    


	}
}



