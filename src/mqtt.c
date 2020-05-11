#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"paho-mqtt3a.lib")
#pragma comment(lib,"paho-mqtt3as.lib")
#pragma comment(lib,"paho-mqtt3c.lib")
#pragma comment(lib,"paho-mqtt3cs.lib")
#pragma comment(lib,"q.lib")
#define EXP __declspec(dllexport)
static SOCKET spair[2];
#else
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#define EXP
#define SOCKET_ERROR -1
static int spair[2];
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "socketpair.c"
#include "k.h"

#include <MQTTClient.h>

#ifdef __GNUC__
#define UNUSED(x) x __attribute__((__unused__))
#else
#define UNUSED(x) x
#endif

static MQTTClient client;
static int validinit;

// Function prototypes needed for q callable functions
static void msgsent(void *context, MQTTClient_deliveryToken dt);
static int msgrcvd(void *context, char* topic, int unused, MQTTClient_message *msg);
static void disconn(void *context, char* cause);

static char* getStringFromList(K propValues,int row, const char** value, char* errStr)
{
  if ((int)(kK(propValues)[row]->t) == -KS)
  {
    *value = strdup(kK(propValues)[row]->s);
    return 0;
  }
  return errStr;
}

static char* getIntFromList(K propValues,int row, int* value, char* errStr)
{
  if ((int)(kK(propValues)[row]->t) == -KI)
    *value = kK(propValues)[row]->i;
  else if ((int)(kK(propValues)[row]->t) == -KJ)
    *value = (int)(kK(propValues)[row]->j);
  else 
    return errStr;
  return 0;
}

/* Establish a tcp connection from a q process to mqtt client
 * tcpconn = tcp connection being connected to (symbol)
 * pname   = name to be associated with the connecting process (symbol)
 * opt     = connection options (dict - keys 'username' and 'password' currently supported as symbol types)
*/
EXP K connX(K tcpconn,K pname, K opt){
  int err;
  if(tcpconn->t != -KS)
    return krr("addr type");
  if(pname->t != -KS)
    return krr("client type");
  if(opt->t != XD)
    return krr("options");
  client = 0;

  MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

  K propNames = (kK(opt)[0]);
  K propValues = (kK(opt)[1]);
  if(propNames->n > 0)
  {
    if(propNames->t != KS)
      return krr("options key type incorrect");
    if(propValues->t != 0)
      return krr("options value type incorrect");
  }
  int row = 0;
  char* errStr = 0;
  for (;(row<propNames->n && errStr==0);++row)
  {
    if ((kS(propNames)[row])[0] == '\0')
      continue;

    if (strcmp(kS(propNames)[row],"username")==0)
      errStr = getStringFromList(propValues,row,&conn_opts.username,"username type incorrect");
    else if (strcmp(kS(propNames)[row],"password")==0)
      errStr = getStringFromList(propValues,row,&conn_opts.password,"password type incorrect");
    else if (strcmp(kS(propNames)[row],"keepAliveInterval")==0)
      errStr = getIntFromList(propValues,row,&conn_opts.keepAliveInterval,"keepAliveInterval type incorrect");
    else if (strcmp(kS(propNames)[row],"cleansession")==0)
      errStr = getIntFromList(propValues,row,&conn_opts.cleansession,"cleansession type incorrect"); 
    else if (strcmp(kS(propNames)[row],"reliable")==0)
      errStr = getIntFromList(propValues,row,&conn_opts.reliable,"reliable type incorrect");
    else if (strcmp(kS(propNames)[row],"connectTimeout")==0)
      errStr = getIntFromList(propValues,row,&conn_opts.connectTimeout,"connectTimeout type incorrect");
    else if (strcmp(kS(propNames)[row],"retryInterval")==0)
      errStr = getIntFromList(propValues,row,&conn_opts.retryInterval,"retryInterval type incorrect");
    else if (strcmp(kS(propNames)[row],"MQTTVersion")==0)
      errStr = getIntFromList(propValues,row,&conn_opts.MQTTVersion,"MQTTVersion type incorrect");
    else if (strcmp(kS(propNames)[row],"maxInflightMessages")==0)
      errStr = getIntFromList(propValues,row,&conn_opts.maxInflightMessages,"maxInflightMessages type incorrect");
    else if (strcmp(kS(propNames)[row],"cleanstart")==0)
      errStr = getIntFromList(propValues,row,&conn_opts.cleanstart,"cleanstart type incorrect");
    else
      errStr = "Unsupported conn opt name in dictionary";
  }

  if (errStr != 0)
  {
    free((void*)conn_opts.username);
    free((void*)conn_opts.password);
    return krr(errStr);
  }

  if(MQTTCLIENT_SUCCESS != (err = MQTTClient_create(&client, tcpconn->s, pname->s, MQTTCLIENT_PERSISTENCE_NONE, NULL)))
  {
    free((void*)conn_opts.username);
    free((void*)conn_opts.password);
    return krr((S)MQTTClient_strerror(err));
  }

  MQTTClient_setCallbacks(client, NULL, disconn, msgrcvd, msgsent);
  err = MQTTClient_connect(client, &conn_opts);
  free((void*)conn_opts.username);
  free((void*)conn_opts.password);

  if(MQTTCLIENT_SUCCESS != err)
    return krr((S)MQTTClient_strerror(err));
  return (K)0;
}

/* Publish a message to a specified topic
 * topic = topic name as a symbol
 * msg   = message content as a string
 * kqos  = quality of service to be set "ijh"
 * kret  = does the broker retain messages after sending to current subscribers "bijh"
 * For kret/kqos see https://mosquitto.org/man/mqtt-7.html
*/
EXP K pub(K topic, K msg, K kqos, K kret){
  long ret, qos;
  int err;
  if(topic->t != -KS)
    return krr("topic type");
  if(msg->t != KC)
    return krr("payload type");
  switch(kqos->t){
    case -KH:
      qos = kqos->h;
      break;
    case -KI:
      qos = kqos->i;
      break;
    case -KJ:
      qos = kqos->j;
      break;
    default:
      return krr("qos type");
  }
  if(qos<0 || qos>2)
    return krr("invalid qos");
  switch(kret->t){
    case -KB:
      ret = kret->g;
      break;
    case -KH:
      ret = kret->h;
      break;
    case -KI:
      ret = kret->i;
      break;
    case -KJ:
      ret = kret->j;
      break;
    default:
      return krr("retained type");
  }
  if(client == 0)
    return krr("not connected");
  MQTTClient_message pubmsg = MQTTClient_message_initializer;
  pubmsg.payload    = (char*)kG(msg);
  pubmsg.payloadlen = msg->n;
  pubmsg.qos        = qos;
  pubmsg.retained   = ret;
  MQTTClient_deliveryToken token = 0;
  if(MQTTCLIENT_SUCCESS != (err = MQTTClient_publishMessage(client, topic->s, &pubmsg, &token)))
    return krr((S)MQTTClient_strerror(err));
  return kj((J)token);
}

/* Subscribe to a topic
 * topic = topic name as a symbol
*/
EXP K sub(K topic){
  int err;
  if(topic->t != -KS)
    return krr("topic type");
  if(client == 0)
    return krr("not connected");
  if(MQTTCLIENT_SUCCESS != (err = MQTTClient_subscribe(client, topic->s, 1)))
    return krr((S)MQTTClient_strerror(err));
  return (K)0;
}

/* Unsubscribe from a topic
 * topic = topic name as a symbol
*/
EXP K unsub(K topic){
  int err;
  if(topic->t != -KS)
    return krr("topic type");
  if(client==0)
    return krr("not connected");
  if(MQTTCLIENT_SUCCESS != (err = MQTTClient_unsubscribe(client, topic->s)))
    return krr((S)MQTTClient_strerror(err));
  return (K)0;
}

// Callback data structure
struct CallbackDataStr{
  union{
    char reserved[8];   // reserve space for flags, aligned to 64 bit word
    enum{
      MSG_TYPE_SEND = 9876,   // arbitrary uncommon value
      MSG_TYPE_RCVD,
      MSG_TYPE_DISCONN
    } msg_type;
  } header;

  long body_size;
  // Start of dynamic data
};
typedef struct CallbackDataStr CallbackData;

// Function definitions for above prototypes
static void msgsent(void* context, MQTTClient_deliveryToken dt){
  (void)context;
  // Body contains: <dt>
  const long msg_size = sizeof(CallbackData) + sizeof(dt);
  CallbackData* msg = malloc(msg_size);
  msg->body_size = sizeof(dt);
  msg->header.msg_type = MSG_TYPE_SEND;
  memcpy(&(msg[1]), &dt, sizeof(dt));
  send(spair[1], (char*)msg, msg_size, 0);
  free(msg);
}

static int msgrcvd(void* context, char* topic, int unused, MQTTClient_message* mq_msg){
  // Body contains: <topic_len><topic><payload>
  (void)unused;(void)context;
  long topic_len = strlen(topic);
  long msg_size = sizeof(CallbackData) + sizeof(topic_len) + topic_len + mq_msg->payloadlen;
  CallbackData* msg = malloc(msg_size);
  msg->body_size = sizeof(topic_len) + topic_len + mq_msg->payloadlen;
  msg->header.msg_type = MSG_TYPE_RCVD;
  char* p = (char*)&(msg[1]);
  memcpy(p, &topic_len, sizeof(topic_len));
  memcpy(p += sizeof(topic_len), topic, topic_len);
  memcpy(p += topic_len, mq_msg->payload, mq_msg->payloadlen);
  send(spair[1], (char*)msg, msg_size, 0);
  free(msg);
  return 1;
}

static void disconn(void* context, char* cause){
  (void)context;(void)cause;
  // Body contains: <>
  const long msg_size = sizeof(CallbackData);
  CallbackData* msg = malloc(msg_size);
  msg->body_size = 0;
  msg->header.msg_type = MSG_TYPE_DISCONN;
  send(spair[1], (char*)msg, msg_size, 0);
  free(msg);
}

// release K object (print any errors) 
static void pr0(K x){
  if(!x)
  return;
  if(-128 == x->t)
    fprintf(stderr,"%s\n",x->s);
  r0(x);
}

// Callback function definitions
void qmsgsent(char* p,long sz){
  (void)sz;
  K dt = kj(*(MQTTClient_deliveryToken*)p);
  pr0(k(0, (char*)".mqtt.msgsent", dt, (K)0));
}

void qmsgrcvd(char* p,long sz){
  long tsz = *(long*)p;
  p += sizeof(long);
  K topic = kpn(p,tsz);
  p += tsz;
  K msg = kpn(p, sz - (tsz+sizeof(long)));
  pr0(k(0, (char*)".mqtt.msgrcvd", topic, msg, (K)0));
}

void qdisconn(char* p,long sz){
  (void)p;(void)sz;
  pr0(k(0, (char*)".mqtt.disconn", ktn(0,0), (K)0));
}


/* Socketpair initialization, callback definition and clean up functionality
 * detach function initialized at exit, socketpair start issues handled
 * callback function set to loop on socketpair connection
*/
K mqttCallback(int fd){
  CallbackData cb_hdr;
  long rc = recv(fd, (char*)&cb_hdr, sizeof(cb_hdr), 0);
  const long expected = cb_hdr.body_size;
  if (rc < (long)sizeof(cb_hdr) || expected < 0){
    fprintf(stderr, "recv(1) error: %li, expected: %li\n", rc, expected);
    return (K)0;
  }

  // Don't use MSG_WAITALL since async sd1(-fd, ...) was used.  So call recv until we have 
  // the expected number of bytes otherwise the message sequencing can get out of step.
  long actual;
  char* body = malloc(expected);
  for (rc = 0, actual = 0;
       actual < expected && rc >= 0;
       actual += rc = recv(fd, body + actual, expected - actual, 0));

  if (rc < 0)
    fprintf(stderr, "recv(2) error: %li, expected: %li, actual: %li\n", rc, expected, actual);
  else{
    switch (cb_hdr.header.msg_type){
      case MSG_TYPE_SEND:
        qmsgsent(body, actual);
        break;
      case MSG_TYPE_RCVD:
        qmsgrcvd(body, actual);
        break;
      case MSG_TYPE_DISCONN:
        qdisconn(body, actual);
        break;
      default:
        fprintf(stderr, "mqttCallback - invalid callback type: %u\n", cb_hdr.header.msg_type);
    }
  }

  free(body);
  return (K)0;
}

static void detach(void){
  int sp;
  if((sp=spair[0])){
    sd0x(sp,0);
    close(sp);
  }
  if((sp=spair[1]))
    close(sp);
  spair[0]=0;
  spair[1]=0;
  validinit=0;
}

EXP K init(K UNUSED(X)){
  if(!(0==validinit))
    return 0;
  if(dumb_socketpair(spair,1) == SOCKET_ERROR){
    fprintf(stderr,"Init failed. socketpair: %s\n", strerror(errno));
    return 0;
  }
  // Have to use (0 - fd) rather than simple negate, since SOCKET on Windows is unsigned ptr.
  pr0(sd1(0 - spair[0], &mqttCallback));
  validinit = 1;
  atexit(detach);
  return 0;
}

