#pragma once

#include <QObject>


class QSharedMemory;

class CLogWriter  : public QObject
{
	Q_OBJECT

public:
	CLogWriter(QString strShmemName, QObject *parent =nullptr);
	~CLogWriter();
public slots:
	// 共享内存写入函数
	void Slot_WriteLog();

private:
	void InitializeSharedMemory();

private:
	QSharedMemory* m_pShmem;

	QVector<QString> vecMesgs;

	int nLogCount;
};
