#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <sstream>

using namespace std;

#define MAX_BUFFER  1024


int main()
{

    int host_port = 87404;
    struct sockaddr_in saddr, caddr;
    struct sctp_initmsg initmsg;
    char buff[INET_ADDRSTRLEN];
    char buffer[MAX_BUFFER+1] = "Message ##\n";

    int sfd = socket( AF_INET, SOCK_STREAM, IPPROTO_SCTP );

if(sfd == -1){
cout << "socket open" << sfd << " error:" << errno << "  "<< strerror(errno) << endl;
        exit (0);
}


    bzero( (void *)&saddr, sizeof(saddr) );
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl( INADDR_ANY );
    saddr.sin_port = htons(host_port);

if(bind( sfd, (struct sockaddr *)&saddr, sizeof(saddr)) == -1){
cout << "bind failed" << sfd << " error:" << errno << "  "<< strerror(errno) << endl;
        exit (0);
}

    memset( &initmsg, 0, sizeof(initmsg) );
    initmsg.sinit_num_ostreams = 3;
    initmsg.sinit_max_instreams = 3;
    initmsg.sinit_max_attempts = 2;
    setsockopt(sfd, IPPROTO_SCTP, SCTP_INITMSG,&initmsg, sizeof(initmsg));

for(;;)
{
    if(listen( sfd, 3 ) == -1)
        cout << "listen " << sfd << " error:" << errno << "  "<< strerror(errno) << endl;


    cout << "listen" << endl;

    for(;;) {
        printf("Server Running\n");

	socklen_t len = sizeof(caddr);
        int cfd = accept(sfd, (struct sockaddr *)&caddr, &len);

	if(cfd == -1)
	cout << "accept " << cfd << " error:" << errno << "  "<< strerror(errno) << endl;


      	cout << "Connected to " << inet_ntop(AF_INET, &caddr.sin_addr, buff,sizeof(buff)) << endl;

        for(int i=0; i< 3; i++) {
/* Changing 9th character the character after # in the message buffer */
	cout << "next" << endl;

	stringstream ss("");

	ss << " send message " << i ;
	strcpy(buffer, ss.str().c_str());

	cout << ss.str() << endl;

	sctp_sendmsg( cfd, ss.str().c_str(),ss.str().length(), NULL, 0, 0, 0, i , 0, 0);
	//cout << "send failed " << cfd << " error:" << errno << "  "<< strerror(errno) << endl;

	}

	cout << "close connection" << endl;

	close( cfd );
//== -1)
//	cout << "client close " << cfd << " error:" << errno << "  "<< strerror(errno) << endl;
    }
}

cout << "end infinity" << endl;

//if(close(sfd)==-1){
//	cout << "close failed" << sfd << " error:" << errno << "  "<< strerror(errno) << endl;
//}

    return 0;
}
