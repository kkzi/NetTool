#include "App.h"
#include "NetworkConfig.h"
#include <QMap>
#include <QSettings>
#include <QTranslator>

App::App(int argc, char **argv)
    : QApplication(argc, argv)
{
    lang2trans_["English"] = { "", nullptr };
    lang2trans_["Chinese"] = { ":/lang/nettool_zh_CN.qm", new QTranslator };

    {
        QSettings s(CONFIG_FILE_NAME, QSettings::IniFormat);
        loadTheme(s.value(KEY_THEME).toInt());
        loadLanguage(s.value(KEY_LANG).toInt());
    }
}

void App::restart()
{
    /// todo
}

QStringList App::availableLangs() const
{
    return lang2trans_.keys();
}

void App::loadLanguage(int index)
{
    auto lang = availableLangs().value(index);
    auto text = lang.toStdString();
    auto t = lang2trans_.value(lang);
    if (t.trans != nullptr)
    {
        bool ok = t.trans->load(t.qm);
        installTranslator(t.trans);
    }
}

void App::loadTheme(int index)
{
    setStyleSheet("file:///:/css/light.css");
}
