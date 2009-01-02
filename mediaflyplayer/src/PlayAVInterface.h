#ifndef mfPlayAVInterface_H
#define mfPlayAVInterface_H

class QModelIndex;

namespace mf {

/** Interface that must be provided by classes that provide playback
 *  of audio and video contnent.
 */
class PlayAVInterface
{
public:
	/** Get resources needed to playback the given stream
	 *  and start playback.
	 *
	 *  const QModelIndex& index
	 */
	virtual void show(const QModelIndex& index) = 0;

	/** Stop the stream and release any resources taken to
	 *  provide playback of the stream.
	 */
	virtual void hide() = 0;

	/** Play (from the previous position) the stream.
	 */
	virtual void play() = 0;

	/** Pause the stream.
	 */
	virtual void pause() = 0;

	/** Get state of the stream.
	 *
	 *  int songPosition
	 *  int songLength
	 */
	virtual void getState(int& songPosition, int& songLength) = 0;
};

}

#endif

