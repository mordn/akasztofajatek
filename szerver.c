/***************************************/
/*              TCP server             */
/***************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h> 

#define BUFSIZE 1024                      // buffer size
#define PORT_NO 2001                      // port number
#define error(a,b) fprintf(stderr, a, b)  // error 'function'
int darab(char ellenorizendo[BUFSIZE+1]){
	int i,db=0;
	for(i=0;i<strlen(ellenorizendo);i++)
	   if (ellenorizendo[i]=='_')
		   db++;
	return db;   
}


int main(int argc, char *argv[] ) { 	

   /* Declarations */
   int fd;	        	            // socket endpt
   int player1;                     // socket endpt
   int player2;				        // socket endpt
   int flags;                      	// rcv flags
   struct sockaddr_in server;      	// socket name (addr) of server
   struct sockaddr_in client;	    // socket name of client
   struct sockaddr_in client2;		// socket name of client2
   int server_size;                	// length of the socket addr. server
   int client_size;                	// length of the socket addr. client
   int client_size2;				// length of the socket addr. client
   int bytes;		              	// length of buffer 
   int rcvsize;                    	// received bytes
   int trnmsize;                   	// transmitted bytes
   int err;                        	// error code
   char on;                        	// 
   char buffer[BUFSIZE+1];     		// datagram dat buffer area 
   int kijon;						// ciklusvaltozo
   char aktualis[BUFSIZE+1];		// ebben lesz a kitalálandó szöveg aktuális állapota 
   char szoveg[BUFSIZE+1];			// ebben lesz a kitalálandó szöveg rendesen
   int i;							// ciklusvaltozo	
   int h;							// hibakod
   int voltehiba;					// helyes volt-e a tipp
   int p1,p2;						// jatekosok

   /* Initialization */
   on                     = 1;
   flags                  = 0;
   bytes                  = BUFSIZE;
   server_size            = sizeof server;
   client_size            = sizeof client;
   client_size2           = sizeof client2;
   server.sin_family      = AF_INET;
   server.sin_addr.s_addr = INADDR_ANY;
   server.sin_port        = htons(PORT_NO);
   kijon 				  = 1;

   /* Creating socket */
   fd = socket(AF_INET, SOCK_STREAM, 0 ); //sock dgramm UDP nél
   if (fd < 0) {

      error("%s: Socket creation error\n",argv[0]);
      exit(1);
      }

   /* Setting socket options */
   setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
   setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

   /* Binding socket */
   err = bind(fd, (struct sockaddr *) &server, server_size);
   if (err < 0) {

     error("%s: Cannot bind to the socket\n",argv[0]);
     exit(2);
   }
   /* Listening */
   err = listen(fd, 2); //mennyi feldolgozatlan csatlakozási kérelmet tud kezelni a szerver
   if (err < 0) {

     error("%s: Cannot listen to the socket\n",argv[0]);
     exit(3);
   }

   /* Accepting connection request */
   player1 = accept(fd, (struct sockaddr *) &client, &client_size); //első játékos csatlakozik
   if (player1 < 0) {

     error("%s: Cannot accept on socket\n",argv[0]);
     exit(4);
   }
   
   sprintf(buffer,"1");
   bytes = strlen(buffer) + 1;
   trnmsize = send(player1, buffer, bytes, flags);
   if (trnmsize < 0) {

     error("%s: Cannot send data to the client.\n",argv[0]);
     exit(6);
   }
   
   rcvsize = recv(player1, buffer, BUFSIZE, flags ); 
     if (rcvsize < 0) {

       error("%s: Cannot receive from the socket\n",argv[0]);
       exit(5);
     }

   player2 = accept(fd, (struct sockaddr *) &client2, &client_size2); // második játékos csatlakozik
   if (player2 < 0) {

     error("%s: Cannot accept on socket\n",argv[0]);
     exit(4);
   }
   
   sprintf(buffer, "Az akasztófa játék elindult."); // kiküldjük az adatokat
   bytes = strlen(buffer) + 1;
   trnmsize = send(player1, buffer, bytes, flags);
   if (trnmsize < 0) {

     error("%s: Cannot send data to the client.\n",argv[0]);
     exit(6);
   }

   rcvsize = recv(player1, buffer, BUFSIZE, flags ); 
     if (rcvsize < 0) {

       error("%s: Cannot receive from the socket\n",argv[0]);
       exit(5);
     }

   sprintf(buffer, "Az akasztófa játék elindult.\nAz első játékos kezd"); // kiküldjük az adatokat
   bytes = strlen(buffer) + 1;
   trnmsize = send(player2, buffer, bytes, flags);
   if (trnmsize < 0) {

     error("%s: Cannot send data to the client.\n",argv[0]);
     exit(6);
   }

  rcvsize = recv(player2, buffer, BUFSIZE, flags ); 
       if (rcvsize < 0) {

         error("%s: Cannot receive from the socket\n",argv[0]);
         exit(5);
       }
   for(;;kijon++){  //a játék kezdete
   if (kijon%2){
	   p1=player1;
	   p2=player2;
   }else{
	   p1=player2;
	   p2=player1;
   }

   h=0;
   sprintf(buffer,"1"); 
   bytes = strlen(buffer) + 1;
   trnmsize = send(p1, buffer, bytes, flags);
   if (trnmsize < 0) {

     error("%s: Cannot send data to the client.\n",argv[0]);
     exit(6);
   }
		  
   rcvsize = recv(p1, buffer, BUFSIZE, flags ); 
     if (rcvsize < 0) {

       error("%s: Cannot receive from the socket\n",argv[0]);
       exit(5);
     }  
   sprintf(buffer,"2"); 
   bytes = strlen(buffer) + 1;
   trnmsize = send(p2, buffer, bytes, flags);
   if (trnmsize < 0) {

     error("%s: Cannot send data to the client.\n",argv[0]);
     exit(6);
   }
		  
     rcvsize = recv(p2, buffer, BUFSIZE, flags ); 
     if (rcvsize < 0) {

       error("%s: Cannot receive from the socket\n",argv[0]);
       exit(5);
     }		
     //sorszámok kiküldve ack fogadva
	 rcvsize = recv(p1, buffer, BUFSIZE, flags );  //megkapom a kitalálandó szöveget A
     if (rcvsize < 0) {

       error("%s: Cannot receive from the socket\n",argv[0]);
       exit(5);
      }		
	  printf("a kitalalando szoveg: %s, hossza: %d\n",buffer,(int)strlen(buffer));
	  for (i=0;i<strlen(buffer);i++){   
		  aktualis[i]='_';			//az aktuálist feltöltöm aláhúzásokkal
		  szoveg[i]=buffer[i];		//a szoveg be bemasolom a buffert
	  }   
	  szoveg[i]='\0';
	  aktualis[i]='\0';	 
	 
	 bytes = strlen(aktualis) + 1;      //kiküldöm az aláhúzásokat a 2 kliensnek
     trnmsize = send(p1, aktualis, bytes, flags); //B
     if (trnmsize < 0) {
     error("%s: Cannot send data to the client.\n",argv[0]);
     exit(6);
     }

     rcvsize = recv(p1, buffer, BUFSIZE, flags );  //C
     if (rcvsize < 0) {
       error("%s: Cannot receive from the socket\n",argv[0]);
       exit(5);
     }		  
	 bytes = strlen(aktualis) + 1;
     trnmsize = send(p2, aktualis, bytes, flags);   //D
     if (trnmsize < 0) {
       error("%s: Cannot send data to the client.\n",argv[0]);
       exit(6);
     }  

	 for(;;){
	 rcvsize = recv(p2, buffer, BUFSIZE, flags );  //tipp beolvasása a játékostól G
	 printf("tipp fogadva: %s\n",buffer);
     if (rcvsize < 0) {
       error("%s: Cannot receive from the socket\n",argv[0]);
       exit(5);
     }
	 if (!strcmp(buffer,"feladom")) {
		 h=10;
	 }
	 if (!strcmp(buffer,szoveg)) {
	    for(i=0;i<strlen(szoveg);i++){
			aktualis[i]=szoveg[i];
		}
	 }else{
	  voltehiba=1;
	  for(i=0;i<strlen(szoveg);i++){
		 if (szoveg[i]==buffer[0]){
			 aktualis[i]=buffer[0];
			 voltehiba=0;
		 }	 
	  }
	 }
	 if (voltehiba){
		 h++;
	 }
	 printf("hiba: %d\n",h);
	 if (h>9){
		 printf("a tippelő játékos vesztett!!!\n");
		 
		 sprintf(buffer,"nyertel"); 
         bytes = strlen(buffer) + 1;
         trnmsize = send(p1, buffer, bytes, flags); //I
         if (trnmsize < 0) {
             error("%s: Cannot send data to the client.\n",argv[0]);
             exit(6);
         }
		 rcvsize = recv(p1, buffer, BUFSIZE, flags );  //J 
         if (rcvsize < 0) {
            error("%s: Cannot receive from the socket\n",argv[0]);
            exit(5);
         }
		 sprintf(buffer,"%d",h);
	     bytes = strlen(buffer) + 1;
		 trnmsize = send(p1, buffer, bytes, flags); //X
		 if (trnmsize < 0) {
            error("%s: Cannot send data to the client.\n",argv[0]);
            exit(6);
        }
		 rcvsize = recv(p1, buffer, BUFSIZE, flags );  //XX
         if (rcvsize < 0) {
            error("%s: Cannot receive from the socket\n",argv[0]);
            exit(5);
         }
         sprintf(buffer,"vesztettel"); 
         bytes = strlen(buffer) + 1;
         trnmsize = send(p2, buffer, bytes, flags);  //H
         if (trnmsize < 0) {
            error("%s: Cannot send data to the client.\n",argv[0]);
            exit(6);
         }
		 rcvsize = recv(p2, buffer, BUFSIZE, flags );  //K
         if (rcvsize < 0) {
            error("%s: Cannot receive from the socket\n",argv[0]);
           exit(5);
         }
		 sprintf(buffer,"%d",h);
	     bytes = strlen(buffer) + 1;
		 trnmsize = send(p2, buffer, bytes, flags); //Y
		 if (trnmsize < 0) {
            error("%s: Cannot send data to the client.\n",argv[0]);
            exit(6);
        }
		 rcvsize = recv(p2, buffer, BUFSIZE, flags );  //YY
         if (rcvsize < 0) {
            error("%s: Cannot receive from the socket\n",argv[0]);
           exit(5);
         }

		 break;
		 
	 }else{
	
	 if (darab(aktualis)){
		 
	 
	    bytes = strlen(aktualis) + 1;      
        trnmsize = send(p1, aktualis, bytes, flags); //I
        if (trnmsize < 0) {
          error("%s: Cannot send data to the client.\n",argv[0]);
          exit(6);
        }
		
		rcvsize = recv(p1, buffer, BUFSIZE, flags );  //J 
         if (rcvsize < 0) {
            error("%s: Cannot receive from the socket\n",argv[0]);
            exit(5);
         }
	     sprintf(buffer,"%d",h);
	     bytes = strlen(buffer) + 1;
		 trnmsize = send(p1, buffer, bytes, flags); //X
		 if (trnmsize < 0) {
            error("%s: Cannot send data to the client.\n",argv[0]);
            exit(6);
        }  
		rcvsize = recv(p1, buffer, BUFSIZE, flags );  //XX
         if (rcvsize < 0) {
            error("%s: Cannot receive from the socket\n",argv[0]);
            exit(5);
         }
		 bytes = strlen(aktualis) + 1;
        trnmsize = send(p2, aktualis, bytes, flags); //H
        if (trnmsize < 0) {
          error("%s: Cannot send data to the client.\n",argv[0]);
          exit(6);
        }
		
		rcvsize = recv(p2, buffer, BUFSIZE, flags );  //K
         if (rcvsize < 0) {
            error("%s: Cannot receive from the socket\n",argv[0]);
           exit(5);
         }
		 sprintf(buffer,"%d",h);
	     bytes = strlen(buffer) + 1;
		 trnmsize = send(p2, buffer, bytes, flags); //Y
		 if (trnmsize < 0) {
            error("%s: Cannot send data to the client.\n",argv[0]);
            exit(6);
        }		
		   	 rcvsize = recv(p2, buffer, BUFSIZE, flags );  //YY
         if (rcvsize < 0) {
            error("%s: Cannot receive from the socket\n",argv[0]);
           exit(5);
         }
	 
	    
	   // printf("aktualis: %s hossza:%d\n",aktualis,(int)strlen(aktualis));
	   // printf("alahuzasok szama: %d\n",darab(aktualis));
	    
	  
	   }else{
		   
		 printf("a tippelő játékos nyert!!!\n");
		 
		 sprintf(buffer,"vesztettel"); 
         bytes = strlen(buffer) + 1;
         trnmsize = send(p1, buffer, bytes, flags); //I
         if (trnmsize < 0) {
             error("%s: Cannot send data to the client.\n",argv[0]);
             exit(6);
         }
		 rcvsize = recv(p1, buffer, BUFSIZE, flags );  //J 
         if (rcvsize < 0) {
            error("%s: Cannot receive from the socket\n",argv[0]);
            exit(5);
         }
		 sprintf(buffer,"%d",h);
	     bytes = strlen(buffer) + 1;
		 trnmsize = send(p1, buffer, bytes, flags); //X
		 if (trnmsize < 0) {
            error("%s: Cannot send data to the client.\n",argv[0]);
            exit(6);
        }
		rcvsize = recv(p1, buffer, BUFSIZE, flags );  //XX
         if (rcvsize < 0) {
            error("%s: Cannot receive from the socket\n",argv[0]);
            exit(5);
         }
		 
		 
         sprintf(buffer,"nyertel"); 
         bytes = strlen(buffer) + 1;
         trnmsize = send(p2, buffer, bytes, flags);  //H
         if (trnmsize < 0) {
            error("%s: Cannot send data to the client.\n",argv[0]);
            exit(6);
         }
		 rcvsize = recv(p2, buffer, BUFSIZE, flags );  //K
         if (rcvsize < 0) {
            error("%s: Cannot receive from the socket\n",argv[0]);
           exit(5);
         }
		 sprintf(buffer,"%d",h);
	     bytes = strlen(buffer) + 1;
		 trnmsize = send(p2, buffer, bytes, flags); //Y
		 if (trnmsize < 0) {
            error("%s: Cannot send data to the client.\n",argv[0]);
            exit(6);
        }
	   	 rcvsize = recv(p2, buffer, BUFSIZE, flags );  //YY
         if (rcvsize < 0) {
            error("%s: Cannot receive from the socket\n",argv[0]);
           exit(5);
         }
		 break;
	   } 
	   }
	 }

	 rcvsize = recv(p2, buffer, BUFSIZE, flags );  //V
     if (rcvsize < 0) {
         error("%s: Cannot receive from the socket\n",argv[0]);
         exit(5);
     } 
	 
	 bytes = strlen(buffer) + 1;
	 trnmsize = send(p1, buffer, bytes, flags); //VV
		 if (trnmsize < 0) {
            error("%s: Cannot send data to the client.\n",argv[0]);
            exit(6);
        }
      	if (!strcmp(buffer,"vege")){

			close(player1);
			close(player2);
			exit(0);	
	
	} 	  
   }
} 
