#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>


using namespace std;

#define MAX_BUFFER  1024

int main()
{

int host_port = 87404;

struct sockaddr_in saddr;
struct sctp_initmsg initmsg;
struct sctp_event_subscribe events;
struct sctp_sndrcvinfo sndrcvinfo;
int flags;


char buffer[MAX_BUFFER+1];


//create socket
int cfd = socket(AF_INET,SOCK_STREAM,IPPROTO_SCTP);

if (cfd  == -1){
	cout << "socket open" << cfd << " error:" << errno << "  "<< strerror(errno) << endl;
	exit (0);
}

memset(&initmsg, 0, sizeof(initmsg));

initmsg.sinit_num_ostreams = 3;
initmsg.sinit_max_instreams = 3;
initmsg.sinit_max_attempts = 2;
setsockopt( cfd, IPPROTO_SCTP, SCTP_INITMSG,&initmsg, sizeof(initmsg));
bzero( (void *)&saddr, sizeof(saddr) );

saddr.sin_family = AF_INET;
inet_pton(AF_INET,"127.0.0.1" , &saddr.sin_addr);
saddr.sin_port = htons(host_port);

if (connect(cfd, (struct sockaddr *)&saddr, sizeof(saddr)) == -1){
	cout << "connect failed  error:" << errno << "  "<<  strerror(errno) <<  endl;
	exit(0);
}

memset((void *)&events, 0, sizeof(events));

events.sctp_data_io_event = 1;
setsockopt(cfd, SOL_SCTP, SCTP_EVENTS,(const void *)&events, sizeof(events));

for (int i=0; i<3; i++) {
	bzero( (void *)&buffer, sizeof(buffer) );
	sctp_recvmsg( cfd, (void *)buffer, sizeof(buffer),(struct sockaddr *)NULL, 0,&sndrcvinfo, &flags);
	printf("Received following data on stream %d\n\n%s\n",sndrcvinfo.sinfo_stream, buffer);
}

if(close(cfd)==-1){
cout << "close failed" << cfd << " error:" << errno << "  "<< strerror(errno) << endl;
}

return 0;
}
