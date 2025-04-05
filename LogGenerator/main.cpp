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
    timer.start(1);  // 1ms����
    return a.exec();
}



// ʹ��C++ QTдһ������Qt �Ŀ���̨���򣬳���Ĺ������¡�

//1. ����һ���СΪ10MB�Ĺ����ڴ档
//2. ����ÿ��1ms������һ�������ڴ���д��һ��Log���ݣ�Log���ݵĳ��ȶ�������Ч����ʹ��\0��β��
//3. ������ײ�8�ֽڣ����ڱ�ʾ��ǰ��־дָ��Ͷ�ָ���λ�á�ÿ��дLogʱ���������дָ�롣����ָ������һ����ȡ�����ڴ�ĳ�����£��㲻��Ҫ���ǡ���


// ʹ��C++ QTдһ��UI���򣬳���Ĺ������¡�

// 1. ��ʱ�ӹ����ڴ��ȡLog���ݣ�Log���ݵĳ��ȶ�������Ч����ʹ��\0��β���������ڴ����ж�дָ�룬ÿ��Ӧ�ö�ȡ�Ӷ�ָ�뵽дָ��֮����������ݲ�������ƶ���ָ�롣��
// 2. ��ȡ����ÿ��Log���ݻ���뵽���ݿ��У�������ʾ������
// 3. �����߱��ؼ���ɸѡ���ܡ�