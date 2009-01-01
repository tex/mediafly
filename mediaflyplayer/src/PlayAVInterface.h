#ifndef mfPlayAVInterface_H
#define mfPlayAVInterface_H

class QModelIndex;

namespace mf {

class PlayAVInterface
{
public:
	virtual void show(const QModelIndex& index) = 0;
	virtual void hide() = 0;
	virtual void play() = 0;
	virtual void pause() = 0;

	virtual void getState(int& songPosition, int& songLength) = 0;
};

}

#endif

