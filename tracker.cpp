// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include<iostream>
#include<string>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>
#include<bits/stdc++.h>
using namespace std;

//#define PORT 8084
#define MAX 1024

unordered_map<string,string> users;
class groupsc{
public:
string gid;
string owner;
vector<string> gmebs;
vector<string> pendingreq;
vector<string> sharedfiles;
groupsc()
{
}
};
struct files
{
public:
string ip;
string port;
string filename;
};
vector<files> filedata;
unordered_map<string,groupsc> grp;
//grp["g1"]=groupsc();
//grp["g1"].gmebs.push_back("p1");
////////////////////////////////////////////
void* maintain_connection(void *cli_socketp);
int create_user(string uname,string password);
int login_user(string uname,string password);
int upload_file(string fname,string grpname,int cli_socket);
//string process_request(string s);
//string process_starting(vector<string> request);
//string process_remaining(vector<string> request,string usrname);
string process_starting(vector<string> request,int cli_socket,string *usrname);
int create_group(string username,string grpname);
string list_groups();
int join_group(string username,string grpname);
int leave_group(string username,string grpname);
string list_requests(string grpname);
int download_file(string fname,string grpname,int cli_socket,string user,string dest);
int accept_request(string grpname,string user,string own);
string list_files(string grpname);
int stop_share(string grpname,string fname);
int main(int argc, char const *argv[])
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024+1] = {0};
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
        sscanf(argv[2], "%d", &i);
	address.sin_family = AF_INET;
	//address.sin_addr.s_addr = INADDR_ANY;
	string s=string(argv[1]);
	if(inet_pton(AF_INET, argv[1], &address.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}
	address.sin_port = htons( i );
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
	
	pthread_create(&t,NULL,maintain_connection,clisoc);
	sleep(3);
	}
	
	return 0;
}

void* maintain_connection(void* cli_socketp)
{
int cli_socket=*(int *)cli_socketp;
//cout<<cli_socket;
char buffer[1025] = {0};
free(cli_socketp);
int valread;
int flag1=0;
string uname1="";
while(1==1){
	bzero(buffer, 1024);string s2="";
	valread = read( cli_socket , buffer, 1024);
	printf("msg from client:%s",buffer );
	if ((strncmp(buffer, "exit", 4)) == 0) {
			printf("Client Exit...\n");
			break;
	}
	/*else if((strncmp(buffer, "list_groups", 11)) == 0&&flag1==1){
	s2=list_groups();bzero(buffer, 1024);strcpy(buffer,s2.c_str());
	send(cli_socket , buffer , strlen(buffer) , 0 );continue;
	}*/
	//printf("enter msg:");
	string s=string(buffer);
	vector<string> request;
	
	stringstream ccommand(s);
	string s1;
	request.clear();
	//while(getline(ccommand, s, ' ')) 
	//{request.push_back(s);}
	//cout<<request[request.size()-1];
	//char *token = strtok(buffer, " ");
   
    // Keep printing tokens while one of the
    // delimiters present in str[].
    string k="";
        for(int i=0;i<s.size()-1;i++)
        {
        if(s[i]!=' ')k=k+s[i];
        else {request.push_back(k);k="";}
        }request.push_back(k);
        //cout<<"command"<<request[0]<<request[1];
        //cout<<request[request.size()-1];
	
	if(flag1==0)
	{ s2=process_starting(request,cli_socket,&uname1);if(s2=="Login,sucessful"){flag1=1;uname1=request[1];}}
	/*else if(flag1==1)
	{
	cout<<"hai"<<uname1;
	s2=process_remaining(request,uname1,cli_socket);if(s2=="Logout,sucessful")flag1=0;
	}*/
	bzero(buffer, 1024);
	int n = 0;
	//while ((buffer[n++] = getchar()) != '\n');
	strcpy(buffer,s2.c_str());
	//send(cli_socket , buffer , 1024 , 0 );
	}
	close(cli_socket);
	printf("closed connection\n");
	return NULL;
}

/*string process_request(string s)
{
//cout<<"\n"<<s;
stringstream ccommand(s);
string s1;
request.clear();
while(getline(ccommand, s, ' ')) 
{request.push_back(s);}
//cout<<request[0];
/*for(int i=0;i<request.size();i++)
{cout<<request[i]<<endl;}
return s;
}*/
///////
string process_starting(vector<string> request,int cli_socket,string *usrname){
string s;
char buffer[1025] = {0};
if(request[0]=="create_user")
{
//cout<<" create user"<<endl;
if(request.size()==3){
int x=create_user(request[1],request[2]);
if(x==0)s="User already exist";
else if(x==1)s="sucessful";
}
else return s="arguments not sufficient";
}
else if(request[0]=="login")
{
//cout<<"nenlogin"<<endl;
if(request.size()==3){
int x=login_user(request[1],request[2]);
if(x==0)s="user doesnot exist";
else if(x==1){s="Login,sucessful";*usrname=request[1];}
}
else s= "arguments not sufficient";
}
else if(request[0]=="create_group")
{
//cout<<" create group";
if(request.size()==2){
int x=create_group(*usrname,request[1]);
if(x==1)s= "sucessful";
else if(x==0)s= "group already exist";
}
else s= "arguments not sufficient";
}
else if(request[0]=="join_group")
{
cout<<"join group";
if(request.size()==2){
int x=join_group(*usrname,request[1]);
if(x==1)s="sucessful";
else if(x==0)s= "already in group";}
else s="insufficient arguments";

}
else if(request[0]=="leave_group")
{
cout<<" leave group";
if(request.size()==2){
int x=leave_group(*usrname,request[1]);
if(x==1)s="sucessful";
else if(x==0)s= "Not present in group";}
else s="insufficient arguments";
}
else if(request[0]=="list_groups")
{
cout<<"list groups";
if(request.size()==1){
s=list_groups();
}
else s= "arguments not sufficient";
}
else if(request[0]=="requests")
{
if(request.size()==3){
if(request[1]=="list_requests"){
string req1=list_requests(request[2]);
s=req1;}
else s="wrong input";}
else s="insufficient arguments";
}
else if(request[0]=="accept_request")
{
if(request.size()==3){
int x=accept_request(request[1],request[2],*usrname);
if(x==0)s="not sucessful";
else s= "sucessful";
}
else s="insufficient arguments";
}
else if(request[0]=="upload_file")
{
cout<<"upload file"<<endl;
if(request.size()==3){
int x=upload_file(request[1],request[2],cli_socket);
if(x==1)s="sucessful";
else s="not uploaded";}
else s="insufficient arguments";

}
else if(request[0]=="download_file")
{
//cout<<"download file"<<endl;
//cout<<request[0]<<" "<<request[1]<<" "<<request[2]<<" "<<*usrname<<" "<<request[3]<<endl;
if(request.size()==4){
int x=download_file(request[2],request[1],cli_socket,*usrname,request[3]);
if(x==1)s="started downloading";
else s="Not possible";
}
else s="insufficient arguments";
}
else if(request[0]=="stop_share")
{
cout<<"stop_share"<<endl;
if(request.size()==3){
int x=stop_share(request[1],request[2]);
if(x==1)s="sucessful";
else s="not uploaded";}
else s="insufficient arguments";
}
else if(request[0]=="list_files")
{
cout<<"list files"<<endl;
if(request.size()==2){
s=list_files(request[1]);
}
else s="insufficient arguments";
}
else s="wrong input";
bzero(buffer, 1024);
strcpy(buffer,s.c_str());
send(cli_socket , buffer , 1024 , 0 );
return "wrong command";
}



///**********************************************************************create user

int create_user(string uname,string password)
{
if(users.find(uname)!= users.end())return 0;
else {
users[uname]=password;
return 1;
}
return 0;
}

/////create login
int login_user(string uname,string password)
{
if(users.find(uname)!= users.end())
{
if(users[uname]==password)return 1;
}
return 0;
}
////////create group
int create_group(string username,string grpname)
{
if(grp.find(grpname)!= grp.end())return 0;
else{
grp[grpname]=groupsc();
grp[grpname].gid=grpname;
grp[grpname].owner=username;
grp[grpname].gmebs.push_back(username);
return 1;
}
return 1;
}

////////list groups
string list_groups()
{
//map<string,groupsc>::iterator itr;
//cout<<"list groups";
string s1="";
if(grp.empty())s1= "empty";
else{
for (auto itr = grp.begin(); itr != grp.end(); itr++)
{
s1=s1+" "+itr->first;
}
}
return s1;
}
///////////////////join_group
int join_group(string username,string grpname)
{
cout<<"hello"<<username;
if(grp[grpname].owner==username)return 0;
else {for (auto& it : grp[grpname].gmebs){if(it==username)return 0;}}
grp[grpname].pendingreq.push_back(username);
return 1;
}
/////////////////leave_group
int leave_group(string user,string grpname)
{
int x=0;
 if (grp.find(grpname) == grp.end())return 0;
if(!grp[grpname].gmebs.empty()){
vector<string> temp=grp[grpname].gmebs;
int ind=-1,flag=0;
for(int i=0;i<temp.size();i++)
{
if(temp[i]==user){
ind=i;
flag=1;
break;
}
}
if(flag==0)return 0;
else 
{
grp[grpname].gmebs.erase(ind+grp[grpname].gmebs.begin());
//grp[grpname].gmebs.push_back(user);
return 1;
}}
return 0;
}

///////////////////requests list_requests <group_id>
string list_requests(string grpname)
{
if(grp[grpname].pendingreq.empty())return "empty";
else{
string req="";
for (auto it = grp[grpname].pendingreq.begin();it != grp[grpname].pendingreq.end(); ++it)
{req=req+" "+*it;}
return req;
}
}

//////////////////////////////////////////accept_request <group_id> <user_id>
int accept_request(string grpname,string user,string own)
{
//cout<<"hai"<<endl;
if (grp.find(grpname) == grp.end())return 0;
if(grp[grpname].owner!=own)return 0;
if(!grp[grpname].gmebs.empty()){
//cout<<"hello"<<endl;
for (auto it = grp[grpname].gmebs.begin();it != grp[grpname].gmebs.end(); ++it){
if(*it==user)return 0;}}
int x=0;
if(!grp[grpname].pendingreq.empty()){
vector<string> temp=grp[grpname].pendingreq;
int ind=-1,flag=0;
for(int i=0;i<temp.size();i++)
{
if(temp[i]==user){
ind=i;
flag=1;
break;
}
}
if(flag==0)return 0;
else 
{
grp[grpname].pendingreq.erase(ind+grp[grpname].pendingreq.begin());
grp[grpname].gmebs.push_back(user);
return 1;
}}

//for (auto it = grp[grpname].pendingreq.begin();it != grp[grpname].pendingreq.end(); ++it)
//if(*it==user){cout<<"gn"<<endl;grp[grpname].pendingreq.erase(it);x=1;}}

return 1;}

//////////////////****************************upload_file(request[1],request[2],cli_socket);
int upload_file(string fname,string grpname,int cli_socket)
{

int valread;int flag=0;
if (grp.find(grpname) == grp.end())return 0;


if(!grp[grpname].sharedfiles.empty()){
vector<string> temp=grp[grpname].sharedfiles;

for(int i=0;i<temp.size();i++)
{
if(temp[i]==fname){
flag=1;break;}
}}

if(flag==0)grp[grpname].sharedfiles.push_back(fname);//insert in group files
char buffer[1025];
bzero(buffer, 1024);

valread = read( cli_socket , buffer, 1024);

string s=string(buffer);
cout<<s<<endl;
vector<string> request1;
string k="";
        for(int i=0;i<s.size();i++)
        {
        if(s[i]!=' ')k=k+s[i];
        else {request1.push_back(k);k="";}
        }request1.push_back(k);
       
files f1;

f1.ip=request1[0];

//cout<<f1.ip<<endl;
f1.port=request1[1];
//cout<<f1.port<<endl;
f1.filename=fname;
//cout<<f1.filename<<endl;
filedata.push_back(f1);
return 1;
}

int download_file(string fname,string grpname,int cli_socket,string user,string dest)
{
char buffer[1025]={0};
string res1="notpossile";
strcpy(buffer,res1.c_str());
//check user is present in group

if (grp.find(grpname) == grp.end()){send(cli_socket , buffer , 1024, 0 );return 0;}

int flag=0,flag2=0;
for (auto& it : grp[grpname].gmebs){if(it==user)flag=1;}

if(flag==0){send(cli_socket , buffer , 1024, 0 );return 0;}
//check file is present in grps

if(!grp[grpname].sharedfiles.empty()){
vector<string> temp=grp[grpname].sharedfiles;

for(int i=0;i<temp.size();i++)
{
if(temp[i]==fname){
flag2=1;break;}
}}

if(flag2==0){send(cli_socket , buffer , 1024, 0 );return 0;}
///send details
string res="";
for(int i=0;i<filedata.size();i++)
{
files f2=filedata[i];
if(f2.filename==fname)
{
res=f2.ip+" "+f2.port+" "+f2.filename+" "+dest;
}
}

bzero(buffer,1024);
strcpy(buffer,res.c_str());

send(cli_socket , buffer , 1024, 0 );
return 1;
}

/////////////*************888shop share
int stop_share(string grpname,string fname)
{
int valread;int flag=0,x=-1;
if (grp.find(grpname) == grp.end())return 0;
if(!grp[grpname].sharedfiles.empty()){
vector<string> temp=grp[grpname].sharedfiles;

for(int i=0;i<temp.size();i++)
{
if(temp[i]==fname){
flag=1;x=i;
break;}
}}
if(flag==1)
{grp[grpname].sharedfiles.erase(x+grp[grpname].sharedfiles.begin());
return 1;
}
return 0;
}

/////////////////*********************************list_files <group_id>
string list_files(string grpname)
{
if(grp[grpname].sharedfiles.empty())return "empty";
else{
string req="";
for (auto it = grp[grpname].sharedfiles.begin();it != grp[grpname].sharedfiles.end(); ++it)
{req=req+" "+*it;}
return req;
}
}

