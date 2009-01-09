#include "PlayAVInterface.h"
#include "nmessagebox.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

extern QString currentPath;

const QString mf::PlayAVInterface::m_mountPoint = "/tmp/mf_";

mf::PlayAVInterface::PlayAVInterface()
{
	QString path;

	path = m_mountPoint + "httpfs";
	if (mkdir(path.toAscii(), 0777) == -1)
	{
		NMessageBox::information(0, QObject::tr("System error") + " [" + errno + "]!", QObject::tr("Cannot create directory: ") + path);
		QCoreApplication::exit(1);
	}

	path = m_mountPoint + "preloadfs";
	if (mkdir(path.toAscii(), 0777) == -1)
	{
		NMessageBox::information(0, QObject::tr("System error") + " [" + errno + "]!", QObject::tr("Cannot create directory: ") + path);
		QCoreApplication::exit(1);
	}
}

mf::PlayAVInterface::~PlayAVInterface()
{
	QString path;

	path = m_mountPoint + "httpfs";
	rmdir(path.toAscii());

	path = m_mountPoint + "preloadfs";
	rmdir(path.toAscii());
}

bool mf::PlayAVInterface::mountUrl(QString& url, QString& err)
{
	int r;
	QString cmd;

	cmd = currentPath + "/httpfs \"" + url + "\" " + m_mountPoint + "httpfs";
	r = system(cmd.toAscii());
	if (r == -1)
	{
		err = QObject::tr("Insuficient recources!");
		return false;
	}
	else if ((r = WEXITSTATUS(r)) != 0)
	{
		err = "[SYS " + QString::number(r) + "] " + QObject::tr("Program error!");
		return false;
	}
	url = m_mountPoint + "httpfs" + url.right(url.size() - url.lastIndexOf("/"));

	cmd = currentPath + "/preloadfs \"" + url + "\" " + m_mountPoint + "httpfs " + currentPath + " 130";
	r = system(cmd.toAscii());
	if (r == -1)
	{
		umountUrl();
		err = QObject::tr("Insuficient recources!");
		return false;
	}
	else if ((r = WEXITSTATUS(r)) != 0)
	{
		umountUrl();
		err = "[SYS " + QString::number(r) + "] " + QObject::tr("Program error!");
		return false;
	}
	url = m_mountPoint + "preloadfs" + url.right(url.size() - url.lastIndexOf("/"));
	return true;
}

void mf::PlayAVInterface::umountUrl()
{
	QString cmd;

	cmd = "fusermount -u " + m_mountPoint + "preloadfs";
	system(cmd.toAscii());

	cmd = "fusermount -u " + m_mountPoint + "httpfs";
	system(cmd.toAscii());
}

