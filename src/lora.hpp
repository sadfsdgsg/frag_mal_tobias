#pragma once

#include "pinout.h"
#include "logger.h"
#include "Arduino.h"
#include <RadioLib.h>

typedef void (*msg_handler)(uint8_t*, size_t);


/**
 * starts SPI connection to SX1262, creates the Radiolib module and configures SX1262
*/
void lora_start();
/**
 * sets, whether the LoRa module should receive messages or not.
 * messages are received non-blocking, use 'register_msg_handler' to handle incoming messages
*/
void shall_receive(bool);
/**
 * registers a message handler, to which new messages will be directed.
*/
void register_lora_msg_handler(msg_handler);
/**
 * checks if new messages were received and forwards it to the registered handler
*/
void lora_maintain();


Module m(LORA_SS, DIO1, LORA_RST, LORA_BUSY);
SX1262 radio(&m);
// flag to indicate that a packet was received
volatile bool lora_rcv_flag = false;
// disable interrupt when it's not needed
volatile bool lora_interrupt_enabled = true;
msg_handler handler = nullptr;

void set_rcv_flag(){
    // check if we want to receive messages
    if(!lora_interrupt_enabled) {
        return;
    }
    // we got a packet, set the flag
    lora_rcv_flag = true;
}

void lora_start(){
    log_begin();
    SPI.begin(LORA_SCK,LORA_MISO,LORA_MOSI,LORA_SS);
    // initialize SX1262 with default settings
    log_info("[SX1262] Initializing ... ");
    int state = radio.begin(868.0);
    logf_info("[SX1262] begin, state '%d'", state);
    radio.setDio1Action(set_rcv_flag);
    state = radio.startReceive();
    logf_info("[SX1262] startReceive, state '%d'", state);
}


void lora_shall_receive(bool yes_or_no){
    lora_interrupt_enabled = yes_or_no;
}

void register_lora_msg_handler(msg_handler new_handler){
    handler = new_handler;
}

void lora_maintain(){
    if (!lora_interrupt_enabled){
        /*
            we dont want to receive/handle messages... 
            reset flag and run
        */
        lora_rcv_flag = false;
        return;
    }
    if (!lora_rcv_flag){
        /*
            we didnt receive a message, bye
        */
        return;
    }

    /*
        we received a message,
        stop the isr, to not be disturbed while handling the message
    */
    lora_interrupt_enabled = false;

    log_info("[LoRa] received a message!");

    if (handler == nullptr){
        log_warn("[LoRa] no message handler registered!");
    } else {
        size_t packetLength = radio.getPacketLength();
        logf_info("[LoRa] packet length '%d'", packetLength);
        uint8_t msg[packetLength + 1];
        int state = radio.readData(msg, packetLength);
        msg[packetLength] = '\0';

        #ifdef USE_LOGGER
            logf_info("[LoRa] message '%s' read with status: %d", (char*) msg, state);
        #endif

        handler(msg, packetLength);
    }

    /*
        reactivate "listening" mode 
    */
    lora_rcv_flag = false;
    radio.startReceive();
    lora_interrupt_enabled = true;

}