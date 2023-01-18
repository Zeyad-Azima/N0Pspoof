#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>

void TCP_scan(char* host, int start_port, int end_port, struct timeval timeout) {
    for (int port = start_port; port <= end_port; port++) {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof timeout);
        struct sockaddr_in server;
        server.sin_addr.s_addr = inet_addr(host);
        server.sin_family = AF_INET;
        server.sin_port = htons(port);

        if (connect(sock, (struct sockaddr *) &server, sizeof(server)) == 0) {
            char buffer[32];
            int bytes_sent = send(sock, buffer, sizeof(buffer), 0);
            if (bytes_sent > 0) {
                int bytes_received = recv(sock, buffer, sizeof(buffer), 0);
                if (bytes_received > 0) {
                    printf("[+] Port %d is open\n", port);
                } else if (bytes_received == -1 && errno == EAGAIN) {
                    printf("[+] Port %d is open\n", port);
                }
            }
            close(sock);
        } else {
            //printf("[+] Port %d is closed\n", port);
        }
        close(sock);
    }
}

void UDP_scan(char* host, int start_port, int end_port, struct timeval timeout) {
    for (int port = start_port; port <= end_port; port++) {
        int sock = socket(AF_INET, SOCK_DGRAM, 0);
        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof timeout);
        struct sockaddr_in server;
        server.sin_addr.s_addr = inet_addr(host);
        server.sin_family = AF_INET;
        server.sin_port = htons(port);
        char buffer[32];
        int bytes_sent = sendto(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&server, sizeof(server));
        if (bytes_sent > 0) {
            int bytes_received = recv(sock, buffer, sizeof(buffer), 0);
            if (bytes_received > 0) {
                printf("[+] Port %d is open\n", port);
            } else if (bytes_received == -1 && errno == EAGAIN) {
                printf("[+] Port %d is open\n", port);
            }
        } else {
            //printf("[+] Port %d is closed\n", port);
        }
        close(sock);
    }
}

int main() {
	char host[256];
	int start_port, end_port;
	struct timeval timeout;
	timeout.tv_sec = 5;
   
	timeout.tv_usec = 0;
	int scan_type;
	printf("[+] N0Pspoof is a tool to bypass and evade Portspoof solution \n");
	printf("[+] By: Zeyad Azima\n");
	printf("[+] Github: https://github.com/Zeyad-Azima/N0Pspoof\n");
	
	printf("[+] Target Behind Portspoof: ");
	scanf("%255s", host);

	printf("[+] Enter the start port: ");
	scanf("%d", &start_port);
	printf("[+] Enter the end port: ");
	scanf("%d", &end_port);

	printf("[+] Choose the scan type: \n 1) TCP \n 2) UDP \n");
	scanf("%d", &scan_type);
	printf("\n");
	
	

	if (scan_type == 1) {
		printf("[+] Scan Info:\n[*] Target: %s	[*] Ports Range: %d-%d	[*] Scan Type: TCP\n", host, start_port, end_port);
	    TCP_scan(host, start_port, end_port, timeout);
	} else if (scan_type == 2) {
		printf("[+] Scan Info:\n[*] Target: %s	[*] Ports Range: %d-%d	[*] Scan Type: UDP\n", host, start_port, end_port);
	    UDP_scan(host, start_port, end_port, timeout);
	} else {
	    printf("Invalid scan type\n");
	}
	return 0;

}
