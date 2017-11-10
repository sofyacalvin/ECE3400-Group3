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
//typedef enum { role_ping_out = 1, role_pong_back } role_e;

// The debug-friendly names of those roles
//const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};

// The role of the current running sketch
//role_e role = role_pong_back;
unsigned char x_coord = 0;
unsigned char y_coord = 0;
unsigned char curr_o = 1; // N = 0; E = 1; S = 2; W = 3
unsigned char walls = B1111; // N | E | S | W
unsigned char treasures = 0; // 17 | 12 | 7
unsigned char done = 0;

//unsigned int new_data;
// SPI stuff
//const int slaveSelectPin = 7;

void setup(void)
{
  //pinMode(slaveSelectPin, OUTPUT);
  //SPI.begin();

  //
  // Print preamble
  //
  Serial.begin(57600);
  printf_begin();
 // printf("\n\rRF24/examples/GettingStarted/\n\r");
  //printf("ROLE: %s\n\r",role_friendly_name[role]);
  //printf("*** PRESS 'T' to begin transmitting to the other node\n\r");
  
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
  radio.setPayloadSize(15);

  //
  // Open pipes to other nodes for communication
  //

  // This simple sketch opens two pipes for these two nodes to communicate
  // back and forth.
  // Open 'our' pipe for writing
  // Open the 'other' pipe for reading, in position #1 (we can have up to 5 pipes open for reading)

 // if ( role == role_ping_out )
//  {
    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1,pipes[1]);
//  }
//  else
//  {
//    radio.openWritingPipe(pipes[1]);
//    radio.openReadingPipe(1,pipes[0]);
//  }

  //
  // Start listening
  //

  radio.startListening();

  //
  // Dump the configuration of the rf unit for debugging
  //

  radio.printDetails();

}

void loop(void)
{
  //
  // Ping out role.  Repeatedly send the current time
  //

 // if (role == role_ping_out)
 // {
    // First, stop listening so we can talk.
    radio.stopListening();

    // Take the time, and send it.  This will block until complete
    unsigned long time = millis();

    unsigned int new_data;

    /*
    if (x_coord < 5) {
        x_coord++;
      }
    else { 
      if (y_coord < 4) {
        y_coord++;  
      }
      else {
        y_coord = 0;
      }
      x_coord = 0;
    }
    

    if (x_coord == 0 && y_coord == 0) { //starting over
      if (pos_data == 5) {
        pos_data = 0;
      }
      else {
        pos_data++;
      }
    }
   */

    new_data = done << 14 | treasures << 11 | walls << 7 | curr_o << 5 | x_coord << 2 | y_coord;
    
    // * | D | T T T | W W W W | O O | X X X | Y Y
    // will appear as decimal value
    
    printf("Now sending new map data\n");
    bool ok = radio.write( &new_data, sizeof(unsigned int) );
    
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
      String got_string = "x= " +  String(bitRead(got_data_t, 14)) + String(bitRead(got_data_t, 13)) + String(bitRead(got_data_t, 12)) + String(bitRead(got_data_t, 11)) + String(bitRead(got_data_t, 10)) + String(bitRead(got_data_t, 9)) + String(bitRead(got_data_t, 8)) + String(bitRead(got_data_t, 7)) + String(bitRead(got_data_t, 6)) + ", y= " + String(bitRead(got_data_t, 5)) + String(bitRead(got_data_t, 4)) + String(bitRead(got_data_t, 3)) + ", data= " + String(bitRead(got_data_t, 2)) + String(bitRead(got_data_t, 1)) + String(bitRead(got_data_t, 0));
      // Spew it
      Serial.println("Got response " + got_string);
      }

    // Try again 1s later
    delay(250);
  }

