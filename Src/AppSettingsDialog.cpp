#include "AppSettingsDialog.h"
#include "App.h"
#include "CommonUi.h"
#include "NetworkConfig.h"
#include <QApplication>
#include <QComboBox>
#include <QPushButton>
#include <QSettings>
#include <QTranslator>
#include <QVBoxLayout>

AppSettingsDialog::AppSettingsDialog(QWidget *parent /*= nullptr*/)
    : QDialog(parent)
    , themeBox_(new QComboBox(this))
    , langBox_(new QComboBox(this))
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Settings"));

    themeBox_->addItem(tr("Light"));
    langBox_->addItems(app->availableLangs());
    auto okBtn = new QPushButton(tr("OK"), this);
    okBtn->setFixedWidth(80);

    connect(okBtn, &QPushButton::clicked, this, &AppSettingsDialog::updateSettings);

    auto layout = new QVBoxLayout(this);
    layout->addLayout(makeLabeledField(tr("Theme"), themeBox_, 60, 200));
    layout->addLayout(makeLabeledField(tr("Language"), langBox_, 60, 200));
    layout->addSpacing(20);
    layout->addWidget(okBtn, 0, Qt::AlignHCenter);
    layout->addStretch();

    {
        QSettings s(CONFIG_FILE_NAME, QSettings::IniFormat);
        themeBox_->setCurrentIndex(s.value(KEY_THEME).toInt());
        langBox_->setCurrentIndex(s.value(KEY_LANG).toInt());
    }
}

void AppSettingsDialog::updateSettings()
{
    QSettings s(CONFIG_FILE_NAME, QSettings::IniFormat);
    s.setValue(KEY_THEME, themeBox_->currentIndex());
    s.setValue(KEY_LANG, langBox_->currentIndex());

    app->restart();

    accept();
}
