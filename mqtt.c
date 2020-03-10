#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#include <include/MQTTClient.h>
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"paho-mqtt3a.lib")
#pragma comment(lib,"paho-mqtt3as.lib")
#pragma comment(lib,"paho-mqtt3c.lib")
#pragma comment(lib,"paho-mqtt3cs.lib")
#define EXP __declspec(dllexport)
static SOCKET spair[2];
#else
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include "MQTTClient.h"
#define EXP
#define SOCKET_ERROR -1
static int spair[2];
#endif

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "socketpair.c"
#include "k.h"

#ifdef __GNUC__
#define UNUSED(x) x __attribute__((__unused__))
#else
#define UNUSED(x) x
#endif

#define MSGMAXSIZE 1048576

static MQTTClient client;
static char buf[MSGMAXSIZE];
static long sz0,sz1;
static int validinit;

// Function prototypes needed for q callable functions
static void msgsent(void *context, MQTTClient_deliveryToken dt);
static int msgrcvd(void *context, char* topic, int topicLen, MQTTClient_message *msg);
static void disconn(void *context, char* cause);

/* Establish a tcp connection from a q process to mqtt client
 * tcpconn = tcp connection being connected to (symbol)
 * pname   = name to be associated with the connecting process (symbol)
*/
EXP K conn(K tcpconn,K pname){
  int err;
  if(tcpconn->t != -KS)
    return krr("addr type");
  if(pname->t != -KS)
    return krr("client type");
  client = 0;
  if(MQTTCLIENT_SUCCESS != (err = MQTTClient_create(&client, tcpconn->s, pname->s, MQTTCLIENT_PERSISTENCE_NONE, NULL)))
    return krr((S)MQTTClient_strerror(err));
  MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
  conn_opts.keepAliveInterval = 20;
  conn_opts.cleansession = 1;
  MQTTClient_setCallbacks(client, NULL, disconn, msgrcvd, msgsent);
  if(MQTTCLIENT_SUCCESS != (err = MQTTClient_connect(client, &conn_opts)))
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
  return kj((long)token);
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


// Function definitions for above prototypes
static void msgsent(void *context, MQTTClient_deliveryToken dt){
  long msgsz = 1 + sizeof(dt);
  char *p,*msgbuf = malloc(msgsz + sizeof(long));;
  p = msgbuf;
  memcpy(p, &msgsz, sizeof(long));
  memcpy(p += sizeof(long), "a", 1);
  memcpy(p += 1, &dt, sizeof(dt));
  send(spair[1], msgbuf, msgsz + sizeof(long), 0);
  free(msgbuf);
}

static int msgrcvd(void *context, char* topic, int topicLen, MQTTClient_message *msg){
  long topiclen = strlen(topic);
  long msgsz = 1 + sizeof(long) + topiclen + (msg->payloadlen);
  char *p,*msgbuf = malloc(msgsz + sizeof(long));
  p = msgbuf;
  memcpy(p, &msgsz, sizeof(long));
  memcpy(p += sizeof(long), "b", 1);
  memcpy(p += 1, &topiclen, sizeof(long));
  memcpy(p += sizeof(long), topic, topiclen);
  memcpy(p += topiclen, msg->payload, msg->payloadlen);
  send(spair[1], msgbuf, msgsz + sizeof(long), 0);
  free(msgbuf);
  return 1;
}

static void disconn(void *context, char* cause){
  long msgsz = 1;
  char *p,*msgbuf = malloc(msgsz + sizeof(long));
  p = msgbuf;
  memcpy(p, &msgsz, sizeof(long));
  memcpy(p += sizeof(long), "c", 1);
  send(spair[1], msgbuf, msgsz + sizeof(long), 0);
  free(msgbuf);
}

static void resetsz(void){
  sz0=0;
  sz1=0;
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
  K dt = kj(*(long*)p);
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
  pr0(k(0, (char*)".mqtt.disconn", ktn(0,0), (K)0));
}


/* Socketpair initialization, callback definition and clean up functionality
 * detach function initialized at exit, socketpair start issues handled
 * callback function set to loop on socketpair connection
*/

K mqttCallback(int fd){
  recv(fd, (char *)&sz0, sizeof(long), 0);
  sz1 += recv(fd, buf+sz1, sz0-sz1, MSG_WAITALL);
  if(sz0!=sz1)
    return printf("error in callback\n"),(K)0;
  switch(buf[0]){
    case 'a':
      qmsgsent(buf+1, sz0-1);
      break;
    case 'b':
      qmsgrcvd(buf+1, sz0-1);
      break;
    case 'c':
      qdisconn(buf+1, sz0-1);
      break;
  }
  resetsz();
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
  resetsz();
  pr0(sd1(-spair[0], &mqttCallback));
  validinit = 1;
  atexit(detach);
  return 0;
}

