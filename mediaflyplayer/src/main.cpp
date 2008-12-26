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

#include "Player.h"
#include "nmessagebox.h"
#include "nmultilanguage.h"
#include "network-state.h"
#include <QApplication>
#include <QFileInfo>

QString currentPath;

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	app.setStyle("nosdstyle");

	QFileInfo info(argv[0]);
	currentPath = info.path();

	// Use hard path during test phase...
	//
	currentPath = "/media/SD-card/";

	QCoreApplication::setOrganizationName("Neuros");
	QCoreApplication::setOrganizationDomain("neurostechnology.com");
	QCoreApplication::setApplicationName("mediafly");

	NMultiLanguage lang;

	if (1 != CoolNetworkStateCheck())
	{
		NMessageBox::warning(0, 
		                     QObject::tr("Ethernet cable not detected"), 
		                     QObject::tr("\nCheck your wireless adapter "
		                                 "or connect a networked Ethernet cable and try again."),
		                     QMessageBox::Ok, QMessageBox::Ok, 20 * 1000);
		return 0;
	}

	mf::Player v;
	v.setPreferredBackground(NBackgroundManagedWidget::BackgroundImageOnly);
	v.showMaximized();

	return app.exec();
}

