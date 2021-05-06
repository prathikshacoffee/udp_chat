// Server side implementation of UDP client-server model
#include <iostream>
#include <vector>
#include <memory>
#include <ctime>
#include <chrono>
#include <string.h>

#include <unistd.h> 
#include <sys/socket.h> 
#include <stdlib.h> 	
#include <netinet/in.h> 
 
#include <arpa/inet.h>

#define PORT	 8080
#define MAXLINE 1024

//Server
int server_fd, sock, valread; 
int opt; 
int addrlen; 
std::string name_str;
//udp
struct sockaddr_in  cliaddr, servaddr; 
unsigned int len;
int n;
char buffer[MAXLINE];

bool serverConnect(){

	bool return_server = false;
	std::cout<<"server_conntect called"<<std::endl;

    // Creating socket file descriptor 
    if ( (sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 

    memset(&servaddr, 0, sizeof(servaddr)); 
    
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_addr.s_addr = INADDR_ANY; 

    if (bind(sock, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
        
    // close(sock); 

	return_server = true;

	return return_server;

}

void client_init(){

	memset(&cliaddr, 0, sizeof(cliaddr));
  
    len = sizeof(cliaddr);  //len is value/resuslt
  
    n = recvfrom(sock, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);

    std::cout<<"first test message recived, init successfull"<<std::endl;
}

// Driver code
int main() {
	bool server_status = false;


	while(!server_status){

		std::cout<<"connecting to the server"<<std::endl;
		server_status = serverConnect();
		if(!server_status){
			std::cout<<"connection failed, retrying.."<<std::endl;
			sleep(5);
			continue;
		}
		std::cout<<"server connected, you can now push data"<<std::endl;

	}
	client_init();
	std::string message = "hoi bomsh!";
	while(true){
		std::cout<<"Pampsh: ";
		std::getline(std::cin, message);
		sendto(sock, message.c_str(), strlen(message.c_str()), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, sizeof(cliaddr));

		n = recvfrom(sock, (char *)buffer, MAXLINE,MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
		buffer[n] = '\0';
		std::cout<<"Bumsh: "<<buffer<<std::endl;
	}
	
	return 0;
}
