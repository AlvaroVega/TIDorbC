#include "TIDorb/core.h"

/* -----------------------------------------------------------------------------

File: ConfORB.h


Configuration set for TIDorb.

Autor:
Juan A. Caceres - caceres@tid.es

Version 1.0

Revisions:

// (C) Copyright 2009 Telefonica Investigacion y Desarrollo
//     S.A.Unipersonal (Telefonica I+D)
//
// This file is part of Morfeo CORBA Platform.
//
// Morfeo CORBA Platform is free software: you can redistribute it and/or
// modify it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the License,
// or (at your option) any later version.
//
// Morfeo CORBA Platform is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with Morfeo CORBA Platform. If not, see
//
//   http://www.gnu.org/licenses
//
// Info about members and contributors of the MORFEO project
// is available at
//
//   http://morfeo-project.org
------------------------------------------------------------------------------ */

#ifndef _TIDORB_CORE_CONF_ORB_H_
#define _TIDORB_CORE_CONF_ORB_H_

#include <sys/types.h>

#include "CORBA.h"
#include "BiDirPolicy.h"

#include <vector>

#ifdef TIDORB_HAVE_NAMESPACE_STD
using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
using std::vector;
#endif

namespace TIDorb {
namespace core {

  class ConfORB {

    TIDorb::core::PolicyContext policy_context;
    static TIDorb::core::PolicyContext* st_default_policy_context;

    static TIDorb::core::PolicyContext* createDefaultPolicies();


    static TIDorb::core::PolicyContext st_orb_instances;

  private:
    // Consume -ORB options, moving the rest of elements of argv[]
    int modify_parameters(int& argc, char** argv, int currpos, int numparams);


  public:

    ConfORB();
    ~ConfORB();

    void get_parameters(int& argc, char** argv, const char* orb_identifier) throw (CORBA::SystemException);
    TIDorb::core::PolicyContext& getPolicyContext() { return policy_context; }

    void dump(ostream& os);

    /**
     * TIDorb ORB identifier: 666. This value must be registered in OMG.
     */

    static const TIDorb::core::iop::ORBComponent* ORB_TYPE;

    /**
     * TIDorb ORB_id.
     */

    char* orb_id;
    static const char* orb_id_name;
    static const char* DEFAULT_ORB_ID;

    /**
     * TIDorb Initial References.
     */

    vector<TIDorb::core::util::InitialReference> initial_references;
    static const char* init_refs_name;

    /**
     * TIDorb Default Initial References.
     */

    char* default_initial_reference;
    static const char* def_init_ref_name;

    /**
     * Naming service URL.
     */

    char* naming_service;
    static const char* naming_service_name;

    /**
     * Block size for CDR buffers.
     * Default value: <code>256</code>.
     * Values must be >= 256 and multiple of 8
     */

    size_t block_size;
    static const size_t DEFAULT_BLOCK_SIZE;
    static const char* block_size_name;

    /**
     * Exhaustive equal.
     * Make or not an exhaustive TypeCode matching:
     * If <code>true</code> two TypeCodes equals each other if
     * its RepositoryId and member names and TypeCodes equals.
     * If <code>false</code> only RepositoryIds are compared.
     * Values must be "true" or "false".
     * Default exhaustive TypeCode matching.
     */

    bool exhaustive_equal;
    static const bool DEFAULT_EXHAUSTIVE_EQUAL;
    static const char* exhaustive_equal_name;

    /**
     * Reliable oneway.
     * Assures that a oneway request could be forwarded.
     * Default value: <code>false</code>.
     * Values must be "true" or "false".
     */

    bool reliable_oneway;
    static const bool DEFAULT_RELIABLE_ONEWAY;
    static const char* reliable_oneway_name;

    /**
     * Maximum time waiting a request reply, in milliseconds.
     * Default value: <code>5000</code>.
     * Values must be greater or equal than 0.
     */

    time_t max_blocked_time;
    static const time_t DEFAULT_MAX_BLOCKED_TIME;
    static const char* max_blocked_time_name;

    /**
     * Maximum fragment message size.
     * Value must be >= 1024 and multiple of 8.
     * Fragment size is the sum of message body size
     * and the message header size (12 bytes) .
     * Default value: <code>4096</code>.
     */

    size_t fragment_size;
    static const size_t DEFAULT_FRAGMENT_SIZE;
    static const char* fragment_size_name;

    /**
     * Default GIOP version.
     * GIOP version used. The default version is 1.2, but works with 1.0 and 1.1
     * versions to allow interoperatibility with older ORBs.
     * Values: "1.0", "1.1" and "1.2"
     */

    TIDorb::core::comm::iiop::Version GIOPVersion;
    static const TIDorb::core::comm::iiop::Version DEFAULT_GIOP_VERSION;
    static const char* GIOPVersion_name;

    /**
     * Bidirectional policy used.
     * Values allowed NORMAL or BOTH
     */

    BiDirPolicy::BidirectionalPolicyValue bidirectional;
    static const BiDirPolicy::BidirectionalPolicyValue DEFAULT_BIDIRECTIONAL;
    static const char* bidirectional_name;

    /**
     * Maximum number of open connections.
     * Default value: <code>30</code>.
     * Values must be greater or equal than 0.
     */

    unsigned int max_connections;
    static const unsigned int DEFAULT_MAX_CONNECTIONS;
    static const char* max_connections_name;

    /**
     * Maximum retries to recover communications.
     * Values must be greater or equal than 0.
     * Default value: <code>3</code>.
     */

    unsigned int max_comm_recovering_times;
    static const unsigned int DEFAULT_MAX_COMM_RECOVERING_TIMES;
    static const char* max_comm_recovering_times_name;

    /**
     * Time to wait before recovering communications, in milliseconds.
     * Values must be greater or equal than 0.
     * Default value: <code>1000</code>.
     */

    time_t comm_recover_time;
    static const time_t DEFAULT_COMM_RECOVER_TIME;
    static const char* comm_recover_time_name;

    /**
     * Maximum uncompleted messages per connection.
     * Values must be greater or equal than 0.
     * Default value: <code>25</code>.
     */

    unsigned int max_uncompleted_messages;
    static const unsigned int DEFAULT_MAX_UNCOMPLETED_MESSAGES;
    static const char* max_uncompleted_messages_name;

    /**
     * ORB name.
     * IORs will be generated with this name. It must be the symbolic name.
     */

    char* host;
    static const char* DEFAULT_HOST_NAME;
    static const char* host_name;

    /**
     * Host IP address. Useful if the host has multiple IP interfaces.
     */

    char* ip_address;
    static const char* ip_address_name;

    /**
     * ORB listen port.
     * Values must be greater or equal than 0.
     * Default value: <code>0</code> (assigned by system)
     */

    unsigned short orb_port;
    static const unsigned short DEFAULT_ORB_PORT;
    static const char* orb_port_name;

    /**
     * SO_LINGER socket property.
     * Default value: <code>-1</code> (no linger).
     * Values must be greater or equal than -1.
     */

    int socket_linger;
    static const int DEFAULT_SOCKET_LINGER;
    static const char* socket_linger_name;


    /**
     * SO_TIMEOUT socket property.
     * Values must be greater or equal than 0.
     * Default value: <code>0</code> (no timeout).
     */

    time_t socket_timeout;
    static const time_t DEFAULT_SOCKET_TIMEOUT;
    static const char* socket_timeout_name;

    /**
     * Maximum time waiting for socket connection, in milliseconds.
     * Default value: <code>0</code> (no timeout).
     * Values must be greater or equal than 0.
     */

    time_t socket_connect_timeout;
    static const time_t DEFAULT_SOCKET_CONNECT_TIMEOUT;
    static const char* socket_connect_timeout_name;

    /**
     * TCP_NODELAY socket property.
     * Default value: <code>true</code> (no delay).
     * Values must be "true" or "false".
     */

    bool tcp_nodelay;
    static const bool DEFAULT_TCP_NODELAY;
    static const char* tcp_nodelay_name;

    /**
     * Size of send and receive buffers for TCP sockets, in bytes.
     * Default value: 4000
     * Values must be greater or equal than 0.
     */

    size_t tcp_buffer_size;
    static const size_t DEFAULT_TCP_BUFFER_SIZE;
    static const char* tcp_buffer_size_name;

    /**
     * Maximum backlog queue length for server sockets.
     * Default value: <code>5</code>
     * Values must be greater or equal than 0.
     */

    unsigned int server_socket_backlog;
    static const unsigned int DEFAULT_SERVER_SOCKET_BACKLOG;
    static const char* server_socket_backlog_name;

    /**
     * ServerSockets reconnect property.<p>
     * Default value: <code>0</code>
     * Values must be greater or equal than 0.
     */

    unsigned int server_socket_reconnect;
    static const unsigned int DEFAULT_SERVER_SOCKET_RECONNECT;
    static const char* server_socket_reconnect_name;

    /**
     * ClientSockets reconnect property.<p>
     * Default value: <code>0</code>
     * Values must be greater or equal than 0.
     */

    unsigned int client_socket_reconnect;
    static const unsigned int DEFAULT_CLIENT_SOCKET_RECONNECT;
    static const char* client_socket_reconnect_name;


    /**
     * Maximum number of threads per POAManager.
     * Default value: <code>20</code>.
     * Values must be greater than 0.
     */

    unsigned int poa_max_threads;
    static const unsigned int DEFAULT_POA_MAX_THREADS;
    static const char* poa_max_threads_name;

    /**
     * Minimum number of threads number per POAManager.
     * Default value: <code>0</code>.
     * Values must be greater or equal than 0.
     */

    unsigned int poa_min_threads;
    static const unsigned int DEFAULT_POA_MIN_THREADS;
    static const char* poa_min_threads_name;

    /**
     * Maximum number of queued requests per POAManager.
     * Default value: <code>1000</code>.
     * Values must be greater or equal than 0.
     */

    unsigned int poa_max_queued_requests;
    static const unsigned int DEFAULT_POA_MAX_QUEUED_REQUESTS;
    static const char* poa_max_queued_requests_name;

    /**
     * Maximum thread's idle time per POAManager.
     * Default value: <code>0</code>.
     * If 0, a thread can be idle forever.
     */

    time_t poa_starving_time;
    static const time_t DEFAULT_POA_STARVING_TIME;
    static const char* poa_starving_time_name;

    /**
     * Thread stack size
     * Default value: 50000
     * Values must be greater or equal than 0.
     */

    size_t poa_thread_stack_size;
    static const size_t DEFAULT_POA_THREAD_STACK_SIZE;
    static const char* poa_thread_stack_size_name;

    /**
     * Threads scope (bound or not).
     * Default value: false
     * Values must be true or false.
     */

    bool poa_bound_threads;
    static const bool DEFAULT_POA_BOUND_THREADS;
    static const char* poa_bound_threads_name;

    /**
     * Maximum blocked time waiting for the shutdown completion.
     * Default value: <code>5000</code>.
     * Values must be greater or equal than 0.
     */

    time_t max_time_in_shutdown;
    static const time_t DEFAULT_MAX_TIME_IN_SHUTDOWN;
    static const char* max_time_in_shutdown_name;

    /**
     * TypeCode cache size.
     * Maximum of TypeCodes stored in the TypeCodeCache.<p>
     * Values must be greater or equal than 0.
     * Default value: <code>0</code>, no cache.
     */

    size_t typecode_cache_size;
    static const size_t DEFAULT_TYPECODE_CACHE_SIZE;
    static const char* typecode_cache_size_name;

    /**
     * Trace level.
     * Default value: <code>0</code>.
     * Values must be between 0 and 4.
     */

    unsigned int trace_level;
    static const unsigned int DEFAULT_TRACE_LEVEL;
    static const char* trace_level_name;

    /**
     * Trace file name.
     */

    char* trace_file;
    static const char* DEFAULT_TRACE_FILE;
    static const char* trace_file_name;

    /**
     * Default trace file size.
     * Default value: 10K.
     */

    size_t trace_file_size;
    static const size_t DEFAULT_FILE_SIZE;
    static const char* trace_file_size_name;

    /**
     * Default trace files list length.
     * Default value: 1 file.
     */

    unsigned int trace_num_files;
    static const unsigned int DEFAULT_NUM_FILES;
    static const char* trace_num_files_name;


    static const char* DEFAULT_TRACE_NAME;


//
// pra@tid.es - MIOP extensions
//

    /**
     * Multicast sockets time-to-live property.
     * Default value: 1.
     */

    int socket_time_to_live;
    static const int DEFAULT_SOCKET_TIME_TO_LIVE;
    static const char* socket_time_to_live_name;

    /**
     * Size of send and receive buffers for UCP sockets, in bytes.
     * Default value: 131072 (128K).
     * Values must be greater or equal than 0.
     */

    size_t udp_buffer_size;
    static const size_t DEFAULT_UDP_BUFFER_SIZE;
    static const char* udp_buffer_size_name;

    /**
     * UDP fragment size.
     * Maximum UDP fragment message size.
     * Value must be multiple of 8.
     * Fragment size is the sum of message body size and the message header size
     * Default value: <code>1024</code>.
     */

    size_t udp_fragment_size;
    static const size_t DEFAULT_UDP_FRAGMENT_SIZE;
    static const char* udp_fragment_size_name;

    /**
     * UDP datagram send delay, in milliseconds.
     * Default value: <code>1</code>.
     */

    time_t udp_send_delay;
    static const time_t DEFAULT_UDP_SEND_DELAY;
    static const char* udp_send_delay_name;

    /**
     * IP interface for outgoing multicast messages.
     */

    char* multicast_outgoing_interface;
    static const char* multicast_outgoing_interface_name;

    /**
     * IP interface for incoming multicast messages.
     */

    char* multicast_incoming_interface;
    static const char* multicast_incoming_interface_name;

    /*

    // Multicast address
    char* multicast_address;
    static const char* multicast_address_name;

    // Multicast port
    unsigned short multicast_port;
    static const char* multicast_port_name;

    // Multicast group id
    CORBA::ULongLong multicast_group_id;
    static const char* multicast_group_id_name;

    // Multicast group domain
    char* multicast_group_domain;
    static const char* multicast_group_domain_name;

    // Multicast group component version (Default: "1.0")
    TIDorb::core::comm::miop::Version multicast_group_component_version;
    static const TIDorb::core::comm::miop::Version DEFAULT_MIOP_GROUP_COMPONENT_VERSION;
    static const char* multicast_group_component_version_name;

    // Multicast group reference version (Default: 0)
    CORBA::ULong multicast_group_ref_version;
    static const CORBA::ULong DEFAULT_MIOP_GROUP_REF_VERSION;
    static const char* multicast_group_ref_version_name;

    // Multicast group IIOP URL
    char* multicast_group_iiop;
    static const char* multicast_group_iiop_name;

    // Multicast gateway IIOP URL
    char* multicast_group_gateway;
    static const char* multicast_group_gateway_name;
*/

//
// end MIOP extensions
//



//
// FT extensions
//

    /**
     * Fault tolerant mode.
     * Values must be "true" or "false".
     * Default "false".
     */

    bool fault_tolerant;
    static const char* fault_tolerant_name;
    static const bool DEFAULT_FAULT_TOLERANT;

    /**
     * Heartbeat interval, in milliseconds.
     * Default value: <code>50000</code>.
     * Values must be greater or equal than 0.
     */

    time_t heartbeat_interval;
    static const time_t DEFAULT_HEARTBEAT_INTERVAL;
    static const char* heartbeat_interval_name;

    /**
     * Heartbeat timeout, in milliseconds.
     * Default value: <code>5000</code>.
     * Values must be greater or equal than 0.
     */

    time_t heartbeat_timeout;
    static const time_t DEFAULT_HEARTBEAT_TIMEOUT;
    static const char* heartbeat_timeout_name;

    /**
     * Alternate IIOP addresses.
     * Multiple options are allowed.
     * Syntax is host:port.
     */

    vector<TIDorb::core::comm::iiop::ListenPoint> alternate_addresses;
    static const char* alternate_address_name;


//
// end FT extensions
//

    /**
     * QoS mode.
     * Values must be "true" or "false".
     * Default "false".
     */

    bool qos_enabled;
    static const char* qos_enabled_name;
    static const bool DEFAULT_QOS_ENABLED;


    /**
     * ZIOP chunk size: internal chunk for compressing and decompressing
     * Default value: 32768 (32Kb).
     * Values must be greater or equal than 0.
     */

    size_t ziop_chunk_size;
    static const char* ziop_chunk_size_name;
    static const size_t DEFAULT_ZIOP_CHUNK_SIZE;

    /**
     * ZIOP assume ziop enabled in server: when client connect to server 
     * througth corbaloc
     * Default value: false.
     * Values must be true or false.
     */

    bool assume_ziop_server;
    static const char* assume_ziop_server_name;
    static const bool DEFAULT_ASSUME_ZIOP_SERVER;

    /**
     * Private key file (RSA or DSA) in PEM format
     * Values must be a file name
     * Default value: null
     */
    
    char* ssl_private_key;
    static const char* ssl_private_key_name;
    static const char* DEFAULT_SSL_PRIVATE_KEY;

    /**
     * X509 certificate file (RSA or DSA) in PEM format created using a private key
     * (provided by ssl_private_key argument)
     * Values must be a file name
     * Default value: null
     */

    char* ssl_certificate;
    static const char* ssl_certificate_name;
    static const char* DEFAULT_SSL_CERTIFICATE;

    /**
     * X509 Certificate Autority (CA) file in PEM format (if was used) to generate
     * X509 certificate file  (provided by ssl_certificate argument)
     * Values must be a file name
     * Default value: null
     */

    char* ssl_ca;
    static const char* ssl_ca_name;
    static const char* DEFAULT_SSL_CA;

    /**
     * CSIv2 mode. Enables the usage of SAS context between client and server 
     * to provide authentication
     * Values must be "true" or "false".
     * Default "false".
     */

    bool csiv2;
    static const char* csiv2_name;
    static const bool DEFAULT_CSIv2;


    /**
     * SSL session timeout (in seconds). When timeout is reached a new SSL session
     * is established
     * Values must be integer.
     * Default 300.
     */

    unsigned int ssl_session_timeout;
    static const char* ssl_session_timeout_name;
    static const unsigned int DEFAULT_SSL_SESSION_TIMEOUT;


    /**
     * SSL ORB listen port to be used by SSLIOP layer.
     * Values must be greater or equal than 0.
     * Default value: <code>0</code> (assigned by system)
     */

    unsigned short ssl_port;
    static const char* ssl_port_name;
    static const unsigned short DEFAULT_SSL_PORT;


    /**
     * SSL version to be used by SSLIOP layer
     * SSLv2 is not recomended due to security flaws
     * Values must be 0 (SSLv2), 1 (SSLv3), 2 (SSLv23), 3 (TLSv1)
     * Default value: 2 
     */

    unsigned int ssl_version;
    static const char* ssl_version_name;
    static const unsigned int DEFAULT_SSL_VERSION;


    /**
     * Username to be used by GSSUP user/password authentication protocol
     * Needs the following arguments: -ORB_CSIv2 and -ORB_gssup_password
     * Values must be a string
     * Default value: null
     */

    char* gssup_user;
    static const char* gssup_user_name;
    static const char* DEFAULT_GSSUP_USER;


    /**
     * Passwrod to be used by GSSUP user/password authentication protocol
     * Needs the following arguments: -ORB_CSIv2 and -ORB_gssup_user
     * Values must be a string
     * Default value: null
     */

    char* gssup_password;
    static const char* gssup_password_name;
    static const char* DEFAULT_GSSUP_PASSWORD;


    /**
     * Target name to be used by some GSS protocols like GSSUP
     * Needs the following arguments: -ORB_CSIv2 and -ORB_gssup_user
     * Values must be a string
     * Default value: null
     */

    char* csiv2_target_name;
    static const char* csiv2_target_name_name;
    static const char* DEFAULT_CSIV2_TARGET_NAME;



  };

} // namespace core
} // namespace TIDorb

#endif
