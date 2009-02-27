#include "PlayAVInterface.h"

#ifndef NO_FUSE

#include <QDir>
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

#endif

mf::PlayAVInterface::PlayAVInterface()
{
#ifndef NO_FUSE
	QString path;

	path = m_mountPoint + m_httpfs;
	mkdir(path);

	path = m_mountPoint + m_preloadfs;
	mkdir(path);
#endif
}

mf::PlayAVInterface::~PlayAVInterface()
{
#ifndef NO_FUSE
	QString path;

	path = m_mountPoint + m_httpfs;
	rmdir(path.toAscii());

	path = m_mountPoint + m_preloadfs;
	rmdir(path.toAscii());
#endif
}

#ifndef NO_FUSE

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
		if (r == 3)
		{
			err = QObject::tr("Server not found.");
		}
		else if (r == 4)
		{
			err = QObject::tr("Server doesn't support required feature.");
		}
		else if (r == 8)
		{
			err = QObject::tr("Too many redirections.");
		}
		else
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
	return QString();
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
	if (url.isEmpty())
	{
		err += QObject::tr("Program error!") + " [3]";
		return false;
	}

	cmd = currentPath + "/" + m_preloadfs + " \"" + url + "\" " + m_mountPoint + m_preloadfs + " " + currentPath + " " + QString::number(cacheSize);
	if (mount(cmd, err) == false)
	{
		umountUrl();
		err += " [2]";
		return false;
	}
	url = findName(m_mountPoint + m_preloadfs);
	if (url.isEmpty())
	{
		err += QObject::tr("Program error!") + " [4]";
		return false;
	}

	return true;
}

bool mf::PlayAVInterface::umount(QString& cmd)
{
	for (int i = 0; i < 5; ++i)
	{
		int r = system(cmd.toAscii());
		if ((r == -1) || (WEXITSTATUS(r) != 0))
		{
			usleep(500);
		}
		else
			return true;
	}
	return false;
}

void mf::PlayAVInterface::umountUrl()
{
	QString cmd;

	cmd = "fusermount -u " + m_mountPoint + m_preloadfs;
	umount(cmd);

	cmd = "fusermount -u " + m_mountPoint + m_httpfs;
	umount(cmd);
}

#endif

