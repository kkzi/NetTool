#include "SendFileDialog.h"

SendFileDialog::SendFileDialog(QWidget *parent /*= nullptr*/)
    : QDialog(parent)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Send File"));
}
