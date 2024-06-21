#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <iostream>
#include <cstring>
#include <string>
#include "functions.cpp"

class Connection{
    private:
    GSC_Functions F;
    public:
    bool connected;
    int main_socket;
    sockaddr_in server_adress;
    Connection(){
        main_socket = socket(AF_INET, SOCK_STREAM, 0);
        connected=false;
        F.log("SERVER","Socket created!");
    };
    int serverStart(){
        server_adress.sin_family=AF_INET;
        server_adress.sin_port=htons(12500);
        server_adress.sin_addr.s_addr=INADDR_ANY;
        bind(main_socket,(struct sockaddr*)&server_adress,sizeof(server_adress));
        listen(main_socket,5);
        int client_socket = accept(main_socket, nullptr, nullptr);
        F.log("SERVER","Connected to client!");
        connected=true;
    };
    int clientConnect(){
        server_adress.sin_family=AF_INET; 
        server_adress.sin_port=htons(12500); 
        server_adress.sin_addr.s_addr=INADDR_ANY; 
        connect(main_socket,(struct sockaddr*)&server_adress,sizeof(server_adress));
        connected=true;
        F.log("SERVER","Connected to server!");
    };

};