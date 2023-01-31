
#include <HexUtil.h>
#include <vector>

int main(int argc, char **argv)
{
    // toUpperHex
    {
        std::vector<char> arr{ 0x11, 0x22, 0x33 };
        auto bytes = QByteArray::fromRawData(arr.data(), arr.size());
        QString output = toUpperHex(bytes);
        auto len = output.size();
        Q_ASSERT(len == 8);
        Q_ASSERT(output == QString("11 22 33"));
    }

    // toUpperHex
    {
        std::vector<uchar> arr{ 0x11, 0x22, 0x33, 0xaa, 0xBB, 0xcC };
        auto output = toUpperHex(QByteArray::fromRawData((const char *)arr.data(), arr.size()));
        Q_ASSERT(output == "11 22 33 AA BB CC");
    }

    // fromHexString
    {
        auto output = fromHexString("11 22 33 aa BB cC");
        std::vector<uchar> arr{ 0x11, 0x22, 0x33, 0xaa, 0xbb, 0xcc };
        auto target = QByteArray::fromRawData((const char *)arr.data(), arr.size());
        Q_ASSERT(output == target);
    }
}