#include <iostream>
#include <unistd.h>
#include "ServerSocket.h"

// Main file to test ServerSocket.h
int main(){
  std::string ip_address = "0.0.0.0";
  int socket_port = 20192;

  ServerSocket *server_socket = new ServerSocket(ip_address, socket_port);

  if(server_socket->connectSocket() < 0){
    std::cout<<"Error during initialization, exiting program. \n";
    server_socket->disconnectSocket();
    return -1;
  }

  std::cout<<"Server is listening... \n";

  if(server_socket->acceptConnection() != SOCKET_ERROR){
    std::cout<<"Client accepted. \n";
  }
  
  std::string buffer;

  while(true){
    buffer = server_socket->receiveData();
    if(buffer == ""){
      break;
    }
    std::cout<<buffer;
  }

  std::cout<<"All clients lost connection, exiting program. \n";
  server_socket->disconnectSocket();
  return -1;
}
