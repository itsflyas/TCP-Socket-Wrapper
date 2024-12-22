#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>

class ServerSocket{
private:
  struct sockaddr_in m_sockaddr_server;
  struct sockaddr_in m_sockaddr_client;
  int m_socket_port;
  int m_socket_id;
  int m_communication_socket_id;
  int m_client_addr_size = sizeof(this->m_sockaddr_client);
  int m_server_addr_size;
  int m_bytes;
  std::string m_ip_address;
  std::string m_buffer_string;
  char m_buffer[1024];
  WSADATA wsa_data;


public:
  /**
   * Constructor, it initializes libraries and the socket.
   * 
   * @param ip_address the ip address to connect to.
   * @param socket_port the port to connect to.
   */
  ServerSocket(const std::string t_ip_address, const int t_socket_port){

    if(WSAStartup(MAKEWORD(2,2), &wsa_data) != 0){
      std::cout<<"Error during library initialization. \n";
      this->disconnectSocket();
    }

    this->m_ip_address = t_ip_address;
    this->m_socket_port = t_socket_port;

    memset(&this->m_sockaddr_server, 0, sizeof(this->m_sockaddr_server));
    this->m_sockaddr_server.sin_family = AF_INET;
    this->m_sockaddr_server.sin_addr.s_addr = inet_addr(this->m_ip_address.c_str());
    this->m_sockaddr_server.sin_port = htons(this->m_socket_port);

    this->m_socket_id = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(this->m_socket_id == INVALID_SOCKET){
      this->disconnectSocket();
    }
    }

  /**
   * Perform a bind on the socket.
   * 
   * @return 0, -1 if an error occurs.
   */
  int connectSocket(){
    if(bind(this->m_socket_id, (struct sockaddr*)&this->m_sockaddr_server, sizeof(this->m_sockaddr_server)) == SOCKET_ERROR){
      closesocket(this->m_socket_id);
      WSACleanup();
      return -1;
    }

    if(listen(this->m_socket_id, SOMAXCONN) == SOCKET_ERROR){
      closesocket(this->m_socket_id);
      WSACleanup();
      return -1;
    }

    return 0;
  }

  int acceptConnection(){
    this->m_communication_socket_id = accept(this->m_socket_id, (struct sockaddr*)&this->m_sockaddr_client, &this->m_client_addr_size);
    return this->m_communication_socket_id;
  }

  /**
   * Receive incoming data on the socket.
   * 
   * @return the received data, or an empty string if an error occurs
   */
  std::string receiveData(){
      this->m_buffer_string = "";

      if((this->m_bytes = recv(this->m_communication_socket_id, (char*)this->m_buffer, (size_t)sizeof(this->m_buffer), 0)) > 0){
        this->m_buffer_string.append(this->m_buffer, this->m_bytes);
        return this->m_buffer_string;
      } else {
        this->disconnectSocket();
        return "";
      }
    }

  /**
   * Send data on the socket.
   * 
   * @param data the data to send
   * 
   * @return the number of bytes sent, or -1 if an error occurs.
   */
  int sendData(std::string data){
    int n;

    if((n = send(this->m_socket_id,(char*)data.c_str(), (size_t)data.size(), 0)) < 0){
      return -1;
    }
    return n;


  }

  /**
   * Disconnects the socket.
   * 
   * @return 0
   */
  int disconnectSocket(){
    closesocket(this->m_socket_id);
    WSACleanup();
    return 0;
  }
};
