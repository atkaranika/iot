#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

// =========== P1S4 ============= //
#define MAX_INPUT_COMMAND 100
#define MAX_INPUT_LED 10
#define MAX_INPUT_STATUS 2

// =========== P1S4 ============= //
#define MAX_INPUT_ADDRESS 10
#define MAX_INPUT_DATA 30 
#define MAX_INPUT_LEN 3

// =========== P1S4 ============= //
int set_led(char *uartmessage)
    {
        int fduart; // uart file descriptor //
        int flags; // flags for open //
        int returnwrite; // result of write uart size //
        int returnread; // result of read uart size //

        char uartreadmessage[100];
        flags = O_RDWR| O_NOCTTY;
        fduart = open("/dev/ttyACM0", flags);

        if (fduart == -1)
            {
                printf("Error while opening!\n");
                return -1;   
            }
        returnwrite = write(fduart, uartmessage, strlen(uartmessage));
        if (returnwrite < 0)
            {
                printf("Error while sending!\n");
                return -1;
            }
        printf("wirte to uart set_led message : %s\n", uartmessage);
        // returnread = read(fduart, uartreadmessage, strlen(uartreadmessage));

        // if (returnread < 0)
        //     {
        //         printf("Error while reading!\n");
        //         return -1;
        //     }
        // sleep(0);
        // printf("read from uart -> %s\n", uartreadmessage);
        close(fduart);
        return 0;
    }

int storage(char *uartmessage)
{
    int fduart; // uart file descriptor //
    int flags; // flags for open //
    int returnwrite; // result of write uart size //
    int returnread; // result of read uart size //

    char uartreadmessage[100];
    flags = O_RDWR| O_NOCTTY;
    fduart = open("dev/ttyUSB0", flags);

    if (fduart == -1)
        {
            printf("Error while opening!\n");
            return -1;   
        }
    returnwrite = write(fduart, uartmessage, strlen(uartmessage));

    if (returnwrite < 0)
        {
            printf("Error while sending!\n");
            return -1;
        }

    returnread = read(fduart, uartreadmessage, strlen(uartreadmessage));

    if (returnread < 0)
        {
            printf("Error while reading!\n");
            return -1;
        }
    printf("read from uart -> %s\n", uartreadmessage);
    return 0;
}
int main(int argc, char *argv[])
    {   

        // =========== P1S4 ============= //
        char command[MAX_INPUT_COMMAND]; // user command //
        char led[MAX_INPUT_LED];    // user input led(red, green, blue) //
        char status[MAX_INPUT_STATUS]; //user input status (0, 1) //
        char setledparam[100];  // the message to be sent through uart(concatenated) with '*' delimiter //
                                // parameter to set_led                                                 //

        // =========== P1S5 ============= //
        //  storage write variables //
        char idx[10]; // user input startaddress // 
        char *data;  // user input data to be written at startaddress //
        char storagewriteparam[100];  // the message to be sent through uart(concatenated) with '*' delimiter //
                                      // parameter to storage_write                                           //
        // storage read variables //
        char len[MAX_INPUT_LEN];    // len of data to read //
        char storagereadparam[100];  // the message to be sent through uart(concatenated) with '*' delimiter //
                                      // parameter to storage_read                                           //

        int i = 0;
        // show the available implemented commands //
        printf("===== Command Options ========\n");
        printf("set_led\n");
        printf("storage_write\n");
        printf("storage_read\n");
        printf("sense\n");

        // get input of user //
        while(1)
            {   
                fgets(command, MAX_INPUT_COMMAND, stdin);
                printf("userinput -> %s\n", command);
                if (strcmp(command, "quit") == 0)
                    {
                        printf("Bye\n");
                    }
                else if (strncmp(command, "set_led", 7) == 0)
                    {
                        for (i = 0 ; i < 100; i++)
                            {
                                setledparam[i] = '\0';
                            }
                        // ask user which led (red green blue) //
                        printf("Give led: \n");
                        fgets(led, MAX_INPUT_LED, stdin);
                        // check user input , fgets takes char '\n' as well //
                        while  ((strcmp (led, "red\n") != 0) && (strcmp (led, "blue\n")) != 0 && (strcmp (led, "green\n") != 0))
                            {
                                printf("userinput -> %s", led);
                                if (strcmp(led, "quit") == 0)
                                    {
                                        printf("Bye\n");
                                    }
                                printf("Wrong color! Try again!\n");
                                fgets(led, MAX_INPUT_LED, stdin);
                            }
                        printf("Give status: \n");
                        fgets(status, MAX_INPUT_STATUS, stdin);
                        // check user input //
                        while  ((strcmp(status, "0") != 0) && (strcmp (status, "1") != 0))
                            {
                                if (strcmp(status, "quit") == 0)
                                    {
                                        printf("Bye\n");
                                    }
                                printf("Wrong status! Try again!\n");
                                fgets(status, MAX_INPUT_STATUS, stdin);
                            }
                        strcat(setledparam, command);
                        strcat(setledparam, "*");
                        strcat(setledparam, led);
                        strcat(setledparam, "*");
                        strcat(setledparam, status);
                        strcat(setledparam, "*");
                        set_led(setledparam);
                    }
                else if (strncmp(command, "storage_write", 13) == 0)
                    {   
                        // message format command*address*len* //
                        // ask user for address to write //
                        printf("write the startaddress to be written the data: \n");
                        fgets(idx, MAX_INPUT_ADDRESS, stdin);

                        // ask user for data to be written //
                        printf("write the data: \n");
                        fgets(data, MAX_INPUT_DATA, stdin);
                       
                        strcat(storagewriteparam, command);
                        strcat(storagewriteparam, "*");
                        strcat(storagewriteparam, idx);
                        strcat(storagewriteparam, "*");
                        strcat(storagewriteparam, data);
                        strcat(setledparam, "*");
                        storage(setledparam);
                    }
                else if (strncmp(command, "storage_read", 12) == 0)
                    {   
                        // message format command*address*len* //
                        // ask user for address to write //
                        printf("write the startaddress to read data: \n");
                        fgets(idx, MAX_INPUT_ADDRESS, stdin);

                        // ask user for the len of data to read //
                        printf("write the data: \n");
                        fgets(len, MAX_INPUT_LEN, stdin);
                       
                        strcat(storagewriteparam, command);
                        strcat(storagewriteparam, "*");
                        strcat(storagewriteparam, idx);
                        strcat(storagewriteparam, "*");
                        strcat(storagewriteparam, data);
                        strcat(setledparam, "*");
                        storage(setledparam);
                    }
                
            }
        return 0;
    }



