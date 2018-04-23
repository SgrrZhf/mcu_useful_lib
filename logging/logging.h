
#ifndef __LOGGING_H
#define __LOGGING_H

typedef enum
{
    LOG_NONE = 0,
    LOG_ERROR,
    LOG_WARN,
    LOG_INFO,
    LOG_DEBUG,
    LOG_VERBOSE
} LogLevelTypedef;

#define LOG_LOCAL_LEVEL LOG_INFO

void Logging_Write(const char *format, ...);

#define __Logging_Format(letter,format)  #letter " (%d) %s: "  format "\n"

#define __Logging_E(tag,format,...)      if(LOG_LOCAL_LEVEL >= LOG_ERROR) \
                                        {\
                                            Logging_Write(__Logging_Format(E,format),HAL_GetTick(),tag,##__VA_ARGS__); \
                                        }
#define __Logging_W(tag,format,...)      if(LOG_LOCAL_LEVEL >= LOG_WARN) \
                                        {\
                                            Logging_Write(__Logging_Format(W,format),HAL_GetTick(),tag,##__VA_ARGS__); \
                                        }
#define __Logging_I(tag,format,...)      if(LOG_LOCAL_LEVEL >= LOG_INFO) \
                                        {\
                                            Logging_Write(__Logging_Format(I,format),HAL_GetTick(),tag,##__VA_ARGS__); \
                                        }
#define __Logging_D(tag,format,...)      if(LOG_LOCAL_LEVEL >= LOG_DEBUG) \
                                        {\
                                            Logging_Write(__Logging_Format(D,format),HAL_GetTick(),tag,##__VA_ARGS__); \
                                        }
#define __Logging_V(tag,format,...)      if(LOG_LOCAL_LEVEL >= LOG_VERBOSE) \
                                        {\
                                            Logging_Write(__Logging_Format(V,format),HAL_GetTick(),tag,##__VA_ARGS__); \
                                        }

#endif /* ifndef __LOGGING_H
 */
