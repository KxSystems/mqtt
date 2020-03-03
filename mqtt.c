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

#define K3(f) K f(K x,K y,K z)
#define K4(f) K f(K x,K y,K z,K r)
#define KR -128
#define KNL (K)0
#define MSGMAXSIZE 1048576

Z MQTTClient client;
ZC buf[MSGMAXSIZE];
ZJ sz0,sz1;
ZI validinit;

ZV msgsent(V *context,MQTTClient_deliveryToken dt){
  J msgsz=1+sizeof(dt);
  C *p,*msgbuf = malloc(msgsz+sizeof(J));;
  p=msgbuf;
  memcpy(p,&msgsz,sizeof(J));
  memcpy(p+=sizeof(J),"a",1);
  memcpy(p+=1,&dt,sizeof(dt));
  send(spair[1],msgbuf,msgsz+sizeof(J),0);
  free(msgbuf);
}

ZI msgrcvd(V *context,S topic,I topicLen,MQTTClient_message *msg){
  J topiclen=strlen(topic);
  J msgsz=1+sizeof(J)+topiclen+(msg->payloadlen);
  C *p,*msgbuf=malloc(msgsz+sizeof(J));
  p=msgbuf;
  memcpy(p,&msgsz,sizeof(J));
  memcpy(p+=sizeof(J),"b",1);
  memcpy(p+=1,&topiclen,sizeof(J));
  memcpy(p+=sizeof(J),topic,topiclen);
  memcpy(p+=topiclen,msg->payload,msg->payloadlen);
  send(spair[1],msgbuf,msgsz+sizeof(J),0);
  free(msgbuf);
  R 1;
}

ZV disconn(V *context,S cause){
  J msgsz=1;
  C *p,*msgbuf=malloc(msgsz+sizeof(J));
  p=msgbuf;
  memcpy(p,&msgsz,sizeof(J));
  memcpy(p+=sizeof(J),"c",1);
  send(spair[1],msgbuf,msgsz+sizeof(J),0);
  free(msgbuf);
}

EXP K2(conn){
  if(x->t!=-KS)R krr("addr type");
  if(y->t!=-KS)R krr("client type");
  client=0;
  MQTTClient_create(&client,x->s,y->s,MQTTCLIENT_PERSISTENCE_NONE,NULL);
  MQTTClient_connectOptions conn_opts=MQTTClient_connectOptions_initializer;
  conn_opts.keepAliveInterval=20;
  conn_opts.cleansession=1;
  MQTTClient_setCallbacks(client,NULL,disconn,msgrcvd,msgsent);
  I r=MQTTClient_connect(client,&conn_opts);
  if(r!=MQTTCLIENT_SUCCESS)R krr("connection failed");
  R KNL;
}

EXP K4(pub){
  J jr,jq;
  if(x->t!=-KS)R krr("topic type");
  if(y->t!= KC)R krr("payload type");
  SW(z->t){
    CS(-KH,jq=z->h);
    CS(-KI,jq=z->i);
    CS(-KJ,jq=z->j);
    CD:R krr("qos type");
  }
  if(jq<0||jq>2)R krr("invalid qos");
  SW(r->t){
    CS(-KB,jr=r->g);
    CS(-KH,jr=r->h);
    CS(-KI,jr=r->i);
    CS(-KJ,jr=r->j);
    CD:R krr("retained type");
  }
  if(client==0)R krr("not connected");
  MQTTClient_message pubmsg=MQTTClient_message_initializer;
  pubmsg.payload   =(S)kG(y);
  pubmsg.payloadlen=y->n;
  pubmsg.qos       =jq;
  pubmsg.retained  =jr;
  MQTTClient_deliveryToken token=0;
  MQTTClient_publishMessage(client,x->s,&pubmsg,&token);
  R kj((J)token);
}

EXP K1(sub){
  if(x->t!=-KS)R krr("topic type");
  if(client==0)R krr("not connected");
  MQTTClient_subscribe(client,x->s,1);
  R KNL;
}

EXP K1(unsub){
  if(x->t!=-KS)R krr("topic type");
  if(client==0)R krr("not connected");
  MQTTClient_unsubscribe(client,x->s);
  R KNL;
}
    
ZV resetsz(V){
  sz0=0;
  sz1=0;
}   
   
// release K object (print any errors) 
ZV pr0(K x){
  if(!x)R;
  if(KR==xt)fprintf(stderr,"%s\n",x->s);
  r0(x);
}

V qmsgsent(C* p,J sz){
  K dt=kj(*(J*)p);
  pr0(k(0,(S)".mqtt.msgsent",dt,KNL));
}

V qmsgrcvd(C* p,J sz){
  J tsz=*(J*)p;
  p+=sizeof(J);
  K topic=kpn(p,tsz);
  p+=tsz;
  K msg=kpn(p,sz-(tsz+sizeof(J)));
  pr0(k(0,(S)".mqtt.msgrcvd",topic,msg,KNL));
}

V qdisconn(C* p,J sz){
  pr0(k(0,(S)".mqtt.disconn",ktn(0,0),KNL));
}

K mqttCallback(I fd){
  recv(fd,(char *)&sz0,sizeof(J),0);
  sz1+=recv(fd,buf+sz1,sz0-sz1,MSG_WAITALL);
  if(sz0!=sz1)
    R printf("oops\n"),KNL;
  SW(buf[0]){
    CS('a',qmsgsent(buf+1,sz0-1));
    CS('b',qmsgrcvd(buf+1,sz0-1));
    CS('c',qdisconn(buf+1,sz0-1));
  }
  resetsz();
  R KNL;
}

ZV detach(V){
 I sp;
 if(sp=spair[0]){
  sd0x(sp,0);
  close(sp);
 }
 if(sp=spair[1])
  close(sp);
 spair[0]=0;
 spair[1]=0;
 validinit=0;
}

EXP K init(K UNUSED(X)){
  if(!(0==validinit))
   R 0;
  if(dumb_socketpair(spair,1) == SOCKET_ERROR){
   fprintf(stderr,"Init failed. socketpair: %s\n",strerror(errno));
  R 0;
  }
  resetsz();
  pr0(sd1(-spair[0],&mqttCallback));
  validinit=1;
  atexit(detach);
  R 0;
}
