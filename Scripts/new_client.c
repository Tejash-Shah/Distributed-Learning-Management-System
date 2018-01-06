	#include <stdio.h>
	#include <sys/socket.h> 
	#include <arpa/inet.h> 
	#include <string.h> 
	#include <stdlib.h> 
	#include <fcntl.h> 
	#include <unistd.h> 
	#define server_port 6364
	#define rec_buff 5000
	#define send_buff 5000
	char asscourgra[2048];
	char strings[send_buff]; 
	int f,c; 
	ssize_t sbyte, rf, rfsize, rstr[rec_buff],len;
	char tFilename[2048];
	int rfname(int ,char*);
	int sock;


	int main(int argc, char* argv[])
	{
	 int size= sizeof(asscourgra);
	 int return_value;
	 char strings[2048]; 
	 char ans;
	 char name;
	 if (argc < 3)
	 	{
	 	printf(" \n \n USE: %s  <IP address> [port number] \n \n ", argv[0]);
	 	exit(-1);
	 	}



	 struct sockaddr_in server, client;
	 sock = socket(AF_INET, SOCK_STREAM,0);//
	 if (sock<0)
		{
	 	printf(" \n \n  Error in Socket Creation \n \n ");
		exit(-1);
	 	}	

	 memset(&client, 0, sizeof(client)); 
	 memset(&server, 0, sizeof(server)); 
	 server.sin_family = AF_INET; 
	 server.sin_port =htons(server_port); 

	 if( connect(sock, (struct sockaddr*) &server, sizeof(server)) < 0 )
	 		{
	 		printf(" \n \n  Error in Connecting \n \n ");
	 		exit(-1);
	 		}


	 		
	 for (;;)
	 {
	 return_value=read(sock,strings,sizeof(strings));
	 if(return_value>0)
	 	{
	 	printf("%s \n", strings);
	 	memset(strings,0,return_value);
	 	}

	 scanf("%s",asscourgra);
	 if (write(sock,asscourgra,size)<0)
	 	printf("Error in Writing\n");

	 if(strcmp(asscourgra,"WEEK1")==0)
	 {
	 	//printf("In Week1\n");
	 	sprintf(tFilename,"%s.pdf",asscourgra);
	    printf("Filename received will be : %s\n",tFilename);
	    rfname(sock,tFilename);
	 }
	 else if (strcmp(asscourgra,"WEEK2")==0)
	 {
	 	//printf("In Week2\n");
	 	sprintf(tFilename,"%s.pdf",asscourgra);
	    printf("Filename received will be : %s\n",tFilename);
	    rfname(sock,tFilename);

	 }
	 else if (strcmp(asscourgra,"WEEK3")==0)
	 {
	 	//printf("In Week3\n");
	 	sprintf(tFilename,"%s.pdf",asscourgra);
	    printf("Filename received will be : %s\n",tFilename);
	    rfname(sock,tFilename);

	 }
	 else if (strcmp(asscourgra,"WEEK4")==0)
	 {
	 	//printf("In Week4\n");
	 	sprintf(tFilename,"%s.pdf",asscourgra);
	    printf("Filename received will be : %s\n",tFilename);
	    rfname(sock,tFilename);
	 }
	 else if (strcmp(asscourgra,"Chapterone")==0)
	 {
	 	//printf("In Chapterone\n");
	 	sprintf(tFilename,"%s.pdf",asscourgra);
	    printf("Filename received will be : %s\n",tFilename);
	    rfname(sock,tFilename);
	 }
	  else if (strcmp(asscourgra,"Chaptertwo")==0)
	 {
	 	//printf("In Chapterthree\n");
	 	sprintf(tFilename,"%s.pdf",asscourgra);
	    printf("Filename received will be : %s\n",tFilename);
	    rfname(sock,tFilename);
	 }
	  else if (strcmp(asscourgra,"Chapterfour")==0)
	 {
	 	//printf("In Chapterfour\n");
	 	sprintf(tFilename,"%s.pdf",asscourgra);
	    printf("Filename received will be : %s\n",tFilename);
	    rfname(sock,tFilename);
	 }
	 else if (strcmp(asscourgra,"207Grades")==0)
	 {
	 	//printf("In Maths\n");
	 	sprintf(tFilename,"%s.pdf",asscourgra);
	    printf("Filename received will be : %s\n",tFilename);
	    rfname(sock,tFilename);
	 }
	  else if (strcmp(asscourgra,"209Grades")==0)
	 {
	 	sprintf(tFilename,"%s.pdf",asscourgra);
	    printf("Filename received will be : %s\n",tFilename);
	    rfname(sock,tFilename);
	 }
	}
	}


	 int rfname(int sock, char *tFilename)
			{
			printf("%s\n",tFilename);
			if ( (f = open(tFilename, O_WRONLY|O_CREAT, 0644)) < 0 )
				{
				printf("error creating file\n");
				return(-1);
				}

				c = 0; 
				rfsize = 0; 
			//printf("outside while\n");

			while ( (rf = recv(sock, rstr, rec_buff, 0)) > 0 )
				{
					//printf("Inside While\n");
				c++;
				rfsize += rf;

			if (write(f, rstr, rf) < 0 )
				{
				printf("error writing to file");
				return(-1);
				}
				printf("File Downloaded is %s , of size: %d bytes\n", tFilename, rfsize);
				}
				close(f); 
				return rfsize;
			}






