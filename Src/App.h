#pragma once

#include <QApplication>
#include <QMap>
#include <QString>
#include <QStringList>

class QTranslator;

class App : public QApplication
{
public:
    App(int argc, char **argv);

public:
    void restart();
    QStringList availableLangs() const;

private:
    void loadLanguage(int index);
    void loadTheme(int index);

private:
    struct Translator
    {
        QString qm;
        QTranslator *trans{ nullptr };
    };
    QMap<QString, Translator> lang2trans_;
};

#define app ((App *)qApp)
