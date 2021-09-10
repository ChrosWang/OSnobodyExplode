//Bomb disposal
//Chros Wang
//Ideas from the game Keep talking and nobody explode. 
//Sample run: ./server 1201
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
   // char data[0];    // other data (zero or more bytes)
};

struct t_bomb {
    int serial_number;
    int battery_number;
    int colored_button[6];
    int numbered_button[9];
    int wires[6];
    int colored_button_pressed[6];
    int numbered_button_pressed[9];

};

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"
#define BLD  "\033[1m"

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

// for test no need
void test_bomb(struct t_bomb * bomb) {
    bomb->serial_number = 123466;
    bomb->battery_number = 4;
    for(int i =0; i < 6; i++) {
        bomb->colored_button[i] = 0; 
    }
    bomb->colored_button[5] = 1;
    for(int i =0; i < 9; i++) {
        bomb->numbered_button[i] = 0; 
    }
    for(int i =0; i < 6; i++) {
        bomb->wires[i] = 0; 
    }
}
//initialize a bomb
void initialize_bomb(struct t_bomb * bomb) {
    bomb->serial_number = 0;
    bomb->battery_number = 0;
    for(int i =0; i < 6; i++) {
        bomb->colored_button[i] = 0; 
    }
    for(int i =0; i < 9; i++) {
        bomb->numbered_button[i] = 0; 
    }
    for(int i =0; i < 6; i++) {
        bomb->wires[i] = 0; 
    }
}
//function to generate random number in an interval
int generate_random(int lower, int upper) 
{ 
    
    return (rand() % (upper - lower + 1)) + lower; 
} 
//create a random bomb
void random_bomb(struct t_bomb * bomb) {
    bomb->serial_number = generate_random(500000, 899999);
    bomb->battery_number = generate_random(1, 5);
    for(int i =0; i < 6; i++) {
        bomb->colored_button[i] = generate_random(0,1); 
    }
    for(int i =0; i < 9; i++) {
        bomb->numbered_button[i] = generate_random(0,1);  
    }
    for(int i =0; i < 6; i++) {
        bomb->wires[i] = 1;  
    }
}
//create a preset bomb
void pre_set_bomb(struct t_bomb * bomb, int mode){
    if (mode == 1) {
       bomb->serial_number = 509421; 
       bomb->battery_number = 4;
       for(int i =0; i < 6; i++) {
        bomb->colored_button[i] = 0; 
        }
        bomb->colored_button[2] = 1;
        bomb->colored_button[4] = 1;
        bomb->colored_button[5] = 1;

        for(int i =0; i < 9; i++) {
            bomb->numbered_button[i] = 0;  
        }
        bomb->numbered_button[3] = 1; 
        bomb->numbered_button[1] = 1; 
        bomb->numbered_button[7] = 1; 
        bomb->numbered_button[8] = 1; 
    } else if (mode == 2) {
        bomb->serial_number = 902821; 
        bomb->battery_number = 1;
        for(int i =0; i < 6; i++) {
        bomb->colored_button[i] = 0; 
        }
        bomb->colored_button[3] = 1;
        bomb->colored_button[4] = 1;
        bomb->colored_button[5] = 1;

        for(int i =0; i < 9; i++) {
            bomb->numbered_button[i] = 0;  
        }
        bomb->numbered_button[0] = 1;
        bomb->numbered_button[2] = 1; 
        bomb->numbered_button[1] = 1; 
        bomb->numbered_button[7] = 1; 
        bomb->numbered_button[8] = 1; 
        bomb->numbered_button[3] = 1;
    } else if (mode == 3) {
        bomb->serial_number = 603912; 
        bomb->battery_number = 2;
        for(int i =0; i < 6; i++) {
        bomb->colored_button[i] = 1; 
        }
        for(int i =0; i < 9; i++) {
            bomb->numbered_button[i] = 1;  
        }
    }

    for(int i =0; i < 6; i++) {
        bomb->wires[i] = 1;  
    }
}
//print the information
void print_serial(struct t_bomb * bomb) {
    printf("*You check the front side of the bomb... However you don't find anything.\n");
    usleep(2000000);
    printf("*You flip the bomb over..\n*There is a tag says SERIAL NUMBER the bomb!\n*The serial number of the bomb is: \n"BLD"%d"RESET".\n",bomb->serial_number);
}
void print_batteries(struct t_bomb * bomb) {
    printf("*There is a battery box inside of the bomb. \n*You found "BLD"%d "RESET"batteries inside.\n",bomb->battery_number);
}
void print_buttons(struct t_bomb * bomb) {
    printf("*There are several buttons, \n*You found a module with several colored buttons:\n");
    printf(bomb->colored_button[0] == 1?"*A "RED"red "RESET"button\n":"");
    printf(bomb->colored_button[1] == 1?"*A "BLU"blue "RESET"button\n":"");
    printf(bomb->colored_button[2] == 1?"*A "YEL"yellow "RESET"button\n":"");
    printf(bomb->colored_button[3] == 1?"*A "GRN"green "RESET"button\n":"");
    printf(bomb->colored_button[4] == 1?"*A "MAG"magenta "RESET"button\n":"");
    printf(bomb->colored_button[5] == 1?"*A "CYN"cyan "RESET"button\n":"");

    printf("*On the other side of the bomb, you found another module with several numbered buttons:\n");
    for(int i =0; i < 9; i++) {
        //printf("test "BLD"%d %d"RESET"\n",i+1, bomb->numbered_button[i]);
        if (bomb->numbered_button[i] == 1)
            printf("*A button with "BLD"%d "RESET"\n",i+1);
    }
}
void print_wires(struct t_bomb * bomb) {
    printf("*You found a module with wires:\n");
    printf(bomb->wires[0] == 1?"*A "RED"red "RESET"wire\n":"");
    printf(bomb->wires[1] == 1?"*A "BLU"blue "RESET"wire\n":"");
    printf(bomb->wires[2] == 1?"*A "YEL"yellow "RESET"wire\n":"");
    printf(bomb->wires[3] == 1?"*A "GRN"green "RESET"wire\n":"");
    printf(bomb->wires[4] == 1?"*A "MAG"magenta "RESET"wire\n":"");
    printf(bomb->wires[5] == 1?"*A "CYN"cyan "RESET"wire\n":"");
}

void print_bomb(struct t_bomb * bomb) {
    print_serial(bomb);
    print_batteries(bomb);
    print_buttons(bomb);
    print_wires(bomb);
}
//count the information
int count_numbered_button(struct t_bomb * bomb) {
    int count = 0;
     for(int i =0; i < 6; i++) {
        count = count + bomb->numbered_button[i]; 
    }
    return count;
}
int count_colored_button(struct t_bomb * bomb) {
    int count = 0;
     for(int i =0; i < 6; i++) {
        count = count + bomb->colored_button[i]; 
    }
    return count;
}
int count_wires(struct t_bomb * bomb) {
    int count = 0;
    for(int i =0; i < 6; i++) {
        count = count + bomb->wires[i]; 
    }
    return count;
}
//caculate the password
void caculate_colored_pw(struct t_bomb * bomb, struct t_bomb * pwd) {
    int single; 
    if(count_colored_button(bomb) < 2) {
        for(int i =0; i < 6; i++) {
            if (bomb->colored_button[i]==1)
                single = i;
        }
        if (((bomb->serial_number%100)/10)%2 == 1)
            pwd->colored_button[single] = 1; 
        if (bomb->battery_number%2 == 0)
            pwd->colored_button[single] = 1; 
    } else {
        if(!(bomb->colored_button[1]==bomb->colored_button[3])) {
            if (bomb->colored_button[1] == 1)
                pwd->colored_button[1] = 1; 
            else
                pwd->colored_button[3] = 1; 
        }
        if(!(bomb->colored_button[0]==bomb->colored_button[5])) {
            if (bomb->colored_button[0] == 1)
                pwd->colored_button[0] = 1; 
            else
                pwd->colored_button[5] = 1; 
        }
        if(!(bomb->colored_button[2]==bomb->colored_button[4])) {
            if (bomb->colored_button[2] == 1)
                pwd->colored_button[2] = 1; 
            else
                pwd->colored_button[4] = 1; 
        }
    }
}

void caculate_numbered_pw(struct t_bomb * bomb, struct t_bomb * pwd) {
    if (bomb->numbered_button[0]==1){
        pwd->numbered_button[0] = 1;
    }
    for (int i = 1; i <9; i++){
        if (bomb->numbered_button[i]==1){
            pwd->numbered_button[i] = i + 1 - ((i+1<6)?bomb->battery_number:(bomb->serial_number/100000));
        }
    }
    
}

void caculate_wire_pw(struct t_bomb * bomb, struct t_bomb * pwd) {
    for(int i =0; i < 6; i++) {
        if (pwd->wires[i] != 99)
            pwd->wires[i] = 0; 
    }
    int count = 0; 
    for (int i = 1; i <9; i++){
        if (bomb->numbered_button_pressed[i]%2 == 0 && bomb->numbered_button_pressed[i]!=0){
            count++;
        }
    }
    for (int i = 3; i <6; i++){
        if (bomb->colored_button_pressed[i] >= 1){
            count++;
        }
    }
    if (count == 0 && pwd->wires[0] != 99) 
        pwd->wires[0] = 1; 
   // else if (pwd->wires[0] == 99&&count != 0)
        //printf("explode at wrong wire.\n");
    if (count == 1&& pwd->wires[1] != 99)
        pwd->wires[1] = 1; 
   // else if (pwd->wires[1] == 99&&count != 1)
       // printf("explode at wrong wire.\n");
    if (count == 3&& pwd->wires[2] != 99)
        pwd->wires[2] = 1; 
  //  else if (pwd->wires[2] == 99&&count != 3)
       // printf("explode at wrong wire.\n");
    if (count == 2&& pwd->wires[3] != 99)
        pwd->wires[3] = 1; 
   // else if (pwd->wires[3] == 99&&count != 2)
       // printf("explode at wrong wire.\n");
    if (count >= 2&& pwd->wires[4] != 99)
        pwd->wires[4] = 1; 
  //  else if (pwd->wires[4] == 99&&count <2)
       // printf("explode at wrong wire.\n");
    if (count < 2&& pwd->wires[5] != 99)
        pwd->wires[5] = 1; 
   // else if (pwd->wires[5] == 99&&count>=2)
        //printf("explode at wrong wire.\n");

    //print_bomb(pwd);
}

void caculate_pw(struct t_bomb * bomb, struct t_bomb * pwd){
    caculate_colored_pw(bomb, pwd);
    caculate_numbered_pw(bomb, pwd);
    caculate_wire_pw(bomb, pwd);
}
//check if clear for wires
bool check_colored_clear(struct t_bomb * pwd){
    for(int i =0; i < 6; i++) {
        if (pwd->colored_button[i]>0) {
            //printf("explode at still unpressed %d\n", i);
            return false;
        }
    }
    return true;
}

bool check_numbered_clear(struct t_bomb * pwd){
    for(int i =0; i < 9; i++) {
        if (pwd->numbered_button[i]>0) {
            //printf("explode at still unpressed %d\n", i);
            return false;
        }
    }
    return true;
}
bool check_wire_clear(struct t_bomb * pwd){
    for(int i =0; i < 6; i++) {
        if (pwd->wires[i]>0) {
            if (pwd->wires[i]!=99){
            //printf("explode at still unpressed %d\n", i);
                return false;
            }
        }
    }
    return true;
}
//check the win condition which is all clear
bool check_win(struct t_bomb * bomb, struct t_bomb * pwd) {
    return check_colored_clear(pwd)&&check_numbered_clear(pwd)&&check_wire_clear(pwd);
}
// check each action is ilegal or not
bool explode(struct t_bomb * bomb, struct t_bomb * pwd, int action, int object){
    if(object == 99) {
        printf("*You can't find it on the bomb anywhere...\n");
    } else
    if (action == 1) {
            if(object == 1)
                print_serial(bomb);
            if(object == 2)
                print_buttons(bomb);
            if(object == 3)
                print_wires(bomb);
            if(object == 4)
                print_batteries(bomb);
        } else if (action == 2) {
            if(object < 10) {
                if (check_numbered_clear(pwd)) {
                    printf("*The is a LED showing green on this module, looks like we are all set on this one.\n");
                    return false; 
                }
                printf("*You pressed the button %d.\n", object); 
                bomb->numbered_button_pressed[object-1]++;
                pwd->numbered_button[object-1]--;
                if (pwd->numbered_button[object-1] < 0){
                    //printf("explode\n");
                    return true;
                }  
            } else {
                if (check_colored_clear(pwd)) {
                    printf("*The is a LED showing green on this module, looks like we are all set on this one.\n");
                    return false; 
                }
                if(object == 11)
                    printf("*You pressed the "RED"red "RESET"button.\n"); 
                if(object == 12)
                    printf("*You pressed the "BLU"blue "RESET"button.\n"); 
                if(object == 13)
                    printf("*You pressed the "YEL"yellow "RESET"button.\n"); 
                if(object == 14)
                    printf("*You pressed the "GRN"green "RESET"button.\n"); 
                if(object == 15)
                    printf("*You pressed the "MAG"magenta "RESET"button.\n"); 
                if(object == 16)
                    printf("*You pressed the "CYN"cyan "RESET"button.\n"); 
                bomb->colored_button_pressed[(object%10)-1]++;
                pwd->colored_button[(object%10)-1]--;
                if (pwd->colored_button[(object%10)-1] < 0){
                    //printf("explode\n");
                    return true;
                }  
            }


        } else if (action == 3) {
            caculate_wire_pw(bomb, pwd);
            if(object == 1)
                printf("*You cut the "RED"red "RESET"wire.\n"); 
            if(object == 2)
                printf("*You cut the "BLU"blue "RESET"wire.\n");
            if(object == 3)
                printf("*You cut the "YEL"yellow "RESET"wire.\n"); 
            if(object == 4)
                printf("*You cut the "GRN"green "RESET"wire.\n"); 
            if(object == 5)
                printf("*You cut the "MAG"magenta "RESET"wire.\n");  
            if(object == 6)
                printf("*You cut the "CYN"cyan "RESET"wire.\n");

            if (!check_numbered_clear(pwd)){
                //printf("explode at unfinished numbered operations.\n");
                return true;
            }
            if (!check_numbered_clear(pwd)){
                //printf("explode at unfinished colored operations.\n");
                return true;
            }
            
            if (pwd->wires[object-1] <= 0){
                   // printf("explode\n");
                    pwd->wires[object-1] = 99;
                    return true;
            } 
            pwd->wires[object-1] = 99;
        }

        return false;
}
int q = -1; // id for the queue
// the clean up function stole from ipc
void cleanup(int s) {

	if (q>=0){
		if(msgctl(q, IPC_RMID, NULL) < 0) {
			perror("msgctl");
			printf("Can't remove IPC mailbox queue.\n");
			exit(1);
		}
		//printf("Removed IPC mailbox queue.\n");
	}

	// Print a friendly message then exit.
    //printf("Final event statistics...\n");
    //print_stats();
    exit(1); 
}
void help() {
    printf("\n*************************************************************\n");
    printf("There are three actions you could do:\n");
    printf(RED"check <what you want to check>"RESET": when you want to see something on the bomb.\n");
    printf(RED"press <the button>"RESET": when you want to press a button.\n");
    printf(RED"cut <the wire>"RESET": when you want to cut a wire.\n");
    printf("*************************************************************\n\n");
}

void initialize() {
    char* userID = getenv("USER");
    printf("\n*************************************************************\nWelcome to Bomb Disposal Simulator, %s.\n", userID);
    printf("If you havn't, open another terminal window and execute \n./client1 <the_room_number> to start the game.\n");
    printf("You never need to type in this terminal.\n\n");

    printf("You, the handler, are "GRN"physically"RESET" holding a bomb.\n");
    printf("The other player, the "CYN"specialist"RESET", has the user manual for the bomb.\nSo please send the other player the pdf in the folder.\nHe/she shall not look at the screen and you shall not look at the pdf.\n");
    printf("Both of you need to work together on the disposal.\n\n");

    printf("Type "RED"help"RESET" for more instructions. \nType "RED"start <mode>"RESET" to start the game.\nThe modes are: \n1-3: Pre-set bombs.\nrandom: Random generated bomb.\n");
    printf("If you are the first time playing, we recommand the bomb 1 to start with. \nGood Luck!\n*************************************************************\n\n");
    help();
}



int main(int argc, char **argv)
{
    // This next code registers a signal handler, so that if the user presses
    // Control-C, then we still have the chance to cleanup (i.e. delete the IPC
    // mailbox queue). After all, we don't want to pollute radius with lots of
    // defunct mailbox queues.
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = cleanup;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

    if (argc != 2) {
        printf("Enter the same game room number when executing.\n");
        printf("usage: %s <room_number>\n", argv[0]);
        exit(1);
    }
    key_t key = atoi(argv[1]);
    //int pid = getpid();
    
               // struct timespec t_start;
        //struct timespec t_end; 
    struct t_bomb * bomb = (struct t_bomb *)malloc(sizeof(struct t_bomb));
 
        //test_bomb(bomb);
        //print_bomb(bomb);
    struct t_bomb * pwd = (struct t_bomb *)malloc(sizeof(struct t_bomb));


    q = msgget(key, IPC_CREAT|0660);
    if (q < 0) {
        perror("msgget");
        printf("Can't create IPC mailbox queue.\n");
        exit(1);
    }
    initialize();
    //printf("Created IPC mailbox queue number %d.\n", key);

    //printf("Waiting to receive IPC messages.\n");
    struct ipcmsg *m = (struct ipcmsg *)malloc(MSG_SIZE);
    bool lose = false; 
    bool win = false;
    bool game_start = false; 
    while(!lose) {
        int desired_msgtype = 0; // 0 here means "any"
        int recv_flags = 0;

        int msgsize = msgrcv(q, m, MSG_PAYLOAD_SIZE, desired_msgtype, recv_flags);
        if (msgsize < 0) {
            //perror("msgrecv");
           // printf("Can't receive IPC message.\n");
            exit(1);
        }
        if(m->msgtype == 4){

           initialize_bomb(bomb);
        srand(time(NULL));
        if (m->eventid == 1)
            random_bomb(bomb);
        else 
            pre_set_bomb(bomb, m->eventid-1);
        initialize_bomb(pwd);
        caculate_pw(bomb, pwd);
        // print_bomb(pwd);
            printf("**"GRN"GAME START!"RESET"**\n\n");
            printf("*There is a bomb in front of you. \n\"BEEP BEEP...\"*The numbered on the LED started to decrease from 300.\n *You know you don't have much time left.\n");
            printf("*You start to look carefully at the bomb. Trying not to miss anything suspicious.\n");
            usleep(4000000);
            print_buttons(bomb);
            usleep(2000000);
            print_wires(bomb);
            usleep(2000000);
            printf("*While you are observing, your phone starts ringing.\n");
            usleep(1000000);
            printf("*It's your friend! Your friend tells you he/she has the deactivate manual of the bomb.\n");
            printf("*You "RED"still havn't finished checking everything "RESET"on the bomb. But you decide to follow your friend's instructions...\n");
            game_start = true; 
            continue;
        }

        if(m->msgtype == 6){
            help();
            continue;
        }

        if (game_start) {
            if(m->msgtype == 5){
                if (m->eventid <= 0){
                    printf("Time's UP!!\n");
                    break;
                }
                printf("**CAUTION: EXPLOSION IN "RED"%d"RESET" SECONDS.**\n", m->eventid);
                continue;
            }

            lose = explode(bomb, pwd, m->msgtype, m->eventid);
            win = check_win(bomb, pwd);

            if (win){
                printf("*With your friend's help, you successfully deactivate the bomb!\n");
                printf("**"GRN"MISSION SUCCESSED"RESET"\n");
                cleanup(0);
                return 0;
            }
        }
    }
    printf("*\"BOOOOOOOOM------\"\n "RED"MISSION FAILED"RESET" \n");
    cleanup(0);
}