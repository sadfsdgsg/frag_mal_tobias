#pragma once

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include "logger.h"


BLEScan* pBLEScan;

BLEScanResults bleScanResults;
bool bleNewScanAvailable = false;

void handle_bluetooth_scan(BLEScanResults results) {
    bleScanResults = results;
    bleNewScanAvailable = true;
}

void bluetooth_start() {
    BLEDevice::init("");
}

BLEScanResults bluetooth_scan(uint32_t scanTimeInSeconds) {
    pBLEScan = BLEDevice::getScan(); //create new scan
    return pBLEScan->start(scanTimeInSeconds);
}

bool bluetooth_startScan(uint32_t scanTimeInSeconds) {
    pBLEScan = BLEDevice::getScan(); //create new scan
    return pBLEScan->start(scanTimeInSeconds, &handle_bluetooth_scan);

}

BLEScanResults* bluetooth_getScanResults() {
    return &bleScanResults;
}


std::string bluetooth_getInfo() {
    return BLEDevice::toString();
}

bool bluetooth_newScanAvailable() {
    if (bleNewScanAvailable) {
        bleNewScanAvailable = false;
        return true;
    }
    return false;
}

bool bluetooth_available() {
    return BLEDevice::getInitialized();
}