\l ../mqtt.q

.mqtt.msgrcvd:{cbfn[x;y];0N!"Message received"}

.mqtt.conn[`$"tcp://localhost:1883";`rcv];
.mqtt.sub[`topic1];
.mqtt.sub[`topic2];

.mqtt.tab:([]topic:`symbol$();
       msg_sent:`timestamp$();
       msg_recv:`timestamp$();
       recieved_msg_no:`symbol$())

// Define a callback function to handle the incoming data
cbfn:{[topic;msg]
      data:";" vs msg;
      .mqtt.tab,:(`$topic;"P"$data 0;.z.p;"S"$data 1)}
