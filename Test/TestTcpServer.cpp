
#include "TcpServerTask.h"

int main(int argc, char **argv)
{
    NetworkConfig conf;
    conf.localIp = "0.0.0.0";
    conf.localPort = 44441;
    TcpServerTask task;
    task.setConfig(conf);
    task.start();
}