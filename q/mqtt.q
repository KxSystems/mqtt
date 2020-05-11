\d .mqtt
connX :`mqtt 2:(`connX;3);
init :`mqtt 2:(`init;1);
pubx :`mqtt 2:(`pub ;4);
sub  :`mqtt 2:(`sub ;1);
unsub:`mqtt 2:(`unsub;1);

pub:.mqtt.pubx[;;1;0]
conn:{[tcpconn;pname;opt]connX[tcpconn;pname](enlist[`]!enlist(::)),opt}
msgsent:{0N!(`msgsent;x);}
disconn:{0N!(`disconn;x);}
msgrcvd:{0N!(`msgrecvd;x;y);}

init[]
