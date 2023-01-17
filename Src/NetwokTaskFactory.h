#pragma once

#include <QMap>
#include <QString>
#include <functional>


class NetworkTask;

struct NetworkTaskFactory
{
    template<class T>
    static void registar(const QString& name)
    {
        creators_[name] = []() {return new T; };
    }

    static NetworkTask* create(const QString& name)
    {
        return creators_.contains(name) ? creators_.value(name)() : nullptr;
    }

private:
    static QMap<QString, std::function<NetworkTask* ()>> creators_;
};


