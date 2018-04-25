#include <string.h>
#include "zlog_log.h"

int main()
{
    if ( LogInit("test", "./log.conf") < 0 )
    {
        printf("log init error\n");
        return -1;
    }

    LogDebug("test msg: %d", 100);
    LogInfo("test msg: %d", 100);
    LogNotice("test msg: %d", 100);
    LogWarn("test msg: %d", 100);
    LogError("test msg: %d", 100);
    LogFatal("test msg: %d", 100);

    char buff[100] = "fjkdlas;";

    HLogDebug(buff, strlen(buff));
    HLogInfo(buff, strlen(buff));
    HLogNotice(buff, strlen(buff));
    HLogWarn(buff, strlen(buff));
    HLogError(buff, strlen(buff));
    HLogFatal(buff, strlen(buff));

    LogFini();

    return 0;
}




