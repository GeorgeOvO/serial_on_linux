#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sched.h>
#include <time.h>


#if 0
struct termios{
        tcflag_t c_iflag; /* 输入选项标志 */
        tcflag_t c_oflag; /* 输出选项标志 */
        tcflag_t c_cflag; /* 控制选项标志 */
        tcflag_t c_lflag; /* 本地选项标志 */

        unsigned char c_line /*线控制*/
        cc_t c_cc[NCCS]; /* 控制特性 */
};

struct tm {
          int tm_sec;      /* 秒 – 取值区间为[0,59] */
          int tm_min;      /* 分 - 取值区间为[0,59] */
          int tm_hour;      /* 时 - 取值区间为[0,23] */
          int tm_mday;      /* 一个月中的日期 - 取值区间为[1,31] */
          int tm_mon;      /* 月份（从一月开始，0代表一月） - 取值区间为[0,11] */
          int tm_year;      /* 年份，其值等于实际年份减去1900 */
          int tm_wday;      /* 星期 – 取值区间为[0,6]，其中0代表星期天，1代表星期一，以此类推 */
          int tm_yday;      /* 从每年的1月1日开始的天数 – 取值区间为[0,365]，其中0代表1月1日，1代表1月2日*/

 };
#endif

#define FALSE    0
#define TRUE     1
#define UART     "/dev/ttyUSB0"

void signal_handler_IO(int status);
void quit();
char *curr_time(void);
int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop);


