#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

using namespace std;

void error(const char *msg) {
	perror(msg);
	exit(0);
}

void extractFileName(string command, char **comm) {
	int start = 0;
	for (int i = 0; i < command.length(); i++) {
		if (command[i] == ' ') {
			start = i + 1;
			break;
		}
	}
	int end = 0;
	for (int i = start; i < command.length(); i++) {
		if (command[i] == ' ') {
			end = i - 1;
			break;
		}
	}
	if (end == 0)
		end = command.length();
	string s = command.substr(start, end - start + 1);

	*comm = (char *) malloc((end - start + 1) * sizeof(char));
	strcpy(*comm, s.c_str());
}

int main(int argc, char *argv[]) {

	char* input = "client.in";
	ifstream file(input);
	unsigned int length = sizeof(struct sockaddr_in);
	string serverIpAddress, requiredFileName;
	if (!file) {
		cout << "unable to open file";
		exit(1);
	} else {
		string line;
		getline(file, line);
		serverIpAddress = line;
		cout << serverIpAddress << endl;
		getline(file, line);
		string serverPortNumber = line;
		getline(file, line);
		string clientPortNumber = line;
		getline(file, line);
		requiredFileName = line;
		getline(file, line);
		//int slidingWindowSize=line;
		cout << requiredFileName << endl;

	}

	string line;
//while (getline(file, line)){
	//cout<<"input file : "<<line<<endl;
	//char comm[line.length()];
	//strcpy(comm, line.c_str());
	int sockfd;    //file descriptor of client socket
	int portno = 8000;    //port number of the server
	int n; //return value of the read()/write() calls, actuall size read/written
	struct sockaddr_in serv_addr; //address of the server that we are connecting to
	struct sockaddr_in new_serv_addr; //new address of the server that we are connecting to
	struct hostent *server;

	/*
	 struct  hostent
	 {
	 char    *h_name;        // official name of host
	 char    **h_aliases;    // alias list
	 int     h_addrtype;     // host address type
	 int     h_length;       // length of address
	 char    **h_addr_list;  // list of addresses from name server
	 #define h_addr  h_addr_list[0]  // address, for backward compatiblity
	 };

	 It defines a host computer on the Internet. The members of this structure are:
	 h_name       Official name of the host.
	 h_aliases    A zero  terminated  array  of  alternate
	 names for the host.
	 h_addrtype   The  type  of  address  being  returned;
	 currently always AF_INET.
	 h_length     The length, in bytes, of the address.
	 h_addr_list  A pointer to a list of network addresses


	 returned in network byte order.
	 */
	char buffer[256];    //buffer ro read to/write from server
	//if (argc < 2) {
	// fprintf(stderr,"usage %s hostname port\n", argv[0]);
	//exit(0);
	//}
	//istringstream buf(line);
	//istream_iterator<std::string> beg(buf), end;
	//vector<std::string> tokens(beg, end); // done!
	//string hostname=argv[1];
	//int portnum=80;
	//if(argc>2){
	//portnum=atoi(argv[2]);
	//}

	//if(tokens.size()==3){
	//hostname=tokens[2];
	//}else if(tokens.size()==4){
	//hostname=tokens[2];
	//portnum=atoi(tokens[3].c_str());
	//}
	//portno = portnum;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		error("ERROR opening socket");
	}
	server = gethostbyname(serverIpAddress.c_str());
	if (server == NULL) {
		fprintf(stderr, "ERROR, no such host\n");
		exit(0);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));    //sets it to zero
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr,
	(char *)&serv_addr.sin_addr.s_addr,
	server->h_length);    //copy arg1 to arg2
	serv_addr.sin_port = htons(portno); //htons() converts a port number in host byte order to a port number in network byte order.

	//if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
	//error("ERROR connecting");
	int index = 0;
	string http = "HTTP/1.0";
	//for(int i=0;i<tokens[0].length();i++){
	//comm[index++]=tokens[0][i];
	//}
	//comm[index++]=' ';
	//for(int i=0;i<tokens[1].length();i++){
	//comm[index++]=tokens[1][i];
	//}
	//comm[index++]=' ';
	//for(int i=0;i<http.length();i++){
	//comm[index++]=http[i];
	//}
	//comm[index]='\0';
	n = sendto(sockfd, requiredFileName.c_str(), requiredFileName.length(), 0,
			(struct sockaddr *) &serv_addr, length);
	//write(sockfd,requiredFileName.c_str(),index);
	if (n < 0)
		error("ERROR writing to socket");
	//char* filename;
	//extractFileName(line,&filename);
	// cout<<filename<<endl;
	int bytes = recvfrom(sockfd, buffer, 256, 0,
			(struct sockaddr *) &new_serv_addr, &length);
	buffer[bytes] = '\0';
	cout << buffer << endl;
	cout << "port numbrer " << new_serv_addr.sin_port;
	string msg = "^__^";
	sendto(sockfd, msg.c_str(), msg.length(), 0,
			(struct sockaddr *) &new_serv_addr, length);

	/*  if(line[0]=='G')
	 {
	 // cout<<"IN GET"<<endl;
	 FILE * pFile;

	 string str(filename);
	 size_t found = str.find("txt");
	 size_t found1 = str.find("html");

	 if (found!=string::npos||found1!=string::npos){
	 pFile = fopen (filename, "w");
	 cout<<"write Txt or html"<<endl;
	 }else{
	 pFile = fopen (filename, "wb");
	 cout<<"write image"<<endl;

	 }
	 int bytes=0;
	 char buffer[512];
	 //buf is the buffer to read the information into, len is the maximum length of the buffer, and flags can again be set to 0
	 //if ((bytes = recvfrom(sockfd,buffer, 511, 0,(struct sockaddr *)&serv_addr,&length)) > 0) {

	 if ((bytes = recvfrom(sockfd,buffer,256,0,(struct sockaddr *)&new_serv_addr, &length)) > 0) {
	 buffer[bytes] = '\0';
	 string str(buffer);
	 size_t found = str.find("404");
	 if (found!=string::npos){
	 cout<<str<<endl;
	 }else{
	 while ((bytes =  recvfrom(sockfd,buffer,256,0,(struct sockaddr *)&new_serv_addr, &length)) > 0) {
	 buffer[bytes] = '\0';
	 string str(buffer);
	 size_t found = str.find("404");
	 if (found!=string::npos){
	 cout<<str<<endl;
	 break;
	 }
	 fwrite(buffer,1,bytes,pFile);
	 }
	 }
	 }

	 fclose (pFile);
	 }else{//POST

	 if ((n =  recvfrom(sockfd,buffer,256,0,(struct sockaddr *)&new_serv_addr, &length)) == -1) {
	 exit(1);
	 }
	 buffer[n] = '\0';
	 printf("client: received '%s'\n", buffer);
	 FILE * file;
	 string str(filename);
	 size_t found = str.find("txt");
	 size_t found1 = str.find("html");

	 if (found!=string::npos||found1!=string::npos){
	 file = fopen(filename, "r");

	 }else{
	 file = fopen(filename, "rb");

	 }
	 int bytes,offset,end,sent;
	 char buffer[512];
	 while ((bytes = fread(buffer, 1, 250,file)) > 0) {

	 offset = 0;
	 end = 0;
	 while ((end == 0)
	 && ((sent = sendto(sockfd, buffer + offset, bytes, 0,(struct sockaddr *)&serv_addr, length)) > 0)) {
	 bytes -= sent;
	 offset += sent;
	 if (bytes == 0)
	 end = 1;
	 }
	 if (bytes > 0)
	 return -1;

	 }

	 fclose (file);
	 }*/
	close(sockfd);
//}
	file.close();

	return 0;
}
