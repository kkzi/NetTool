#include "RecvWidget.h"
#include "ClickableLabel.h"
#include "FileStorage.h"
#include "HexUtil.h"
#include "NetworkTaskManager.h"
#include <QCheckBox>
#include <QComboBox>
#include <QDateTime>
#include <QDesktopServices>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollBar>
#include <QTextEdit>
#include <QTimer>
#include <QUrl>
#include <QVBoxLayout>

RecvWidget::RecvWidget(QWidget *parent)
    : TitledWidget(tr("Receive Information"), new QFrame, recvEdit_ = new QTextEdit, parent)
    , saveFile_(new QCheckBox(tr("Save File")))
    , filePath_(new ClickableLabel(this))
    , mode_(new QComboBox(this))
    , timer_(new QTimer(this))
{
    recvEdit_->setReadOnly(true);
    // mode_->addItems(QStringList::fromStdList({ tr("Text"), tr("Hex") }));
    mode_->addItems(QStringList{ tr("Text"), tr("Hex") });

    timer_->setSingleShot(true);
    connect(saveFile_, SIGNAL(clicked(bool)), this, SLOT(setSaveFileEnabled(bool)));
    connect(filePath_, SIGNAL(mouseLeftButtonClicked()), this, SLOT(openStoreFilePath()));
    connect(filePath_, SIGNAL(mouseRightButtonClicked()), this, SLOT(openStoreDir()));
    connect(mode_, SIGNAL(currentIndexChanged(int)), this, SLOT(updateBufferDisplay()));
    connect(timer_, SIGNAL(timeout()), this, SLOT(updateBufferDisplay()));

    setupUi();
}

RecvWidget::~RecvWidget()
{
}

bool RecvWidget::isFileMode() const
{
    return mode_->currentIndex() == 2;
}

QString RecvWidget::storageFilePath() const
{
    return filePath_->text();
}

void RecvWidget::append(const QString &from, const QByteArray &data)
{
    buffer_.append(data);
    timer_->start(250);
}

void RecvWidget::setupUi()
{
    auto cornerLayout = new QHBoxLayout(corner_);
    cornerLayout->setContentsMargins(0, 0, 0, 0);
    {
        cornerLayout->addWidget(saveFile_);
        cornerLayout->addWidget(filePath_);
    }

    cornerLayout->addWidget(mode_);
    {
        auto cleanBtn = new QPushButton(tr("CLEAR"));
        cornerLayout->addWidget(cleanBtn);
        connect(cleanBtn, SIGNAL(clicked()), recvEdit_, SLOT(clear()));
        connect(cleanBtn, SIGNAL(clicked()), this, SLOT(cleanBuffer()));
    }
}

void RecvWidget::updateBufferDisplay()
{
    QString text = mode_->currentIndex() > 0 ? toUpperHex(buffer_) : QString(buffer_);
    recvEdit_->setText(text);
    auto bar = recvEdit_->verticalScrollBar();
    bar->setValue(bar->maximum());
}

void RecvWidget::setSaveFileEnabled(bool checked)
{
    if (!checked)
    {
        NetworkTaskManager::instance()->setStoragePath("");
        return;
    }
    if (auto path = storageFilePath(); path.isEmpty())
    {
        openStoreFilePath();
    }
    else
    {
        NetworkTaskManager::instance()->setStoragePath(path);
    }
}

void RecvWidget::openStoreFilePath()
{
    auto dir = QFileDialog::getExistingDirectory(nullptr, tr("Choose file storage path"));
    if (dir.isEmpty())
    {
        saveFile_->setChecked(false);
        return;
    }
    auto filepath = QString("%1/%2.dat").arg(dir).arg(QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss-zzz"));
    filePath_->setText(filepath);
    NetworkTaskManager::instance()->setStoragePath(filepath);
}

void RecvWidget::openStoreDir()
{
    auto path = storageFilePath();
    if (path.isEmpty())
    {
        return;
    }
    auto dir = QFileInfo(path).dir().absolutePath();
    bool ok = QDesktopServices::openUrl(QUrl(dir));
    Q_ASSERT(ok);
}

void RecvWidget::cleanBuffer()
{
    buffer_.clear();
}
