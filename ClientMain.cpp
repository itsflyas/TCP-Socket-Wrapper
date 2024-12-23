#include <iostream>
#include <unistd.h>
#include "ClientSocket.h"

// Main file to test ClientSocket.h
int main(){
  std::string ip_address = "127.0.0.1";
  int socket_port = 20192;

  ClientSocket *client_socket = new ClientSocket(ip_address, socket_port);

  if(client_socket->connectSocket() < 0){
    std::cout<<"Error during initialization, exiting program.";
    client_socket->disconnectSocket();
    return -1;
  }

  std::string buffer;

  std::cout<<"Connection successfull, starting comunication: \n";

  while(true){
    std::getline(std::cin, buffer);
    std::cout<<"Sent "<<client_socket->sendData(buffer + " \r\n")<<" bytes. \n";
    buffer = "";
  }

  client_socket->disconnectSocket();
  delete client_socket;
  return -1;
}
