#include "cb_mgr.h"

void cb_mgr::register_callback(fc_ptr void_func, unsigned long every_x_ms){
    registered_callback* buf = new registered_callback;
    buf->callback        = void_func;
    buf->interval        = every_x_ms;
    buf->last_executed   = millis() - every_x_ms;
    callbacks.push_back(buf);
}

bool cb_mgr::remove_callback(fc_ptr callback){
    for (registered_callback* cb : callbacks){
        if (cb->callback == callback){
            callbacks.remove(cb);
            delete cb;
            return true;
        }
    }
    return false;
}

bool cb_mgr::remove_callback_by_interval(fc_ptr callback, unsigned long interval){
    for (registered_callback* cb : callbacks){
        if (cb->callback == callback && cb->interval == interval){
            callbacks.remove(cb);
            return true;
        }
    }
    return false;
}


void cb_mgr::maintain(){
    unsigned long current_timestamp = millis();
    for (registered_callback* cb : callbacks){
        if (current_timestamp - cb->last_executed > cb->interval){
            if (cb->callback != nullptr){
                cb->callback();
                cb->last_executed = current_timestamp;
            }
        }
    }
}