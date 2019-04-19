/*
 * Copyright (c) 2017, CATIE, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mbed.h"
#include "zest-radio-atzbrf233.h"
#include "MQTTNetwork.h"
#include "MQTTmbed.h"
#include "MQTTClient.h"

namespace {
#define PERIOD_MS 500
}

static DigitalOut led1(LED1);
static AnalogIn ain(ADC_IN1);


I2C i2c(I2C1_SDA, I2C1_SCL);
uint8_t lm75_adress = 0x48 << 1;

// main() runs in its own thread in the OS
// (note the calls to Thread::wait below for delays)

// Network interface

NetworkInterface *net;

int arrivedcount = 0;

/* Printf the message received and its configuration */

void messageArrived(MQTT::MessageData& md)
{
    MQTT::Message &message = md.message;
    printf("Message arrived: qos %d, retained %d, dup %d, packetid %d\r\n", message.qos, message.retained, message.dup, message.id);
    printf("Payload %.*s\r\n", message.payloadlen, (char*)message.payload);

    char temp[20];
    strncpy(temp, (char *) message.payload, message.payloadlen);

    if (strcmp(temp, "ON") == 0) {
    	led1 = 1;
    }

    else {

    	led1 = 0;
    }
}

float temperature() {

	char cmd[2];
	cmd[0] = 0x00;
	i2c.write(lm75_adress, cmd, 1);
	i2c.read(lm75_adress, cmd, 2);

	float temperature = ((cmd[0] << 8 | cmd[1] ) >> 7) * 0.5;

	return temperature;

}

float humidite() {

	float air_value = 0.0 ;
	float water_value = 2.39;
	float result = ain.read() * 3.3;
	float measure_percent = (result - air_value) * 100.0 / (water_value - air_value);

	return measure_percent;

}

int main()
{
	int result;

	// Add the border router DNS to the DNS table

	nsapi_addr_t new_dns = {

		NSAPI_IPv6,
		{ 0xfd, 0x9f, 0x59, 0x0a, 0xb1, 0x58, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x01 }
	};

	nsapi_dns_add_server(new_dns);

	printf("Starting MQTT demo\n");

	// Get default Network interface (6LowPAN)

	net = NetworkInterface::get_default_instance();

	if (!net) {

		printf("Error! No network interface found.\n");
		return 0;
	}

	// Connect 6LowPAN interface

	result = net->connect();

	if (result != 0) {

		printf("Error! net->connect() returned: %d\n", result);
		return result;
	}

	// Build the socket that will be used for MQTT

	MQTTNetwork mqttNetwork(net);

	// Declare a MQTT Client

	MQTT::Client<MQTTNetwork, Countdown> client(mqttNetwork);

	// Connect the socket to the MQTT Broker

	const char* hostname = "io.adafruit.com";
	uint16_t port = 1883;
	printf("Connecting to %s:%d\r\n", hostname, port);
	int rc = mqttNetwork.connect(hostname, port);

	if (rc != 0)
		printf("rc from TCP connect is %d\r\n", rc);

	// Connect the MQTT Client

	MQTTPacket_connectData data = MQTTPacket_connectData_initializer;

	data.MQTTVersion = 3;
	data.clientID.cstring = "mbed-sample";
	data.username.cstring = "maelpena";
	data.password.cstring = "e48774d5184f4205a77755f17ff789c3";

	if ((rc = client.connect(data)) != 0)
		printf("rc from MQTT connect is %d\r\n", rc);

	// Subscribe to the same topic we will publish in

	if ((rc = client.subscribe("maelpena/feeds/feeds-humid", MQTT::QOS2, messageArrived)) != 0)
		printf("rc from MQTT subscribe is %d\r\n", rc);

	if ((rc = client.subscribe("maelpena/feeds/feeds-cmd", MQTT::QOS2, messageArrived)) != 0)
		printf("rc from MQTT subscribe is %d\r\n", rc);

	// Send a message with QoS 0

	while (true) {

		MQTT::Message message;

		// QoS 0

		char buf[100];

		sprintf(buf, "%f\n",humidite());

		message.qos = MQTT::QOS0;
		message.retained = false;
		message.dup = false;
		message.payload = (void*)buf;
		message.payloadlen = strlen(buf)+1;

		rc = client.publish("maelpena/feeds/feeds-humid", message);

		MQTT::Message message1;

		char buff[100];

		sprintf(buff, "%f\n",temperature());

		message1.qos = MQTT::QOS0;
		message1.retained = false;
		message1.dup = false;
		message1.payload = (void*)buff;
		message1.payloadlen = strlen(buff)+1;

		rc = client.publish("maelpena/feeds/feeds-temp", message1);

		printf("temp : %f\n", temperature());

		// yield function is used to refresh the connexion
		// Here we yield until we receive the message we sent

		client.yield(100);

		wait(10);

	}

	// Disconnect client and socket
	client.disconnect();
	mqttNetwork.disconnect();

	// Bring down the 6LowPAN interface
	net->disconnect();
    printf("Done\n");

}







