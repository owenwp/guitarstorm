#ifndef CONVERTGTP_H
#define CONVERTGTP_H

#include <fstream>

#include "tabsong.h"


/**
 * Converter to/from Guitar Pro files.
 *
 * Manages conversions of tabulatures between internal KGuitar
 * structures and Guitar Pro files.
 */
class ConvertGtp 
{
public:
	ConvertGtp(){}

	/**
	 * Called to save current data from TabSong into Guitar Pro
	 * tabulature format file named fileName.
	 */
	//bool save(string fileName);

	/**
	 * Called to load data from Guitar Pro tabulature format
	 * file named fileName into TabSong.
	 */
	static TabSong* load(string fileName);

private:

	static TabSong* song;

	static void readSignature();
	static void readSongAttributes();
	static void readTrackDefaults();
	static void readBarProperties();
	static void readTrackProperties();
	static void readTabs();
	static void readColumnEffects(vector<TabTrack>::iterator &trk, int x);
	static void readNote(vector<TabTrack>::iterator &trk, int x, int y);
	static void readChromaticGraph();
	static void readChord(vector<TabTrack>::iterator &trk, int x);

	static char readChar();

	/**
	 * Reads Delphi string in GPro format. Delphi string looks pretty
	 * much like: <max-length> <real-length> <real-length * bytes -
	 * string data>
	 */
	static string readDelphiString();

	/**
	 * Reads Pascal-like serialized string. It is fixed-length buffer
	 * of maxlen bytes, which contains from the very beginning a byte
	 * of real length and then string data.
	 */
	static string readPascalString(int maxlen);

	/**
	 * Reads word-sized Pascal-like serialized string
	 */
	static string readWordPascalString();

	/**
	 * Read Delphi-serialized integer
	 */
	static int readDelphiInteger();

	/**
	 * Version of Guitar Pro file
	 */
	static int versionMajor;
	static int versionMinor;

	enum {
		TRACK_MAX_NUMBER = 32,
		LYRIC_LINES_MAX_NUMBER = 5,
		STRING_MAX_NUMBER = 7
	};

	static int numBars;
	static int numTracks;

	static int trackPatch[TRACK_MAX_NUMBER * 2];
	/**
	 * Temporary variable to remember bar data, to propagate it on
	 * all tracks later. GREYFIX: this won't be needed later.
	 */
	static vector<TabBar> bars;

	static ifstream stream;

	/**
	 * Human-readable name of current stage of parsing. Should be
	 * set by stage starting functions. Used for debugging / error
	 * messages.
	 */
	static string currentStage;

	/**
	 * Strong sanity checks, enabled by default. Looks for most
	 * values to lie in reasonable range. Can break loading of very
	 * specific files, thus it should be possible to turn it off.
	 */
	static bool strongChecks;
};

#endif
