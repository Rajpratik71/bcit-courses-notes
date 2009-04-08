#include "server.h"
#include "network.h"
#include "globals.h"
#include "utils.h"

#define MAX_LEN 1024
#define BUF_LEN 64

extern int mode;

int serialize_player(DPlaya *player, char *buf, size_t buflen);
int unserialize_player(char *buf, DPlaya *player);

void *start_udp_server(void *ptr) {
	int sd, port = 8000, n, player_loc;
	socklen_t client_len;
	char udpbuf[MAX_LEN];
	struct sockaddr_in udpserver, udpclient;
	DPlaya p1;
	struct tcp_server *fdcollection;

	fdcollection = (struct tcp_server *)ptr;


	FD_ZERO(&fdcollection->master);
	
	printf("starting udp server..\n");
	fflush(stdout);

	if((sd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("Socket Creation");
		exit(EXIT_FAILURE);
	}

	printf("%d line\n", __LINE__);

	bzero((char *)&udpserver, sizeof(udpserver));
	udpserver.sin_family = AF_INET;
	udpserver.sin_port = htons(port);
	udpserver.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(sd, (struct sockaddr *)&udpserver, sizeof(udpserver)) == -1) {
		perror("Binding Socket");
		exit(EXIT_FAILURE);
	}

printf("%d line\n", __LINE__);

	for(;;) {
		int j;
		client_len = sizeof(udpclient);
printf("%d line\n", __LINE__);
		if((n = recvfrom(sd, udpbuf, 2, 0, (struct sockaddr *)&udpclient, &client_len)) <= 0) {
printf("%d line\n", __LINE__);
			if(n == 0) {
				fprintf(stderr,"Connection closed on socket: %d\n", sd);
				continue;
			} else {
				perror("Recv From");
				exit(EXIT_FAILURE);
			}
		}

		printf("%s\n", udpbuf);
		
		if((player_loc = get_player_loc(udpclient.sin_addr.s_addr)) >= 0 && player_loc < MAX_PLAYERS) {
			receive_packet(udpbuf[0], player_loc);
		}

		p1.setX(1);
		p1.setY(2);
		p1.setNumBombs(3);
		p1.setName("david");

		/*for (j = 0; j <= fdcollection->fd_max; j++) {
			if (FD_ISSET(j, &fdcollection->master)) {
				if (j != fdcollection->listener && j == i) {*/
					send_udp_player(&p1, j, udpclient);
				/*}
			}
		}*/
		

		pthread_testcancel();
	}
	return 0;
}


