/*
 *  Copyright(C) 2007 Neuros Technology International LLC. 
 *               <www.neurostechnology.com>
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
 ****************************************************************************
 *
 * xmms2 mainloop implement header.
 *
 * REVISION:
 * 
 * 1) Initial creation. ----------------------------------- 2007-12-14 NW
 *
 */
#ifndef __NXMMSMAINLOOP_H__
#define __NXMMSMAINLOOP_H__

#include <xmmsclient/xmmsclient++/mainloop.h>

#include <QApplication>
#include <QObject>
#include <QSocketNotifier>

class NXmmsMainloop : public QObject, public Xmms::MainloopInterface
{
	Q_OBJECT
public:
	NXmmsMainloop(xmmsc_connection_t *xmmsc);
	~NXmmsMainloop();

	void run ();

	void ToggleWrite(bool toggle);
	xmmsc_connection_t *GetXmmsConnection();

public slots:
	void OnRead ();
	void OnWrite ();

private:
	int m_fd;
	QSocketNotifier *m_rsock;
	QSocketNotifier *m_wsock;
	xmmsc_connection_t *m_xmmsc;
};

#endif
