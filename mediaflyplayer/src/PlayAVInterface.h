#ifndef mfPlayAVInterface_H
#define mfPlayAVInterface_H

#include <QString>
#include <QModelIndex>

namespace mf {

/** Interface that must be provided by classes that provide playback
 *  of audio and video contnent.
 */
class PlayAVInterface
{
public:
	PlayAVInterface();
	virtual ~PlayAVInterface();

	/** Start playback.
	 *
	 *  const QModelIndex& index
	 */
	virtual bool show(const QModelIndex& index, QString& err) = 0;

	/** Stop the stream.
	 */
	virtual void hide() = 0;

	/** Release all resources.
	 */
	virtual void quit() = 0;

	/** Play (from the previous position) the stream.
	 */
	virtual void play() = 0;

	/** Pause the stream.
	 */
	virtual void pause() = 0;

	/** Relative seek in seconds.
	 */
	virtual void seek(int sec)= 0;

	/** Get state of the stream.
	 *
	 *  int songPosition
	 *  int songLength
	 */
	virtual void getState(int& songPosition, int& songLength) = 0;

protected:
	/**
	 * Mount given url with httpfs fuse filesystem.
	 * QString& url - input - url to mount
	 *              - output - full path to mounted file in local mount point
	 * @return true - succes, false - mount failed
	 */
	bool mountUrl(QString& url, QString& err);

	/**
	 * Unmount httpfs fuse filesystem.
	 */
	void umountUrl();

	static const QString m_mountPoint;
};

}

#endif

