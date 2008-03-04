#include "tabsong.h"
//#include "settings.h"

//#include "musicxml.h"

//#include <qxml.h>
//#include <qfile.h>
//#include <qdatastream.h>
//#include <kconfig.h>

#ifdef WITH_TSE3
#include <tse3/Track.h>
#include <tse3/Part.h>
#include <tse3/TempoTrack.h>
#include <string>
#endif

TabSong::TabSong()
{
}

int TabSong::freeChannel()
{
	bool fc[17];
	for (int i = 1; i <= 16; i++)
		fc[i] = true;

	vector<TabTrack>::iterator it = t.begin();
	for (; it != t.end(); ++it)
		fc[it->channel] = false;

	int res;
	for (res = 1; res <= 16; res++)
		if (fc[res])
			break;

	if (res > 16)
		res = 1;

	return res;
}

size_t TabSong::maxLen()
{
	size_t res = 0;

	vector<TabTrack>::iterator it = t.begin();
	for (; it != t.end(); ++it)
		res = it->b.size() > res ? it->b.size() : res;

	return res;
}

void TabSong::arrangeBars()
{
	vector<TabTrack>::iterator it = t.begin();
	for (; it != t.end(); ++it) {		// For every track
		it->arrangeBars();
	}
}

#ifdef WITH_TSE3
// Assembles the whole TSE song from various tracks, generated with
// corresponding midiTrack() calls.
TSE3::Song *TabSong::midiSong(bool tracking)
{
	TSE3::Song *song = new TSE3::Song(0);

	// Create tempo track
	TSE3::Event<TSE3::Tempo> tempoEvent(tempo, TSE3::Clock(0));
	song->tempoTrack()->insert(tempoEvent);

	// Create data tracks
	int tn = 0;
	QListIterator<TabTrack> it(t);
	for (; it.current(); ++it) {
		TSE3::PhraseEdit *trackData = it.current()->midiTrack(tracking, tn);
		TSE3::Phrase *phrase = trackData->createPhrase(song->phraseList());
		TSE3::Part *part = new TSE3::Part(0, trackData->lastClock() + 1); // GREYFIX: this may be why last event got clipped?
		part->setPhrase(phrase);
		TSE3::Track *trk = new TSE3::Track();
		trk->insert(part);
		song->insert(trk);
		delete trackData;
		tn++;
	}

	return song;
}
#endif
