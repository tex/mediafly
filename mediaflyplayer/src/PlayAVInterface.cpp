/*
 *  Copyright(C) 2009 Milan Svoboda. 
 *               <milan.svoboda@centrum.cz>
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 2 of the License.
 *  
 *
 *  This program is distributed in the hope that, in addition to its 
 *  original purpose to support Neuros hardware, it will be useful 
 *  otherwise, but WITHOUT ANY WARRANTY; without even the implied 
 *  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
 *  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 ****************************************************************************/

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
			err = QObject::tr("Server doesn't support required features.");
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
	qDebug() << __PRETTY_FUNCTION__ << url;

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

	qDebug() << __PRETTY_FUNCTION__ << url;

	return true;
}

bool mf::PlayAVInterface::umount(QString& cmd)
{
	qDebug() << __PRETTY_FUNCTION__ << cmd;

	for (int i = 0; i < 20; ++i)
	{
		int r = system(cmd.toAscii());
		if ((r == -1) || (WEXITSTATUS(r) != 0))
		{
			qDebug() << __PRETTY_FUNCTION__ << "WAITING...";
			usleep(1000000);
		}
		else
		{
			qDebug() << __PRETTY_FUNCTION__ << "SUCCESS";
			return true;
		}
	}
	return false;
}

bool mf::PlayAVInterface::isMounted(QString mountPath, QString fileSystem)
{
	QString find(fileSystem + " " + mountPath);
	QFile file("/proc/mounts");

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << __PRETTY_FUNCTION__ << "Filed to open /proc/mounts";
		return false;
	}
	while (!file.atEnd())
	{
		QByteArray line = file.readLine();
		if (line.indexOf(find) != -1)
			return true;
	}
	return false;
}

void mf::PlayAVInterface::umountUrl()
{
	QString cmd;

	if (isMounted(m_mountPoint, m_preloadfs))
	{
		cmd = "fusermount -u " + m_mountPoint + m_preloadfs;
		umount(cmd);
	}

	if (isMounted(m_mountPoint, m_httpfs))
	{
		cmd = "fusermount -u " + m_mountPoint + m_httpfs;
		umount(cmd);
	}
}

#endif

