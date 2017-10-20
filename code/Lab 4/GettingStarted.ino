/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * Example for Getting Started with nRF24L01+ radios.
 *
 * This is an example of how to use the RF24 class.  Write this sketch to two
 * different nodes.  Put one of the nodes into 'transmit' mode by connecting
 * with the serial monitor and sending a 'T'.  The ping node sends the current
 * time to the pong node, which responds by sending the value back.  The ping
 * node can then see how long the whole cycle took.
 */

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10

RF24 radio(9,10);

//
// Topology
//

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0x0000000006LL, 0x0000000007LL };

//
// Role management
//
// Set up role.  This sketch uses the same software for all the nodes
// in this system.  Doing so greatly simplifies testing.
//

// The various roles supported by this sketch
typedef enum { role_ping_out = 1, role_pong_back } role_e;

// The debug-friendly names of those roles
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};

// The role of the current running sketch
role_e role = role_pong_back;

unsigned char y_coord = 0;
unsigned char x_coord = 0;
unsigned char pos_data = 0;

void setup(void)
{
  //
  // Print preamble
  //

  Serial.begin(57600);
  printf_begin();
  printf("\n\rRF24/examples/GettingStarted/\n\r");
  printf("ROLE: %s\n\r",role_friendly_name[role]);
  printf("*** PRESS 'T' to begin transmitting to the other node\n\r");

  //
  // Setup and configure rf radio
  //

  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);
  radio.setAutoAck(true);
  // set the channel
  radio.setChannel(0x50);
  // set the power
  // RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_MED=-6dBM, and RF24_PA_HIGH=0dBm.
  radio.setPALevel(RF24_PA_MIN);
  //RF24_250KBPS for 250kbs, RF24_1MBPS for 1Mbps, or RF24_2MBPS for 2Mbps
  radio.setDataRate(RF24_250KBPS);

  // optionally, reduce the payload size.  seems to
  // improve reliability
  radio.setPayloadSize(7);

  //
  // Open pipes to other nodes for communication
  //

  // This simple sketch opens two pipes for these two nodes to communicate
  // back and forth.
  // Open 'our' pipe for writing
  // Open the 'other' pipe for reading, in position #1 (we can have up to 5 pipes open for reading)

  if ( role == role_ping_out )
  {
    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1,pipes[1]);
  }
  else
  {
    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1,pipes[0]);
  }

  //
  // Start listening
  //

  radio.startListening();

  //
  // Dump the configuration of the rf unit for debugging
  //

  radio.printDetails();

  
  //pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
}

void loop(void)
{
  //
  // Ping out role.  Repeatedly send the current time
  //

  if (role == role_ping_out)
  {
    // First, stop listening so we can talk.
    radio.stopListening();

    // Take the time, and send it.  This will block until complete
    unsigned long time = millis();

    unsigned char new_data;
    /*
    if (x_coord == 4) {
      if (y_coord < 3) {
        x_coord = 0;
        y_coord++;
      }
      else { //last
        x_coord = 0;
        y_coord = 0;
      }
    }
    else {
      x_coord++;
    }
    */
    
    if (x_coord < 4) {
        x_coord++;
      }
    else {
      x_coord = 0;
    }
    
    /*
    if (pos_data == 3) {
      pos_data = 0;
    }
    else {
      pos_data++;
    }
    */
    
    if (x_coord == 0) {
      if (pos_data == 3) {
        pos_data = 0;
      }
      else {
        pos_data++;
      }
    }

    //pos_data = 3;

  
    
    new_data = x_coord << 4 | y_coord << 2 | pos_data;
    // x x x | y y | p p 
    // will appear as decimal value
    
    printf("Now sending new map data\n");
    bool ok = radio.write( &new_data, sizeof(unsigned char) );
    
    if (ok)
      printf("ok... \n");
    else
      printf("failed.\n\r");

    // Now, continue listening
    radio.startListening();

    // Wait here until we get a response, or timeout (250ms)
    unsigned long started_waiting_at = millis();
    bool timeout = false;
    while ( ! radio.available() && ! timeout )
      if (millis() - started_waiting_at > 200 )
        timeout = true;

    // Describe the results
    if ( timeout )
    {
      printf("Failed, response timed out.\n\r");
    }
    else
    {

    unsigned long got_data_t;
      radio.read( &got_data_t, sizeof(unsigned long) );
      //String got_string = String(bitRead(got_data_t, 7)) + String(bitRead(got_data_t, 6)) + String(bitRead(got_data_t, 5)) + String(bitRead(got_data_t, 4)) + String(bitRead(got_data_t, 3)) + String(bitRead(got_data_t, 2)) + String(bitRead(got_data_t, 1)) + String(bitRead(got_data_t, 0));
      String got_string = String(bitRead(got_data_t, 6)) + String(bitRead(got_data_t, 5)) + String(bitRead(got_data_t, 4)) + String(bitRead(got_data_t, 3)) + String(bitRead(got_data_t, 2)) + String(bitRead(got_data_t, 1)) + String(bitRead(got_data_t, 0));
      // Spew it
      Serial.println("Got response " + got_string);
      }

    // Try again 1s later
    delay(2000);
  }

  //
  // Pong back role.  Receive each packet, dump it out, and send it back
  //

  if ( role == role_pong_back )
  {
    // if there is data ready
    if ( radio.available() )
    {
      // Dump the payloads until we've gotten everything
      
      unsigned char got_data;
      
      bool done = false;
      while (!done) {
        // Fetch the payload, and see if this was the last one.
        done = radio.read( &got_data, sizeof(unsigned char) );
        // Spew it
        // Print the received data as a decimal

        /*
        digitalWrite(8, LOW);
        delay(1000);
        digitalWrite(8, HIGH);
        delay(1000);
        */
        /*
        digitalWrite(8, bitRead(got_data, 0) ? HIGH : LOW);
        digitalWrite(1, bitRead(got_data, 2) ? HIGH : LOW);
        digitalWrite(2, bitRead(got_data, 1) ? HIGH : LOW);
        digitalWrite(3, bitRead(got_data, 3) ? HIGH : LOW);
        digitalWrite(4, bitRead(got_data, 4) ? HIGH : LOW);
        digitalWrite(5, bitRead(got_data, 5) ? HIGH : LOW);
        digitalWrite(6, bitRead(got_data, 6) ? HIGH : LOW);
        digitalWrite(7, bitRead(got_data, 7) ? HIGH : LOW);
        */

        digitalWrite(2, bitRead(got_data, 0) ? HIGH : LOW);
        digitalWrite(3, bitRead(got_data, 1) ? HIGH : LOW);
        digitalWrite(4, bitRead(got_data, 2) ? HIGH : LOW);
        digitalWrite(5, bitRead(got_data, 3) ? HIGH : LOW);
        digitalWrite(6, bitRead(got_data, 4) ? HIGH : LOW);
        digitalWrite(7, bitRead(got_data, 5) ? HIGH : LOW);
        digitalWrite(8, bitRead(got_data, 6) ? HIGH : LOW);
        //String bin_string = String(bitRead(got_data, 7)) + String(bitRead(got_data, 6)) + String(bitRead(got_data, 5)) + String(bitRead(got_data, 4)) + String(bitRead(got_data, 3)) + String(bitRead(got_data, 2)) + String(bitRead(got_data, 1)) + String(bitRead(got_data, 0));
        String bin_string = String(bitRead(got_data, 6)) + String(bitRead(got_data, 5)) + String(bitRead(got_data, 4)) + String(bitRead(got_data, 3)) + String(bitRead(got_data, 2)) + String(bitRead(got_data, 1)) + String(bitRead(got_data, 0));
        
        printf("Got payload... ");
        Serial.println(bin_string);

        // Delay just a little bit to let the other unit
        // make the transition to receiver
        delay(20);

      }

      // First, stop listening so we can talk
      radio.stopListening();

      // Send the final one back.
      //radio.write( &got_maze, sizeof(unsigned char) );
      radio.write( &got_data, sizeof(unsigned char) );
      printf("Sent response.\n\r");

      // Now, resume listening so we catch the next packets.
      radio.startListening();
    }
  }

  //
  // Change roles
  //

  if ( Serial.available() )
  {
    char c = toupper(Serial.read());
    if ( c == 'T' && role == role_pong_back )
    {
      printf("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK\n\r");

      // Become the primary transmitter (ping out)
      role = role_ping_out;
      radio.openWritingPipe(pipes[0]);
      radio.openReadingPipe(1,pipes[1]);
    }
    else if ( c == 'R' && role == role_ping_out )
    {
      printf("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK\n\r");

      // Become the primary receiver (pong back)
      role = role_pong_back;
      radio.openWritingPipe(pipes[1]);
      radio.openReadingPipe(1,pipes[0]);
    }
  }
}
// vim:cin:ai:sts=2 sw=2 ft=cpp
