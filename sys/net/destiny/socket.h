/*
 * socket.h
 *
 *  Created on: 16.09.2011
 *      Author: Oliver
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <stdint.h>
#include "tcp.h"
#include "udp.h"
#include "in.h"
#include "sys/net/sixlowpan/sixlowip.h"

/*
 * Types
 */
#define	SOCK_STREAM			1					/* stream socket */
#define	SOCK_DGRAM			2					/* datagram socket */
#define	SOCK_RAW			3					/* raw-protocol interface */
#define	SOCK_RDM			4					/* reliably-delivered message */
#define	SOCK_SEQPACKET		5					/* sequenced packet stream */

/*
 * Address families.
 */
#define	AF_UNSPEC			0					/* unspecified */
#define	AF_LOCAL			1					/* local to host (pipes, portals) */
#define	AF_UNIX				AF_LOCAL			/* backward compatibility */
#define	AF_INET				2					/* internetwork: UDP, TCP, etc. */
#define	AF_IMPLINK			3					/* arpanet imp addresses */
#define	AF_PUP				4					/* pup protocols: e.g. BSP */
#define	AF_CHAOS			5					/* mit CHAOS protocols */
#define	AF_NS				6					/* XEROX NS protocols */
#define	AF_ISO				7					/* ISO protocols */
#define	AF_OSI				AF_ISO
#define	AF_ECMA				8					/* European computer manufacturers */
#define	AF_DATAKIT			9					/* datakit protocols */
#define	AF_CCITT			10					/* CCITT protocols, X.25 etc */
#define	AF_SNA				11					/* IBM SNA */
#define AF_DECnet			12					/* DECnet */
#define AF_DLI				13					/* DEC Direct data link interface */
#define AF_LAT				14					/* LAT */
#define	AF_HYLINK			15					/* NSC Hyperchannel */
#define	AF_APPLETALK		16					/* Apple Talk */
#define	AF_ROUTE			17					/* Internal Routing Protocol */
#define	AF_LINK				18					/* Link layer interface */
#define	pseudo_AF_XTP		19					/* eXpress Transfer Protocol (no AF) */
#define	AF_COIP				20					/* connection-oriented IP, aka ST II */
#define	AF_CNT				21					/* Computer Network Technology */
#define pseudo_AF_RTIP		22					/* Help Identify RTIP packets */
#define	AF_IPX				23					/* Novell Internet Protocol */
#define	AF_SIP				24					/* Simple Internet Protocol */
#define	pseudo_AF_PIP		25					/* Help Identify PIP packets */
#define	AF_ISDN				26					/* Integrated Services Digital Network*/
#define	AF_E164				AF_ISDN				/* CCITT E.164 recommendation */
#define	pseudo_AF_KEY		27					/* Internal key-management function */
#define	AF_INET6			28					/* IPv6 */
#define	AF_NATM				29					/* native ATM access */
#define	AF_ATM				30					/* ATM */
#define pseudo_AF_HDRCMPLT 	31					/* Used by BPF to not rewrite headers
												 * in interface output routine
												 */
#define	AF_NETGRAPH			32					/* Netgraph sockets */
#define	AF_MAX				33

/*
 * Protocol families, same as address families for now.
 */
#define	PF_UNSPEC			AF_UNSPEC
#define	PF_LOCAL			AF_LOCAL
#define	PF_UNIX				PF_LOCAL			/* backward compatibility */
#define	PF_INET				AF_INET
#define	PF_IMPLINK			AF_IMPLINK
#define	PF_PUP				AF_PUP
#define	PF_CHAOS			AF_CHAOS
#define	PF_NS				AF_NS
#define	PF_ISO				AF_ISO
#define	PF_OSI				AF_ISO
#define	PF_ECMA				AF_ECMA
#define	PF_DATAKIT			AF_DATAKIT
#define	PF_CCITT			AF_CCITT
#define	PF_SNA				AF_SNA
#define PF_DECnet			AF_DECnet
#define PF_DLI				AF_DLI
#define PF_LAT				AF_LAT
#define	PF_HYLINK			AF_HYLINK
#define	PF_APPLETALK		AF_APPLETALK
#define	PF_ROUTE			AF_ROUTE
#define	PF_LINK				AF_LINK
#define	PF_XTP				pseudo_AF_XTP		/* really just proto family, no AF */
#define	PF_COIP				AF_COIP
#define	PF_CNT				AF_CNT
#define	PF_SIP				AF_SIP
#define	PF_IPX				AF_IPX				/* same format as AF_NS */
#define PF_RTIP				pseudo_AF_RTIP		/* same format as AF_INET */
#define PF_PIP				pseudo_AF_PIP
#define	PF_ISDN				AF_ISDN
#define	PF_KEY				pseudo_AF_KEY
#define	PF_INET6			AF_INET6
#define	PF_NATM				AF_NATM
#define	PF_ATM				AF_ATM
#define	PF_NETGRAPH			AF_NETGRAPH
#define	PF_MAX				AF_MAX

#define MAX_SOCKETS			8
#define MAX_QUEUED_SOCKETS	5

#define EPHEMERAL_PORTS 	49152

#define STATIC_MSS			32
#define STATIC_WINDOW		32

typedef struct __attribute__ ((packed)) socka6
	{
    uint8_t     		sin6_family;    		/* AF_INET6 */
    uint16_t       		sin6_port;      		/* transport layer port # */
    uint32_t        	sin6_flowinfo;  		/* IPv6 flow information */
    ipv6_addr_t 		sin6_addr;      		/* IPv6 address */
	} sockaddr6_t;

typedef struct __attribute__ ((packed)) sock_t
	{
	uint8_t				domain;
	uint8_t				type;
	uint8_t				protocol;
	tcp_socket_status_t local_tcp_status;
	tcp_socket_status_t foreign_tcp_status;
	sockaddr6_t			local_address;
	sockaddr6_t			foreign_address;
	} socket_t;

typedef struct __attribute__ ((packed)) socket_in_t
	{
	uint8_t				socket_id;
	uint8_t				pid;
	socket_t			in_socket;
	socket_t			queued_sockets[MAX_QUEUED_SOCKETS];
	} socket_internal_t;

socket_internal_t sockets[MAX_SOCKETS];

int socket(int domain, int type, int protocol);
int connect(int socket, sockaddr6_t *addr, uint32_t addrlen, uint8_t tcp_client_thread);
socket_t *getWaitingConnectionSocket(int socket);
int32_t recvfrom( int s, void *buf, uint64_t len, int flags, sockaddr6_t *from, uint32_t *fromlen );
int32_t sendto( int s, void *msg, uint64_t len, int flags, sockaddr6_t *to, uint32_t tolen);
int32_t send(int s, void *msg, uint64_t len, int flags);
int32_t recv(int s, void *buf, uint64_t len, int flags);
int close(int s);
int bind(int s, sockaddr6_t *name, int namelen, uint8_t pid);
int listen(int s, int backlog);
int accept(int s, sockaddr6_t *addr, uint32_t addrlen, uint8_t pid);
int shutdown(int s , int how);
void socket_init(void);
socket_internal_t *get_udp_socket(ipv6_hdr_t *ipv6_header, udp_hdr_t *udp_header);
socket_internal_t *get_tcp_socket(ipv6_hdr_t *ipv6_header, tcp_hdr_t *tcp_header);
void print_sockets(void);
void print_socket(uint8_t socket);
bool exists_socket(uint8_t socket);
socket_t *new_tcp_queued_socket(ipv6_hdr_t *ipv6_header, tcp_hdr_t *tcp_header, socket_internal_t *socket);

#endif /* SOCKET_H_ */
