/*
 * pp_net.h
 *
 *  Created on: Mar 12, 2018
 *      Author: purperzhang
 */

#ifndef COMMON_PP_COMMON_PP_NET_H_
#define COMMON_PP_COMMON_PP_NET_H_

#include<arpa/inet.h>
#include<sys/socket.h>
#include<netdb.h>
#include<ifaddrs.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<vector>
#include"pp_udp.h"
namespace pp
{

static std::vector<pp::UA> getaddrs()
{//bind all network card

	std::vector<pp::UA> ips;

	struct ifaddrs *ifaddr, *ifa;
	int family, s;
	char host[NI_MAXHOST];

	if (getifaddrs(&ifaddr) == -1) {
		perror("getifaddrs");
//		exit(EXIT_FAILURE);
		return ips;
	}

	/* Walk through linked list, maintaining head pointer so we
   can free list later */

	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr == NULL)
			continue;

		family = ifa->ifa_addr->sa_family;

		/* Display interface name and family (including symbolic
       form of the latter for the common families) */

		printf("%s  address family: %d%s\n",
				ifa->ifa_name, family,
				(family == AF_PACKET) ? " (AF_PACKET)" :
						(family == AF_INET) ?   " (AF_INET)" :
								(family == AF_INET6) ?  " (AF_INET6)" : "");

		/* For an AF_INET* interface address, display the address */

		if (family == AF_INET || family == AF_INET6) {
			s = getnameinfo(ifa->ifa_addr,
					(family == AF_INET) ? sizeof(struct sockaddr_in) :
							sizeof(struct sockaddr_in6),
							host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
			if (s != 0) {
				printf("getnameinfo() failed: %s\n", gai_strerror(s));
				return ips;
			}
			printf("\taddress: <%s>\n", host);

			pp::UA ua;
			ua.set_sockaddr(family,ifa->ifa_addr,(family == AF_INET) ? sizeof(struct sockaddr_in):sizeof(struct sockaddr_in6));
			ips.push_back(ua);
		}
	}

	freeifaddrs(ifaddr);

	return ips;

}
}




#endif /* COMMON_PP_COMMON_PP_NET_H_ */
