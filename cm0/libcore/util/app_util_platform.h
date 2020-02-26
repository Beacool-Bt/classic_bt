/*************************************************************************
*
* Yichip  
*
*************************************************************************/

#ifndef APP_UTIL_PLATFORM_H
#define APP_UTIL_PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

//#define _PRIO_SD_HIGH       0
#define _PRIO_APP_HIGH      1
#define _PRIO_APP_MID       1
//#define _PRIO_SD_LOW        2
#define _PRIO_APP_LOW       3
#define _PRIO_APP_LOWEST    3
#define _PRIO_THREAD        4


//lint -save -e113 -e452
/**@brief The interrupt priorities available to the application while the SoftDevice is active. */
typedef enum
{
    APP_IRQ_PRIORITY_HIGHEST = _PRIO_APP_HIGH,
    APP_IRQ_PRIORITY_HIGH    = _PRIO_APP_HIGH,
    APP_IRQ_PRIORITY_MID     = _PRIO_APP_MID,
    APP_IRQ_PRIORITY_LOW     = _PRIO_APP_LOW,
    APP_IRQ_PRIORITY_LOWEST  = _PRIO_APP_LOWEST,
    APP_IRQ_PRIORITY_THREAD  = _PRIO_THREAD     /**< "Interrupt level" when running in Thread Mode. */
} app_irq_priority_t;




#ifdef __cplusplus
}
#endif

#endif /* APP_UTIL_PLATFORM_H */

