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
#include <unistd.h>
#include <errno.h>

using namespace std;

#define MAX_BUFFER  1024


int main()
{
 int listenSock, connSock, ret;
  struct sockaddr_in servaddr;
  char buffer[MAX_BUFFER+1];
  time_t currentTime;

  /* Создание сокета SCTP в стиле TCP */
  listenSock = socket( AF_INET, SOCK_STREAM, IPPROTO_SCTP );

  cout << "server socket " << listenSock << endl;
  /* Принимается соединение с любого интерфейса */
  bzero( (void *)&servaddr, sizeof(servaddr) );
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl( INADDR_ANY );
  servaddr.sin_port = htons(4048);

  /* Адрес привязки – любой, порт - MY_PORT_NUM */
  ret = bind( listenSock,
               (struct sockaddr *)&servaddr,
                (socklen_t)sizeof(servaddr) );

 cout << "server socket bind" << ret << endl;
if(ret == -1)
  cout << "error " << errno << " " << strerror(errno) << endl;

  /* Сокет сервера переводится в состояние ожидания соединения */
  int listen_result = listen( listenSock, 5 );
  
 cout << "server socket listen" << listen_result << endl;
if(listen_result == -1)
  cout << "error " << errno << " " << strerror(errno) << endl;

  /* Цикл работы сервера... */
  while( 1 ) {

    /* Ожидание соединения клиента */
    connSock = accept( listenSock,
                        (struct sockaddr *)NULL,
                        (socklen_t *)NULL );
                        
 cout << "server socket accept " << connSock << endl;
 
if(connSock == -1)
  cout << "error " << errno << " " << strerror(errno) << endl;

    /* Соединение с новым клиентом */

    /* Выясняется текущее время */
    currentTime = time(NULL);

    /* Посылается текущее время по потоку 0 (поток для локального времени) */
    snprintf( buffer, MAX_BUFFER, "%s\n", ctime(&currentTime) );

    ret = sctp_sendmsg( connSock,
                          (void *)buffer, (size_t)strlen(buffer),
                          NULL, 0, 0, 0, 1, 0, 0 );
                          
 cout << "server socket 1 sctp_sendmsg " << ret << endl;
 
if(ret == -1)
  cout << "error " << errno << " " << strerror(errno) << endl;


    /* Посылается GMT по потоку 1 (поток для GMT) */
    snprintf( buffer, MAX_BUFFER, "%s\n",
               asctime( gmtime( &currentTime ) ) );

    ret = sctp_sendmsg( connSock,
                          (void *)buffer, (size_t)strlen(buffer),
                          NULL, 0, 0, 0, 2, 0, 0 );

 cout << "server socket 2 sctp_sendmsg " << ret << endl;
 
if(ret == -1)
  cout << "error " << errno << " " << strerror(errno) << endl;


    /* Закрывается клиентское соединение */
    close( connSock );

  }

  return 0;
}
