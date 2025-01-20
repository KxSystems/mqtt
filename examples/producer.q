\l ../q/mqtt.q

// Initialize a counter to allow us to stop after a set number of published messages
n:1
topics:`topic1`topic2
// Connect to the host
//.mqtt.conn[`$"tcp://host.docker.internal:1883";`src;()!()]
.mqtt.conn[`$"tcp://localhost:1883";`src;()!()]
// Set up a timed message publisher
.z.ts:{if[n>=200;0N!"Finished sending ",(string n)," messages to each topic";system"t 0"];
       .mqtt.pubx[;string[.z.p],";","topic1_",string n;0;0b] each topics;
       n+:1}

-1"Configured topics: ",","sv string topics;
-1"Type `\\t 100` to publish a message to each topic every 100ms (up to 200 messages), to stop type `\\t 0`";
