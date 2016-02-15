#ifndef TRACK_H
#define TRACK_H

#include <QMap>
#include <QString>

class Track {
public:
	Track(const QString &name) : name(name) {}

	struct KeyFrame {
		float value;
		enum Type {
			Step,
			Linear,
			Smooth,
			Ramp,
			TypeMax
		} type;
	};

	void setKeyFrame(int row, const KeyFrame &key) { keys.insert(row, key); }
	void removeKeyFrame(int row) { keys.remove(row); }

	bool isKeyFrame(int row) const { return keys.contains(row); }
	const KeyFrame &getKeyFrame(int row) const
	{
		Q_ASSERT(isKeyFrame(row));
		return keys.find(row).value();
	}

	const QString &getName() const { return name; }
private:
	QString name;
	QMap<int, KeyFrame> keys;
};

#endif // TRACK_H
