#include "convertgtp.h"

//#include <klocale.h>
//#include <qfile.h>
//#include <qdatastream.h>

// initialize static members
TabSong* ConvertGtp::song;
int ConvertGtp::versionMajor;
int ConvertGtp::versionMinor;
int ConvertGtp::numBars;
int ConvertGtp::numTracks;
int ConvertGtp::trackPatch[TRACK_MAX_NUMBER * 2];
vector<TabBar> ConvertGtp::bars;
ifstream ConvertGtp::stream;
string ConvertGtp::currentStage;
bool ConvertGtp::strongChecks = true;

char ConvertGtp::readChar()
{
	char temp[1];
	stream.read(temp, 1);
	return temp[0];
}

string ConvertGtp::readDelphiString()
{
	string str;
	char l;
	char *c;

	int maxl = readDelphiInteger();
	if (stream.eof())  
		throw string("readDelphiString: EOF");

	l = readChar();

	if (maxl != l + 1)  
		throw string("readDelphiString: first word doesn't match second byte");

	c = (char *) malloc(l + 5);

	//if (stream.device()->size() - stream.tellg() < l) {
	//	throw string("readDelphiString: not enough bytes to read %1 byte string").arg(l);
	//}

	if (c) {
		stream.read(c, l);
		c[l] = 0;
		str = string(c);
		free(c);
	}

	return str;
}

string ConvertGtp::readPascalString(int maxlen)
{
	string str;
	char l;
	char *c;

	//stream >> str;

	l = readChar();

	c = (char *) malloc(l + 5);

	if (c) {
		stream.read(c, l);
		c[l] = 0;
		str = string(c);
		free(c);
	}

	// Skip garbage after pascal string end
	c = (char *) malloc(maxlen + 5);
	stream.read(c, maxlen - l);
	free(c);

	return str;
}

string ConvertGtp::readWordPascalString()
{
	string str;
	char *c;

	int l = readDelphiInteger();

	c = (char *) malloc(l + 5);

	if (c) {
		stream.read(c, l);
		c[l] = 0;
		str = string(c);
		free(c);
	}

	return str;
}

int ConvertGtp::readDelphiInteger()
{
	int r;
	char tmp[1];
	unsigned char byte;
	if (stream.eof())  
		throw string("readDelphiInteger: EOF");
	stream.read(tmp,1);
	byte = tmp[0];
	r = byte;
	if (stream.eof())  
		throw string("readDelphiInteger: EOF");
	stream.read(tmp,1);
	byte = tmp[0];
	r += byte << 8;
	if (stream.eof())  
		throw string("readDelphiInteger: EOF");
	stream.read(tmp,1);
	byte = tmp[0];
	r += byte << 16;
	if (stream.eof())  
		throw string("readDelphiInteger: EOF");
	stream.read(tmp,1);
	byte = tmp[0];
	r += byte << 24;

	return r;
}

void ConvertGtp::readChromaticGraph(TabColumn &col)
{
	char num;
	int n;

	int t, p;

	// GREYFIX: currently just skips over chromatic graph
	num = readChar();                        // icon
	readDelphiInteger();                     // shown amplitude
	n = readDelphiInteger();                 // number of points
	for (int i = 0; i < n; i++) {
		t = readDelphiInteger();                 // time
		p = readDelphiInteger();                 // pitch
		if(p % 50 == 0)
			col.bend.push_back(osg::Vec2(t,p));
		num = readChar();                    // vibrato
	}
}

void ConvertGtp::readChord(vector<TabTrack>::iterator &trk, int x)
{
	int x1, x2, x3, x4;
	char num;
	string text;
	char garbage[50];
	// GREYFIX: currently just skips over chord diagram

	// GREYFIX: chord diagram
	x1 = readDelphiInteger();
	if (x1 != 257)
		osg::notify(osg::INFO) << "Chord INT1=" << x1 << ", not 257\n";
	x2 = readDelphiInteger();
	if (x2 != 0)
		osg::notify(osg::INFO) << "Chord INT2=" << x2 << ", not 0\n";
	x3 = readDelphiInteger();
	osg::notify(osg::INFO) << "Chord INT3: " << x3 << "\n"; // FF FF FF FF if there is diagram
	x4 = readDelphiInteger();
	if (x4 != 0)
		osg::notify(osg::INFO) << "Chord INT4=" << x4 << ", not 0\n";
	num = readChar();
	if (num != 0)
		osg::notify(osg::INFO) << "Chord BYTE5=" << (int) num << ", not 0\n";
	text = readPascalString(25);
	osg::notify(osg::INFO) << "Chord diagram: " << text << "\n";
	
	// Chord diagram parameters - for every string
	for (int i = 0; i < STRING_MAX_NUMBER; i++) {
		x1 = readDelphiInteger();
		trk->c[x].a[i] = x1;
		osg::notify(osg::INFO) << x1 << "\n";
	}
	
	// Unknown bytes
	stream.read(garbage, 36);

	osg::notify(osg::INFO) << "after chord, position: " << stream.tellg() << "\n";
}

void ConvertGtp::readSignature()
{
	currentStage = string("readSignature");

	string s = readPascalString(30);        // Format string
	osg::notify(osg::INFO) << "GTP format: \"" << s << "\"\n";

	// Parse version string
	if (s == "FICHIER GUITARE PRO v1") {
		versionMajor = 1; versionMinor = 0;
	} else if (s == "FICHIER GUITARE PRO v1.01") {
		versionMajor = 1; versionMinor = 1;
	} else if (s == "FICHIER GUITARE PRO v1.02") {
		versionMajor = 1; versionMinor = 2;
	} else if (s == "FICHIER GUITARE PRO v1.03") {
		versionMajor = 1; versionMinor = 3;
	} else if (s == "FICHIER GUITARE PRO v1.04") {
		versionMajor = 1; versionMinor = 4;
	} else if (s == "FICHIER GUITAR PRO v2.20") {
		versionMajor = 2; versionMinor = 20;
	} else if (s == "FICHIER GUITAR PRO v2.21") {
		versionMajor = 2; versionMinor = 21;
	} else if (s == "FICHIER GUITAR PRO v3.00") {
		versionMajor = 3; versionMinor = 0;
	} else if (s == "FICHIER GUITAR PRO v4.00") {
		versionMajor = 4; versionMinor = 0;
	} else if (s == "FICHIER GUITAR PRO v4.06") {
		versionMajor = 4; versionMinor = 6;
	} else if (s == "FICHIER GUITAR PRO L4.06") {
		versionMajor = 4; versionMinor = 6;
	} else {
		throw string("Invalid file format\n");
	}
}

void ConvertGtp::readSongAttributes()
{
	string s;

	char num;

	currentStage = string("readSongAttributes: song->info");

	song->info["TITLE"] = readDelphiString();
	song->info["SUBTITLE"] = readDelphiString();
	song->info["ARTIST"] = readDelphiString();
	song->info["ALBUM"] = readDelphiString();
	song->info["COMPOSER"] = readDelphiString();
	song->info["COPYRIGHT"] = readDelphiString();
	song->info["TRANSCRIBER"] = readDelphiString();
	song->info["INSTRUCTIONS"] = readDelphiString();

	// Notice lines
	currentStage = string("readSongAttributes: notice lines");
	song->info["COMMENTS"] = "";
	int n = readDelphiInteger();
	for (int i = 0; i < n; i++)
		song->info["COMMENTS"] += readDelphiString() + "\n";

	currentStage = string("readSongAttributes: shuffle rhythm feel");
	num = readChar();                        // GREYFIX: Shuffle rhythm feel

	if (versionMajor >= 4) {
		currentStage = string("readSongAttributes: lyrics");
		// Lyrics
		num = readDelphiInteger();                     // GREYFIX: Lyric track number start
		for (int i = 0; i < LYRIC_LINES_MAX_NUMBER; i++) {
			num = readDelphiInteger();                 // GREYFIX: Start from bar
			readWordPascalString();              // GREYFIX: Lyric line
		}
	}

	currentStage = string("readSongAttributes: tempo");
	song->tempo = readDelphiInteger();       // Tempo
	osg::notify(osg::INFO) << "tempo: " << song->tempo << "\n";

	int oct = -1;
	if (versionMajor >= 4) {
		num = readChar();                // GREYFIX: key
		oct = readDelphiInteger();             // GREYFIX: octave
	} else {
		readDelphiInteger();             // GREYFIX: key
	}
}

void ConvertGtp::readTrackDefaults()
{
	char t[1];
	char num, volume, pan, chorus, reverb, phase, tremolo;
	currentStage = string("readTrackDefaults");

	for (int i = 0; i < TRACK_MAX_NUMBER * 2; i++) {
		trackPatch[i] = readDelphiInteger(); // MIDI Patch
		volume = readChar();
		pan = readChar();                    // GREYFIX: pan
		chorus = readChar();                 // GREYFIX: chorus
		reverb = readChar();                 // GREYFIX: reverb
		phase = readChar();                 // GREYFIX: phase
		tremolo = readChar();                // GREYFIX: tremolo
		osg::notify(osg::INFO) << "=== TrackDefaults: " << i <<
			" (patch=" << trackPatch[i] <<
			" vol=" << (int) volume <<
			" p=" << (int) pan <<
			" c=" << (int) chorus <<
			" ph=" << (int) phase <<
			" tr=" << (int) tremolo << "\n";

		num = readChar();                    // 2 byte padding: must be 00 00
		if (num != 0)  osg::notify(osg::INFO) << "1 of 2 byte padding: there is %1, must be 0\n" << num;
		num = readChar();
		if (num != 0)  osg::notify(osg::INFO) << "2 of 2 byte padding: there is %1, must be 0\n" << num;
	}
}

void ConvertGtp::readBarProperties()
{
	char bar_bitmask, num;

	int time1 = 4;
	int time2 = 4;
	int keysig = 0;
	string text = "";

	bars.resize(numBars);

	currentStage = string("readBarProperties");
	osg::notify(osg::INFO) << "readBarProperties(): start\n";

	for (int i = 0; i < numBars; i++) 
	{
		bar_bitmask = readChar();                   // bar property bitmask
		if (bar_bitmask != 0)
			osg::notify(osg::INFO) << "BAR #" << i << " - flags " << (int) bar_bitmask << "\n";
		// GREYFIX: new_time_numerator
		if (bar_bitmask & 0x01) {
			num = readChar();
			time1 = num;
			osg::notify(osg::INFO) << "new time1 signature: " << time1 << ":" << time2 << "\n";
		}
		// GREYFIX: new_time_denominator
		if (bar_bitmask & 0x02) {
			num = readChar();
			time2 = num;
			osg::notify(osg::INFO) << "new time2 signature: " << time1 << ":" << time2 << "\n";
		}
		// GREYFIX: begin repeat
		if (bar_bitmask & 0x04) {
			osg::notify(osg::INFO) << "begin repeat\n";
		}
		// GREYFIX: number_of_repeats
		if (bar_bitmask & 0x08) {
			num = readChar();
			osg::notify(osg::INFO) << "end repeat " << (int) num << "x\n";
		}
		// GREYFIX: alternative_ending_to
		if (bar_bitmask & 0x10) {
			num = readChar();
			osg::notify(osg::INFO) << "alternative ending to " << (int) num << "\n";
		}
		// GREYFIX: new section
		if (bar_bitmask & 0x20) {
			text = readDelphiString();
			readDelphiInteger(); // color?
			osg::notify(osg::INFO) << "new section: " << text << "\n";
		}
		if (bar_bitmask & 0x40) {
			num = readChar();                // GREYFIX: alterations_number
			keysig = num;
			num = readChar();                // GREYFIX: minor
			osg::notify(osg::INFO) << "new key signature (" << keysig << ", " << num << ")\n";
		}
		// GREYFIX: double bar
		if (bar_bitmask & 0x80) {
			osg::notify(osg::INFO) << "double bar\n";
		}

		bars[i].start = 0;
		bars[i].time1 = time1;
		bars[i].time2 = time1;
		bars[i].keysig = keysig;
		bars[i].section = text;
	}
	osg::notify(osg::INFO) << "readBarProperties(): end\n";
}

void ConvertGtp::readTrackProperties()
{
	char num = readChar();
	int i;
	int strings, midiChannel2, capo, color;

	i=0;
	while(stream.peek() == 0)
	{
		i++;
		readChar();
	}

	char tmp[256];
	//stream.read(tmp, 256);

	currentStage = string("readTrackProperties");
	osg::notify(osg::INFO) << "readTrackProperties(): start\n";

	for (int i = 0; i < numTracks; i++) 
	{
		if(i>0) 
			num = readChar();                    // GREYFIX: simulations bitmask
		
		osg::notify(osg::INFO) << "Simulations: " << num << "\n";

		TabTrack *trk = new TabTrack(TabTrack::FretTab, "", 0, 0, 0, 6, 24);

		trk->name = readPascalString(40);    // Track name
		osg::notify(osg::INFO) << "Track: " << trk->name << "\n";

		// Tuning information

		osg::notify(osg::INFO) << "pos: " << stream.tellg() << "\n";

		strings = readDelphiInteger();
		if (strings <= 0 || strings > STRING_MAX_NUMBER)  
			throw string("Track %1: insane # of strings (%2)\n");
		trk->strings = strings;

		// Parse [0..string-1] with real string tune data in reverse order
		for (int j = trk->strings - 1; j >= 0; j--) {
			trk->tune[j] = readDelphiInteger();
			if (trk->tune[j] > 127)  
				throw string("Track %1: insane tuning on string %2 = %3\n");
		}

		// Throw out the other useless garbage in [string..MAX-1] range
		for (int j = trk->strings; j < STRING_MAX_NUMBER; j++)
			readDelphiInteger();

		// GREYFIX: auto flag here?

		readDelphiInteger();                 // GREYFIX: MIDI port
		trk->channel = readDelphiInteger();  // MIDI channel 1
		midiChannel2 = readDelphiInteger();  // GREYFIX: MIDI channel 2
		trk->frets = readDelphiInteger();    // Frets
		capo = readDelphiInteger();          // GREYFIX: Capo
		color = readDelphiInteger();         // GREYFIX: Color

		osg::notify(osg::INFO) <<
			"MIDI #" << trk->channel << "/" << (int) midiChannel2 << ", " <<
			(int) trk->strings << " strings, " <<
			(int) trk->frets << " frets, capo " <<
			capo << "\n";

		if (trk->frets <= 0 || (strongChecks && trk->frets > 100))  
			throw string("Track %1: insane number of frets (%2)\n");
		if (trk->channel > 16)  
			throw string("Track %1: insane MIDI channel 1 (%2)\n");
		if (midiChannel2 < 0 || midiChannel2 > 16) 
			throw string("Track %1: insane MIDI channel 2 (%2)\n");

		// Fill remembered values from defaults
		trk->patch = trackPatch[i];

		song->t.push_back(*trk);
	}
	osg::notify(osg::INFO) << "readTrackProperties(): end\n";
}

void ConvertGtp::readTabs()
{
	char beat_bitmask, strings, num;
	char length, volume, pan, chorus, reverb, phase, tremolo;
	int x;

	currentStage = string("readTabs");

	vector<TabTrack>::iterator trk = song->t.begin();
	for (int tr = 0; tr < numTracks; tr++) {
		trk->b.resize(numBars);
		trk->c.resize(0);
		trk++;
	}

	for (int j = 0; j < numBars; j++) {
		for (trk = song->t.begin(); trk != song->t.end(); trk++) {
			int numBeats = readDelphiInteger();
			//osg::notify(osg::INFO) << "TRACK " << tr << ", BAR " << j << ", numBeats " << numBeats << " (position: " << stream.tellg() << ")\n";

			if (numBeats < 0 || (strongChecks && numBeats > 128))  
				throw string("Track %1, bar %2, insane number of beats: %3");

			x = trk->c.size();
			trk->c.resize(trk->c.size() + numBeats);
			trk->b[j].time1 = bars[j].time1;
			trk->b[j].time2 = bars[j].time2;
			trk->b[j].keysig = bars[j].keysig;
			trk->b[j].start = x;
			trk->b[j].notes = numBeats;
			trk->b[j].tempo = -1;

			for (int k = 0; k < numBeats; k++) {
				trk->c[x].flags = 0;

				beat_bitmask = readChar();   // beat bitmask
				
				if (beat_bitmask & 0x01)     // dotted column
					trk->c[x].flags |= FLAG_DOT;

				if (beat_bitmask & 0x40) {
					num = readChar();        // GREYFIX: pause_kind
				}

				// Guitar Pro 4 beat lengths are as following:
				// -2 = 1    => 480     3-l = 5  2^(3-l)*15
				// -1 = 1/2  => 240           4
				//  0 = 1/4  => 120           3
				//  1 = 1/8  => 60            2
				//  2 = 1/16 => 30 ... etc    1
				//  3 = 1/32 => 15            0

				length = readChar();            // length
				//osg::notify(osg::INFO) << "beat_bitmask: " << (int) beat_bitmask << "; length: " << length << "\n";

				float len = 1.0f / (1 << (2 + length));
				trk->c[x].l = len;

				if (beat_bitmask & 0x20) {
					int tuple = readDelphiInteger();
					osg::notify(osg::INFO) << "Tuple: " << tuple << "\n"; // GREYFIX: t for tuples
					if (!(tuple == 3 || (tuple >= 5 && tuple <= 7) || (tuple >= 9 && tuple <= 13)))  
						throw string("Insane tuple t: %1");
				}
				
				if (beat_bitmask & 0x02)     // Chord diagram
				{
					readChord(trk, x);
				}

				if (beat_bitmask & 0x04) {
					readDelphiString();
				}  

				// GREYFIX: column-wide effects
				if (beat_bitmask & 0x08)
				{
					readColumnEffects(trk, x);
				}

				if (beat_bitmask & 0x10) {     // mixer variations
					num = readChar();          // GREYFIX: new MIDI patch
					volume = readChar();       // GREYFIX: new
					pan = readChar();          // GREYFIX: new
					chorus = readChar();       // GREYFIX: new
					reverb = readChar();       // GREYFIX: new
					phase = readChar();        // GREYFIX: new
					tremolo = readChar();      // GREYFIX: new
					int tempo = readDelphiInteger(); // GREYFIX: new tempo

					trk->b[j].tempo = tempo;

					// GREYFIX: transitions
					if (volume != -1)   num = readChar();
					if (pan != -1)      num = readChar();
					if (chorus != -1)   num = readChar();
					if (reverb != -1)   num = readChar();
					if (tremolo != -1)  num = readChar();
					if (tempo != -1)    num = readChar();

					if (versionMajor >= 4) {
						num = readChar();          // bitmask: what should be applied to all tracks
					}
				}

				strings = readChar();          // used strings mask
				
				for (int y = STRING_MAX_NUMBER - 1; y >= 0; y--) {
					trk->c[x].e[y] = 0;
					trk->c[x].a[y] = NULL_NOTE;
					if (strings & (1 << (y + STRING_MAX_NUMBER - trk->strings)))
						readNote(trk, x, y);
				}
				
				// Dump column
				string tmp = "";
				for (int y = 0; y <= trk->strings; y++) {
					if (trk->c[x].a[y] == NULL_NOTE) {
						tmp += ".";
					} else {
						tmp += '0' + trk->c[x].a[y];
					}
				}
				//osg::notify(osg::INFO) << "[" << tmp << "]\n";
				
				x++;
			}
		}
	}
}

void ConvertGtp::readColumnEffects(vector<TabTrack>::iterator &trk, int x)
{
	char fx_bitmask1 = 0, fx_bitmask2 = 0, num;

	fx_bitmask1 = readChar();
	if (versionMajor >= 4) {
		fx_bitmask2 = readChar();
		osg::notify(osg::INFO) << "column-wide fx: " << (int) fx_bitmask1 << "/" << (int) fx_bitmask2 << "\n";
	} else {
		osg::notify(osg::INFO) << "column-wide fx: " << (int) fx_bitmask1 << "\n";
	}

	if (fx_bitmask1 & 0x20) {      // GREYFIX: string torture
		num = readChar();
		switch (num) {
		case 0:                    // GREYFIX: tremolo bar
			if (versionMajor < 4)  readDelphiInteger();
			break;
		case 1:                    // GREYFIX: tapping
			if (versionMajor < 4)  readDelphiInteger(); // ?
			break;
		case 2:                    // GREYFIX: slapping
			if (versionMajor < 4)  readDelphiInteger(); // ?
			break;
		case 3:                    // GREYFIX: popping
			if (versionMajor < 4)  readDelphiInteger(); // ?
			break;
		default:
			throw string("Unknown string torture effect: %1");
		}
	}
	if (fx_bitmask1 & 0x04) {      // GP3 column-wide natural harmonic
		osg::notify(osg::INFO) << "GP3 column-wide natural harmonic\n";
		for (int y = 0; y < trk->strings; y++)
			trk->c[x].e[y] |= EFFECT_HARMONIC;
	}
	if (fx_bitmask1 & 0x08) {      // GP3 column-wide artificial harmonic
		osg::notify(osg::INFO) << "GP3 column-wide artificial harmonic\n";
		for (int y = 0; y < trk->strings; y++)
			trk->c[x].e[y] |= EFFECT_ARTHARM;
        }
	if (fx_bitmask2 & 0x04)
		readChromaticGraph(trk->c[x]);  // GREYFIX: tremolo graph
	if (fx_bitmask1 & 0x40) {
		num = readChar();      // GREYFIX: down stroke length
		num = readChar();      // GREYFIX: up stroke length
	}
	if (fx_bitmask2 & 0x02) {
		num = readChar();      // GREYFIX: stroke pick direction
	}
	if (fx_bitmask1 & 0x01) {      // GREYFIX: GP3 column-wide vibrato
	}
	if (fx_bitmask1 & 0x02) {      // GREYFIX: GP3 column-wide wide vibrato (="tremolo" in GP3)
	}
}

void ConvertGtp::readNote(vector<TabTrack>::iterator &trk, int x, int y)
{
	char note_bitmask, variant, num, mod_mask1, mod_mask2;

	note_bitmask = readChar();               // note bitmask
	variant = readChar();                    // variant

	if (note_bitmask) {
		//osg::notify(osg::INFO) << "note_bitmask: " << (int) note_bitmask << "\n";
	}

	if (note_bitmask & 0x01) {               // GREYFIX: note != beat
		num = readChar();                    // length
		num = readChar();                    // t
	}

	if (note_bitmask & 0x02) {};             // GREYFIX: note is dotted

	if (note_bitmask & 0x10) {               // GREYFIX: dynamic
		num = readChar();
	}

	num = readChar();                        // fret number
	trk->c[x].a[y] = num;

	if (variant == 2) {                      // link with previous beat
		trk->c[x].flags |= FLAG_ARC;
		//for (int i = 0; i < MAX_STRINGS; i++) {
			//trk->c[x].a[i] = NULL_NOTE;
			trk->c[x].a[y] = NULL_NOTE;
			//trk->c[x].e[i] = 0;
		//}
	}

	if (variant == 3)                        // dead notes
		trk->c[x].a[y] = DEAD_NOTE;

	if (note_bitmask & 0x80) {               // GREYFIX: fingering
		num = readChar();
		num = readChar();
	}

	if (note_bitmask & 0x08) {
		mod_mask1 = readChar();
		if (versionMajor >= 4) {
			mod_mask2 = readChar();
			osg::notify(osg::INFO) << "note mod: mask1=" << mod_mask1 << " mask2=" << mod_mask2 << "\n";
		} else {
			osg::notify(osg::INFO) << "note mod: mask1=" << mod_mask1 << "\n";
		}
		if (mod_mask1 & 0x01) {
			trk->c[x].e[y] |= EFFECT_BEND;
			readChromaticGraph(trk->c[x]);            // GREYFIX: bend graph
		}
		if (mod_mask1 & 0x02)                // hammer on / pull off
			trk->c[x].e[y] |= EFFECT_LEGATO;
		if (mod_mask1 & 0x08)                // let ring
			trk->c[x].e[y] |= EFFECT_LETRING;
		if (mod_mask1 & 0x10) {              // GREYFIX: graces
			num = readChar();                // GREYFIX: grace fret
			num = readChar();                // GREYFIX: grace dynamic
			num = readChar();                // GREYFIX: grace transition
			num = readChar();                // GREYFIX: grace length
		}
		if (versionMajor >= 4) {
			if (mod_mask2 & 0x01)                // staccato - we do palm mute
				trk->c[x].flags |= FLAG_PM;
			if (mod_mask2 & 0x02)                // palm mute - we mute the whole column
				trk->c[x].flags |= FLAG_PM;
			if (mod_mask2 & 0x04) {              // GREYFIX: tremolo
				num = readChar();                // GREYFIX: tremolo picking length
			}
			if (mod_mask2 & 0x08) {              // slide
				trk->c[x].e[y] |= EFFECT_SLIDE;
				num = readChar();                // GREYFIX: slide kind
			}
			if (mod_mask2 & 0x10) {              // GREYFIX: harmonic
				num = readChar();                // GREYFIX: harmonic kind
			}
			if (mod_mask2 & 0x20) {              // GREYFIX: trill
				num = readChar();                // GREYFIX: trill fret
				num = readChar();                // GREYFIX: trill length
			}
		}
	}
}

map<string, string> ConvertGtp::header(string fileName)
{
	stream.open(fileName.c_str() , ios::binary);

	try 
	{
		song = new TabSong();
		
		if(stream.fail())
			return song->info;

		readSignature();
		song->t.clear();
		readSongAttributes();
	}
	catch (string msg) 
	{
		osg::notify(osg::WARN) << "Error loading Header: " << msg;
		song->info.clear();
	}
	catch ( exception e )
	{
		osg::notify(osg::WARN) << "Error loading Header: " << e.what();	
		song->info.clear();
	}

	stream.close();
	return song->info;
}

TabSong* ConvertGtp::load(string fileName)
{
	stream.open(fileName.c_str() , ios::binary);
	if(stream.fail())
		return NULL;

	int size = stream.tellg();

	try {
		song = new TabSong();

		readSignature();
		song->t.clear();
		readSongAttributes();
	 	readTrackDefaults();

		//char st[50];
		//stream.read(st, 50);

	 	numBars = readDelphiInteger();           // Number of bars
		if (numBars <= 0 || (strongChecks && numBars > 16384))  
			throw string("Insane number of bars: %1");
		osg::notify(osg::INFO) << "Bars: " << numBars << "\n";

	 	numTracks = readDelphiInteger();         // Number of tracks
		if (numTracks <= 0 || (strongChecks && numTracks > 32))  
			throw string("Insane number of tracks: %1");
		osg::notify(osg::INFO) << "Tracks: " << numTracks << "\n";

	 	readBarProperties();
	 	readTrackProperties();
	 	readTabs();

		currentStage = string("Exit code");
		if (stream.eof()) {
			int ex = readDelphiInteger();            // Exit code: 00 00 00 00
			if (ex != 0)
				osg::notify(osg::INFO) << "File not ended with 00 00 00 00\n";
			if (!stream.eof())
				osg::notify(osg::INFO) << "File not ended - there's more data!\n";
		}
	} 
	catch (string msg) 
	{
		osg::notify(osg::WARN) << "Error loading Tab: " << msg;
		stream.close();
		return NULL;
	}
	catch ( exception e )
	{
		osg::notify(osg::WARN) << "Error loading Tab: " << e.what();
		stream.close();
		return NULL;
	}

	stream.close();

	return song;
}
