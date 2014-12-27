#ifndef COMMON_LOGGING_H
#define COMMON_LOGGING_H

/* 
 * Printipi/common/logging.h
 * (c) 2014 Colin Wallace
 *
 * This file provides some functions that allow for logging information to stdout.
 * Use LOG for information logging, LOGE to log an error, LOGW for warnings, LOGD for debug logging, and LOGV for verbose debug logging.
 */

#include <stdio.h>
#include "compileflags.h"

#if DO_LOG
    //NOTE: these logging functions must be implemented as macros, instead of templated functions
    // in order to get format verification at compile time.
    #define _LOG(tag, enableFunc, outputFile, format, args...) \
        if (enableFunc()) { \
            fprintf(outputFile, "[" tag "] " format, ## args); \
        }
#else
    #include <tuple>
    //make a tuple with the arguments and make it as (void) to avoid unused variable warnings.
    #define _LOG(args...) do { (void)std::make_tuple(##args); } while(0);
#endif

#define LOGE(format, args...) _LOG("ERR ",   logging::isInfoEnabled,    stderr, format, ##args)
#define LOGW(format, args...) _LOG("WARN",    logging::isInfoEnabled,    stdout, format, ##args)
#define LOG(format, args...)  _LOG("INFO",    logging::isInfoEnabled,    stdout, format, ##args)
#define LOGD(format, args...) _LOG("DBG ",   logging::isDebugEnabled,   stdout, format, ##args)
#define LOGV(format, args...) _LOG("VERB", logging::isVerboseEnabled, stdout, format, ##args)


#define _UNIQUE_NAME_LINE2( name, line ) name##line
#define _UNIQUE_NAME_LINE( name, line ) _UNIQUE_NAME_LINE2( name, line )
#define _UNIQUE_NAME( name ) _UNIQUE_NAME_LINE( name, __LINE__ )

#define _LOGONCE2(LOG_FUNC, FLAG_NAME, format, args...) \
    do { \
        static int FLAG_NAME=0; \
        if (!FLAG_NAME) { \
            FLAG_NAME=1; \
            LOG_FUNC("[ONCE] " format, ## args); \
        } \
    } while (0);
#define _LOGONCE(LOG_FUNC, format, args...) _LOGONCE2(LOG_FUNC, _UNIQUE_NAME(_LOG_ONCE_FLAG_), format, ## args)
#define LOGE_ONCE(format, args...) _LOGONCE(LOGE, format, ## args)
#define LOGW_ONCE(format, args...) _LOGONCE(LOGW, format, ## args)
#define LOG_ONCE(format, args...)  _LOGONCE(LOG, format, ## args)
#define LOGD_ONCE(format, args...) _LOGONCE(LOGD, format, ## args)
#define LOGV_ONCE(format, args...) _LOGONCE(LOGV, format, ## args)

    
    
namespace logging {

#if DO_LOG == 1

    extern bool _info;
    extern bool _debug;
    extern bool _verbose;

    inline bool isInfoEnabled() {
        return _info;
    }
    inline bool isDebugEnabled() {
        return _debug;
    }
    inline bool isVerboseEnabled() {
        return _verbose;
    }

    inline void disable() {
        _info = false;
        _debug = false;
        _verbose = false;
    }

    inline void enableDebug() {
        _debug = true;
        LOG("debug logging enabled\n");
    }
    inline void enableVerbose() {
        enableDebug();
        _verbose = true;
        LOG("verbose logging enabled\n");
    }

#else

    inline bool isInfoEnabled() {
        return false;
    }
    inline bool isVerboseEnabled() {
        return false;
    }
    inline bool isDebugEnabled() {
        return false;
    }

    inline void disable() {}
    inline void enableVerbose() {}
    inline void enableDebug() {}

#endif

}

#endif
