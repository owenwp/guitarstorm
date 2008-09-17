#ifndef TABTRACK_H
#define TABTRACK_H

#include "global.h"

//#include <qmemarray.h>
//#include <qrect.h>

#ifdef WITH_TSE3
#include <tse3/PhraseEdit.h>
#endif

/**
 * Represents one bar of a song.
 *
 * Stores start column index, time and key signatures.
 */
typedef struct {
	int start;                          // Starting column
	int notes;
	int time1,time2;                  // Time signature
	short keysig;                       // Key signature
	string section;
	int tempo;
	bool begin;
	int end;
} TabBar;

#include "tabcolumn.h"

/**
 * Represents one track of a song.
 *
 * Includes a collection of columns (array c), bars that organize them
 * (array b), MIDI settings (channel/bank/patch), and lots of other
 * necessary stuff.
 */
class TabTrack {
public:
	/**
	 * Enum to designate various track modes.
	 */
	typedef enum {
		FretTab,
		DrumTab
	} TrackMode;

	TabTrack(TrackMode _tm, string _name, int _channel,
			 int _bank, int _patch, char _string, char _frets);

	/**
	 * Array of columns.
	 */
	vector<TabColumn> c;

	/**
	 * Array of bars.
	 */
	vector<TabBar> b;

	/**
	 * Number of strings
	 */
	int strings;

	/**
	 * Number of frets.
	 */
	int frets;

	/**
	 * Tuning, if applicable.
	 */
	int tune[MAX_STRINGS];

	TrackMode trackMode() { return tm; }
	void setTrackMode(TrackMode t) { tm = t; }

	int channel;                      // MIDI channel
	int bank;                           // MIDI bank
	int patch;                        // MIDI patch

//	  QListIterator<TabColumn> xi(QListT<TabColumn>);  // Current tab col iterator

	string name;                       // Track text name

	int x;                              // Current tab column
	int xb;                             // Current tab bar
	int y;                              // Current tab string

	int cursortimer;                    // After MIDI calculations -
	                                    // timer value on current
	                                    // column, otherwise -
	                                    // undefined

	bool sel;                           // Selection mode enabled
	int xsel;                           // If yes, then selection start column

#ifdef WITH_TSE3
	static TSE3::MidiCommand encodeTimeTracking(int track, int x);
	static void decodeTimeTracking(TSE3::MidiCommand mc, int &track, int &x);
#endif

	int barNr(int c);
	int lastColumn(int n);
	bool showBarSig(int n);
	bool barStatus(int n);
	size_t currentBarDuration();
	int trackDuration();
	size_t maxCurrentBarDuration();
	size_t noteDuration(size_t t, int i);
	int noteNrCols(size_t t, int i);
	int findCStart(int t, int & dur);
	int findCEnd(int t, int & dur);
	bool isRingingAt(int str, int col);

	void removeColumn(int n);
	void insertColumn(int n);
	int insertColumn(int ts, int te);
	void splitColumn(int col, int dur);
	void arrangeBars();
	void addFX(char fx);
	void updateXB();
	void calcBeams();
	void calcStepAltOct();
	void calcVoices();
	bool hasMultiVoices();
	bool isExactNoteDur(int d);
	bool getNoteTypeAndDots(int t, int v, int & tp, int & dt, bool & tr);

#ifdef WITH_TSE3
	TSE3::PhraseEdit *midiTrack(bool tracking = FALSE, int tracknum = 0);
#endif

private:
	void addNewColumn(TabColumn dat, int len, bool *arc);

	TrackMode tm;                       // Track mode
};

#endif
