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
    : QFrame(parent)
    , recvEdit_(new QTextEdit(this))
    , mode_(new QComboBox(this))
    , filePath_(new ClickableLabel(this))
    , bufferLimit_(new QLineEdit(this))
    , timer_(new QTimer(this))
{
    bufferLimit_->setText("1000");
    bufferLimit_->setAlignment(Qt::AlignCenter);
    bufferLimit_->setFixedWidth(50);
    recvEdit_->setReadOnly(true);
    mode_->addItems(QStringList::fromStdList({ "文本", "十六进制", "文件" }));

    timer_->setSingleShot(false);
    connect(mode_, SIGNAL(currentIndexChanged(int)), this, SLOT(showModeDetail(int)));
    connect(filePath_, SIGNAL(mouseLeftButtonClicked()), this, SLOT(openStoreFilePath()));
    connect(filePath_, SIGNAL(mouseRightButtonClicked()), this, SLOT(openStoreDir()));
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
    auto have = buffer_.size();
    auto come = data.size();
    auto capa = bufferLimit_->text().toInt();

    if (come >= capa)
    {
        buffer_.clear();
        buffer_ = data.right(capa);
    }
    else
    {
        if (auto len = come + have - capa; len > 0)
        {
            buffer_.remove(0, len);
        }
        buffer_.append(data);
    }
    if (!timer_->isActive())
    {
        timer_->start(250);
    }
}

void RecvWidget::setupUi()
{
    auto ctrl = new QHBoxLayout;
    {
        auto title = new QLabel("接收信息");
        title->setObjectName("title");
        ctrl->addWidget(title);
    }
    {
        ctrl->addWidget(new QLabel("缓冲区大小"));
        ctrl->addWidget(bufferLimit_);
        ctrl->addWidget(new QLabel("字节"));
    }
    ctrl->addSpacing(16);
    ctrl->addWidget(mode_);
    {
        ctrl->addWidget(filePath_);
        filePath_->hide();
    }

    ctrl->addStretch(1);
    {
        auto cleanBtn = new QPushButton("清空");
        ctrl->addWidget(cleanBtn);
        connect(cleanBtn, SIGNAL(clicked()), recvEdit_, SLOT(clear()));
        connect(cleanBtn, SIGNAL(clicked()), this, SLOT(cleanBuffer()));
    }

    auto layout = new QVBoxLayout(this);
    layout->addLayout(ctrl);
    layout->addWidget(recvEdit_, 1);
}

void RecvWidget::updateBufferDisplay()
{
    QString text = mode_->currentIndex() > 0 ? toUpperHex(buffer_) : QString(buffer_);
    recvEdit_->setText(text);
    auto bar = recvEdit_->verticalScrollBar();
    bar->setValue(bar->maximum());
}

void RecvWidget::showModeDetail(int index)
{
    bool fileMode = isFileMode();
    filePath_->setVisible(fileMode);
    if (!fileMode)
    {
        updateBufferDisplay();
        return;
    }

    if (filePath_->text().isEmpty())
    {
        openStoreFilePath();
    }
}

void RecvWidget::openStoreFilePath()
{
    auto dir = QFileDialog::getExistingDirectory(nullptr, "选择文件存储目录");
    if (dir.isEmpty())
    {
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
