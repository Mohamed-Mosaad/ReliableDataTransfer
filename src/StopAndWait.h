/*
 * StopAndWait.h
 *
 *  Created on: Dec 5, 2015
 *      Author: mos3aad
 */

#ifndef STOPANDWAIT_H_
#define STOPANDWAIT_H_

#include "ReliableDataTransfer.h"

class StopAndWait: public ReliableDataTransfer {
public:
	StopAndWait();
	virtual ~StopAndWait();
	void setDest(sockaddr* dest);
	int sendPacket(Packet packet,int packLen, int socketFD);
	int recvPacket(Packet* packet,int* packLen,int socketFD);
	int sendFile(string fileName,int socketFD);
	int recvFile(string fileName,int socketFD);
	void packetToBuffer(char* buffer,Packet packet);
	void bufferToAckPacket(char* buffer,AckPacket* packet);
	void bufferToPacket(char* buffer,Packet* packet);
	void ackPacketToBuffer(char* buffer, AckPacket packet);
	int get_mode(const char *filename);

};

#endif /* STOPANDWAIT_H_ */
