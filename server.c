/*

status
0    Activo   
1    Idle
2    Away

*/



#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/wait.h>
#include <signal.h>
#include <netinet/in.h>
#include <pthread.h>
#include <assert.h>
#include "hashmap.h"

#define KEY_MAX_LENGTH (30)

typedef struct data_struct_s {
	char key_string[KEY_MAX_LENGTH];
	int number;
	char ip[INET_ADDRSTRLEN];
	int port;
	int status;
} data_struct_t;

struct threadParams {

	int clientFD;
	fd_set *master;
	fd_set *read_fds;
	int listener;
	int *fdmax;
	map_t map;

};


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

void usrMsg(char sender[], char target[], char msg[], map_t map){
	

}
void getUsrInfo(char user[], char usrAsk[], map_t map){
	

}
void getUsers(char user[], map_t map){
	

}

void changeStat(char user[], int status, map_t map){
	
}

void dcUser(char user[], map_t map){
	

}

void regUser(char user[], char ip[INET_ADDRSTRLEN], int port, int status, map_t map ){

	int error;
	data_struct_t* value;
	char key_string[KEY_MAX_LENGTH];
	
	value = malloc(sizeof(data_struct_t));
	snprintf(value->key_string, KEY_MAX_LENGTH, "%s", user);
	
	value-> number = hashmap_length(map) + 1;
	printf("string is %s\n", value->key_string);
	error = hashmap_put(map, value->key_string, value);
	assert(error == MAP_OK);	

}

void errorReg(char user[], char ip[INET_ADDRSTRLEN]){
	
}

void handleRequest(int protocol, char msge[], map_t *map){

	msge = "07|mochila|mochila2|mensajestupido";
	char delim[1] = "|";
	char *token;
	int i;
	char *msg = strdup(msge);
	protocol = 8;
	
	
	token = strtok(msg, delim);
	
	switch(protocol){
		case 0 :
			/*
			Registro de Usuario
				[Registro {cliente a servidor}]
				00|ususario|direccionIP|puerto|status¬
			*/
			i = 0;
			char *params[4]; 
			
			while((token = strtok(NULL, delim)) != NULL){
				
				params[i] = token;
				i++;
			}
			

			int port;
			int status;
			char *ptr;
			port = (int)strtol(params[2], &ptr, 10);
			status = (int)strtol(params[3], &ptr, 10);
						
			regUser(params[0], params[1], port, status, map);

			return;
		case 1 :
			/*
			[Error de registro {servidor a cliente}]
				01|usuario|direccionIP¬
			*/
			i = 0;
			char *params1[2]; 
			while ((token = strtok(NULL, delim)) != NULL){
				
				params1[i] = token;
				i ++;
			}
			return;
		case 2 :
			/*
			
			Liberacion de Usuario
			[Cliente cierra chat {cliente a servidor}]
				02|usuario¬

			*/
			
			while ((token = strtok(NULL, delim)) != NULL){
				
				char *params2 = token;
				dcUser(params2, map);
			}
			
			return;
		case 3 :
			/*
			
			Cambio de status
			[Cliente cambio estado {cliente a servidor}]
				03|usuario|status¬
			*/
			i = 0;
			char *params3[2]; 
			while ((token = strtok(NULL, delim)) != NULL){
				
				params3[i] = token;
				i ++;
			}
			int status3;
			char *ptr3;
			status3 = (int)strtol(params3[1], &ptr3, 10);
			changeStat(params3[0], status3, map);
			return;
		case 4 :
			/*
			     
			Obtencion de informacion de usuario
			[Peticion de informacion {cliente a servidor}]
				04|usuarioDeInfo|usuarioQuePide¬
			*/
			i = 0;
			char *params4[2]; 
			while ((token = strtok(NULL, delim)) != NULL){
				
				params4[i] = token;
				i ++;
			}
			getUsrInfo(params4[0], params4[1], map);
			return;
		case 5 :
			/*			
			[Retorno de informacion {servidor a cliente}]
				05|usuario|direccionIP|puerto|status¬
			*/
			i = 0;
			char *params5[4]; 
			while ((token = strtok(NULL, delim)) != NULL){
				
				params5[i] = token;
				i ++;
			}
			return;
		case 6 :
			/*
			
			Listado de usuarios conectados
			[Peticion de listado {cliente a servidor}]
				06|usuarioQuePide¬
			*/
			
			while ((token = strtok(NULL, delim)) != NULL){
				
				char *params6 = (char*)token;
				getUsers(params6, map);
			}
			return;
		case 7 :
			/*

			[Retorno de listado {servidor a cliente}]
				07|usuarioQuePide|usuario1+estado1&usuario2+estado2...usuarioN+estadoN¬
				TODO FUNCTION TO GET USERS+STATES
			*/
			i = 0;
			char *params7[2]; 
			while ((token = strtok(NULL, delim)) != NULL){
				
				params7[i] = token;
				i ++;
			}
			return;
		case 8:
			/*
				
			[Mensaje {cliente a servidor}]
				08|emisor|receptor|mensaje
			*/
			i = 0;
			char *params8[3]; 
			while ((token = strtok(NULL, delim)) != NULL){
				
				params8[i] = token;
				i ++;
			}
			usrMsg(params8[0], params8[1], params8[2], map);
			return;
		default : 
			printf("Invalid protocol %d", protocol);
			return;	
	}

}

int getProt(char msg[]){
	
	msg = "07|user|192.168.0.1|1100|1";
	
	long protocol;
	char *str;
	protocol = strtol(msg, &str, 10);
	
	
	if (protocol){
		return (int)protocol;
	}
	else{
		
		printf("No protocol code was found in %s\n", msg);
		fflush(stdout);
	}

	return 10;
}

void *connection_handler( void *arg){
	
	struct threadParams *params = arg;
	int sockFD = params->clientFD;
	int listener = params->listener;
	fd_set *read_fds = params->read_fds;
	fd_set *master = params->master;
	int *fdmax = params->fdmax;
	map_t *map = params->map;

	int cbuff;
	char buf [256] = " ";
	memset(buf, 0, sizeof buf);
	int accept = 1;

	while(accept == 1){
		if (FD_ISSET(sockFD, read_fds)){
			if(sockFD != listener){

				if ((cbuff = recv(sockFD, buf, sizeof buf, 0)) <= 0) {
					if (cbuff == 0) {
						printf("selectserver: socket %d hung up\n", sockFD);	
					}
					else {
						perror("recv");			
					}
					close(sockFD);
					FD_CLR(sockFD, master);
				}
				else{	
						
					handleRequest(getProt(buf), buf, map);									
					int i;
					for(i = 0; i <= *(int*)fdmax; i++){
					
						if (FD_ISSET(i, master)){
							if (i != listener && i != 1){
								if (sendMsg(i, buf, &cbuff) == -1) {
									perror("send");
									printf("Only %d bytes were sent due to error \n", cbuff);
								}
							}
						}
					}
								
				}
			}else{
				printf("sockfd == listener");
			}
		}

		else{
			printf("FD_ISSET == false");
		}
	}
	return 0;
}


int main(int argc, char *argv[])
{

	map_t *map;
	map = hashmap_new();
/*

	for (index = 0; index < KEY_COUNT; index += 1){
		value = malloc(sizeof(data_struct_t));
		snprintf(value->key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, index);
		
		value-> number = index;
		printf("string is %s\n", value->key_string);
		error = hashmap_put(mymap, value->key_string, value);
		assert(error == MAP_OK);	
	} 

	
	
	// Now, check all of the expected values are there 

	for (index=0; index<KEY_COUNT; index+=1){
		snprintf(key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, index);
	
		error = hashmap_get(mymap, key_string, (void**)(&value));
	
		// Make sure the value was both found and the correct number 
		assert(error==MAP_OK);
		assert(value->number==index);
	}
	
	// Make sure that a value that wasn't in the map can't be found 
	
	snprintf(key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, KEY_COUNT);
	
	error = hashmap_get(mymap, key_string, (void**)(&value));
	
	// Make sure the value was not found 
	
	assert(error==MAP_MISSING);
	
	// Free all of the values we allocated and remove them from the map 
	
	for (index=0; index<KEY_COUNT; index+=1){
		snprintf(key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, index);
	
		error = hashmap_get(mymap, key_string, (void**)(&value));
		assert(error==MAP_OK);
	
		error = hashmap_remove(mymap, key_string);
		assert(error==MAP_OK);
	
		free(value);
	}
	
	// Now, destroy the map 
	hashmap_free(mymap);

*/

    fd_set master;
    fd_set read_fds;
    int fdmax;
    
    int listener = 0, connfd = 0;

    int status = 0;
    int enable = 1;
    int cbuff, i, j;

    struct threadParams *params;
    struct addrinfo servinfo, *result, *p; 
    struct sockaddr_storage client_addr;
    struct sockaddr *addr;
    char s[INET_ADDRSTRLEN],buf [256] = " ";
    
    socklen_t addr_size;

    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    memset(&servinfo, 0 , sizeof servinfo);

    servinfo.ai_family = AF_INET;
    servinfo.ai_socktype = SOCK_STREAM;
    servinfo.ai_flags = AI_PASSIVE;
   
    if((status = getaddrinfo(NULL, "1100", &servinfo, &result)) != 0) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
	return 1;
    }
    
    for (p = result; p != NULL; p = p->ai_next){
	if((listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
    		perror("cannot create socket");
		continue;
	}
	if(setsockopt(listener,SOL_SOCKET, SO_REUSEADDR, &enable, sizeof enable ) == -1){
		perror("setsockopt(SO_REUSEADDR) failed");
		exit(1);
	}
	if (bind(listener, p->ai_addr, p->ai_addrlen) == -1 ) {
	    
		perror("bind failed");
		close(listener);
		continue;
    	}
	break;
    
    
    }
     
    
    if (p == NULL) {
    	fprintf(stderr,"server: failed to bind\n");
	exit(1);
    }

    freeaddrinfo(result); 

    if( listen(listener, 10) == -1 ){
    	perror("listen failed");
	exit(3);
    } 

    FD_SET(listener, &master);
    fdmax = listener;

    printf("server: waiting for connections...\n");


    while(1)
    {
	read_fds = master;
	if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1){
		perror("select");
		exit(4);
    	}

        for(i = 0; i <= fdmax; i++){
		if (FD_ISSET(i, &read_fds)){
			if(i == listener){
				addr_size = sizeof client_addr;
				connfd = accept(listener, (struct sockaddr*)&client_addr, &addr_size); 
									
				if (connfd == -1){
					perror("accept failed");
					continue;
				}
		
				else {
					FD_SET(connfd, &master);
					if(connfd > fdmax) {
						fdmax = connfd;
					}
					struct sockaddr_in *ipv4 = (struct sockaddr_in *)&client_addr;
					printf("selectserver: new connection from %s on socket %d\n", inet_ntop(client_addr.ss_family, &(ipv4->sin_addr), s, sizeof s),connfd);


					//duda existencial de freddie = usar detach ? es necesario aqui en algun momento join? 

					pthread_t client_thread;
					params = malloc(sizeof (*params));
					params->clientFD = connfd;
					params->master = &master; 
					params->listener = listener;
					params->read_fds = &read_fds;
					params->fdmax = &fdmax;
					params->map = &map;
					
					if(pthread_create( &client_thread, NULL, connection_handler, (void*) params) < 0){
						perror("Could not create thread");
						return 1;
					}
					
				}
			}
		}
	}
    }
    return 0;
}

