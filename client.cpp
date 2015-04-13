//#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
//#include <sys/types.h>
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
 int connSock, in, i, flags;
  struct sockaddr_in servaddr;
  struct sctp_sndrcvinfo sndrcvinfo;
  struct sctp_event_subscribe events;
  char buffer[MAX_BUFFER+1];

  /* Создание сокета SCTP в стиле TCP */
  connSock = socket( AF_INET, SOCK_STREAM, IPPROTO_SCTP );

  cout << "socket create " << connSock << endl;
  
  /* Определяется адрес точки соединения */
  bzero( (void *)&servaddr, sizeof(servaddr) );
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(4048);
  servaddr.sin_addr.s_addr = inet_addr( "127.0.0.1" );

  /* Соединение с сервером */
  int connect_result = connect( connSock, (struct sockaddr *)&servaddr, sizeof(servaddr) );
  
  cout << "socket connect " << connect_result << endl;
  
if(connect_result == -1)
  cout << "error " << errno << " " << strerror(errno) << endl;
   
  /* Ожидается получение данных SCTP Snd/Rcv с помощью функции sctp_recvmsg */
  memset( (void *)&events, 0, sizeof(events) );
  events.sctp_data_io_event = 1;
  
 int sock_opt_result = setsockopt( connSock, SOL_SCTP, SCTP_EVENTS,(const void *)&events, sizeof(events));
  
  cout << "socket option" <<  sock_opt_result << endl;
  
if(sock_opt_result == -1)
  cout << "error " << errno << " " << strerror(errno) << endl;


  /* Ожидается получение двух сообщений */
  for (i = 0 ; i < 2 ; i++) {

    in = sctp_recvmsg( connSock, (void *)buffer, sizeof(buffer),
                        (struct sockaddr *)NULL, 0,
                        &sndrcvinfo, &flags );

  cout << "sctp_recvmsg " <<  i << " result " << in << endl;

if(in == -1)
  cout << "error " << errno << " " << strerror(errno) << endl;

    /* Завершающий символ строки – 0 */
    buffer[in] = 0;

    if        (sndrcvinfo.sinfo_stream == 1) {
      printf("(Local) %s\n", buffer);
    } else if (sndrcvinfo.sinfo_stream == 2) {
      printf("(GMT  ) %s\n", buffer);
    }

  }

  /* Закрытие сокета и выход */
  close(connSock);

  return 0;
}
