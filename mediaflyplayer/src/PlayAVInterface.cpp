#include "PlayAVInterface.h"
#include <stdlib.h>
#include "nmessagebox.h"
extern QString currentPath;

const QString mf::PlayAVInterface::m_mountPoint = "/tmp/httpfs_mf";

bool mf::PlayAVInterface::mountUrl(QString& url, QString& err)
{
	int r;
	QString cmd;

	cmd = "mkdir " + m_mountPoint;
	system(cmd.toAscii());

	cmd = currentPath + "/httpfs \"" + url + "\" " + m_mountPoint;
NMessageBox::information(0, "", cmd);
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

	url = m_mountPoint + url.right(url.size() - url.lastIndexOf("/"));

	return true;
}

void mf::PlayAVInterface::umountUrl()
{
	QString cmd;

	cmd = "fusermount -u " + m_mountPoint;
	system(cmd.toAscii());
}

