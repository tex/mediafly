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

#ifndef mfPlayAudio_H
#define mfPlayAudio_H

#include "PlayAVInterface.h"

#include "ui_MediaflyPlayAudio.h"

#include <QModelIndex>
#include <phonon/mediaobject.h>
#include <phonon/audiooutput.h>

namespace mf {

class PlayAudio : public QWidget, public PlayAVInterface, private Ui::MediaflyPlayAudio
{
	Q_OBJECT	
public:
	PlayAudio(QWidget *parent = 0);

	bool show(const QModelIndex& index, QString& err);
	void hide();
	void quit();
	void play();
	void pause();
	void seek(int sec);

	void getState(int& songPosition, int& songLength, bool& isFinished);

	void keyPressEvent(QKeyEvent* event);
signals:
	void stateChange();

private:
	QModelIndex          m_index;
	Phonon::MediaObject *m_mediaObject;
	Phonon::AudioOutput *m_audioOutput;

private slots:
	void updateImage();
	void handleStateChanged(Phonon::State newState, Phonon::State oldState);
	void handleFinished();
	void handleTick(qint64);
};

}

#endif

