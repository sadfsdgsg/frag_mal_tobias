#include "cb_mgr.h"

int16_t cb_mgr::register_callback(fc_ptr void_func, unsigned long every_x_ms){
    registered_callback buf;
    buf.callback        = void_func;
    buf.period          = every_x_ms;
    buf.last_executed   = millis() - every_x_ms;
    callbacks.push_back(buf);
    return false;
}

bool cb_mgr::remove_callback(fc_ptr callback){
    for (auto cb : callbacks){
        if (cb.callback == callback){
            callbacks.remove(cb);
            return true;
        }
    }
    return false;
}

void cb_mgr::maintain(){
    unsigned long current = millis();
    for (auto cb : callbacks){
        if (current - cb.last_executed > cb.period){
            if (cb.callback != nullptr)
                cb.callback();
        }
    }
}