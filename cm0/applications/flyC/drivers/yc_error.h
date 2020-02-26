/*************************************************************************
*
* Yichip  
*
*************************************************************************/

#ifndef YC_ERROR_H
#define YC_ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t return_code;


#define YC_ERROR_BASE_NUM      (0x00)       ///< Global error base


#define YC_SUCCESS                              (YC_ERROR_BASE_NUM + 0)  ///< Successful command
#define YC_ERROR_NOT_FOUND                      (YC_ERROR_BASE_NUM + 1)
#define YC_ERROR_OPENED                         (YC_ERROR_BASE_NUM + 2)
#define YC_ERROR_DATA_TOO_LOOG                  (YC_ERROR_BASE_NUM + 3)



#ifdef __cplusplus
}
#endif

#endif /* YC_ERROR */

