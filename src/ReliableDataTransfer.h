/*
 * ReliableDataTransfer.h
 *
 *  Created on: Dec 5, 2015
 *      Author: mos3aad
 */
#include <string>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

#ifndef RELIABLEDATATRANSFER_H_
#define RELIABLEDATATRANSFER_H_
#define PSIZE 512
#define CHUNK 500
#define ASCII 0
#define BINARY 1
using namespace std;

typedef struct Packet {
	uint16_t cksum;
	uint16_t len;
	uint32_t seqno;
	char data[PSIZE];
} Packet;
typedef struct AckPacket {
	uint16_t cksum;
	uint16_t len;
	uint32_t ackno;
}AckPacket;
class ReliableDataTransfer {
public:
	ReliableDataTransfer();
	virtual ~ReliableDataTransfer();
	// pure virtual function
	virtual void setDest(sockaddr* dest)=0;
	virtual int sendPacket(Packet packet,int packLen, int socketFD) = 0;
	virtual int recvPacket(Packet* packet,int* packLen,int socketFD) = 0;
	virtual int sendFile(string fileName,int socketFD) = 0;
	virtual int recvFile(string fileName,int socketFD) = 0;
protected:
	sockaddr * dest;
};

#endif /* RELIABLEDATATRANSFER_H_*/
