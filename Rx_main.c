#include "Rx_main.h"

int fd,i,j,counter=0;
char data=0;
char Rx_buf[50];
char *tim;
FILE *f;
char file_path[34]="Rxlog/";
char *filename;

int main(void)
{
	struct sigaction saio; /*definition of signal axtion */

	signal(SIGINT,quit);
	memset(Rx_buf,0,50);

	/*打开串口*/
	if((fd = open(UART,O_RDONLY))<0)
	{
		perror("open_port error!\n");
		return (-1);
	}

	/*设置处理句柄, install the signal handle before making the device asynchronous*/
	saio.sa_handler = signal_handler_IO;
	sigemptyset(&saio.sa_mask); //saio.sa_mask = 0; 必须用sigemptyset函数初始话act结构的sa_mask成员 
	saio.sa_flags = 0;
	saio.sa_restorer = NULL;
	sigaction(SIGIO,&saio,NULL);
	fcntl(fd,F_SETOWN,getpid()); // allow the process to recevie SIGIO
	fcntl(fd,F_SETFL,FASYNC); //Make the file descriptor asynchronous

	/*设置串口*/
	if((i= set_opt(fd,115200,8,'N',1))<0)
	{
		perror("set_opt error!\n");
		return (-1);
	}

	/*open the log file*/
	char *temp;
	time_t stdtime1;
        time(&stdtime1);
        filename = ctime(&stdtime1);
	temp = filename;
	for(j=1;j<strlen(filename);j++,temp++)
	{
		if(*temp==' ')
		{
			*temp = '_';
		}
		*(file_path+5+j) = *temp;
	}
	*(file_path+5+j) = '.';
	*(file_path+6+j) = 't';
	*(file_path+7+j) = 'x';
	*(file_path+8+j) = 't';
	printf("The filename is: %s\n",file_path);
	if((f=fopen(file_path,"wt+"))==NULL)
	{
		perror("open log file error\n");
		return (-1);
	}

	/* loop while waiting for input,normally we would do something useful here*/
	while(TRUE)
	{
		//sleep(1000);
	}
	return 0;
}

void signal_handler_IO(int status)
{
	//printf("received SIGIO signale.\n");
	char data = 0;
	read(fd,&data,1);
	Rx_buf[counter++] = data;
	if(data == '>')
	{
		time_t stdtime;
		time(&stdtime);
		tim = ctime(&stdtime);
		printf("%s: %s\n",tim,Rx_buf);

		fwrite(tim,24,1,f);  //size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
		fwrite(" : ",3,1,f);
		fwrite(Rx_buf,counter,1,f);
		fwrite("\n",1,1,f);

		counter = 0;
		memset(Rx_buf,0,50);
	}
}

#if 0
char *curr_time()   
{   
	time_t stdtime;
	time(&stdtime);
	return ctime(&stdtime);
}  
#endif

void quit()
{
	close(fd);
	fclose(f);
	printf("Quit the process!\n");
	exit(0);
}

int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
	struct termios newtio,oldtio;

	if ( tcgetattr( fd,&oldtio) != 0) 
	{
		perror("SetupSerial 1");
		return -1;
	}
	bzero( &newtio, sizeof( newtio ) );
	newtio.c_cflag |= CLOCAL | CREAD;
	newtio.c_cflag &= ~CSIZE;
	switch( nBits )
	{
		case 7:
			newtio.c_cflag |= CS7;
			break;
		case 8:
			newtio.c_cflag |= CS8;
			break;
	}
	switch( nEvent )
	{
		case 'O':
			newtio.c_cflag |= PARENB;
			newtio.c_cflag |= PARODD;
			newtio.c_iflag |= (INPCK | ISTRIP);
			break;
		case 'E':
			newtio.c_iflag |= (INPCK | ISTRIP);
			newtio.c_cflag |= PARENB;
			newtio.c_cflag &= ~PARODD;
			break;
		case 'N':
			newtio.c_cflag &= ~PARENB;
			break;
	}
	switch( nSpeed )
	{
		case 2400:
			cfsetispeed(&newtio, B2400);
			cfsetospeed(&newtio, B2400);
			break;
		case 4800:
			cfsetispeed(&newtio, B4800);
			cfsetospeed(&newtio, B4800);
			break;
		case 9600:
			cfsetispeed(&newtio, B9600);
			cfsetospeed(&newtio, B9600);
			break;
		case 115200:
			cfsetispeed(&newtio, B115200);
			cfsetospeed(&newtio, B115200);
			break;
		case 460800:
			cfsetispeed(&newtio, B460800);
			cfsetospeed(&newtio, B460800);
			break;
		default:
			cfsetispeed(&newtio, B9600);
			cfsetospeed(&newtio, B9600);
			break;
	}

	if( nStop == 1 )
		newtio.c_cflag &= ~CSTOPB;
	else if ( nStop == 2 )
		newtio.c_cflag |= CSTOPB;

	newtio.c_cc[VTIME] = 0;
	newtio.c_cc[VMIN] = 0;
	tcflush(fd,TCIFLUSH);
	if((tcsetattr(fd,TCSANOW,&newtio))!=0)
	{
		perror("com set error");
		return -1;
	}
	// printf("set done!\n\r");
	return 0;
}
