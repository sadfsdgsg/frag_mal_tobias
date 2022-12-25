#pragma once

#include <stdlib.h>
#include <list>
#include <Arduino.h>

typedef void (*fc_ptr)(void);

typedef struct registered_callback{
    unsigned long period;
    unsigned long last_executed;
    fc_ptr callback;
} registered_callback;

bool operator ==(registered_callback a, registered_callback b){
    return a.callback == b.callback;
}

#define VALID_CALLBACK 0
#define INVALID_CALLBACK -1
#define REMOVED_CALLBACK -2


class cb_mgr{
    public:
        int16_t register_callback(fc_ptr void_func, unsigned long every_x_ms);
        bool remove_callback(fc_ptr callback);
        void maintain();

    private:
        std::list<registered_callback> callbacks;
};