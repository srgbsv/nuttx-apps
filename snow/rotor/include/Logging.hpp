/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <debug.h>
#include <syslog.h>


#ifndef __APPS_SNOW_ROTOR_LOGER_HPP
#define __APPS_SNOW_ROTOR_LOGER_HPP

#ifdef CONFIG_DEBUG_ROTOR_INFO
# define snowinfo printf
#else
# define snowinfo _none
#endif

#ifdef CONFIG_DEBUG_ROTOR_ERROR
# define snowerror printf
#else
# define snowerror _none
#endif

#ifdef CONFIG_DEBUG_ROTOR_DEBUG
# define snowdebug printf
#else
# define snowdebug _none
#endif

#endif