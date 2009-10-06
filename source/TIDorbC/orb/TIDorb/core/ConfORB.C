/////////////////////////////////////////////////////////////////////////
//
// File:        ConfORB.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
//
// Revised:    
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/ConfORB.h"
#include "TIDSocket.h"

#include <strings.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>

#ifdef TIDORB_HAVE_FSTREAM
#include <fstream>
#else
#include <fstream.h>
#endif


extern int errno;

TIDorb::core::PolicyContext* TIDorb::core::ConfORB::st_default_policy_context = 
  TIDorb::core::ConfORB::createDefaultPolicies();

TIDorb::core::PolicyContext* TIDorb::core::ConfORB::createDefaultPolicies()
{
  TIDorb::core::PolicyContext* context = new TIDorb::core::PolicyContext(NULL);
  
  try {
    context->setPolicy(new TIDorb::core::messaging::RelativeRoundtripTimeoutPolicyImpl(
                               TIDorb::core::util::Time::toTimeT(DEFAULT_MAX_BLOCKED_TIME)));
    context->setPolicy(new TIDorb::core::BidirectionalPolicyImpl(DEFAULT_BIDIRECTIONAL));
  } catch (const CORBA::PolicyError& pe) {
    throw CORBA::INTERNAL("cannot create default policies");
  }

  return context;
}


TIDorb::core::ConfORB::ConfORB()
:
  policy_context(st_default_policy_context),
  orb_id(CORBA::string_dup(DEFAULT_ORB_ID)),
  default_initial_reference(NULL),
  naming_service(NULL),
  block_size(DEFAULT_BLOCK_SIZE),
  exhaustive_equal(DEFAULT_EXHAUSTIVE_EQUAL),
  reliable_oneway(DEFAULT_RELIABLE_ONEWAY),
  max_blocked_time(DEFAULT_MAX_BLOCKED_TIME),
  fragment_size(DEFAULT_FRAGMENT_SIZE),
  GIOPVersion(DEFAULT_GIOP_VERSION),
  bidirectional(DEFAULT_BIDIRECTIONAL),
  max_connections(DEFAULT_MAX_CONNECTIONS),
  max_comm_recovering_times(DEFAULT_MAX_COMM_RECOVERING_TIMES),
  comm_recover_time(DEFAULT_COMM_RECOVER_TIME),
  max_uncompleted_messages(DEFAULT_MAX_UNCOMPLETED_MESSAGES),
  host(NULL),
  ip_address(NULL),
  orb_port(DEFAULT_ORB_PORT),
  socket_linger(DEFAULT_SOCKET_LINGER),
  socket_timeout(DEFAULT_SOCKET_TIMEOUT),
  socket_connect_timeout(DEFAULT_SOCKET_CONNECT_TIMEOUT),
  tcp_nodelay(DEFAULT_TCP_NODELAY),
  tcp_buffer_size(DEFAULT_TCP_BUFFER_SIZE),
  server_socket_backlog(DEFAULT_SERVER_SOCKET_BACKLOG),
  server_socket_reconnect(DEFAULT_SERVER_SOCKET_RECONNECT),
  client_socket_reconnect(DEFAULT_CLIENT_SOCKET_RECONNECT),
  poa_max_threads(DEFAULT_POA_MAX_THREADS),
  poa_min_threads(DEFAULT_POA_MIN_THREADS),
  poa_max_queued_requests(DEFAULT_POA_MAX_QUEUED_REQUESTS),
  poa_starving_time(DEFAULT_POA_STARVING_TIME),
  poa_thread_stack_size(DEFAULT_POA_THREAD_STACK_SIZE),
  poa_bound_threads(DEFAULT_POA_BOUND_THREADS),
  max_time_in_shutdown(DEFAULT_MAX_BLOCKED_TIME),
  typecode_cache_size(DEFAULT_TYPECODE_CACHE_SIZE),
  trace_level(DEFAULT_TRACE_LEVEL),
  trace_file(CORBA::string_dup(DEFAULT_TRACE_FILE)),
  trace_file_size(DEFAULT_FILE_SIZE),
  trace_num_files(DEFAULT_NUM_FILES),

  // MIOP extensions
  socket_time_to_live(DEFAULT_SOCKET_TIME_TO_LIVE),
  udp_buffer_size(DEFAULT_UDP_BUFFER_SIZE),
  udp_fragment_size(DEFAULT_UDP_FRAGMENT_SIZE),
  udp_send_delay(DEFAULT_UDP_SEND_DELAY),
  multicast_outgoing_interface(NULL),
  multicast_incoming_interface(NULL),
  //multicast_address(NULL),
  //multicast_port(0),
  //multicast_group_id(0),
  //multicast_group_domain(NULL),
  //multicast_group_component_version(DEFAULT_MIOP_GROUP_COMPONENT_VERSION),
  //multicast_group_ref_version(DEFAULT_MIOP_GROUP_REF_VERSION),
  //multicast_group_iiop(NULL),
  //multicast_group_gateway(NULL),
  // end MIOP extensions

  // FT extensions
  fault_tolerant(DEFAULT_FAULT_TOLERANT),
  heartbeat_interval(DEFAULT_HEARTBEAT_INTERVAL),
  heartbeat_timeout(DEFAULT_HEARTBEAT_TIMEOUT),
  // end FT extensions

  qos_enabled(DEFAULT_QOS_ENABLED),
  ziop_chunk_size(DEFAULT_ZIOP_CHUNK_SIZE),
  assume_ziop_server(DEFAULT_ASSUME_ZIOP_SERVER)

{}



TIDorb::core::ConfORB::~ConfORB()
{
  CORBA::string_free(orb_id);
  CORBA::string_free(default_initial_reference);
  CORBA::string_free(naming_service);
  CORBA::string_free(host);
  CORBA::string_free(ip_address);
  CORBA::string_free(trace_file);

  // MIOP extensions
  CORBA::string_free(multicast_outgoing_interface);
  CORBA::string_free(multicast_incoming_interface);
  //CORBA::string_free(multicast_address);
  //CORBA::string_free(multicast_group_domain);
  //CORBA::string_free(multicast_group_iiop);
  //CORBA::string_free(multicast_group_gateway);
  // end MIOP extensions

}
  


const TIDorb::core::iop::ORBComponent* TIDorb::core::ConfORB::ORB_TYPE =
  new TIDorb::core::iop::ORBComponent(1414087680);

const char* TIDorb::core::ConfORB::orb_id_name = "-ORBid";
const char* TIDorb::core::ConfORB::DEFAULT_ORB_ID = "";

const char* TIDorb::core::ConfORB::init_refs_name = "-ORBInitRef";

const char* TIDorb::core::ConfORB::def_init_ref_name = "-ORBDefaultInitRef";

const char* TIDorb::core::ConfORB::naming_service_name = "-ORB_naming_service"; // Obsolete??

const size_t TIDorb::core::ConfORB::DEFAULT_BLOCK_SIZE = 256;
const char* TIDorb::core::ConfORB::block_size_name = "-ORB_block_size";

const bool TIDorb::core::ConfORB::DEFAULT_EXHAUSTIVE_EQUAL = false;
const char* TIDorb::core::ConfORB::exhaustive_equal_name = "-ORB_exhaustive_equal";

const bool TIDorb::core::ConfORB::DEFAULT_RELIABLE_ONEWAY = false;
const char* TIDorb::core::ConfORB::reliable_oneway_name = "-ORB_reliable_oneway";

const time_t TIDorb::core::ConfORB::DEFAULT_MAX_BLOCKED_TIME = 5000;
const char* TIDorb::core::ConfORB::max_blocked_time_name = "-ORB_max_blocked_time";

const size_t TIDorb::core::ConfORB::DEFAULT_FRAGMENT_SIZE = 4096 - 32 - 192;
const char* TIDorb::core::ConfORB::fragment_size_name = "-ORB_iiop_fragment_size";

const TIDorb::core::comm::iiop::Version TIDorb::core::ConfORB::DEFAULT_GIOP_VERSION(1,2);
const char* TIDorb::core::ConfORB::GIOPVersion_name = "-ORB_iiop_GIOPVersion";

const BiDirPolicy::BidirectionalPolicyValue TIDorb::core::ConfORB::DEFAULT_BIDIRECTIONAL = BiDirPolicy::NORMAL;
const char* TIDorb::core::ConfORB::bidirectional_name = "-ORB_iiop_bidirectional";

const unsigned int TIDorb::core::ConfORB::DEFAULT_MAX_CONNECTIONS = 30;
const char* TIDorb::core::ConfORB::max_connections_name = "-ORB_iiop_max_connections";

const unsigned int TIDorb::core::ConfORB::DEFAULT_MAX_COMM_RECOVERING_TIMES = 3;
const char* TIDorb::core::ConfORB::max_comm_recovering_times_name = "-ORB_iiop_max_comm_recovering_times";

const time_t TIDorb::core::ConfORB::DEFAULT_COMM_RECOVER_TIME = 1000;
const char*  TIDorb::core::ConfORB::comm_recover_time_name = "-ORB_iiop_comm_recover_time";

const unsigned int TIDorb::core::ConfORB::DEFAULT_MAX_UNCOMPLETED_MESSAGES = 25;
const char* TIDorb::core::ConfORB::max_uncompleted_messages_name ="-ORB_iiop_max_uncompleted_messages";

const char* TIDorb::core::ConfORB::DEFAULT_HOST_NAME = "";
const char* TIDorb::core::ConfORB::host_name = "-ORB_iiop_host";

const char* TIDorb::core::ConfORB::ip_address_name = "-ORB_ip_address";

const unsigned short TIDorb::core::ConfORB::DEFAULT_ORB_PORT = 0;
const char* TIDorb::core::ConfORB::orb_port_name = "-ORB_iiop_orb_port";

const int TIDorb::core::ConfORB::DEFAULT_SOCKET_LINGER = -1;
const char* TIDorb::core::ConfORB::socket_linger_name = "-ORB_iiop_socket_linger";

const time_t TIDorb::core::ConfORB::DEFAULT_SOCKET_TIMEOUT = 0;
const char* TIDorb::core::ConfORB::socket_timeout_name = "-ORB_iiop_socket_timeout";

const time_t TIDorb::core::ConfORB::DEFAULT_SOCKET_CONNECT_TIMEOUT = 1000;
const char* TIDorb::core::ConfORB::socket_connect_timeout_name = "-ORB_iiop_socket_connect_timeout";

const bool TIDorb::core::ConfORB::DEFAULT_TCP_NODELAY = true;
const char* TIDorb::core::ConfORB::tcp_nodelay_name = "-ORB_iiop_tcp_nodelay";

const size_t TIDorb::core::ConfORB::DEFAULT_TCP_BUFFER_SIZE = 4000;
const char* TIDorb::core::ConfORB::tcp_buffer_size_name = "-ORB_iiop_tcp_buffer_size";

const unsigned int TIDorb::core::ConfORB::DEFAULT_SERVER_SOCKET_BACKLOG = 10;
const char* TIDorb::core::ConfORB::server_socket_backlog_name = "-ORB_iiop_server_socket_backlog";

const unsigned int TIDorb::core::ConfORB::DEFAULT_SERVER_SOCKET_RECONNECT = 0;
const char* TIDorb::core::ConfORB::server_socket_reconnect_name = "-ORB_iiop_server_socket_reconnect";

const unsigned int TIDorb::core::ConfORB::DEFAULT_CLIENT_SOCKET_RECONNECT = 0;
const char* TIDorb::core::ConfORB::client_socket_reconnect_name = "-ORB_iiop_client_socket_reconnect";

const unsigned int TIDorb::core::ConfORB::DEFAULT_POA_MAX_THREADS = 50;
const char* TIDorb::core::ConfORB::poa_max_threads_name = "-ORB_poa_max_threads";

const unsigned int TIDorb::core::ConfORB::DEFAULT_POA_MIN_THREADS = 10;
const char* TIDorb::core::ConfORB::poa_min_threads_name = "-ORB_poa_min_threads";

const unsigned int TIDorb::core::ConfORB::DEFAULT_POA_MAX_QUEUED_REQUESTS = 1000;
const char* TIDorb::core::ConfORB::poa_max_queued_requests_name = "-ORB_poa_max_queued_requests";

const time_t TIDorb::core::ConfORB::DEFAULT_POA_STARVING_TIME = 0;
const char* TIDorb::core::ConfORB::poa_starving_time_name = "-ORB_poa_starving_time";

const size_t TIDorb::core::ConfORB::DEFAULT_POA_THREAD_STACK_SIZE = 50000;
const char* TIDorb::core::ConfORB::poa_thread_stack_size_name = "-ORB_poa_thread_stack_size";

const bool TIDorb::core::ConfORB::DEFAULT_POA_BOUND_THREADS = false;
const char* TIDorb::core::ConfORB::poa_bound_threads_name = "-ORB_poa_bound_threads";

const time_t TIDorb::core::ConfORB::DEFAULT_MAX_TIME_IN_SHUTDOWN = 5000;
const char* TIDorb::core::ConfORB::max_time_in_shutdown_name = "-ORB_max_time_in_shutdown";

const size_t TIDorb::core::ConfORB::DEFAULT_TYPECODE_CACHE_SIZE = 0;
const char* TIDorb::core::ConfORB::typecode_cache_size_name = "-ORB_typecode_cache_size";

const unsigned int TIDorb::core::ConfORB::DEFAULT_TRACE_LEVEL = 0;
const char* TIDorb::core::ConfORB::trace_level_name = "-ORB_trace_level";

const char* TIDorb::core::ConfORB::DEFAULT_TRACE_FILE = "trace.log";
const char* TIDorb::core::ConfORB::trace_file_name = "-ORB_trace_file";

const size_t TIDorb::core::ConfORB::DEFAULT_FILE_SIZE = 10240;
const char* TIDorb::core::ConfORB::trace_file_size_name = "-ORB_trace_file_size";

const unsigned int TIDorb::core::ConfORB::DEFAULT_NUM_FILES = 1;
const char* TIDorb::core::ConfORB::trace_num_files_name = "-ORB_trace_num_files";

const char* TIDorb::core::ConfORB::DEFAULT_TRACE_NAME = "TIDorb";

// pra@tid.es - MIOP extensions
const int TIDorb::core::ConfORB::DEFAULT_SOCKET_TIME_TO_LIVE = 1;
const char* TIDorb::core::ConfORB::socket_time_to_live_name = "-ORB_miop_socket_time_to_live";

const size_t TIDorb::core::ConfORB::DEFAULT_UDP_BUFFER_SIZE = 131072;
const char* TIDorb::core::ConfORB::udp_buffer_size_name = "-ORB_miop_udp_buffer_size";

const size_t TIDorb::core::ConfORB::DEFAULT_UDP_FRAGMENT_SIZE = 32768 - 32 - 8;
const char* TIDorb::core::ConfORB::udp_fragment_size_name = "-ORB_miop_fragment_size";

const time_t TIDorb::core::ConfORB::DEFAULT_UDP_SEND_DELAY = 1;
const char* TIDorb::core::ConfORB::udp_send_delay_name = "-ORB_miop_send_delay";

const char* TIDorb::core::ConfORB::multicast_outgoing_interface_name = "-ORB_miop_multicast_outgoing_interface";

const char* TIDorb::core::ConfORB::multicast_incoming_interface_name = "-ORB_miop_multicast_incoming_interface";
/*
const char* TIDorb::core::ConfORB::multicast_address_name = "-ORB_miop_multicast_address";

const char* TIDorb::core::ConfORB::multicast_port_name = "-ORB_miop_multicast_port";

const char* TIDorb::core::ConfORB::multicast_group_id_name = "-ORB_miop_multicast_group_id";

const char* TIDorb::core::ConfORB::multicast_group_domain_name = "-ORB_miop_multicast_group_domain";

const TIDorb::core::comm::miop::Version TIDorb::core::ConfORB::DEFAULT_MIOP_GROUP_COMPONENT_VERSION(1,0);
const char* TIDorb::core::ConfORB::multicast_group_component_version_name = "-ORB_miop_multicast_group_component_version";

const CORBA::ULong TIDorb::core::ConfORB::DEFAULT_MIOP_GROUP_REF_VERSION = 0;
const char* TIDorb::core::ConfORB::multicast_group_ref_version_name = "-ORB_miop_multicast_group_ref_version";

const char* TIDorb::core::ConfORB::multicast_group_iiop_name = "-ORB_miop_multicast_group_iiop";

const char* TIDorb::core::ConfORB::multicast_group_gateway_name = "-ORB_miop_multicast_group_gateway";
*/
// end MIOP extensions

// pra@tid.es - FT extensions
const bool TIDorb::core::ConfORB::DEFAULT_FAULT_TOLERANT = false;
const char* TIDorb::core::ConfORB::fault_tolerant_name = "-ORB_fault_tolerant";

const time_t TIDorb::core::ConfORB::DEFAULT_HEARTBEAT_INTERVAL = 50000;
const char* TIDorb::core::ConfORB::heartbeat_interval_name = "-ORB_ft_heartbeat_interval";

const time_t TIDorb::core::ConfORB::DEFAULT_HEARTBEAT_TIMEOUT = 5000;
const char* TIDorb::core::ConfORB::heartbeat_timeout_name = "-ORB_ft_heartbeat_timeout";

const char* TIDorb::core::ConfORB::alternate_address_name = "-ORB_alternate_address";
// end FT extensions

const bool TIDorb::core::ConfORB::DEFAULT_QOS_ENABLED = true;
const char* TIDorb::core::ConfORB::qos_enabled_name = "-ORB_qos_enabled";

const size_t TIDorb::core::ConfORB::DEFAULT_ZIOP_CHUNK_SIZE = 32768;
const char* TIDorb::core::ConfORB::ziop_chunk_size_name = "-ORB_ziop_chunk_size";

const bool TIDorb::core::ConfORB::DEFAULT_ASSUME_ZIOP_SERVER = false;
const char* TIDorb::core::ConfORB::assume_ziop_server_name = "-ORB_assume_ziop_server";

//PRA
int TIDorb::core::ConfORB::modify_parameters(int& argc, char** argv, int currpos, int numparams)
{
  for (int i = currpos + numparams; i < argc; i++) {
     argv[i - numparams] = argv[i];
  }
  argc -= numparams;
  return (currpos - 1);
}
//EPRA



void TIDorb::core::ConfORB::get_parameters(int& argc, char** argv, const char* orb_identifier)
  throw (CORBA::SystemException)
{

  // some auxiliary variables to parse -ORB options
  char*         ptr;
  long          laux;
  unsigned long ulaux;

  // Get orb_id from ORB::init()
  if (strcmp(orb_identifier,"") != 0) {
    CORBA::string_free(orb_id);
    orb_id = CORBA::string_dup(orb_identifier);
  }

  for (int i = 1; i < argc-1; i++)
  {
    errno = 0;

    if (strcmp(argv[i], orb_id_name)==0) {
      CORBA::string_free(orb_id);
      orb_id = CORBA::string_dup(argv[i+1]);
    }

    else if (strcmp(argv[i], init_refs_name)==0) {
      char* obj_url  = NULL;
      char* obj_name = strtok_r(argv[i+1], "=", &obj_url);
      if (obj_name == NULL || obj_url == NULL || obj_url[0] == '\0')
        throw CORBA::INITIALIZE("Initial ref is not correct.");

      TIDorb::core::util::InitialReference ref(obj_name, obj_url);
      initial_references.insert(initial_references.begin(), ref);
    }

    else if (strcmp(argv[i], def_init_ref_name)==0) {
      default_initial_reference = CORBA::string_dup(argv[i+1]);
    }

    else if (strcmp(argv[i], naming_service_name)==0) {
      ifstream file(argv[i+1]);
      if (!file)
        throw CORBA::INITIALIZE("Name server file ref is not correct.");

      TIDorb::util::StringBuffer buffer;
      buffer << file.rdbuf();
      naming_service = CORBA::string_dup(buffer.str().data());
      file.close();
    }

    else if (strcmp(argv[i], block_size_name)==0) {
      ulaux = strtoul(argv[i+1], &ptr, 10);
      if (errno || *ptr || ulaux > UINT_MAX
          || (ulaux < 256 || ulaux%8 != 0)) {
        throw CORBA::INITIALIZE("block_size value must be >= 256 and multiple of 8.");
      }
      else
        block_size = ulaux;
    }

    else if (strcmp(argv[i], exhaustive_equal_name)==0) {
      if (strcasecmp(argv[i+1], "true")==0)
        exhaustive_equal = true;
      else if (strcasecmp(argv[i+1], "false")==0)
        exhaustive_equal = false;
      else {
        throw CORBA::INITIALIZE("exhaustive_equal value must be true or false.");
      }
    }

    else if (strcmp(argv[i], reliable_oneway_name)==0) {
      if (strcasecmp(argv[i+1], "true")==0)
        reliable_oneway = true;
      else if (strcasecmp(argv[i+1], "false")==0)
        reliable_oneway = false;
      else {
        throw CORBA::INITIALIZE("reliable_oneway value must be true or false.");
      }
    }

    else if (strcmp(argv[i], max_blocked_time_name)==0) {
      ulaux = strtoul(argv[i+1], &ptr, 10);
      if (errno || *ptr || ulaux > UINT_MAX) {
        throw CORBA::INITIALIZE("max_blocked_time value must be greater or equal than 0.");
      }
      else
        max_blocked_time = ulaux;
    }

    else if (strcmp(argv[i], fragment_size_name)==0){
      ulaux = strtoul(argv[i+1], &ptr, 10);
      if (errno || *ptr || ulaux > UINT_MAX
          || (ulaux < 1024 || ulaux%8 != 0)) {
        throw CORBA::INITIALIZE("fragment_size value must be >= 1024 and multiple of 8.");
      }
      else
        fragment_size = ulaux - 32 - 192; 
    }

    else if (strcmp(argv[i], GIOPVersion_name)==0){
      if (strcasecmp(argv[i+1], "1.0")==0) {
        GIOPVersion._major = 1;
        GIOPVersion._minor = 0;
      } else if (strcasecmp(argv[i+1], "1.1")==0) {
        GIOPVersion._major = 1;
        GIOPVersion._minor = 1;
      } else if (strcasecmp(argv[i+1], "1.2")==0) {
        GIOPVersion._major = 1;
        GIOPVersion._minor = 2;
      } else {
        throw CORBA::INITIALIZE("GIOPVersion must be 1.0, 1.1 or 1.2");
      }
    }

    else if (strcmp(argv[i], bidirectional_name)==0) {
      if (strcasecmp(argv[i+1], "NORMAL")==0)
        bidirectional = BiDirPolicy::NORMAL;
      else if (strcasecmp(argv[i+1], "BOTH")==0)
        bidirectional = BiDirPolicy::BOTH;
      else {
        throw CORBA::INITIALIZE("bidirectional values allowed are NORMAL or BOTH.");
      }
    }

    else if (strcmp(argv[i], max_connections_name)==0) {
      ulaux = strtoul(argv[i+1], &ptr, 10);
      if (errno || *ptr || ulaux > UINT_MAX) {
        throw CORBA::INITIALIZE("max_connections value must be greater or equal than 0.");
      }
      else
        max_connections = ulaux;
    }

    else if (strcmp(argv[i], max_comm_recovering_times_name)==0) {
      ulaux = strtoul(argv[i+1], &ptr, 10);
      if (errno || *ptr || ulaux > UINT_MAX) {
        throw CORBA::INITIALIZE("max_comm_recovering_times value must be greater or equal than 0.");
      }
      else
        max_comm_recovering_times = ulaux;
    }

    else if (strcmp(argv[i], comm_recover_time_name)==0) {
      ulaux = strtoul(argv[i+1], &ptr, 10);
      if (errno || *ptr || ulaux > UINT_MAX) {
        throw CORBA::INITIALIZE("comm_recover_time value must be greater or equal than 0.");
      }
      else
        comm_recover_time = ulaux;
    }

    else if (strcmp(argv[i], max_uncompleted_messages_name)==0) {
      ulaux = strtoul(argv[i+1], &ptr, 10);
      if (errno || *ptr || ulaux > UINT_MAX) {
        throw CORBA::INITIALIZE("max_uncompleted_messages value must be greater or equal than 0.");
      }
      else
        max_uncompleted_messages = ulaux;
    }

    else if (strcmp(argv[i], host_name)==0) {
      host = CORBA::string_dup(argv[i+1]);
    }

    else if (strcmp(argv[i], ip_address_name)==0) {
      ip_address = CORBA::string_dup(argv[i+1]);
    }

    else if (strcmp(argv[i], orb_port_name)==0) {
      ulaux = strtoul(argv[i+1], &ptr, 10);
      if (errno || *ptr || ulaux > USHRT_MAX) {
        throw CORBA::INITIALIZE("orb_port value must be greater or equal than 0.");
      }
      else
        orb_port = ulaux;
    }

    else if (strcmp(argv[i], socket_linger_name)==0) {
      laux = strtol(argv[i+1], &ptr, 10);
      if (errno || *ptr || laux > INT_MAX || laux < -1) {
        throw CORBA::INITIALIZE("socket_linger value must be greater or equal than -1.");
      }
      else
        socket_linger = laux;
    }

    else if (strcmp(argv[i], socket_timeout_name)==0) {
      ulaux = strtoul(argv[i+1], &ptr, 10);
      if (errno || *ptr || ulaux > UINT_MAX) {
        throw CORBA::INITIALIZE("socket_timeout value must be greater or equal than 0.");
      }
      else
        socket_timeout = ulaux;
    }

    else if (strcmp(argv[i], socket_connect_timeout_name)==0) {
      ulaux = strtoul(argv[i+1], &ptr, 10);
      if (errno || *ptr || ulaux > UINT_MAX) {
        throw CORBA::INITIALIZE("socket_connection_timeout value must be greater or equal than 0.");
      }
      else
        socket_connect_timeout = ulaux;
    }

    else if (strcmp(argv[i], tcp_nodelay_name)==0) {
      if (strcasecmp(argv[i+1], "true")==0)
        tcp_nodelay = true;
      else if (strcasecmp(argv[i+1], "false")==0)
        tcp_nodelay = false;
      else {
        throw CORBA::INITIALIZE("tcp_no_delay value must be true or false.");
      }
    }

    else if (strcmp(argv[i], tcp_buffer_size_name)==0) {
      ulaux = strtoul(argv[i+1], &ptr, 10);
      if (errno || *ptr || ulaux > UINT_MAX) {
        throw CORBA::INITIALIZE("tcp_buffer_size value must be greater or equal than 0.");
      }
      else
        tcp_buffer_size = ulaux;
    }

    else if (strcmp(argv[i], server_socket_backlog_name)==0) {
      ulaux = strtoul(argv[i+1], &ptr, 10);
      if (errno || *ptr || ulaux > UINT_MAX) {
        throw CORBA::INITIALIZE("server_socket_backlog value must be greater or equal than 0.");
      }
      else
        server_socket_backlog = ulaux;
    }

    else if (strcmp(argv[i], server_socket_reconnect_name)==0) {
      ulaux = strtoul(argv[i+1], &ptr, 10);
      if (errno || *ptr || ulaux > UINT_MAX) {
        throw CORBA::INITIALIZE("server_socket_reconnect value must be greater or equal than 0.");
      }
      else
        server_socket_reconnect = ulaux;
    }

    else if (strcmp(argv[i], client_socket_reconnect_name)==0) {
      ulaux = strtoul(argv[i+1], &ptr, 10);
      if (errno || *ptr || ulaux > UINT_MAX) {
        throw CORBA::INITIALIZE("client_socket_reconnect value must be greater or equal than 0.");
      }
      else
        client_socket_reconnect = ulaux;
    }

    else if (strcmp(argv[i], poa_max_threads_name)==0) {
      ulaux = strtoul(argv[i+1], &ptr, 10);
      if (errno || *ptr || ulaux > UINT_MAX || ulaux == 0) {
        throw CORBA::INITIALIZE("poa_max_threads value must be greater than 0.");
      }
      else
        poa_max_threads = ulaux;
    }

    else if (strcmp(argv[i], poa_min_threads_name)==0) {
      ulaux = strtoul(argv[i+1], &ptr, 10);
      if (errno || *ptr || ulaux > UINT_MAX || ulaux == 0) {
        throw CORBA::INITIALIZE("poa_min_threads value must be greater than 0.");
      }
      else
        poa_min_threads = ulaux;
    }

    else if (strcmp(argv[i], poa_max_queued_requests_name)==0) {
      ulaux = strtoul(argv[i+1], &ptr, 10);
      if (errno || *ptr || ulaux > UINT_MAX) {
        throw CORBA::INITIALIZE("poa_max_queued_requests value must be greater or equal than 0.");
      }
      else
        poa_max_queued_requests = ulaux;
    }

    else if (strcmp(argv[i], poa_starving_time_name)==0) {
      ulaux = strtoul(argv[i+1], &ptr, 10);
      if (errno || *ptr || ulaux > UINT_MAX) {
        throw CORBA::INITIALIZE("poa_starving_time value must be greater or equal than 0.");
      }
      else
        poa_starving_time = ulaux;
    }

    else if (strcmp(argv[i], poa_thread_stack_size_name)==0) {
      ulaux = strtoul(argv[i+1], &ptr, 10);
      if (errno || *ptr || ulaux > UINT_MAX) {
        throw CORBA::INITIALIZE("poa_thread_stack_size value must be greater or equal than 0.");
      }
      else
        poa_thread_stack_size = ulaux;
    }

    else if (strcmp(argv[i], poa_bound_threads_name)==0) {
      if (strcasecmp(argv[i+1], "true")==0)
        poa_bound_threads = true;
      else if (strcasecmp(argv[i+1], "false")==0)
        poa_bound_threads = false;
      else {
        throw CORBA::INITIALIZE("poa_bound_threads value must be true or false.");
      }
    }

    else if (strcmp(argv[i], typecode_cache_size_name)==0) {
      ulaux = strtoul(argv[i+1], &ptr, 10);
      if (errno || *ptr || ulaux > UINT_MAX) {
        throw CORBA::INITIALIZE("typecode_cache_size value must be greater or equal than 0.");
      }
      else
        typecode_cache_size = ulaux;
    }

    else if (strcmp(argv[i], max_time_in_shutdown_name)==0) {
      ulaux = strtoul(argv[i+1], &ptr, 10);
      if (errno || *ptr || ulaux > UINT_MAX) {
        throw CORBA::INITIALIZE("max_time_in_shutdown value must be greater or equal than 0.");
      }
      else
        max_time_in_shutdown = ulaux;
    }

    else if (strcmp(argv[i], trace_level_name)==0) {
      ulaux = strtoul(argv[i+1], &ptr, 10);
      if (errno || *ptr || ulaux > UINT_MAX) {
        throw CORBA::INITIALIZE("trace_level value must be greater or equal than 0.");
      }
      else
        trace_level = ulaux;
    }

    else if (strcmp(argv[i], trace_file_name)==0) {
      CORBA::string_free(trace_file);
      trace_file = CORBA::string_dup(argv[i+1]);
    }

    else if (strcmp(argv[i], trace_file_size_name)==0) {
      ulaux = strtoul(argv[i+1], &ptr, 10);
      if (errno || *ptr || ulaux > UINT_MAX) {
        throw CORBA::INITIALIZE("trace_file_size value must be greater or equal than 0.");
      }
      else
        trace_file_size = ulaux;
    }

    else if (strcmp(argv[i], trace_num_files_name)==0) {
      ulaux = strtoul(argv[i+1], &ptr, 10);
      if (errno || *ptr || ulaux > UINT_MAX) {
        throw CORBA::INITIALIZE("trace_num_files value must be greater or equal than 0.");
      }
      else
        trace_num_files = ulaux;
    }

    // MIOP extensions
    else if (strcmp(argv[i], socket_time_to_live_name)==0) {
      ulaux = strtoul(argv[i+1], &ptr, 10);
      if (errno || *ptr || ulaux > UINT_MAX) {
        throw CORBA::INITIALIZE("socket_time_to_live value must be greater or equal than 0.");
      }
      else
        socket_time_to_live = ulaux;
    }

    else if (strcmp(argv[i], udp_buffer_size_name)==0) {
      ulaux = strtoul(argv[i+1], &ptr, 10);
      if (errno || *ptr || ulaux > UINT_MAX) {
        throw CORBA::INITIALIZE("udp_buffer_size value must be greater or equal than 0.");
      }
      else
        udp_buffer_size = ulaux;
    }

    else if (strcmp(argv[i], udp_fragment_size_name)==0) {
      ulaux = strtoul(argv[i+1], &ptr, 10);
      if (errno || *ptr || ulaux > UINT_MAX
          || (ulaux%8 != 0) || ulaux > 65536) {
        throw CORBA::INITIALIZE("udp_fragment_size value must be multiple of 8 an minor than 65536 (IP packet limit.");
      }
      else
        udp_fragment_size = ulaux - 32 - 8; // decrement to IP and UDP heades, 32 and 8 respectively
    }

    else if (strcmp(argv[i], udp_send_delay_name)==0) {
      ulaux = strtoul(argv[i+1], &ptr, 10);
      if (errno || *ptr || ulaux > UINT_MAX) {
        throw CORBA::INITIALIZE("udp_send_delay value must be greater or equal than 0.");
      }
      else
        udp_send_delay = ulaux;
    }

    else if (strcmp(argv[i], multicast_outgoing_interface_name)==0) {
      multicast_outgoing_interface = CORBA::string_dup(argv[i+1]);
    }

    else if (strcmp(argv[i], multicast_incoming_interface_name)==0) {
      multicast_incoming_interface = CORBA::string_dup(argv[i+1]);
    }

    /*  
    else if (strcmp(argv[i], multicast_address_name)==0) {
      multicast_address = CORBA::string_dup(argv[i+1]);
    }

    else if (strcmp(argv[i], multicast_port_name)==0) {
      ulaux = strtoul(argv[i+1], &ptr, 10);
      if (errno || *ptr || ulaux > USHRT_MAX) {
        throw CORBA::INITIALIZE("multicast_port value must be in the interval 0..65535");
      }
      multicast_port = ulaux;
    }

    else if (strcmp(argv[i], multicast_group_id_name)==0) {
      ulaux = strtoul(argv[i+1], &ptr, 10);
      if (errno || *ptr || ulaux > ULONG_MAX) {
        throw CORBA::INITIALIZE("multicast_group_id must be valid positive integer.");
      }
      multicast_group_id = ulaux;
    }

    else if (strcmp(argv[i], multicast_group_domain_name)==0) {
      multicast_group_domain = CORBA::string_dup(argv[i+1]);
    }

    else if (strcmp(argv[i], multicast_group_component_version_name)==0) {
      unsigned long major, minor;
      if (sscanf(argv[i+1], "%lu.%lu", &major, &minor)!=2) {
        throw CORBA::INITIALIZE("invalid multicast_group_component_version.");
      }
      multicast_group_component_version._major = major;
      multicast_group_component_version._minor = minor;
    }

    else if (strcmp(argv[i], multicast_group_ref_version_name)==0) {
      ulaux = strtoul(argv[i+1], &ptr, 10);
      if (errno || *ptr || ulaux > UINT_MAX) {
        throw CORBA::INITIALIZE(" multicast_group_ref_version must be valid positive integer.");
      }
      multicast_group_ref_version = ulaux;
    }

    else if (strcmp(argv[i], multicast_group_iiop_name)==0) {
      multicast_group_iiop = CORBA::string_dup(argv[i+1]);
    }
    */
    // end MIOP extensions

    // FT extensions
    else if (strcmp(argv[i], fault_tolerant_name)==0) {
      if (strcasecmp(argv[i+1], "true")==0)
        fault_tolerant = true;
      else if (strcasecmp(argv[i+1], "false")==0)
        fault_tolerant = false;
      else
        throw CORBA::INITIALIZE("fault_tolerant value must be true or false.");
    }

    else if (strcmp(argv[i], heartbeat_interval_name)==0) {
      ulaux = strtoul(argv[i+1], &ptr, 10);
      if (errno || *ptr || ulaux > UINT_MAX) {
        throw CORBA::INITIALIZE("heartbeat_interval value must be greater or equal than 0.");
      }
      else
        heartbeat_interval = ulaux;
    }

    else if (strcmp(argv[i], heartbeat_timeout_name)==0) {
      ulaux = strtoul(argv[i+1], &ptr, 10);
      if (errno || *ptr || ulaux > UINT_MAX) {
        throw CORBA::INITIALIZE("heartbeat_timeout value must be greater or equal than 0.");
      }
      else
        heartbeat_timeout = ulaux;
    }

    else if (strcmp(argv[i], alternate_address_name)==0) {
      TIDorb::core::comm::iiop::ListenPoint listen_point;
      if (listen_point.fromString(argv[i+1]))
        alternate_addresses.push_back(listen_point);
      else
        throw CORBA::INITIALIZE("alternate_address must be a valid host:port pair.");
    }
    // end FT extensions

    else if (strcmp(argv[i], qos_enabled_name)==0) {
      if (strcasecmp(argv[i+1], "true")==0)
        qos_enabled = true;
      else if (strcasecmp(argv[i+1], "false")==0)
        qos_enabled = false;
      else
        throw CORBA::INITIALIZE("qos_enabled value must be true or false.");
    }

    else if (strcmp(argv[i], ziop_chunk_size_name)==0) {
      ulaux = strtoul(argv[i+1], &ptr, 10);
      if (errno || *ptr || ulaux > UINT_MAX) {
        throw CORBA::INITIALIZE("ziop_chunk_size value must be greater or equal than 0.");
      }
      else
        ziop_chunk_size = ulaux;
    }

    else if (strcmp(argv[i], assume_ziop_server_name)==0) {
      if (strcasecmp(argv[i+1], "true")==0)
        assume_ziop_server = true;
      else if (strcasecmp(argv[i+1], "false")==0)
        assume_ziop_server = false;
      else
        throw CORBA::INITIALIZE("assume_ziop_server value must be true or false.");
    }

    else if (strncmp("-ORB", argv[i], 4) == 0) { // Not a valid option
      TIDorb::util::StringBuffer msg;
      msg << "Option not found: " << argv[i];
      throw CORBA::INITIALIZE(msg.str().data());
    }

    else {
      // no processing needed
      continue;
    }

    // consume options from argv[]
    i = modify_parameters(argc, argv, i, 2);

  } // for


  // FT extensions
  if (fault_tolerant) {
    try {
      policy_context.setPolicy(new HeartbeatEnabledPolicyImpl(true));
      policy_context.setPolicy(new HeartbeatPolicyImpl(heartbeat_interval, heartbeat_timeout));
    } catch (CORBA::PolicyError& pe) {
    }
  }
  // end FT extensions


  if (bidirectional == BiDirPolicy::BOTH) {
    try {
      TIDorb::core::BidirectionalPolicyImpl* policy;
      policy = new TIDorb::core::BidirectionalPolicyImpl(bidirectional);
      policy_context.setPolicy(policy);
    }
    catch (CORBA::PolicyError& pe) {}
  }


  // QoS extensions
  if (max_blocked_time != DEFAULT_MAX_BLOCKED_TIME) {
    try {
      TIDorb::core::messaging::RelativeRoundtripTimeoutPolicyImpl* policy;
      policy = new TIDorb::core::messaging::RelativeRoundtripTimeoutPolicyImpl(
                                  TIDorb::core::util::Time::toTimeT(max_blocked_time));
      policy_context.setPolicy(policy);
    }
    catch (CORBA::PolicyError& pe) {}
  }



}




void TIDorb::core::ConfORB::dump(ostream& os)
{
  os.setf(ios_base::boolalpha);
  os << endl
     << "\tORBId=" << orb_id << endl
     << "\tdefault_initial_reference=" << ((default_initial_reference) ? default_initial_reference : "") << endl
     << "\tnaming_service=" << ((naming_service) ? naming_service : "") << endl
     << "\tblock_size=" << block_size << endl
     << "\texhaustive_equal=" << exhaustive_equal <<  endl
     << "\treliable_oneway=" << reliable_oneway << endl
     << "\tmax_blocked_time=" << max_blocked_time << endl
     << "\tfragment_size=" << fragment_size << endl
     << "\tGIOPVersion=" << GIOPVersion.toString() << endl
     << "\tbidirectional=" << ((bidirectional == BiDirPolicy::NORMAL) ? "NORMAL":"BOTH") << endl
     << "\tmax_connections=" << max_connections << endl
     << "\tmax_comm_recovering_times=" << max_comm_recovering_times << endl
     << "\tcomm_recover_time=" << comm_recover_time << endl
     << "\tmax_uncompleted_messages=" << max_uncompleted_messages << endl
     << "\thost=" << ((host) ? host : "") << endl
     << "\tip_address=" << ((ip_address) ? ip_address : "") << endl
     << "\torb_port=" << orb_port << endl
     << "\tsocket_linger=" << socket_linger << endl
     << "\tsocket_timeout=" << socket_timeout << endl
     << "\tsocket_connect_timeout=" << socket_connect_timeout << endl
     << "\tsocket_time_to_live=" << socket_time_to_live << endl
     << "\ttcp_nodelay=" << tcp_nodelay << endl
     << "\ttcp_buffer_size=" << tcp_buffer_size << endl
     << "\tudp_buffer_size=" << udp_buffer_size << endl
     << "\tudp_fragment_size=" << udp_fragment_size << endl
     << "\tudp_send_delay=" << udp_send_delay << endl
     << "\tserver_socket_backlog=" << server_socket_backlog << endl
     << "\tserver_socket_reconnect=" << server_socket_reconnect << endl
     << "\tclient_socket_reconnect=" << client_socket_reconnect << endl
     << "\tpoa_max_threads=" << poa_max_threads << endl
     << "\tpoa_min_threads=" << poa_min_threads << endl
     << "\tpoa_max_queued_requests=" << poa_max_queued_requests << endl
     << "\tpoa_starving_time=" << poa_starving_time << endl
     << "\tpoa_thread_stack_size=" << poa_thread_stack_size << endl
     << "\tpoa_bound_threads=" << poa_bound_threads << endl
     << "\tmax_time_in_shutdown=" << max_time_in_shutdown << endl
     << "\ttypecode_cache_size=" << typecode_cache_size << endl
     << "\tmulticast_outgoing_interface=" << ((multicast_outgoing_interface) ? multicast_outgoing_interface : "") << endl
     << "\tmulticast_incoming_interface=" << ((multicast_incoming_interface) ? multicast_incoming_interface : "") << endl
     << "\tfault_tolerant=" << fault_tolerant << endl
     << "\theartbeat_interval=" << heartbeat_interval << endl
     << "\theartbeat_timeout=" << heartbeat_timeout << endl
     << "\tqos_enabled=" << qos_enabled << endl
     << "\tziop_chunk_size=" << ziop_chunk_size << endl
     << "\tassume_ziop_server=" << assume_ziop_server << endl
     << "\ttrace_level=" << trace_level << endl
     << "\ttrace_file=" << trace_file << endl
     << "\ttrace_file_size=" << trace_file_size << endl
     << "\ttrace_num_files=" << trace_num_files << endl;
  
  os << "\tDefault Policies: " << endl;
  st_default_policy_context->dump(os);
  os << "\tUser Policies: " << endl;
  policy_context.dump(os);
  os << endl;

}
