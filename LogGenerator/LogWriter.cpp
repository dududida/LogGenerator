#include "LogWriter.h"

#include <QSharedMemory>
#include <QDateTime>
#include <QDebug>

constexpr uint32_t SHMEM_SIZE = 10 * 1024 * 1024;  // 10MB共享内存
constexpr int LOG_SIZE = 512;                      // 单条日志长度




CLogWriter::CLogWriter(QString strShmemName, QObject* parent)
	: QObject(parent)
	, m_pShmem(nullptr)
{
    nLogCount = 0;
    vecMesgs.push_back("Error");
    vecMesgs.push_back("Warning");
	m_pShmem = new QSharedMemory(strShmemName);
    InitializeSharedMemory();
}

CLogWriter::~CLogWriter()
{
    if (m_pShmem->isAttached()) {
        m_pShmem->detach();
    }
}

void CLogWriter::Slot_WriteLog() {
    // 准备日志数据
    const QDateTime now = QDateTime::currentDateTime();
    QString msg = now.toString("[yyyy-MM-dd HH:mm:ss.zzz]");

    msg += vecMesgs[nLogCount % 2];
    nLogCount = nLogCount % 2;
    nLogCount++;

    qDebug() << "Log: " + msg;
    char buffer[LOG_SIZE] = { 0 };
    qstrncpy(buffer, msg.toLocal8Bit().constData(), LOG_SIZE - 1);
    buffer[LOG_SIZE - 1] = '\0';

    // 写入共享内存
    if (!m_pShmem->lock()) {
        qWarning() << "Lock failed:" << m_pShmem->errorString();
        return;
    }

    char* data = static_cast<char*>(m_pShmem->data());
    const uint32_t logAreaSize = SHMEM_SIZE - 8;
    uint32_t& writePos = *reinterpret_cast<uint32_t*>(data);
    char* logArea = data + 8;

    // 处理内存回绕
    if (writePos + LOG_SIZE > logAreaSize) {
        writePos = 0;
    }

    // 写入日志并更新指针
    memcpy(logArea + writePos, buffer, LOG_SIZE);
    writePos = (writePos + LOG_SIZE) % logAreaSize;

    m_pShmem->unlock();
}

void CLogWriter::InitializeSharedMemory()
{
    if (!m_pShmem->create(SHMEM_SIZE)) {
        if (m_pShmem->error() == QSharedMemory::AlreadyExists) {
            if (!m_pShmem->attach()) {
                qFatal("Attach failed: %s", qPrintable(m_pShmem->errorString()));
            }
        }
        else {
            qFatal("Create failed: %s", qPrintable(m_pShmem->errorString()));
        }
    }
    else {
        // 初始化指针位置
        if (m_pShmem->lock()) {
            memset(m_pShmem->data(), 0, 8);
            m_pShmem->unlock();
        }
    }
}
