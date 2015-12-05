/*
 * StopAndWait.cpp
 *
 *  Created on: Dec 5, 2015
 *      Author: mos3aad
 */

#include "StopAndWait.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
using namespace std;
StopAndWait::StopAndWait() {
	// TODO Auto-generated constructor stub

}
void StopAndWait::setDest(sockaddr* dest) {
	this->dest = dest;
}

int StopAndWait::sendPacket(Packet packet, int packLen, int socketFD) {
	char* buffer;
	packetToBuffer(buffer, packet);
	int bytes = sendto(socketFD, buffer, packLen, 0, dest,
			sizeof(struct sockaddr));
	if (bytes < 0) {
		perror("ERROR writing to socket");
	}
	struct timeval tv;
	fd_set readfds;
	tv.tv_sec = 2;
	tv.tv_usec = 500000;
	FD_ZERO(&readfds);
	FD_SET(socketFD, &readfds);
	select(socketFD + 1, &readfds, NULL, NULL, &tv);

	if (FD_ISSET(socketFD, &readfds)) {
		char buffer[CHUNK];
		socklen_t dest_len;
		struct sockaddr_in new_serv_addr;
		if (recvfrom(socketFD, buffer, CHUNK, 0,
				(struct sockaddr*) &new_serv_addr, &dest_len) >= 0) {
			AckPacket ackPack;
			bufferToAckPacket(buffer, &ackPack);
			if (ackPack.ackno == packet.seqno) {
				cout << "Acked" << endl;

			}
		} else {
			cout << "Wrong Ack" << endl;

		}
	} else {
		sendPacket(packet, packLen, socketFD);
	}
	return 0;
}

int StopAndWait::recvPacket(Packet* packet, int* packLen, int socketFD) {
	char buffer[2*CHUNK];
	struct sockaddr new_serv_addr;
	socklen_t dest_len;
	if (recvfrom(socketFD, buffer, 2*CHUNK - 1, 0,
			(struct sockaddr*) &new_serv_addr, &dest_len) >= 0) {
		bufferToPacket(buffer, packet);
		AckPacket* ack = (AckPacket*) malloc(sizeof(AckPacket));
		ack->ackno = packet->seqno;
		ack->len = sizeof(AckPacket);
		char *buffer;
		ackPacketToBuffer(buffer,*ack);
		sendto(socketFD,buffer,sizeof(AckPacket),0,&new_serv_addr,dest_len);
	} else {
		//recv error
		perror("recv error in receive packet\n");
	}
	return strlen(packet->data);
}

int StopAndWait::sendFile(string fileName, int socketFD) {
	int mode = get_mode(fileName.c_str());
		FILE * file;
		if(mode == ASCII)
			file = fopen(fileName.c_str(),"r");
		else
			file = fopen(fileName.c_str(),"rb");
		if(file == NULL){
			//if(send(sockfd,not_found,strlen(not_found),0)<(int)strlen(not_found))return -1;
		}else{
			//if (send(sockfd,found,strlen(found),0)<(int)strlen(found))return -1;
			int bytes,sent,offset,end;
			char buffer [CHUNK];
			int seq = 1;
			while((bytes = fread(buffer,1,CHUNK,file))>0){
				Packet *packet =(Packet *)malloc(sizeof(Packet));
				strcpy(packet->data,buffer);
				packet->seqno = seq;
				packet->len = sizeof(Packet);
				seq++;
				sendPacket(*packet,packet->len,socketFD);
			}
			fclose(file);
		}

	return 0;
}

int StopAndWait::recvFile(string fileName, int socketFD) {
	//if (send(sockfd,found,strlen(found),0)<(int)strlen(found))return -1;
		FILE * file;
		int mode = get_mode(fileName.c_str());
		if(mode == ASCII)
			file = fopen(fileName.c_str(),"w");
		else
			file = fopen(fileName.c_str(),"wb");
		int bytes;
		char buffer[2*CHUNK+1];
		Packet outputPacket;
		int sizeOfPacket;
		while((bytes = recvPacket(&outputPacket,&sizeOfPacket,socketFD))>0){
            strcpy(buffer,outputPacket.data);
            fwrite(buffer,1,bytes,file);
            if(buffer[strlen(buffer)-1]==EOF) {
            	break;
            }
		}
		fclose(file);
	return 0;
}
void StopAndWait::packetToBuffer(char* buffer, Packet packet) {
	buffer = (char*) malloc(sizeof(Packet));
	memcpy(buffer, &packet, sizeof(Packet));
}
void StopAndWait::bufferToAckPacket(char* buffer, AckPacket* packet) {
	packet = (AckPacket*) malloc(sizeof(AckPacket));
	memcpy(packet, buffer, sizeof(AckPacket));
}
void StopAndWait::bufferToPacket(char* buffer, Packet* packet) {
	packet = (Packet*) malloc(sizeof(Packet));
	memcpy(packet, buffer, sizeof(Packet));
}
void StopAndWait::ackPacketToBuffer(char* buffer, AckPacket packet) {
	buffer = (char*) malloc(sizeof(AckPacket));
	memcpy(buffer, &packet, sizeof(AckPacket));
}
int StopAndWait:: get_mode(const char *filename){
	char * new_name = (char *) malloc(strlen(filename)*sizeof(char));
	strcpy(new_name,filename);
	char * tmp = strtok(new_name,".");
	tmp = strtok(NULL,".");
	if((strcmp(tmp,"html")==0) ||(strcmp(tmp,"txt")==0) ){
		free(new_name);
		return ASCII;
	}
	free(new_name);
	return BINARY;
}

StopAndWait::~StopAndWait() {
// TODO Auto-generated destructor stub
}

