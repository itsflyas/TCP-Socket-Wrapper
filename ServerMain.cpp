#include <iostream>
#include <unistd.h>
#include "ServerSocket.h"

// Function executed in the thread
DWORD WINAPI handleThread(LPVOID t_server_socket) {
  ServerSocket* server_socket = (ServerSocket*)t_server_socket;
  std::string buffer;
  int communication_socket_id = server_socket->getLastConnectedClient();

  while(true){
    buffer = server_socket->receiveData(communication_socket_id);
    if(buffer == ""){
      break;
    }
    std::cout<<communication_socket_id<<":"<<buffer;
  }

  std::cout<<"Client "<<communication_socket_id<<" disconnected.\n";
  return 0;
}

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

  while(true){
    if(server_socket->acceptConnection() != SOCKET_ERROR){
    std::cout<<"New client accepted. \n";
    HANDLE thread = CreateThread(NULL, 0, handleThread, LPVOID(server_socket), 0, NULL);
    }
  }

  std::cout<<"Server was shutted down, exiting program. \n";
  server_socket->disconnectSocket();
  delete server_socket;
  return -1;
}
