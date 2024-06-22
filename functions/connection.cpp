#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>

#include "functions.cpp"

class Connection{
    private:
    GSC_Functions F;
    public:
    bool connected;
    bool server;
    bool client;
    int main_socket;
    int server_socket;
    int client_socket;
    char buffer[1024];
    std::vector<int> other_player_pos;
    sockaddr_in server_adress;
    Connection(){
        main_socket = socket(AF_INET, SOCK_STREAM, 0);
        connected=false;
        other_player_pos={0,0};
        F.log("SERVER","Socket created!");
    };
    int serverStart(){
        server_adress.sin_family=AF_INET;
        server_adress.sin_port=htons(12500);
        server_adress.sin_addr.s_addr=INADDR_ANY;
        bind(main_socket,(struct sockaddr*)&server_adress,sizeof(server_adress));
        listen(main_socket,5);
        client_socket = accept(main_socket, nullptr, nullptr);
        recv(main_socket , buffer , sizeof(buffer) , 0);
        connected=true;
        std::string result;
        result=buffer;
        F.log("SERVER","Client connected!"+result);
        server=true;
        return 0;
    };
    int clientConnect(){
        server_adress.sin_family=AF_INET;
        server_adress.sin_port=htons(12500);
        server_adress.sin_addr.s_addr=INADDR_ANY;
        connect(server_socket,(struct sockaddr*)&server_adress,sizeof(server_adress));
        std::string fish="salut";
        const char* message = fish.c_str();
        send(main_socket, message, strlen(message), 0); 
        connected=true;
        F.log("SERVER","Connected to server!");
        client=true;
        return 0;
    };
    int sendPos(std::vector<int> player_pos){
        F.log("SERVER","Waiting for other player pos");
        std::string fish="salut";
        const char* message = fish.c_str();
        send(main_socket,message,strlen(message),0);
        F.log("SERVER","Recieved!");
        return 0;
    };

    int recvPos(){
        F.log("SERVER","Waiting for other player pos");
        recv(main_socket,buffer,sizeof(buffer),0);
        F.log("SERVER","Recieved!");
        return 0;
    };

    std::vector<int> getOtherPlayerPos(){
        return other_player_pos;
    };

};