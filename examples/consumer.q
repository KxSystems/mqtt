\l ../q/mqtt.q

// consumer topics
topics:`topic1`topic2

// Define the table schema to handle incoming messages
.mqtt.tab:([]topic:`symbol$();
       msg_sent:`timestamp$();
       msg_recv:`timestamp$();
       received_msg_no:`symbol$())

// Define a callback function to handle the incoming data
cbfn:{[topic;msg]
      data:";" vs msg;
      .mqtt.tab,:(`$topic;"P"$data 0;.z.p;"S"$data 1)}

.mqtt.msgrcvd:{cbfn[x;y];0N!"Message received"}

// Connect and subscribe
//.mqtt.conn[`$"tcp://host.docker.internal:1883";`rcv;()!()];
.mqtt.conn[`$"tcp://localhost:1883";`rcv;()!()];

-1"Configured topics: ",","sv string topics;
-1"Populating .mqtt.tab with each received message";
.mqtt.sub each topics;
