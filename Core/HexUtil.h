#pragma once

#include <QByteArray>
#include <QString>

static QByteArray toUpperHex(const QByteArray &input)
{
    static const char *TABLE = "0123456789ABCDEF";
    QByteArray output(input.size() * 3 - 1, 0);
    auto len = output.size();
    auto ptr = (unsigned char *)input.data();
    for (auto i = 0, j = 0; i < input.size(); ++i)
    {
        output[j++] = TABLE[ptr[i] >> 4];
        output[j++] = TABLE[ptr[i] & 0x0f];
        if (j + 1 < len)
        {
            output[j++] = ' ';
        }
    }
    return output;
};

static QByteArray fromHexString(QString input)
{
    input = input.replace(' ', "");
    auto output = QByteArray::fromHex(input.toLocal8Bit());
    return output;
}
