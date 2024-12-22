#include <winsock2.h>
#include <ws2tcpip.h>

class ClientSocket{
private:
  struct sockaddr_in m_sockaddr_client;
  int m_socket_port;
  int m_socket_id;
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
  ClientSocket(const std::string t_ip_address, const int t_socket_port){

    if(WSAStartup(MAKEWORD(2,2), &wsa_data) != 0){
      std::cout<<"Error during library initialization. \n";
      this->disconnectSocket();
    }

    this->m_ip_address = t_ip_address;
    this->m_socket_port = t_socket_port;

    memset(&this->m_sockaddr_client, 0, sizeof(this->m_sockaddr_client));
    this->m_sockaddr_client.sin_family = AF_INET;
    this->m_sockaddr_client.sin_addr.s_addr = inet_addr(this->m_ip_address.c_str());
    this->m_sockaddr_client.sin_port = htons(this->m_socket_port);

    this->m_socket_id = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    if(this->m_socket_id == INVALID_SOCKET){
      this->disconnectSocket();
      }
    }

  /**
   * Connects the socket to the IPAddress and Port specified in the constructor.
   * 
   * @return 0, or -1 if an error occurs.
   */
  int connectSocket(){
    if(connect(this->m_socket_id, (struct sockaddr*)&this->m_sockaddr_client, sizeof(this->m_sockaddr_client)) == SOCKET_ERROR){
      closesocket(this->m_socket_id);
      WSACleanup();
      return -1;
    }

    return 0;

  }

  /**
   * Receives data sent by the server on the socket.
   * 
   * @return the data received, or an empty string if an error occurs.
   */
  std::string receiveData(){
    this->m_buffer_string = "";

    if((this->m_bytes = recv(this->m_socket_id, (char*)this->m_buffer, (int)sizeof(this->m_buffer), 0)) > 0){
      this->m_buffer_string.append(this->m_buffer, this->m_bytes);
      return m_buffer_string;
    }
    return "";
  }

  /**
   * Send data on the socket.
   * 
   * @param data the data to send.
   * 
   * @return the number of bytes sent, or -1 if an error occurs.
   */
  int sendData(std::string data){
    if((this->m_bytes = send(this->m_socket_id,(char*)data.c_str(), (int)data.size(), 0)) < 0){
      return -1;
    }
    return this->m_bytes;
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
