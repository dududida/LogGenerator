#include <QtCore/QCoreApplication>
#include <QTimer>

#include "LogWriter.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CLogWriter writer("TestLog",&a);
    QTimer timer;
    timer.setTimerType(Qt::PreciseTimer);

    QObject::connect(&timer, &QTimer::timeout, &writer, &CLogWriter::Slot_WriteLog);
    timer.start(1);  // 1ms触发
    return a.exec();
}



// 使用C++ QT写一个程序Qt 的控制台程序，程序的功能如下。

//1. 开辟一块大小为10MB的共享内存。
//2. 程序每隔1ms，就往一个共享内存中写入一条Log数据，Log数据的长度定长，有效数据使用\0结尾。
//3. 共享的首部8字节，用于表示当前日志写指针和读指针的位置。每次写Log时，都会更新写指针。（读指针由另一个读取共享内存的程序更新，你不需要考虑。）


// 使用C++ QT写一个UI程序，程序的功能如下。

// 1. 定时从共享内存读取Log数据，Log数据的长度定长，有效数据使用\0结尾。（共享内存中有读写指针，每次应该读取从读指针到写指针之间的所有数据并且向后移动读指针。）
// 2. 读取到的每条Log数据会加入到数据库中，并且显示表表格中
// 3. 表格需具备关键字筛选功能。