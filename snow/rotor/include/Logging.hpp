/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <debug.h>

#ifndef __APPS_SNOW_ROTOR_LOGER_HPP
#define __APPS_SNOW_ROTOR_LOGER_HPP

#ifdef CONFIG_DEBUG_SNOW_INFO
# define snowinfo _info
#else
# define snowinfo _none
#endif

#ifdef CONFIG_DEBUG_SNOW_ERROR
# define snowerror _error
#else
# define snowerror _none
#endif

#endif