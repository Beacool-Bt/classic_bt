#ifndef __YC11XX_WDT_H__
#define __YC11XX_WDT_H__
#include "yc11xx.h"
/*
 * @brief:Enable WDT
 * @param:none
 * @return: none
 */
void WDT_Enable(void);

/*
 * @brief:Disable WDT
 * @param:none
 * @return: none
 */
void WDT_Disable(void);

/*
 * @brief:Kick WDT
 * @param:none
 * @return: none
 */
void WDT_Kick(void);


#endif 