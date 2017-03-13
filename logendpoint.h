#pragma once

#include "endpoint.h"
#include "timeout.h"

#define ALIVE_TIMEOUT 5
#define TARGET_SYSTEM_ID 1
#define SYSTEM_ID 2

class LogEndpoint : public Endpoint {
public:
    LogEndpoint(const char *logs_dir)
        : Endpoint{"BinLog", false}
        , _logs_dir{logs_dir} { assert(_logs_dir); };

    virtual bool start();
    virtual void stop();

    virtual bool _start_timeout() = 0;
    virtual bool _alive_timeout();

protected:
    const char *_logs_dir;
    const int _target_system_id = TARGET_SYSTEM_ID;
    int _file = -1;

    Timeout *_logging_start_timeout = nullptr;
    Timeout *_alive_check_timeout = nullptr;
    uint32_t _timeout_write_total = 0;

    virtual const char *_get_logfile_extension() = 0;

    void _send_msg(const mavlink_message_t *msg, int target_sysid);
    void _remove_start_timeout();
    bool _start_alive_timeout();

private:
    char *_get_filename(const char *extension);
};
