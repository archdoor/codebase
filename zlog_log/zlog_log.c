#include <stdio.h>
#include "zlog_log.h"

// 全局分类变量
zlog_category_t *g_cat = NULL;

int color_output(zlog_msg_t *msg)
{
    char *level = msg->buf + 1;

    if ( *level == 'D' )
        printf("\e[32;2m%s\e[0m%s", msg->path, msg->buf);
    else if ( *level == 'I' )
        printf("\e[34;2m%s\e[0m%s", msg->path, msg->buf);
    else if ( *level == 'N' )
        printf("\e[33;2m%s\e[0m%s", msg->path, msg->buf);
    else if ( *level == 'W' )
        printf("\e[33;1m%s\e[0m%s", msg->path, msg->buf);
    else if ( *level == 'E' )
        printf("\e[31;2m%s\e[0m%s", msg->path, msg->buf);
    else if ( *level == 'F' )
        printf("\e[31;1m%s\e[0m%s", msg->path, msg->buf);
    else;
        // printf("%s%s", msg->path, msg->buf);

    return 0;
}


int zlog_init_with_category(const char *category, const char *confpath)
{
    if ( zlog_init(confpath) < 0 )
    {
		return -1;
    }

    zlog_set_record("myoutput", color_output);
	g_cat = zlog_get_category(category);
    if ( !g_cat )
    {
		zlog_fini();
		return -2;
    }

    return 0;
}


