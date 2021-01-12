\d .mqtt

connX:`mqttkdb 2:(`connX;3);
init :`mqttkdb 2:(`init;1);
pubx :`mqttkdb 2:(`pub ;4);
sub  :`mqttkdb 2:(`sub ;1);
unsub:`mqttkdb 2:(`unsub;1);
isConnected:`mqttkdb 2:(`isConnected;1);
disconnect :`mqttkdb 2:(`disconnect;1);


pub:.mqtt.pubx[;;1;0];
conn:{[tcpconn;pname;opt]connX[tcpconn;pname](enlist[`]!enlist(::)),opt};
msgsent:{0N!(`msgsent;x);};
disconn:{0N!(`disconn;x);};
msgrcvd:{0N!(`msgrecvd;x;y);};
\d .

.mqtt.init[];
