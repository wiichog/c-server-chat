#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

int sendMsg(int csocket, char *buf, int *len){
	int total = 0;
	int bytesleft = *len;
	int n;
	while(total<*len){
		n = send(csocket, buf+ total, bytesleft, 0);
		if(n==-1){
			break;		
		}	
	*len = total;
	return n == -1?-1:0;
	}
}

int cliente(int argc, char *argv[]){
	int sockfd = 0, nbytes = 0, status = 0, msglen;
	char recvBuff[1024]= " ";
	char msg[1024] = " ";
	char s[INET_ADDRSTRLEN];
	struct addrinfo serverinfo, *result, *p;
	if(argc!=3){
		printf("\n Porfavor ingresar direccion ip del server seguida del puerto del server");
		return 1;
	}
	memset(&serverinfo,0,sizeof serverinfo);
	serverinfo.ai_family = AF_INET;//para ipv4
	serverinfo.ai_socktype = SOCK_STREAM;//tipo del socket es stream
	if(status = getaddrinfo(argv[1],argv[2],&serverinfo,&result) != 0){
	//verificamos el server le enviamos la direccion ip, el puerto
		fprintf(stderr, "getaddrinfo: %s \n ",gai_strerror(status));
		return 1;
	}
	for(p=result; p!=NULL; p=p->ai_next){
		if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
			perror("\n Error: Could not creat socket \n");
			continue;
		}
		if(connect(sockfd, p->ai_addr, p->ai_addrlen)==-1){
			close(sockfd);
			printf("\n Error: Connect Failed \n");
			continue;		
		}
		break;
	}
	if(p==NULL){
		fprintf(stderr, "\n Client: failed to connect \n");
	}
	freeaddrinfo(result);
	struct sockaddr_in *ipv4 = (struct sockaddr_in *)p;
	inet_ntop(p->ai_family, &(ipv4->sin_addr), s, sizeof s);
	while(1){
		memset(recvBuff,0,sizeof recvBuff);
		printf("\n Client: connection to %s\n",s);
		printf("\n Enter message: ");
		scanf("%s", msg);
		msglen = strlen(msg);
		if(sendMsg(sockfd, msg, &msglen)==-1){
			perror("send");
			printf("Only %d bytes were sent due to error \n", msglen);
		}
		if(nbytes = recv(sockfd, recvBuff,sizeof(recvBuff)-1,0) == -1){
			perror("Error receiving msg");
			exit(1);
		}	
		else if(nbytes==0){
			perror("Remote host closed connection");
		}
		printf("client:received %s\n", recvBuff);
	}
	close(sockfd);
	return 0;
}

void Ayuda(void){
	printf("\n  Aqui tendremos la ayuda");	
	return;
}

void ListarUsuarios(void){
	char* token;
	char* token2;
	char* string;
	char* tofree;
	string = strdup("usuario1+estado1&usuario2+estado2&usuarioN+estadoN");
	if(string!=NULL){
		tofree = string;
		while((token=strsep(&string,"&"))){
		printf("\n******************\n");
			while((token2=strsep(&token,"+"))){
				printf("%s \n", token2);		
			}	
		printf("\n******************");	
		}	
	
	}
	return;
}

void Menu(int argc, char *argv[]){
	int opcion;
	do{
		printf("\n  1. Chatear");
		printf("\n  2. Cambiar de Estado");
		printf("\n  3. Listar Usuarios");
		printf("\n  4. Ayuda");
		printf("\n  5. Salir");
		printf("\n  Elegir un opcion (1-4)", 162);
		scanf("\%d", &opcion);
	switch(opcion){
		case 1:cliente(argc,argv);
		break;
		case 2:printf("\n  funcion2");
		break;
		case 3:ListarUsuarios();
		break;
		case 4:Ayuda();
		break;
		}
	}
	while(opcion!=5);
	return;
}

int main(int argc, char*argv[]){
	Menu(argc,argv);
	return 0;
}


