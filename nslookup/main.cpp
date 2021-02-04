#include "Client.h"

int main(int argc, char *argv[]){
#if defined(_WIN32)
	if (argc < 3) {
		printf("[+] Usage: File.exe [IPv4/Domain/IPv6] [Port]");
		return 1;
	}
	WSADATA d;
	if (WSAStartup(MAKEWORD(2, 2), &d)) {
		printf("Failed to initialize.\n");
		return 1;
	}
#endif

	printf("Configuring remote address...\n");
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM;
	struct addrinfo* peer_address;
	if (getaddrinfo(argv[1], argv[2] ? argv[2] : "80", &hints, &peer_address)) {
		printf("getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
		return 1;
	}
	printf("Remote address is: ");
	char address_buffer[100];
	char service_buffer[100];
	getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen, address_buffer, sizeof(address_buffer), service_buffer, sizeof(service_buffer), NI_NUMERICHOST);
	printf("Address : %s\nProtocol/Service: %s\n", address_buffer, service_buffer);
}
