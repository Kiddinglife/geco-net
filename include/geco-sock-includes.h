/*
 * Copyright (c) 2016
 * Geco Gaming Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for GECO purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation. Geco Gaming makes no
 * representations about the suitability of this software for GECO
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 */

// created on 19-March-2016 by Jackie Zhang

#ifndef __INCLUDE_GECO_SOCK_INCLUDE_H
#define __INCLUDE_GECO_SOCK_INCLUDE_H

// All this crap just to include type SOCKET

#ifdef __native_client__
#define _PP_Instance_ PP_Instance
#else
#define _PP_Instance_ int
#endif

#ifdef WINDOWS_STORE_RT
#include <windows.h>
#include "WinRTSockAddr.h"
typedef Windows::Networking::Sockets::DatagramSocket^ __UDPSOCKET__;
typedef Windows::Networking::Sockets::StreamSocket^ __TCPSOCKET__;
typedef unsigned int socklen_t;
#define FORMAT_MESSAGE_ALLOCATE_BUFFER 0
#define FIONBIO 0
#define LocalFree(x)
// using Windows.Networking;
// using Windows.Networking.Sockets;
// See http://msdn.microsoft.com/en-us/library/windows/apps/windows.networking.sockets.datagramsocketcontrol
#elif defined(_WIN32)
// IP_DONTFRAGMENT is different between winsock 1 and winsock 2.  Therefore, Winsock2.h must be linked againt Ws2_32.lib
// winsock.h must be linked against WSock32.lib.  If these two are mixed up the flag won't work correctly
// WinRT: http://msdn.microsoft.com/en-us/library/windows/apps/windows.networking.sockets
// Sample code: http://stackoverflow.com/questions/10290945/correct-use-of-udp-datagramsocket
#include <WinSock2.h>
#include <windows.h>
typedef SOCKET __UDPSOCKET__;
typedef SOCKET __TCPSOCKET__;
typedef int socklen_t;
#else
// For CFSocket
// https://developer.apple.com/library/mac/#documentation/CoreFOundation/Reference/CFSocketRef/Reference/reference.html
// Reason: http://sourceforge.net/p/open-dis/discussion/683284/thread/0929d6a0
#   if defined(__APPLE__)
#import <CoreFoundation/CoreFoundation.h>
#   endif

#   if !defined(ANDROID)
#include <ifaddrs.h>
#   endif

#   if (defined(__GNUC__)  || defined(__GCCXML__))
#include <netdb.h>
#include <errno.h>
#   endif

#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

#   ifdef __native_client__
#include "ppapi/cpp/private/net_address_private.h"
#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_errors.h"
#include "ppapi/cpp/completion_callback.h"
#include "ppapi/cpp/instance_handle.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/module_impl.h"
#include "ppapi/c/pp_errors.h"
#include "ppapi/c/pp_module.h"
#include "ppapi/c/pp_var.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/ppb.h"
#include "ppapi/c/ppb_instance.h"
#include "ppapi/c/ppb_messaging.h"
#include "ppapi/c/ppb_var.h"
#include "ppapi/c/ppp.h"
#include "ppapi/c/ppb_core.h"
#include "ppapi/c/ppp_instance.h"
#include "ppapi/c/ppp_messaging.h"
#include "ppapi/c/pp_input_event.h"
#include "ppapi/c/pp_completion_callback.h"
//UDP specific - the 'private' folder was copied from the chromium src/ppapi/c headers folder
#include "ppapi/c/private/ppb_udp_socket_private.h"
#include "ppapi/cpp/private/net_address_private.h"
typedef PP_Resource __UDPSOCKET__;
typedef PP_Resource __TCPSOCKET__;
#   else
//#include "RakMemoryOverride.h"
/// Unix/Linux uses ints for sockets
typedef int __UDPSOCKET__;
typedef int __TCPSOCKET__;
#   endif
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET (-1)
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR (-1)
#endif

#define SAFE_CLOSE_SOCK(rns2Socket) \
if( rns2Socket != INVALID_SOCKET ) { closesocket__(rns2Socket); rns2Socket = (JISSocket) INVALID_SOCKET; }

#endif // end
