/*
���ϸ� : time_server.c
��  �� : time ���񽺸� �����ϴ� ����
������ : cc - o time_server time_server.c
���� : time_server[30000]
*/
#include <winsock.h>
#include <signal.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

WSADATA wsadata;
int	main_socket;

void exit_callback(int sig)
{
    closesocket(main_socket);
    WSACleanup();
    exit(0);
}

void init_winsock()
{
    WORD sversion;
    u_long iMode = 1;

    // winsock ����� ���� �ʼ�����
    signal(SIGINT, exit_callback);
    sversion = MAKEWORD(1, 1);
    WSAStartup(sversion, &wsadata);
}

#define BUF_LEN 128
#define TIME_SERVER "0.0.0.0"
#define TIME_PORT "30000"

void main(int argc, char* argv[]) {
    struct sockaddr_in server, client;
    int sock, sock2;                        /* ���Ϲ�ȣ */
    int len;
    char buf[256];
    unsigned int set = 1;
    time_t today;
    char* port_no = TIME_PORT, *ip_addr = TIME_SERVER;
    init_winsock();
    if (argc == 2)
        port_no = argv[1];
    /* ���� ���� */
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Server: Can't open stream socket.");
        exit(1);
    }
    main_socket = sock;
    //printf("sock = %d\n", sock);
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&set, sizeof(set));
    /* server ���� */
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip_addr);   //htonl(INADDR_ANY);
    server.sin_port = htons(atoi(port_no));

    /* bind() ȣ�� */
    if (bind(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        printf("Server: Can't bind local address.\n");
        exit(1);
    }
    /* ������ ���� ������ ���� */
    listen(sock, 5);

    /* iterative  time ���� ���� */
    printf("Server : waiting connection request.\n");
    len = sizeof(client);

    while (1) {
        /* �����û�� ��ٸ� */
        sock2 = accept(sock, (struct sockaddr*)&client, &len);
        //printf("sock2 = %d\n", sock2);
        if (sock2 < 0) {
            printf("Server: accept failed.\n");
            exit(1);
        }

        printf("Server : A client connected.\n");
        printf("From %s : %d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        time(&today);
        strcpy(buf, ctime(&today));
        printf("Server time=%s", buf);
        send(sock2, buf, strlen(buf) + 1, 0);
        closesocket(sock2);
    }
}