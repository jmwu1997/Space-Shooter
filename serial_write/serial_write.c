#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>   /* File Control Definitions           */
#include <termios.h> /* POSIX Terminal Control Definitions */
#include <unistd.h>  /* UNIX Standard Definitions 	   */ 
#include <errno.h>   /* ERROR Number Definitions           */
#include <signal.h>
#include <string.h>

#define BUFFER_SIZE 256

static bool stop = false;	
static struct termios SerialPortSettings;
static struct sigaction act; 
void intHandler(int sig) {
	printf("\n Stopping serial write\n");
	stop = true;
}

int main(void){
	int fd;/*File Descriptor*/
	act.sa_handler = intHandler;
	sigaction(SIGINT, &act, NULL);
	fd = open("/dev/ttyACM0",O_RDWR | O_NOCTTY | O_NDELAY);
	if(fd == -1)						/* Error Checking */
	   printf("\n  Error! in Opening ttyACM0  ");
	else
	   printf("\n  ttyACM0 Opened Successfully ");

	tcgetattr(fd, &SerialPortSettings);	/* Get the current attributes of the Serial port */
	cfsetispeed(&SerialPortSettings,B9600); /* Set Read  Speed as 9600                       */
	cfsetospeed(&SerialPortSettings,B9600); /* Set Write Speed as 9600                       */
	SerialPortSettings.c_cflag &= ~PARENB;   /* Disables the Parity Enable bit(PARENB),So No Parity   */
	SerialPortSettings.c_cflag &= ~CSTOPB;   /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
	SerialPortSettings.c_cflag &= ~CSIZE;	 /* Clears the mask for setting the data size             */
	SerialPortSettings.c_cflag |=  CS8;      /* Set the data bits = 8                                 */
	SerialPortSettings.c_cflag &= ~CRTSCTS;       /* No Hardware flow Control                         */
	SerialPortSettings.c_cflag |= CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines       */ 
	SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);          /* Disable XON/XOFF flow control both i/p and o/p */
	SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  /* Non Cannonical mode                            */
	SerialPortSettings.c_oflag &= ~OPOST;/*No Output Processing*/
	if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0) /* Set the attributes to the termios structure*/
	    printf("\n  ERROR ! in Setting attributes");
	else
        printf("\n  BaudRate = 9600 \n  StopBits = 1 \n  Parity   = none");

    char buffer[BUFFER_SIZE];
    int bytes_written = 0;
    while(!stop){
    	// memset(buffer, ' ', sizeof(char)*sizeof(buffer));
    	sigaction(SIGINT, &act, NULL);
    	printf("\nEnter text you want to sent\n");
    	scanf("%s",&buffer);
    	if(!stop){
    		// str_strip(buffer);
    		bytes_written = write(fd,buffer,strlen(buffer));
    		printf("\nNumber of bytes written=%d\n",bytes_written);
    	}
    }
    return 0;
}