/*************************************************************************
*
* Yichip  
*
*************************************************************************/

#include "yc_drv_uart.h"

#define YC_LOG_MAX_STRING_LENGTH             225

#define NUM_TO_STR_INTERNAL(val) #val
/** Converts numeric value to string.
 */
#define NUM_TO_STR(val) NUM_TO_STR_INTERNAL(val)
#define TIMESTAMP_STR(val) "[%0" NUM_TO_STR(val) "d]"


#define YC_DRV_UART_DEFAULT_CONFIG                                                   \
    {                                                                                 \
        .pseltxd            = 13,                                                      \
        .pselrxd            = 14,                                                      \
        .pselcts            = 3,                                                      \
        .pselrts            = 6,                                                      \
        .hwfc               = (yc_uart_hwfc_t)NRF_UART_HWFC_DISABLED,                 \
        .parity             = (yc_uart_parity_t)NRF_UART_PARITY_EXCLUDED,             \
        .baudrate           = (yc_uart_baudrate_t)YC_UART_BAUDRATE_115200,            \
        .interrupt_priority = 0,                                                      \
    }

yc_drv_uart_t uart_insert;
static char m_uart_buffer[YC_LOG_MAX_STRING_LENGTH];


static bool yc_log_send_direct(
    uint8_t                severity_level,
    const char * const     p_str,
    uint32_t             * p_args,
    uint32_t               nargs);


void yc_log_std_0(uint8_t severity, char const * const p_str)
{
    yc_log_send_direct(severity, p_str, NULL, 0);
}


void yc_log_std_1(uint8_t            severity,
                            char const * const p_str,
                            uint32_t           val0)
{
    uint32_t args[] = {val0};
    yc_log_send_direct(severity, p_str, args, ARRAY_SIZE(args));
}


void yc_log_std_2(uint8_t            severity,
                            char const * const p_str,
                            uint32_t           val0,
                            uint32_t           val1)
{
    uint32_t args[] = {val0, val1};
    yc_log_send_direct(severity, p_str, args, ARRAY_SIZE(args));
}


void yc_log_std_3(uint8_t            severity,
                            char const * const p_str,
                            uint32_t           val0,
                            uint32_t           val1,
                            uint32_t           val2)
{
    uint32_t args[] = {val0, val1, val2};
    yc_log_send_direct(severity, p_str, args, ARRAY_SIZE(args));
}


void yc_log_std_4(uint8_t            severity,
                            char const * const p_str,
                            uint32_t           val0,
                            uint32_t           val1,
                            uint32_t           val2,
                            uint32_t           val3)
{
    uint32_t args[] = {val0, val1, val2, val3};
    yc_log_send_direct(severity, p_str, args, ARRAY_SIZE(args));
}


void yc_log_std_5(uint8_t            severity,
                            char const * const p_str,
                            uint32_t           val0,
                            uint32_t           val1,
                            uint32_t           val2,
                            uint32_t           val3,
                            uint32_t           val4)
{
    uint32_t args[] = {val0, val1, val2, val3, val4};
    yc_log_send_direct(severity, p_str, args, ARRAY_SIZE(args));
}


void yc_log_std_6(uint8_t            severity,
                            char const * const p_str,
                            uint32_t           val0,
                            uint32_t           val1,
                            uint32_t           val2,
                            uint32_t           val3,
                            uint32_t           val4,
                            uint32_t           val5)
{
    uint32_t args[] = {val0, val1, val2, val3, val4, val5};
    yc_log_send_direct(severity, p_str, args, ARRAY_SIZE(args));
}

static bool buf_len_update(uint32_t * p_buf_len, int32_t new_len)
{
    bool ret;

    *p_buf_len += (uint32_t)new_len;
     ret = true;
    return ret;
}

static bool timestamp_process(const uint32_t * const p_timestamp, char * p_str, uint32_t * p_len)
{
    int32_t len = 0;
    bool    ret = true;
    if (p_timestamp)
    {
        //len = snprintf(&p_str[len],YC_LOG_MAX_STRING_LENGTH, TIMESTAMP_STR(YC_LOG_TIMESTAMP_DIGITS), (int)*p_timestamp);
        ret = buf_len_update(p_len, len);
    }
    else
    {
        *p_len = 0;
    }
    return ret;
}

static uint32_t yc_drv_uart_w_data(const uint8_t * p_buf, uint32_t len)
{
    uint32_t w_len = 0;

    if (len <= YC_LOG_MAX_STRING_LENGTH){
        //memcpy(m_uart_buffer, p_buf, len);
        for (w_len =0; w_len < len; w_len++)
        {
            yc_drv_uart_w_char(&uart_insert, p_buf[w_len]);
        }
        return YC_SUCCESS;
    }
    else{
        return YC_ERROR_DATA_TOO_LOOG;
    }
}

uint32_t yc_snprintf(char *str, size_t size, const char *format, ...)
{
    double vargflt = 0;
    int  vargint = 0;
    char* vargpch = NULL;
    char vargch = 0;
    char* pfmt = NULL;
    uint32_t re_len = 0;
    char temp_read_ptr[254];
    uint32_t temp_len =0;
    
    va_list vp;

    va_start(vp, format);
    pfmt = format;
    HW_REG_8BIT(0x10001b04, 6);
    while(*pfmt)
    {
      if(*pfmt == '%')
      {
          switch(*(++pfmt))
          {
                
              case 'c':
                  vargch = va_arg(vp, int);
                  *str++ = vargch;
                  re_len++;
                  break;
              case 'd':
              case 'i':
                  vargint = va_arg(vp, int);
                  itoa(vargint, temp_read_ptr, 10);
                  while(temp_read_ptr[temp_len])
                  {
                      *str++ = (temp_read_ptr[temp_len]);
                      re_len++;
                      temp_len++;
                  }
                  break;
              case 's':
                  vargpch = va_arg(vp, char*);
                  while(*vargpch)
                  {
                      *str++ = (*vargpch++);
                      re_len++;
                  }
                  break;
              case 'b':
              case 'B':
                  vargint = va_arg(vp, int);
                  itoa(vargint, temp_read_ptr, 2);
                  while(temp_read_ptr[temp_len] != '\0')
                  {
                      *str++ = (temp_read_ptr[temp_len]);
                      re_len++;
                      temp_len++;
                  }
                break;
              case 'x':
              case 'X':
                  vargint = va_arg(vp, int);
                  itoa(vargint, temp_read_ptr, 16);
                  while(temp_read_ptr[temp_len])
                  {
                      *str++ = (temp_read_ptr[temp_len]);
                      re_len++;
                      temp_len++;
                  }

                  break;
              case '%':
                  *str++ = '%';
                  re_len++;
                  break;
              default:
                  *str++ = '%';
                  re_len++;
                  *str++ = vargch;
                  re_len++;
                  break;
          }
          pfmt++;
      }
      else
      {
          *str++ = *pfmt++;
          re_len++;
      }
    }
    va_end(vp);
    return re_len;
}

static bool yc_log_send_direct(
    uint8_t                severity_level,
    const char * const     p_str,
    uint32_t             * p_args,
    uint32_t               nargs)
{
    char     str[YC_LOG_MAX_STRING_LENGTH];
    int32_t  tmp_str_len     = 0;
    uint32_t buffer_len      = 0;
    bool     status          = true;


    switch (nargs)
    {
        case 0:
        {
            tmp_str_len = xstrlen(p_str);
            
            if ((tmp_str_len + buffer_len) < YC_LOG_MAX_STRING_LENGTH)
            {
                xmemcpy(&str[buffer_len], p_str, tmp_str_len);
            }
            else
            {
                xmemcpy(&str[buffer_len], p_str, YC_LOG_MAX_STRING_LENGTH);
                tmp_str_len = YC_LOG_MAX_STRING_LENGTH;
            }
            break;

        }

        case 1:
            HW_REG_16BIT(0x10001b01, tmp_str_len);
            tmp_str_len = yc_snprintf(&str[buffer_len], YC_LOG_MAX_STRING_LENGTH-buffer_len, p_str, p_args[0]);
            break;

        case 2:
            tmp_str_len = yc_snprintf(&str[buffer_len], YC_LOG_MAX_STRING_LENGTH-buffer_len, p_str, p_args[0], p_args[1]);
            break;

        case 3:
            tmp_str_len = yc_snprintf(&str[buffer_len], YC_LOG_MAX_STRING_LENGTH-buffer_len, p_str, p_args[0], p_args[1], p_args[2]);
            break;

        case 4:
            tmp_str_len =
                yc_snprintf(&str[buffer_len], YC_LOG_MAX_STRING_LENGTH-buffer_len, p_str, p_args[0], p_args[1], p_args[2], p_args[3]);
            break;

        case 5:
            tmp_str_len =
                yc_snprintf(&str[buffer_len],
                        YC_LOG_MAX_STRING_LENGTH-buffer_len,
                        p_str,
                        p_args[0],
                        p_args[1],
                        p_args[2],
                        p_args[3],
                        p_args[4]);
            break;

        case 6:
            tmp_str_len =
                yc_snprintf(&str[buffer_len],
                        YC_LOG_MAX_STRING_LENGTH-buffer_len,
                        p_str,
                        p_args[0],
                        p_args[1],
                        p_args[2],
                        p_args[3],
                        p_args[4],
                        p_args[5]);
            break;

        default:
            break;
    }
    status = buf_len_update(&buffer_len, tmp_str_len);
    if (status)
    {
        return yc_drv_uart_w_data((uint8_t *)str, buffer_len);
    }
    else
    {
        return false;
    }

}




uint32_t yc_log_init(void)
{
    uint32_t return_code;
    uart_insert.drv_inst_idx = UART_B_INST_IDX;
    yc_drv_uart_config_t uart_config = YC_DRV_UART_DEFAULT_CONFIG;
    return_code = yc_drv_uart_init(&uart_insert, &uart_config);
    return return_code;
}

