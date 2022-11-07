// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include<string>
#include<iostream>
#include <arpa/inet.h>
#include<bits/stdc++.h>
#include<iostream>
#include <fstream>
#include<string>
#include <bits/stdc++.h>
#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <dirent.h>
#include <filesystem>
#include <string>
#include <unistd.h>
#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <termios.h>
#include<fcntl.h>
#include<stdlib.h>
//#define PORT 8084
#define MAX 1024
#include <pthread.h>
using namespace std;
vector<string> ips;
//void* maintain_connection(void *cli_socketp);
void maintain_connection(int ser_socket);
void* server_fun(void *para);
void* filetrans(void* cli_socketp);
void * dfun(void *para);
string result="";
int main(int argc, char const *argv[])
{	       
	pthread_t t1;
	//int *clisoc=malloc(sizeof(int));
	//*clisoc=new_socket;
	ips.push_back(argv[1]);
	ips.push_back(argv[2]);
	ips.push_back(argv[3]);
	ips.push_back(argv[4]);
	pthread_create(&t1,NULL,server_fun,NULL);
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	//char *hello = "Hello from client";
	char buffer[1024+1] = {0};
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}
        int i;
        sscanf(argv[2], "%d", &i);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(i);
	//string s=string(argv[1]);
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET,argv[1], &serv_addr.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}
	//printf("hai");
	char serveradd[1024+1];
	inet_ntop(AF_INET,&serv_addr,serveradd,1024);
	printf("server add:%s\n",serveradd);
	/*while(1==1){
	bzero(buffer, sizeof(buffer));
	printf("enter msg:");
	int n = 0;
	while ((buffer[n++] = getchar()) != '\n');
	send(sock , buffer , strlen(buffer) , 0 );
	if ((strncmp(buffer, "exit", 4)) == 0) {
			printf("Client Exit...\n");
			break;
	}
	bzero(buffer, sizeof(buffer));
	//printf(" message sent\n");
	valread = read( sock , buffer, 1024);
	printf("msg from server::%s\n",buffer );
	}*/
	maintain_connection(sock);
	//func(sock);
	close(sock);
	return 0;
}
////////////////////////////////////
void maintain_connection(int ser_socket)
{
//int cli_socket=*(int *)cli_socketp;

char buffer[1024+1] = {0};
//free(cli_socketp);
int valread;
while(1==1){
	printf("enter msg:");
	bzero(buffer, 1024);
	int n = 0;
	while ((buffer[n++] = getchar()) != '\n');
	//strcpy(buffer,s1.c_str());
	send(ser_socket , buffer , 1024, 0 );
	if ((strncmp(buffer, "logout", 6)) == 0) {
			printf("Client Exit...\n");
			break;
	}
	string s=string(buffer);
	vector<string> request;
	
	stringstream ccommand(s);
	request.clear();
	string k="";
        for(int i=0;i<s.size()-1;i++)
        {
        if(s[i]!=' ')k=k+s[i];
        else {request.push_back(k);k="";}
        }request.push_back(k);
        bzero(buffer, sizeof(buffer));
        if(request[0]=="upload_file")
        {
        string data=ips[2]+" "+ips[3];
        bzero(buffer, 1024);
        strcpy(buffer,data.c_str());
        //cout<<data;
        
        send(ser_socket , buffer , 1024, 0 );
        }
        else if(request[0]=="download_file")       //download
        {
        cout<<"download file";
        bzero(buffer, 1024);
	valread = read( ser_socket , buffer, 1024);
	string res=buffer;
	result=res;
	//cout<<"not thread"<<res<<endl;
	/*vector<string> request1;
	string k="";
        for(int i=0;i<res.size();i++)
        {
        if(res[i]!=' ')k=k+res[i];
        else {request1.push_back(k);k="";}
        }request1.push_back(k);*/
        char buf[res.length()+1];
        strcpy(buf,res.c_str());
        //cout<<"before thread"<<endl;
       pthread_t tr;
       
       pthread_create(&tr,NULL,dfun,(void *)buffer);
        }
        bzero(buffer, 1024);
	valread = read( ser_socket , buffer, 1024);
	printf("msg from server::%s\n",buffer );
        
	}
	close(ser_socket);
	printf("closed connection\n");
	//return NULL;
}
/////////////////////
void* server_fun(void *para)
{
int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};
	//char *hello = "Hello from server";
	
	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	
	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
												&opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	int i;
	char a1[1024]={0};
	//cout<<ips[3];
	strcpy(a1,ips[3].c_str());
        sscanf(a1, "%d", &i);
	address.sin_family = AF_INET;
	//address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( i );
	char a2[1024]={0};
	strcpy(a2,ips[2].c_str());
	//cout<<ips[2];
	if(inet_pton(AF_INET, a2, &address.sin_addr)<=0)
	{
		printf("\nserver Invalid address/ Address not supported \n");
		return NULL;
	}
	char serveradd[1024+1];
	
	inet_ntop(AF_INET,&address,serveradd,1024);
	printf("server add:%s\n",serveradd);
	
	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address,sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	while(1==1){
	printf("waiting for connection\n");
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}
	char clientadd[1024+1];
	inet_ntop(AF_INET,&address,clientadd,1024);
	printf("client add:%s\n",clientadd);
	//maintain_connection(new_socket);
	pthread_t t;
	int *clisoc=new int();
	*clisoc=new_socket;
	pthread_create(&t,NULL,filetrans,clisoc);
	}
	return NULL;
}
void* filetrans(void* cli_socketp)
{
int cli_socket=*(int *)cli_socketp;
int valread ;
char buffer[1024];
bzero(buffer, 1024);
valread = read( cli_socket , buffer, 1024);
//printf("msg from server::%s\n",buffer );
string s=buffer;
char fname[1024];
strcpy(fname,s.c_str());
int finput,nread;
finput = open(fname,O_RDONLY);
while((nread = read(finput,buffer,sizeof(buffer)))>0)
{
send(cli_socket,buffer,nread,0);
bzero(buffer, 1024);
}
	/*int n = 0;
	while ((buffer[n++] = getchar()) != '\n');
	string s1="hello";
	//getline(cin,s1);
	strcpy(buffer,s1.c_str());
	send(cli_socket , buffer , 1024, 0 );*/

return NULL;
}



/////**************download function
void * dfun(void *para)
{
//cout<<"inside thread";
char *buff;
buff=(char *)para;
string res(buff);
res=result;
vector<string> request1;
	string k="";
	//cout<<""<<res<<endl;
        for(int i=0;i<res.size();i++)
        {
        if(res[i]!=' ')k=k+res[i];
        else {request1.push_back(k);k="";}
        }request1.push_back(k);
        if(request1.size()<4)return NULL;
//0 ip,1 port ,2 file ,3 dest 
int sock = 0, valread;
	struct sockaddr_in serv_addr;
	//char *hello = "Hello from client";
	char buffer[1025] = {0};
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return NULL;
	}
int i;
char a1[1024]={0};
	
	strcpy(a1,request1[1].c_str());
        sscanf(a1, "%d", &i);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(i);
char a2[1024]={0};
	strcpy(a2,request1[0].c_str());
	
	if(inet_pton(AF_INET, a2, &serv_addr.sin_addr)<=0)
	{
		printf("\nserver Invalid address/ Address not supported \n");
		return NULL;
	}
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return NULL;
	}
	char bu[1025];
	int foutput;
	strcpy(bu,request1[2].c_str());
	send(sock,bu,1024,0);
	bzero(bu,1024);
	strcpy(bu,request1[3].c_str());
foutput = open(bu,O_CREAT|O_RDWR | O_APPEND,S_IRUSR|S_IWUSR);	
//char buffer[1024];
bzero(buffer,1024);
while((valread = read( sock , buffer, sizeof(buffer)))>0)
{
write(foutput,buffer,valread);
}
cout<<"completed downoad";
return NULL;
}
