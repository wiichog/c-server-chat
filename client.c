#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 


int sendMsg(int csocket, char *buf, int *len ){

	int total = 0;
	int bytesleft = *len;
	int n;
	while (total < *len) {
		n = send(csocket, buf + total, bytesleft, 0);
		if (n == -1) {
			break;
		}
		total += n;
		bytesleft -= n;
	}
	
	*len = total;
	return n == -1?-1:0; //return -1 on fail, 0 on success
}

int main(int argc, char *argv[])
{
    int sockfd = 0, nbytes = 0, status = 0, msglen;
    char recvBuff[1024] = " ";
    char msg[1024] = " ";
    char s[INET_ADDRSTRLEN];
    
    struct addrinfo serverinfo, *result, *p;
    

    if(argc != 3)
    {
        printf("\n Usage: %s <ip of server> <port of server>\n",argv[0]);
        return 1;
    } 

    memset(&serverinfo, 0, sizeof serverinfo);
    serverinfo.ai_family = AF_INET;
    serverinfo.ai_socktype = SOCK_STREAM;

    if(status = getaddrinfo(argv[1], argv[2], &serverinfo, &result) != 0){
    	fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }

    for (p = result; p != NULL; p = p->ai_next){

	    if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
	    {
		perror("\n Error : Could not create socket \n");
		continue;
	    } 

	    if( connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
	    {
	       close(sockfd);
	       printf("\n Error : Connect Failed \n");
	       continue;
	    } 
	    break;
    }
    if (p == NULL){
    	fprintf(stderr, "client: failed to connect\n");
        return 2;
    }
	freeaddrinfo(result);  
	struct sockaddr_in *ipv4 = (struct sockaddr_in *)p;

	inet_ntop(p->ai_family, &(ipv4->sin_addr), s, sizeof s);
    while(1){
		memset(recvBuff, 0, sizeof recvBuff);
	    
		//revisar el print de abajo, siempre esta devolviendo 2.0.0.0 tiene que ver con la struct de p

	    printf("client: connecting to %s\n",s);
	    

	    printf("Enter message : ");
	    scanf("%s" , msg);
	
	    msglen = strlen(msg);
	    if( sendMsg(sockfd, msg, &msglen) == -1){
	    	perror("send");
			printf("Only %d bytes were sent due to error \n", msglen);
								 
	    }
		puts(recvBuff);
	    if( nbytes = recv(sockfd, recvBuff, sizeof(recvBuff)-1, 0) == -1){
	    	perror ("Error receiving msg");
			exit(1);
	    }
	    else if(nbytes == 0){
	    	perror ("Remote host closed connection");
	    }
	
	    printf("client:received %s\n", recvBuff);
		
    }	    
    close(sockfd);

    return 0;
}
