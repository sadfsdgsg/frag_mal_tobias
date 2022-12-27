#pragma once

#include <stdlib.h>
#include <list>
#include <Arduino.h>

typedef void (*fc_ptr)(void);

class registered_callback {
    public:
        unsigned long interval;
        unsigned long last_executed;
        fc_ptr callback;
};

class cb_mgr{
    public:
        void register_callback(fc_ptr void_func, unsigned long every_x_ms);
        bool remove_callback(fc_ptr callback);
        bool remove_callback_by_interval(fc_ptr callback, unsigned long every_x_ms);
        void maintain();

    private:
        std::list<registered_callback*> callbacks;
};