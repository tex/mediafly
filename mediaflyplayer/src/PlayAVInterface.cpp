#include <QDir>
#include "PlayAVInterface.h"
#include "nmessagebox.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

extern QString currentPath;

const QString mf::PlayAVInterface::m_mountPoint = "/tmp/mf_";
const QString mf::PlayAVInterface::m_httpfs = "httpfs";
const QString mf::PlayAVInterface::m_preloadfs = "preloadfs";

void mf::PlayAVInterface::mkdir(QString& path) const
{
	struct stat st;

	errno = 0;

	if (::stat(path.toAscii(), &st) == -1)
	{
		if (errno == ENOENT)
		{
			if (::mkdir(path.toAscii(), 0777) == 0)
			{
				return;
			}
		}
	}
	else if (S_ISDIR(st.st_mode))
	{
		return;
	}

	NMessageBox::information(0, QObject::tr("System error") + " [" + strerror(errno) + "]!",
				    QObject::tr("Cannot create directory: ") + path);
	::exit(EXIT_FAILURE);
}

mf::PlayAVInterface::PlayAVInterface()
{
	QString path;

	path = m_mountPoint + m_httpfs;
	mkdir(path);

	path = m_mountPoint + m_preloadfs;
	mkdir(path);
}

mf::PlayAVInterface::~PlayAVInterface()
{
	QString path;

	path = m_mountPoint + m_httpfs;
	rmdir(path.toAscii());

	path = m_mountPoint + m_preloadfs;
	rmdir(path.toAscii());
}

bool mf::PlayAVInterface::mount(QString& cmd, QString& err)
{
	int r = system(cmd.toAscii());
	if (r == -1)
	{
		err = QObject::tr("Insuficient resources!");
		return false;
	}
	else if ((r = WEXITSTATUS(r)) != 0)
	{
		err = "[SYS " + QString::number(r) + "] " + QObject::tr("Program error!");
		return false;
	}
	return true;
}

QString mf::PlayAVInterface::findName(const QString& path)
{
	QDir dir(path);
	QFileInfoList list = dir.entryInfoList();

	for (int i = 0; i < list.size(); ++i)
	{
		QFileInfo fileInfo = list.at(i);
		if (fileInfo.isFile())
			return fileInfo.absoluteFilePath();
	}
}

bool mf::PlayAVInterface::mountUrl(QString& url, QString& err, int cacheSize)
{
	QString cmd;

	cmd = currentPath + "/" + m_httpfs + " \"" + url + "\" " + m_mountPoint + m_httpfs;
	if (mount(cmd, err) == false)
	{
		err += " [1]";
		return false;
	}
	url = findName(m_mountPoint + m_httpfs);
	NMessageBox::information(0, "", url);

	cmd = currentPath + "/" + m_preloadfs + " \"" + url + "\" " + m_mountPoint + m_preloadfs + " " + currentPath + " " + QString::number(cacheSize);
	if (mount(cmd, err) == false)
	{
		umountUrl();
		err += " [2]";
		return false;
	}
	url = findName(m_mountPoint + m_preloadfs);
	NMessageBox::information(0, "", url);
	return true;
}

void mf::PlayAVInterface::umountUrl()
{
	QString cmd;

	cmd = "fusermount -u " + m_mountPoint + m_preloadfs;
	system(cmd.toAscii());

	cmd = "fusermount -u " + m_mountPoint + m_httpfs;
	system(cmd.toAscii());
}

