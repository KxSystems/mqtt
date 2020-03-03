\d .mqtt
conn:`mqtt 2:(`conn;2);
init:`mqtt 2:(`init;1);
pubx:`mqtt 2:(`pub ;4);
sub :`mqtt 2:(`sub ;1);

pub:.mqtt.pubx[;;1;0]

msgsent:{0N!(`msgsent;x);}
disconn:{0N!(`disconn;x);}
msgrcvd:{0N!(`msgrecvd;x;y);}

init[]
