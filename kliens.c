/***************************************/
/*             TCP client              */
/***************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFSIZE 1024
#define PORT_NO 2001
#define error(a,b) fprintf(stderr, a, b)

void kirajzol(int h){
	int i;
	printf("Hibapontok: ");
	for (i=0;i<h;i++)
	   printf("|");
    printf("\n");   
	return;
}

int main(int argc, char *argv[] ) { 
 
   
   /* Declarations */
   int fd;	                       // socket endpt	
   int flags;                      // rcv flags
   struct sockaddr_in server;	   // socket name (addr) of server 
   struct sockaddr_in client;	   // socket name of client 
   int server_size;                // length of the socket addr. server 
   int client_size;                // length of the socket addr. client 
   int bytes;    	               // length of buffer 
   int rcvsize;                    // received bytes
   int trnmsize;                   // transmitted bytes
   int err;                        // error code
   int ip;			 			   // ip address
   char on;                        // 
   char buffer[BUFSIZE+1];         // datagram dat buffer area
   char buffer2[BUFSIZE+1];        // datagram dat buffer area
   char server_addr[16];           // server address	
   int sorszam;


   /* Initialization */
   on    = 1;
   flags = 0;
   server_size = sizeof server;
   client_size = sizeof client;
    if (argc == 2) {
        sprintf(server_addr, "%s", argv[1]);
    }else{
      sprintf(server_addr, "%s", "127.0.0.1");
    }  
   ip = inet_addr(server_addr);
   server.sin_family      = AF_INET;
   server.sin_addr.s_addr = ip;
   server.sin_port        = htons(PORT_NO);
   
  
   /* Creating socket */
   fd = socket(AF_INET, SOCK_STREAM, 0);   //sock_dgramm UDP nél
   if (fd < 0) {

      error("%s: Socket creation error.\n",argv[0]);
      exit(1);
   }

   /* Setting socket options */
   setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
   setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

   /* Connecting to the server */
   err = connect(fd, (struct sockaddr *) &server, server_size);
   if (err < 0) {

      error("%s: Cannot connect to the server.\n", argv[0]);
      exit(2);
   }

   rcvsize = recv( fd, buffer, BUFSIZE, flags ); //első játékos csatlakozásának jóváhagyása
   if (rcvsize < 0) {

      error("%s: Cannot receive data from the socket.\n", argv[0]);
      exit(4);
   }

   if (!strcmp(buffer, "1") ) {
     bytes = strlen(buffer)+1;
     trnmsize = send(fd, buffer, bytes, flags);
     if (trnmsize < 0) {

       error("%s: Cannot send data to server.\n", argv[0]);
       exit(3);
     }
     printf("Várakozunk a 2. játékosra\n");
     rcvsize = recv( fd, buffer, BUFSIZE, flags ); // második játékos csatlakozásának jóváhagyása
     if (rcvsize < 0) {

       error("%s: Cannot receive data from the socket.\n", argv[0]);
       exit(4);
     }

   }

   printf("%s\n",buffer); 
   bytes = strlen(buffer)+1;
     trnmsize = send(fd, buffer, bytes, flags);
     if (trnmsize < 0) {

       error("%s: Cannot send data to server.\n", argv[0]);
       exit(3);
     }
  for(;;) { // a játék
   
  
    rcvsize = recv(fd, buffer, BUFSIZE, flags ); 
    if (rcvsize < 0) {
       error("%s: Cannot receive from the socket\n",argv[0]);
       exit(5);
    }
	bytes = strlen(buffer) + 1;
	trnmsize = send(fd, buffer, bytes, flags);
     if (trnmsize < 0) {

       error("%s: Cannot send data to server.\n", argv[0]);
       exit(3);
     }
	 sorszam=atoi(buffer);      
	 
	 if (sorszam==1){
		 printf("szerep: felado\n");
		 printf("Adja meg a kitalálandó szöveget: ");
		 scanf("%s",buffer);
		 bytes = strlen(buffer) + 1;
		 trnmsize = send(fd, buffer, bytes, flags);//kitalálandó szöveg elküldése A
         if (trnmsize < 0) {
			error("%s: Cannot send data to server.\n", argv[0]);
			exit(3);
		 }
		 rcvsize = recv(fd, buffer, BUFSIZE, flags );  //B
         if (rcvsize < 0) {
           error("%s: Cannot receive from the socket\n",argv[0]);
           exit(5);
         } 
	     printf("%s\n",buffer);
		 
		 bytes = strlen(buffer) + 1;
	     trnmsize = send(fd, buffer, bytes, flags); //C
         if (trnmsize < 0) {

           error("%s: Cannot send data to server.\n", argv[0]);
           exit(3);
         } 
		 for(;;){
			 rcvsize = recv(fd, buffer, BUFSIZE, flags ); //I
          if (rcvsize < 0) {
             error("%s: Cannot receive from the socket\n",argv[0]);
             exit(5);
          }
		  
		  bytes = strlen(buffer) + 1;
	      trnmsize = send(fd, buffer, bytes, flags); //J
          if (trnmsize < 0) {
           error("%s: Cannot send data to server.\n", argv[0]);
           exit(3);
           } 
		  rcvsize = recv(fd, buffer2, BUFSIZE, flags ); //X
          if (rcvsize < 0) {
             error("%s: Cannot receive from the socket\n",argv[0]);
             exit(5);
          }
		  
		  bytes = strlen(buffer2) + 1;
	      trnmsize = send(fd, buffer2, bytes, flags); //XX
          if (trnmsize < 0) {
           error("%s: Cannot send data to server.\n", argv[0]);
           exit(3);
           }
		  
		  if (!strcmp(buffer,"nyertel")){
			  printf("Gratulálok, nyertél!\n");
			  break;
		  }	  
		  else if (!strcmp(buffer,"vesztettel")){
			  printf("Sajnálom, vesztettél\n");
			  break;	  
		  }else{  
		     printf("%s\n\n",buffer);
			 kirajzol(atoi(buffer2));
		  }
		  
		 } 
		 
		 rcvsize = recv(fd, buffer, BUFSIZE, flags ); //VV
          if (rcvsize < 0) {
             error("%s: Cannot receive from the socket\n",argv[0]);
             exit(5);
          }
		 	if (!strcmp(buffer,"vege")){

			close(fd);
			exit(0);
	
		}
		 
		 
		 
	 }else{
		printf("szerep: kitalalo. A játék feladásához írd be hogy 'feladom'\n"); 
		rcvsize = recv(fd, buffer, BUFSIZE, flags ); //D
        if (rcvsize < 0) {
        error("%s: Cannot receive from the socket\n",argv[0]);
        exit(5);
        }
	    printf("%s\n",buffer);
		bytes = strlen(buffer) + 1;
		

		
		for(;;){
		  printf("Add meg a tipped: "); //tipp küldése a szervernek G
		  scanf("%s",buffer);
		  bytes = strlen(buffer) + 1;
		  trnmsize = send(fd, buffer, bytes, flags);
          if (trnmsize < 0) {
			error("%s: Cannot send data to server.\n", argv[0]);
			exit(3);
		  }
		  rcvsize = recv(fd, buffer, BUFSIZE, flags ); //H
          if (rcvsize < 0) {
             error("%s: Cannot receive from the socket\n",argv[0]);
             exit(5);
          }
		  bytes = strlen(buffer) + 1;
	      trnmsize = send(fd, buffer, bytes, flags); //K
          if (trnmsize < 0) {
           error("%s: Cannot send data to server.\n", argv[0]);
           exit(3);
           } 
		   rcvsize = recv(fd, buffer2, BUFSIZE, flags ); //Y
          if (rcvsize < 0) {
             error("%s: Cannot receive from the socket\n",argv[0]);
             exit(5);
          }
		  bytes = strlen(buffer2) + 1;
	      trnmsize = send(fd, buffer2, bytes, flags); //YY
          if (trnmsize < 0) {
           error("%s: Cannot send data to server.\n", argv[0]);
           exit(3);
           }
		  
		  
		  if (!strcmp(buffer,"vesztettel")){
			  printf("Sajnálom, vesztettél\n");
			  break;
		  }
		  else if (!strcmp(buffer,"nyertel")){
		      printf("Gratulálok, nyertél!\n");
			  break;
		}else{
		  printf("%s\n\n",buffer);
		  kirajzol(atoi(buffer2));
			} 
		}
	 		 
	printf("Akarsz még játszani? ujra/vege\n");
    scanf("%s",buffer);
	
	
	bytes = strlen(buffer) + 1;
	trnmsize = send(fd, buffer, bytes, flags); //V
    if (trnmsize < 0) {
        error("%s: Cannot send data to server.\n", argv[0]);
        exit(3);
    }
	
	if (!strcmp(buffer,"vege")){

		close(fd);
        exit(0);
	
	}
	}	
	
  } 
} 


