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

bool mf::PlayAVInterface::mountUrl(QString& url, QString& err, int cacheSize)
{
	QString cmd;

	cmd = currentPath + "/" + m_httpfs + " \"" + url + "\" " + m_mountPoint + m_httpfs;
	if (mount(cmd, err) == false)
		return false;
	url = m_mountPoint + m_httpfs + url.right(url.size() - url.lastIndexOf("/"));

	cmd = currentPath + "/" + m_preloadfs + " \"" + url + "\" " + m_mountPoint + m_httpfs + " " + currentPath + " " + QString::number(cacheSize);
	if (mount(cmd, err) == false)
	{
		umountUrl();
		return false;
	}
	url = m_mountPoint + m_preloadfs + url.right(url.size() - url.lastIndexOf("/"));
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

