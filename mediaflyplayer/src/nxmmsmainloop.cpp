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
 * xmms2 mainloop implement routines.
 *
 * REVISION:
 * 
 * 1) Initial creation. ----------------------------------- 2007-12-14 NW
 *
 */
#include <xmmsclient/xmmsclient++/mainloop.h>
#include <xmmsclient/xmmsclient.h>
#include "nxmmsmainloop.h"

#include <QApplication>
#include <QObject>
#include <QSocketNotifier>

static void CheckWrite (int i, void *userdata);

NXmmsMainloop::NXmmsMainloop (xmmsc_connection_t *xmmsc) :
QObject (), Xmms::MainloopInterface (xmmsc),
m_fd (0), m_rsock (0), m_wsock (0), m_xmmsc (xmmsc)
{
	m_fd = xmmsc_io_fd_get (xmmsc);
	xmmsc_io_need_out_callback_set (xmmsc, CheckWrite, this);

	m_rsock = new QSocketNotifier (m_fd, QSocketNotifier::Read, this);
	connect (m_rsock, SIGNAL (activated (int)), SLOT (OnRead ()));
	m_rsock->setEnabled (true);

	m_wsock = new QSocketNotifier (m_fd, QSocketNotifier::Write, this);
	connect (m_wsock, SIGNAL (activated (int)), SLOT (OnWrite ()));
	m_wsock->setEnabled (false);
	running_ = true;
}


NXmmsMainloop::~NXmmsMainloop ()
{
	delete m_rsock;
	delete m_wsock;
}

void NXmmsMainloop::run ()
{
}

xmmsc_connection_t *NXmmsMainloop::GetXmmsConnection ()
{
	return m_xmmsc;
}


void NXmmsMainloop::OnRead ()
{
	if(!xmmsc_io_in_handle (m_xmmsc))
	{
		return;	/* exception? */
	}
}


void NXmmsMainloop::OnWrite ()
{
	if(!xmmsc_io_out_handle (m_xmmsc))
	{
		return;	/* exception? */
	}
}

void NXmmsMainloop::ToggleWrite (bool toggle)
{
	m_wsock->setEnabled (toggle);
}

static void CheckWrite (int i, void *userdata)
{
	Q_UNUSED( i );

	NXmmsMainloop *obj = static_cast< NXmmsMainloop* > (userdata);

	if(xmmsc_io_want_out (obj->GetXmmsConnection ()))
	{
		obj->ToggleWrite (true);
	}
	else
	{
		obj->ToggleWrite (false);
	}
}
