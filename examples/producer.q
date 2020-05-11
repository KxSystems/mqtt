\l ../q/mqtt.q

// Load in log file provided by Keysight to be published to user
// Initialize a counter to allow us to stop after a set number of published messages
n:0
// Connect to the host
//.mqtt.conn[`$"tcp://host.docker.internal:1883";`src;()!()]
.mqtt.conn[`$"tcp://localhost:1883";`src;()!()]
// Set up a timed message publisher
.z.ts:{if[n>=199;system"t 0"];
       .mqtt.pub[`topic1;string[.z.p],";","topic1_",string n];
       .mqtt.pub[`topic2;string[.z.p],";","topic2_",string n];
       n+:1}

-1"Type `\\t 100` to publish a message every 100ms up to 200 messages, to stop type `\\t 0`";
