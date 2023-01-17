#include "NetwokTaskFactory.h"



QMap<QString, std::function<NetworkTask* ()>> NetworkTaskFactory::creators_;
