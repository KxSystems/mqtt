# MQTT kdb+ library function reference

:globe_with_meridians:
[MQTT manual](https://mosquitto.org/man/mqtt-7.html)

The following functions are exposed in the `.mqtt` namespace.
They allow you to interact with MQTT brokers and send and receive messages.

`.mqtt`   **MQTT interface**

**Broker interaction**<br>
[`conn`](#mqttconn)      connect to a MQTT broker<br>
[`pub`](#mqttpub)       publish a message to a topic<br>
[`pubx`](#mqttpubx)      publish a message to a topic controlling qos and ret<br>
[`sub`](#mqttsub)       subscribe to a topic<br>
[`unsub`](#mqttunsub)     unsubscribe from a topic

**Callback functions**<br>
[`disconn`](#mqttdisconn)   manage disconnection events<br>
[`msgrecv`](#mqttmsgrecv)   manage receipt of messages<br>
[`msgsent`](#mqttmsgsent)   manage sending of messages

The callbacks here to handle disconnections and the sending and receipt of messages are minimal implementations. 

You can adapt them to the messages being sent to or received from your broker.


## `.mqtt.conn`

_Connect to a Mosquitto host_

```txt
.mqtt.conn[uri;name;opts]
```

Where

-   `uri` is the IP address or hostname of the MQTT broker being connected to as a symbol.
-   `name` is a symbol denoting the name to be given to the connecting process
-   `opts` dictionary of connection options to the MQTT broker, for default options use `()!()`

returns a failure notice if connnection to host could not be established otherwise does not return output.

> The `opts` parameter lets you provide a username and password to brokers that require them. Further options will be added.

Within MQTT the Client Identifier identifies a client to the server. This must be unique to ensure connections are appropriately established. Within this interface the parameter `name` maps to `ClientID` and as such each defined `name` must be unique across all processes connecting to a broker.

```q
// In this example Mosquitto is not started on the defined host
q)hst:`$"tcp://localhost:1883"
q).mqtt.conn[hst;`src;()!()]
'Failure

// Attempt to connect to a host using an invalid protocol with default options
q).mqtt.conn[`$"https://localhost:1883";`src;()!()]
'Invalid protocol scheme

// Mosquitto now started on appropriate host with default options
q).mqtt.conn[hst;`src;()!()]

// Connect to Mosquitto broker providing username and password
q).mqtt.conn[hst;`src;`username`password!`myuser`mypass]

// Connect to a TLS/SSL enabled server
q)opts:`trustStore`enableServerCertAuth`verify`sslVersion!((`$"/mqttq/server-certs/ca.crt");(0i);(0i);(0i))
q).mqtt.conn[`$"ssl://localhost:1883";`rcv;opts];
```

### General Connection Options
| Name | Type | Details | Default |
| ----------- | ----------- | ----------- | ----------- |
| username | sym | MQTT servers that support the MQTT v3.1.1 protocol provide authentication and authorisation by user name and password. This is the user name parameter. | |
| password | sym | MQTT servers that support the MQTT v3.1.1 protocol provide authentication and authorisation by user name and password. This is the password parameter. | |
| keepAliveInterval | int/long | The 'keep alive' interval, measured in seconds, defines the maximum time that should pass without communication between the client and the server The client will ensure that at least one message travels across the network within each keep alive period. In the absence of a data-related message during the time period, the client sends a very small MQTT 'ping' message, which the server will acknowledge. The keep alive interval enables the client to detect when the server is no longer available without having to wait for the long TCP/IP timeout. | 60 |
| cleansession | int/long | This is a boolean value. The cleansession setting controls the behaviour of both the client and the server at connection and disconnection time. The client and server both maintain session state information. This information is used to ensure "at least once" and "exactly once" delivery, and "exactly once" receipt of messages. Session state also includes subscriptions created by an MQTT client. You can choose to maintain or discard state information between sessions. When cleansession is true (1), the state information is discarded at connect and disconnect. Setting cleansession to false (0) keeps the state information. When you connect, the client identifies the connection using the client identifier and the address of the server. The server checks whether session information for this client has been saved from a previous connection to the server. If a previous session still exists, and cleansession=true, then the previous session information at the client and server is cleared. If cleansession=false, the previous session is resumed. If no previous session exists, a new session is started. | 1 |
| reliable | int/long | This is a boolean value that controls how many messages can be in-flight simultaneously. Setting reliable to true (1) means that a published message must be completed (acknowledgements received) before another can be sent. Attempts to publish additional messages receive an `MQTTCLIENT_MAX_MESSAGES_INFLIGHT` return code. Setting this flag to false allows up to 10 messages to be in-flight. This can increase overall throughput in some circumstances. | 1 |
| connectTimeout | int/long | The time interval in seconds to allow a connect to complete | 30 |
| retryInterval | int/long | The time interval in seconds after which unacknowledged publish requests are retried during a TCP session. With MQTT 3.1.1 and later, retries are not required except on reconnect. 0 turns off in-session retries, and is the recommended setting. Adding retries to an already overloaded network only exacerbates the problem. | 0 |
| MQTTVersion | int/long | Sets the version of MQTT to be used on the connect. `MQTTVERSION_DEFAULT (0)` = default: start with 3.1.1, and if that fails, fall back to 3.1 `MQTTVERSION_3_1 (3)` = only try version 3.1 `MQTTVERSION_3_1_1 (4)` = only try version 3.1.1 `MQTTVERSION_5 (5)` = only try version 5.0 | 0 |
| maxInflightMessages | int/long | The maximum number of messages in flight | -1 |
| cleanstart | int/long | V5 clean start flag.  Only clears state at the beginning of the session. | 0 |

### Last-Will Connection Options

Used by the MQTT client to tell the broker to publish a pre-defined message if your client disconnects.


| Name | Type | Details | Default |
| ----------- | ----------- | ----------- | ----------- |
| lastWillTopic | sym | topic to publish the Last Will message | |
| lastWillQos | int/long | QoS to use when sending the Last Will message. Options are :  0 (at most once) 1 (at least once) 2 (exactly once) | 0 |
| lastWillMessage | char vector | a message that will be sent to the subscribers of a particular topic in case of a client’s disconnection | |
| lastWillRetain | int/long | a boolean value indicating whether the Last Will message is to be retained or not. If retained, the Last Will message will be sent to all the new subscribers of the lastWillTopic. If not, the new subscribers won’t receive the Last Will message | 0 |

### TLS/SSL Connections Options

Used when connecting to an `ssl://` uri, which enables a TLS/SSL connection.
> NOTE: you must alter the q script to load the mqttkdb_ssl.so instead of mqttkdb.so, in order to load the TLS/SSL libraries


| Name | Type | Details | Default |
| ----------- | ----------- | ----------- | ----------- |
| trustStore | sym | The file in PEM format containing the public digital certificates trusted by the client. | |
| keyStore | sym | The file in PEM format containing the public certificate chain of the client. It may also include the client's private key. | |
| privateKey | sym | If not included in the sslKeyStore, this setting points to the file in PEM format containing the client's private key. | |
| privateKeyPassword | sym | The password to load the client's privateKey if encrypted. | |
| enabledCipherSuites | sym | The list of cipher suites that the client will present to the server during the SSL handshake. For a full explanation of the cipher list format, please see the OpenSSL on-line documentation: http://www.openssl.org/docs/apps/ciphers.html#CIPHER_LIST_FORMAT If this setting is ommitted, its default value will be "ALL", that is, all the cipher suites -excluding those offering no encryption- will be considered. This setting can be used to set an SSL anonymous connection ("aNULL" string value, for instance) | |
| enableServerCertAuth | int/long | True/False (i.e. 1/0) option to enable verification of the server certificate | 1 |
| sslVersion | int/long | The SSL/TLS version to use. Specify one of `MQTT_SSL_VERSION_DEFAULT (0)`, `MQTT_SSL_VERSION_TLS_1_0 (1)`, `MQTT_SSL_VERSION_TLS_1_1 (2)` or `MQTT_SSL_VERSION_TLS_1_2 (3)`. | 0 |
| verify | int/long | Whether to carry out post-connect checks, including that a certificate matches the given host name. | 0 |
| CApath | sym | From the OpenSSL documentation: If CApath is not NULL, it points to a directory containing CA certificates in PEM format | |


## `.mqtt.disconn`

_Handle disconnections from an MQTT broker_

```txt
.mqtt.disconn[]
```

Returns the output of user-defined logic for handling disconnections.

```q
// Default occurrence of a disconnection
(`disconn;())

// Modify the disconnection callback function
q).mqtt.disconn:{0N!"Disconnection from broker at: ",string[.z.p];}

// Disconnect with the new disconnection logic
"Disconnection from broker at: 2020.05.07D08:28:47.836698000"
```


## `.mqtt.msgrcvd`

_Handle messages received from an MQTT broker_

```txt
.mqtt.msgrcvd[topic;msg]
```

Where

-  `topic` is a string denoting the topic from which the message was received
-  `msg` is the content of the message received from the MQTT broker

```q
// Default occurrence of a message being received
(`msgrecvd;"topic1";"Test message")

// Modify the receiving callback function
q).mqtt.msgrcvd:{0N!"Message - '",string[y],"' received from, ",string[x];}

// The same message received with the new logic
"Message - 'Test message' received from, topic1"
```


## `.mqtt.msgsent`

_Handle callback on successfuly sending a message to an MQTT broker_

```txt
.mqtt.msgsent token
```

Where `token` is a long representing the `MqttDeliveryToken` to monitor delivery

```q
// Default occurrence of a message being sent
q).mqtt.pub[`tcp://localhost:1883;"Test message"];
(`msgsent;1)

// Modify the sending callback function
q).mqtt.msgsent:{0N!"Message was sent with delivery token - ,string[x];}

// The same message sent with the new logic
"Message was sent with delivery token - 1" 
```


## `.mqtt.pub`

_Publish a message to a Mosquitto broker_

```txt
.mqtt.pub[topic;msg]
```

Where

-   `topic` is a symbol denoting the topic that the message is to be sent to
-   `msg` is a string of the message being sent to the broker

returns a callback to the process stating that the message has been sent to the broker.

```q
// Connect to the host broker
q).mqtt.conn[`$"tcp://localhost:1883";`src]

// Publish a message to a topic named `topic1
q).mqtt.pub[`topic1;"This is a test message"];
(`msgsent;1)
```

> This function is a projection of the function `.mqtt.pubx` defined below.
> 
> Where 
> 
> 1. `kqos` is set to 1. The broker/client will deliver the message at least once, with confirmation required.
> 2. `kret` is set to `0b`. Messages are not retained after sending.


## `.mqtt.pubx`

_Publish a message to a Mosquitto broker, controlling quality of service and message retention_

```txt
.mqtt.pubx[topic;msg;kqos;kret]
```

Where

- `topic` is a symbol denoting the topic that the message is to be sent to
- `msg` is a string of the message being sent to the broker
- `kqos` is an long denoting the quality of service to be used
- `kret` is a boolean denoting if published messages are to be retained

returns a callback to the process stating that the message has been sent to the broker.

```q
// Connect to the host broker
q).mqtt.conn[`$"tcp://localhost:1883";`src]

// Publish a message to topic named topic2 with kqos=2, kret=1b
q).mqtt.pubx[`topic2;"Sending test message";2;1b]
(`msgsent;1)
```


## `.mqtt.sub`

_Subscribe to a topic on a Mosquitto broker process_

```txt
.mqtt.sub topic
```

Where `topic` is a symbol denoting the topic that the process should listen to, returns a callback to the process when a message is received on topic stating that the message was received and what that message is.

```q
// Connect to the host broker and publish a message
q).mqtt.conn[`$"tcp://localhost:1883";`rcv]

// Subscribe to topic1 and recieve a message sent to that topic
q).mqtt.sub[`topic1]
(`msgrcvd;"topic1";"This is a test message")
```


## `.mqtt.unsub`

_Unsubscribe from a Mosquitto broker topic_

```txt
.mqtt.unsub topic
```

Where `topic` is a symbol denoting the topic to be unsubscribed from, does not return a message on correct application, but signals an error on incorrect input.

```q
// Connect to the host broker with the name `rcv
q).mqtt.conn[`$"tcp://localhost:1883";`rcv]
// Subscribe to `topic1
q).mqtt.sub[`topic1]

// publish a message to `topic1 on the broker
(`msgrcvd;"This is a test message")

// Unsubscribe from the topic 
q).mqtt.unsub[`topic1]
// publish another message to `topic1 (note, no message received)
```

