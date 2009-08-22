//#include "accidentals.h"
//#include "settings.h"
#include "tabtrack.h"
//#include "playbacktracker.h"

// local functions used by beam code
// LVIFIX: change into private TabTrack member functions

static char beamL1(int t, int v, int bn, TabTrack *trk);
static char beamL2plus(int t, int v, int bn, int lvl, TabTrack *trk);
static bool isRest(int t, TabTrack *trk);
static bool mustBreakBeam(int t, int bn, TabTrack *trk);
static int tStart(int t, int bn, TabTrack *trk);

// determine if column t in track trk is a rest

static bool isRest(int t, TabTrack *trk)
{
	for (int i = 0; i < trk->strings; i++) {
		if (trk->c[t].a[i] > -1) {
			return false;
		}
	}
	return true;
}

// determine starting time of note t in bar bn of track trk

static int tStart(int t, int bn, TabTrack *trk)
{
	int tstart = 0;
	for (int i = trk->b[bn].start; (int) i < t; i++) {
		// add real duration including dots/triplets
		tstart += trk->c[i].fullDuration();
	}
	return tstart;
}

// determine if beam needs to be broken at note t in track trk for bar bn
// break beam if t does not end in the beat where it started
// results in drawing three beams for six 1/8 notes in a 3/4 measure
// this is independent of voice allocation

static bool mustBreakBeam(int t, int bn, TabTrack *trk)
{
	int bv = 1;					// this bar's beat value as duration
	switch (trk->b[bn].time2) {
	case  1: bv = 480; break;
	case  2: bv = 240; break;
	case  4: bv = 120; break;
	case  8: bv =  60; break;
	case 16: bv =  30; break;
	case 32: bv =  15; break;
	default: bv =   1; break;	// safe default
	}
	int ts = tStart(t, bn, trk);			// note start
	int te = ts + trk->c[t].fullDuration();	// note end
	int bs = ts / bv;						// beat where note starts
	int be = te / bv;						// beat where note ends
	return (bs != be);
}

// determine L1 beam for note t in voice v in bar bn of track trk
// returns: c (continue) e (end) n (none) s (start)
// LVIFIX: save results ?

static char beamL1(int t, int v, int bn, TabTrack *trk)
{
	// if column is a rest, then no beam
	if (isRest(t, trk)) {
		return 'n';
	}
	// if no note in this voice, then no beam
	int dt;						// dots (not used)
	int tp;						// note type
	bool tr;					// triplet (not used)
	if (!trk->getNoteTypeAndDots(t, v, tp, dt, tr)) {
		return 'n';
	}
	// if note is 1/4 or longer, then no beam
	if (tp >= 120) {
		return 'n';
	}

	int f = trk->b[bn].start;	// first note of bar
	int l = trk->lastColumn(bn);// last note of bar
	int p = 0;					// previous note
	int n = 0;					// next nnote
	p = (t == f) ? -1 : (t - 1);
	n = (t == l) ? -1 : (t + 1);
	int ptp = 480;				// previous note type, default to 1/1
	int ntp = 480;				// next note type, default to 1/1
	if ((p == -1) || !trk->getNoteTypeAndDots(p, v, ptp, dt, tr)) {
		// no previous note (or not in this voice),
		// therefore pretend 1/1 (which has no stem)
		ptp = 480;
	}
	if ((n == -1) || !trk->getNoteTypeAndDots(n, v, ntp, dt, tr)) {
		// no previous note (or not in this voice),
		// therefore pretend 1/1 (which has no stem)
		ntp = 480;
	}
	if (mustBreakBeam(t, bn, trk)) {
		// note ends at n * divisor
		if ((p != -1) && (ptp <= 60) && !mustBreakBeam(p, bn, trk)
			&& !isRest(p, trk)) {
			// previous note exists which has beam to this one
			return 'e';
		} else {
			return 'n';
		}
	} else {
		// note does not end at n * divisor
		bool left = false;		// beam at left side ?
		bool right = false;		// beam at right side ?
		if ((p != -1) && (ptp <= 60) && !mustBreakBeam(p, bn, trk)
			&& !isRest(p, trk)) {
			// previous note exists which has beam to this one
			left = true;
		}
		if ((n != -1) && (ntp <= 60) && !isRest(n, trk)) {
			// next note exists to draw beam to
			right = true;
		}
		// test all possible combinations of left and right
		if (left && right) {
			return 'c';
		}
		if (left && !right) {
			return 'e';
		}
		if (!left && right) {
			return 's';
		}
		if (!left && !right) {
			return 'n';
		}
	}
	return 'n';
}

// determine beam for note t in voice v in bar bn in track trk
// at beam level lvl
// returns: b (backward) c (continue) e (end) f (forward) n (none) s (start)
// note: no need to check for rests (done in beamL1)
// LVIFIX: save results ?

static char beamL2plus(int t, int v, int bn, int lvl, TabTrack *trk)
{
	// if no note in this voice, then no beam
	int dt;						// dots (not used)
	int tp;						// note type
	bool tr;					// triplet (not used)
	if (!trk->getNoteTypeAndDots(t, v, tp, dt, tr)) {
		return 'n';
	}
	// determine duration for this level
	int dur = 0;
	if (lvl == 2) {
		// if note is 1/8 or longer, then no beam
		if (tp >= 60) {
			return 'n';
		} else {
			dur = 30;
		}
	} else if (lvl == 3) {
		// if note is 1/16 or longer, then no beam
		if (tp >= 30) {
			return 'n';
		} else {
			dur = 15;
		}
	} else {
		return 'n';
	}
	int f = trk->b[bn].start;	// first note of bar
	int l = trk->lastColumn(bn);// last note of bar
	int p = 0;					// previous note
	int n = 0;					// next next
	p = (t == f) ? -1 : (t - 1);
	n = (t == l) ? -1 : (t + 1);
	int ptp = 480;				// previous note type, default to 1/1
	int ntp = 480;				// next note type, default to 1/1
	if ((p == -1) || !trk->getNoteTypeAndDots(p, v, ptp, dt, tr)) {
		// no previous note (or not in this voice),
		// therefore pretend 1/1 (which has no stem)
		ptp = 480;
	}
	if ((n == -1) || !trk->getNoteTypeAndDots(n, v, ntp, dt, tr)) {
		// no previous note (or not in this voice),
		// therefore pretend 1/1 (which has no stem)
		ntp = 480;
	}
	char L1 = beamL1(t, v, bn, trk);
	if (L1 == 's') {
		if ((n != -1) && (ntp <= dur)) {
			return 's';
		} else {
			return 'f';
		}
	} else if (L1 == 'c') {
		bool left = false;		// beam at left side ?
		bool right = false;		// beam at right side ?
		if ((p != -1) && (ptp <= dur) && !(mustBreakBeam(p, bn, trk))) {
			// previous note exists which has beam to this one
			left = true;
		}
		if ((n != -1) && (ntp <= dur)) {
			// next note exists to draw beam to
			right = true;
		}
		// test all possible combinations of left and right
		if (left && right) {
			return 'c';
		}
		if (left && !right) {
			return 'e';
		}
		if (!left && right) {
			return 's';
		}
		if (!left && !right) {
			return 'f';
		}
	} else if (L1 == 'e') {
		if ((p != -1) && (ptp <= dur)) {
			// previous note exists which has beam to this one
			return 'e';
		} else {
			return 'b';
		}
	} else {
		return 'n';
	}
	return 'n';
}

TabTrack::TabTrack(TrackMode _tm, string _name, int _channel,
				   int _bank, int _patch, char _string, char _frets)
{
	tm=_tm;
	name=_name;
	channel=_channel;
	bank=_bank;
	patch=_patch;
	strings=_string;
	frets=_frets;

	// Simple & brutal initialization follows
	// GREYFIX: change to something that makes more sense

	int standtune[6] = {40, 45, 50, 55, 59, 64};

	for (int i = 0; i < 6; i++)
		tune[i] = standtune[i];

	c.resize(1);
	b.resize(1);

	for (int i = 0; i < MAX_STRINGS; i++) {
		c[0].a[i] = -1;
		c[0].e[i] = 0;
	}
	c[0].l = 120;
	c[0].flags = 0;

	b[0].start = 0;
	b[0].time1 = 4;
	b[0].time2 = 4;
	b[0].keysig = 0;

	x = 0;
	xb = 0;
	y = 0;

	sel = false;
	xsel = 0;
}

// Pretty sophisticated expression that determines if we can omit the time sig
bool TabTrack::showBarSig(int n)
{
	return !((n > 0) &&
			 (b[n - 1].time1 == b[n].time1) &&
			 (b[n - 1].time2 == b[n].time2));
}

// Returns the bar number for column c
int TabTrack::barNr(int c)
{
	unsigned int i = 0;
	int res = -1;
	while (i < b.size()) {
		if (i + 1 >= b.size()) {
			if (b[i].start <= c)
				break;
		} else {
			if ((b[i].start <= c) && (c < b[i+1].start))
				break;
		}
		i++;
	}
	// LVIFIX: what to do if c < 0 ?
	if (c < 0)
		res = -1;
	else
		res = i;
	return res;
}

// Returns the column that ends bar <n>. Thus bar <n> is all columns
// from b[n].start to lastColumn(n) inclusive
int TabTrack::lastColumn(int n)
{
	int last;
	if (b.size() == n + 1)       // Current bar is the last one
		last = c.size() - 1;      // Draw till the last note
	else							    // Else draw till the end of this bar
		last = b[n + 1].start - 1;
	if (last == -1)  last = 0;          // gotemfix: avoid overflow
	return last;
}

// Returns bar status - what to show in track pane
bool TabTrack::barStatus(int n)
{
	if (n < 0 || n >= b.size())
		return false;

	bool res = false;

	for (int i = b[n].start; i <= lastColumn(n); i++) {
		for (int k = 0; k < strings; k++) {
			if (c[i].a[k] != -1) {
				res = true;
				break;
			}
		}
		if (res)
			break;
	}

	return res;
}

// Returns a cumulative duration of all columns that belong to current
// bar, determined with track parameters
size_t TabTrack::currentBarDuration()
{
	size_t dur = 0;
	for (int i = b[xb].start; i <= lastColumn(xb); i++)
		dur += c[i].fullDuration();
	return dur;
}

// Returns a cumulative duration of all columns that belong to current
// track, determined with track parameters
int TabTrack::trackDuration()
{
	int dur = 0;
	for (int i = 0; i < c.size(); i++)
		dur += c[i].fullDuration();
	return dur;
}

// Calculates and returns maximum current bar duration, based on time
// signature
size_t TabTrack::maxCurrentBarDuration()
{
	return 4 * 120 * b[xb].time1 / b[xb].time2;
}

// Returns the duration of the note in column t and string i
// Ringing and bar end are taken into account
size_t TabTrack::noteDuration(size_t t, int i)
{
	size_t dur = 0;
	for (int j = 0; j < noteNrCols(t, i); j++)
		dur += c[t + j].fullDuration();
	return dur;
}

// Returns the number of columns used by the note in column t and string i
// Ringing and bar end are taken into account

int TabTrack::noteNrCols(size_t t, int i)
{
	if ((t >= c.size()) || (i < 0) || (i >= strings)) {
		return 1;
	}
	if (c[t].a[i] == NULL_NOTE) {
		return 1;
	}
	if (c[t].e[i] == EFFECT_LETRING) {
		int b  = barNr(t);
		int lc = lastColumn(b);	// last column of current bar
		// ringing: find next note, stop ringing or end of bar
		if (t == lc) {
			// note is in last column of bar
			return 1;
		}
		int cc = t + 1;			// current column
		while ((cc < lc)
				&& (c[cc].a[i] == NULL_NOTE)
				&& (c[cc].e[i] != EFFECT_STOPRING))
			cc++;
		int res = cc - t;
		// if necessary, add last column of bar
		if ((cc == lc)
			&& (c[cc].a[i] == NULL_NOTE)
			&& (c[cc].e[i] != EFFECT_STOPRING))
				res++;
		return res;
	}
	return 1;
}

// Returns the column number of the note that contains start time t
// Sets dur to the offset of t within the column
int TabTrack::findCStart(int t, int & dur)
{
	int res = -1;
	int tstart = 0;
	dur = 0;
	if ((t < 0) || (t >= trackDuration()))
		return -1;
	for (int i = 0; i < c.size(); i++) {
		if ((tstart <= t) && (t < (tstart + c[i].fullDuration()))) {
			res = i;
			dur = t - tstart;
		}
		tstart += c[i].fullDuration();
	}
	return res;
}

// Returns the column number of the note that contains end time t
// Sets dur to the offset of t within the column
int TabTrack::findCEnd(int t, int & dur)
{
	int res = -1;
	int tstart = 0;
	dur = 0;
	if ((t <= 0) || (t > trackDuration()))
		return -1;
	for (int i = 0; i < c.size(); i++) {
		if ((tstart < t) && (t <= (tstart + c[i].fullDuration()))) {
			res = i;
			dur = t - tstart;
		}
		tstart += c[i].fullDuration();
	}
	return res;
}

// Returns if string str is ringing at the start of column col
bool TabTrack::isRingingAt(int str, int col)
{
	int bn = barNr(col);
	bool res = false;
	for (int i = b[bn].start; i < col; i++) {
		if ((c[i].a[str] >= 0) || (c[i].e[str] == EFFECT_STOPRING)) {
			res = false;
		}
		if ((c[i].a[str] >= 0) && (c[i].e[str] == EFFECT_LETRING)) {
			res = true;
		}
	}
	return res;
}

/*
// LVIFIX: remove this
void dumpCols(TabTrack * trk)
{
	cout << "column duration:";
	for (int i = 0; i < trk->c.size(); i++) {
		cout << " " << trk->c[i].fullDuration();
	}
	cout << endl;
}
*/

// Inserts n columns at current cursor position
void TabTrack::insertColumn(int n)
{
//	cout << "TabTrack::insertColumn(" << n << ") x=" << x << endl;
	c.resize(c.size() + n);
	for (int i = c.size() - n; i > x; i--) {
//		cout << "copy column " << i - n << " to column " << i << endl;
		c[i] = c[i - n];
	}
	for (int i = 0; i < n; i++) {
//		cout << "initialize column " << x + i << endl;
		for (int j = 0; j < MAX_STRINGS; j++) {
			 c[x + i].a[j] = -1;
			 c[x + i].e[j] = 0;
		}
	}
}

// Inserts column(s) such that a note can start at ts and end at te
// Sets current position to starting column
// Returns note duration in columns
// Ignores bar structure, therefore use only in last bar
int TabTrack::insertColumn(int ts, int te)
{
//	cout << "TabTrack::insertColumn("
//		<< ts << ", " << te << ")" << endl;
	int cstart;
	int cend;
	int dur;
	int res = 0;

	if ((ts<0) || (te<=ts)) {
		// LVIFIX: report error ?
//		cout << "TabTrack::insertColumn() -> input error" << endl;
		return -1;
	}

	int td = trackDuration();
//	cout << "td=" << td << endl;
	if (ts > td) {
//		cout << "TabTrack::insertColumn() ->"
//			<< " ts > td, append rest: " << ts - td
//			<< endl;
		x = c.size();
		insertColumn(1);
		c[x].flags = 0;
		c[x].setFullDuration(ts - td);
//		cout << "stop ringing:";
		for (int j = 0; j < MAX_STRINGS; j++) {
			if (isRingingAt(j, x)) {
//				cout << " " << j;
				c[x].e[j] = EFFECT_STOPRING;
			}
		}
//		cout << endl;
//		cout << "after append: ";
//		dumpCols(this);
		td = ts;
	}
	if (te > td) {
//		cout << "TabTrack::insertColumn() ->"
//			<< " te > td, append column: " << te - td
//			<< endl;
		x = c.size();
		insertColumn(1);
		c[x].flags = 0;
		c[x].setFullDuration(te - td);
//		cout << "stop ringing:";
		for (int j = 0; j < MAX_STRINGS; j++) {
			if (isRingingAt(j, x)) {
//				cout << " " << j;
				c[x].e[j] = EFFECT_STOPRING;
			}
		}
//		cout << endl;
//		cout << "after append: ";
//		dumpCols(this);
		td = te;
	}
	// find starting column
//	cout << "TabTrack::insertColumn() ->"
//		<< " find starting column";
	cstart = findCStart(ts, dur);
//	cout
//		<< " cstart=" << cstart
//		<< " dur=" << dur
//		<< endl;
	if (dur > 0) {
		splitColumn(cstart, dur);
		cstart++;
//		cout << "after split start: ";
//		dumpCols(this);
	}
	// find ending column
//	cout << "TabTrack::insertColumn() ->"
//		<< " find ending column";
	cend = findCEnd(te, dur);
//	cout
//		<< " cend=" << cend
//		<< " dur=" << dur
//		<< endl;
	if (dur < c[cend].fullDuration()) {
		splitColumn(cend, dur);
//		cout << "after split end: ";
//		dumpCols(this);
	}

	x = cstart;
	res = cend - cstart + 1;
//	cout << "TabTrack::insertColumn() ->"
//		<< " x=" << x
//		<< " res=" << res
//		<< endl;
	return res;
}

// Split column col at dur
void TabTrack::splitColumn(int col, int dur)
{
	if ((col < 0) || (c.size() <= col))
		return;
	int prevdur = c[col].fullDuration();
	if ((dur < 0) || (prevdur <= dur))
		return;
	x = col + 1;
	insertColumn(1);
	c[x - 1].setFullDuration(dur);
	c[x].flags = 0;
	c[x].setFullDuration(prevdur - dur);
	// set existing notes ringing
	for (int j = 0; j < MAX_STRINGS; j++) {
		if (c[x - 1].a[j] >= 0) {
			c[x - 1].e[j] = EFFECT_LETRING;
		}
	}
	// stop ringing at column x+1 (if it exists)
	// needed only if x-1 has note and x+1 hasn't
	if (x < (c.size() - 1)) {
		for (int j = 0; j < MAX_STRINGS; j++) {
			if ((c[x - 1].a[j] >= 0) && (c[x + 1].a[j] < 0)) {
				c[x + 1].e[j] = EFFECT_STOPRING;
			}
		}
	}
}

// Removes n columns starting with current cursor position
void TabTrack::removeColumn(int n)
{
	for (int i = x; i < c.size() - n; i++)
		c[i]=c[i+n];

	// Remove empty bars
	while (c.size()-n<=b[b.size()-1].start)
		b.resize(b.size()-1);

	c.resize(c.size()-n);

	if (x >= c.size())
		x = c.size() - 1;

	if (xb >= b.size())
		xb = b.size() - 1;
}

// Toggles effect FX on current cursor position
// Stop ringing is the only effect allowed when no note is played at x,y
void TabTrack::addFX(char fx)
{
	if ((c[x].a[y]>=0) || ((c[x].a[y]==NULL_NOTE) && (fx==EFFECT_STOPRING))) {
		if (c[x].e[y] != fx)
			c[x].e[y] = fx;
		else
			c[x].e[y] = 0;
	}
}

// Find what measure the cursor is in by searching measure list
void TabTrack::updateXB()
{
	if (x>=b[b.size()-1].start)
		xb = b.size()-1;
	else
		for (int i=0; i<b.size()-1; i++)
			if ((x>=b[i].start) && (x<b[i+1].start)) {
				xb = i;
				break;
			}
}

// Add column "dat" to the end of the track, making this column span
// as many real columns as needed to sum up to duration of "value"

void TabTrack::addNewColumn(TabColumn dat, int value, bool *arc)
{
	// Cleverly sorted lookup array of all possible "standard" note durations
	// Norm  Dot  Triplet
	const int bits[] = {
		     720,
		480, 360, 320,
		240, 180, 160,
		120, 90,  80,
		60,  45,  40,
		30,  23,  20,
		15,       10,
		0
	};

	int toput = value; // What's left to distribute

	while (toput > 0) {
		int bit = toput;

		for (int i = 0; bits[i]; i++)
			if (toput >= bits[i]) {
				bit = bits[i];
				break;
			}

		toput -= bit;

		int last = c.size();
		c.resize(last + 1);
		c[last] = dat;
		c[last].setFullDuration(bit);
		if (*arc) {
			c[last].flags |= FLAG_ARC;
			for (int i = 0; i < MAX_STRINGS; i++)
				c[last].a[i] = -1;
		}
		*arc = true;
	}
}

// Arrange the track: collect all columns into an additional array,
// then add them one by one to clean track, taking care about bar
// limits

void TabTrack::arrangeBars()
{
	int barnum = 1;

	// COLLECT ALL NOTES INFORMATION

	vector<TabColumn> an;        // Collected columns information
	int nn = 0;                     // Number of already made columns

	for (int i = 0; i < c.size(); i++) {
		if (!(c[i].flags & FLAG_ARC)) { // Add the note, if it's not arc
			nn++;
			an.resize(nn);
			an[nn-1] = c[i];
			an[nn-1].l = c[i].fullDuration();
		} else {						// Add to duration of previous note
			an[nn-1].l += c[i].fullDuration();
		}
	}

	// RECONSTRUCTING BARS & COLUMNS ARRAYS

	bool arc;
	int cl;
	int cbl = 480 * b[0].time1 / b[0].time2;

	b[0].start = 0;
	barnum = 0;

	c.resize(0);

	for (nn = 0; nn < an.size(); nn++) {
		cl = an[nn].l;
		arc = false;

		while (cl > 0) {
			if (cl < cbl) {
				addNewColumn(an[nn], cl, &arc);
				cbl -= cl;
				cl = 0;
			} else {
				addNewColumn(an[nn], cbl, &arc);
				cl -= cbl;

				barnum++;
				if (b.size() < barnum + 1) {
					b.resize(barnum + 1);
					b[barnum].time1 = b[barnum-1].time1;
					b[barnum].time2 = b[barnum-1].time2;
				}
				b[barnum].start = c.size();
				cbl = 480 * b[barnum].time1 / b[barnum].time2;
			}
		}
	}

	// Clean up last bar if it's empty
	if (b[barnum].start == c.size())
		b.resize(barnum);

	// Make sure that cursor x is in legal range
	if (x >= c.size())
		x = c.size() - 1;

	// Find the bar the cursor is in
	updateXB();
}

#ifdef WITH_TSE3
// Generate a single midi data list for TSE3 from all current track
// tabulature.
TSE3::PhraseEdit *TabTrack::midiTrack(bool tracking, int tracknum)
{
	TSE3::PhraseEdit *midi = new TSE3::PhraseEdit();

	long timer = 0;				// midi timestamp for each note
	int midilen = 0;			// midi ticks until start of next note
	int duration;				// note duration (dead note: less than midilen)
	uchar pitch;				// note pitch
	const int velocity = 0x60;	// note velocity

	// Ugly MIDI shift between real start of notes and things like
	// patch/bank changes, etc. Required for stupid MIDI devices that
	// can't handle patch/bank change and note on event on the same tick.
	int midiShift = tracking ? 5 : 0;

	cursortimer = -1;

	for (uint x = 0; x < c.size(); x++) {

		// Calculate real duration (including all the linked beats)
		// for the non-ringing notes, which determines midilen
		// note: need to keep x unchanged, because pitch and effects are stored
		// in the first column of a set linked beats
		// remember x of the last linked beat, though

		int xl;					// x last linked note
		xl = x;
		midilen = c[xl].fullDuration();
		while ((xl + 1 < c.size()) && (c[xl + 1].flags & FLAG_ARC)) {
			xl++;
			midilen += c[xl].fullDuration();
		}

		if (x == this->x || (cursortimer == -1 && x > this->x))
			cursortimer = timer;

		// Note on/off events
		for (int i = 0; i < string; i++) {
			if (c[x].a[i] == -1)  continue;

			if (c[x].a[i] == DEAD_NOTE) {
				pitch = tune[i];
				duration = 5;
			} else {
				pitch = c[x].a[i] + tune[i];
				duration = midilen;
			}

			if (c[x].flags & FLAG_PM)
				duration = duration / 2;

			// ringing overrides the duration
			if (c[x].e[i] == EFFECT_LETRING) {
				// LVIFIX: add support for notes linked to ringing note
				// LVIFIX: the linked note should be able to be ringing itself
				duration = noteDuration(x, i);
			}

			if (c[x].e[i] == EFFECT_ARTHARM)
				pitch += 12;
			if (c[x].e[i] == EFFECT_HARMONIC) {
				switch (c[x].a[i]) {
				case 3:  pitch += 28; break;
				case 4:  pitch += 24; break;
				case 5:  pitch += 19; break;
				case 7:  pitch += 12; break;
				case 9:  pitch += 19; break;
				case 12: pitch += 0;  break;
				case 16: pitch += 12; break;    // same as 9th fret
				case 19: pitch += 0;  break;	// same as 7th fret
				case 24: pitch += 0;  break;    // same as 5th fret
				}
			}

// 			midi->insert(
// 				TSE3::MidiEvent(TSE3::MidiCommand(TSE3::MidiCommand_NoteOn,
// 				                                  channel - 1, Settings::midiPort(),
// 				                                  pitch, velocity),
// 				                timer + midiShift, velocity, timer + duration));

			midi->insert(
				TSE3::MidiEvent(TSE3::MidiCommand(TSE3::MidiCommand_NoteOn,
				                                  channel - 1, Settings::midiPort(),
				                                  pitch, velocity),
				                timer + midiShift));
			midi->insert(
				TSE3::MidiEvent(TSE3::MidiCommand(TSE3::MidiCommand_NoteOff,
				                                  channel - 1, Settings::midiPort(),
				                                  pitch, velocity),
				                timer + midiShift + duration - 1));

//			cout << "Inserted note pitch " << (int) pitch
//				 << ", start " << timer << ", duration " << duration << "\n";

		} // for (int i = 0; i < string ...

		if (tracking)
			midi->insert(TSE3::MidiEvent(encodeTimeTracking(tracknum, x), timer));

		timer += midilen;
		x = xl;					// step over linked notes

	} // for (uint x = 0; x < c.size() ...

	if (tracking) {
		// GREYFIX: Workaround for TSE3 bug (?) - last event not playing
		midi->insert(
			TSE3::MidiEvent(TSE3::MidiCommand(TSE3::MidiCommand_NoteOff,
											  0, Settings::midiPort(),
											  0, 0), timer + 120)
			);
	}

	// Initial setup, patches, midi volumes, choruses, etc.
	midi->insert(TSE3::MidiEvent(TSE3::MidiCommand(
										TSE3::MidiCommand_ProgramChange,
										channel - 1, Settings::midiPort(), patch),
								 tracking ? cursortimer : 0)
				 );

	return midi;
}
#endif

// Functions used to calculate TabColumn "volatile" data, that need access
// to more than one column.
// Used by MusicXML export and PostScript output.

// Calculate beams for this track

void TabTrack::calcBeams()
{
	// loop bn over all bars in this track
	for (size_t bn = 0; bn < b.size(); bn++) {
		// loop t over all columns in this bar and calculate beams
		for (size_t t = b[bn].start; (int) t <= lastColumn(bn); t++) {
			c[t].stl.bp.x=0;
			c[t].stl.bp.y=0;
			c[t].stl.l1 = beamL1(t, 0, bn, this);
			c[t].stl.l2 = beamL2plus(t, 0, bn, 2, this);
			c[t].stl.l3 = beamL2plus(t, 0, bn, 3, this);
			c[t].stu.bp.x=0;
			c[t].stu.bp.y=0;
			c[t].stu.l1 = beamL1(t, 1, bn, this);
			c[t].stu.l2 = beamL2plus(t, 1, bn, 2, this);
			c[t].stu.l3 = beamL2plus(t, 1, bn, 3, this);
		}
	}
}

// Determine step/alter/octave/accidental for each note

void  TabTrack::calcStepAltOct()
{
	int t;
	// initialize all data
	for (t = 0; t < c.size(); t++) {
		for (int i = 0; i < strings; i++) {
			c[t].stp[i] = ' ';
			c[t].alt[i] = 0;
			c[t].oct[i] = 0;
			c[t].acc[i] = Accidentals::None;
		}
	}
	Accidentals accSt;
	accSt.setKeySig(b[0].keysig);
	// calculate data for each bar
	for (size_t bn = 0; bn < b.size(); bn++) {
		accSt.resetToKeySig();
		// loop t over all columns in this bar and calculate saoa
		for (t = b[bn].start; (int) t <= lastColumn(bn); t++) {
			accSt.startChord();
			for (int i = 0; i < strings; i++) {
				if (c[t].a[i] > -1) {
					accSt.addPitch(tune[i] + c[t].a[i]);
				}
			}
			accSt.calcChord();
			for (int i = 0; i < strings; i++) {
				if (c[t].a[i] > -1) {
					Accidentals::Accid tmpacc = Accidentals::None;
					int tmpalt = 0;
					int tmpoct = 0;
					string tmpnam = " ";
					accSt.getNote(tune[i] + c[t].a[i],
									tmpnam, tmpalt, tmpoct, tmpacc);
					c[t].stp[i] = tmpnam[0];
					c[t].alt[i] = tmpalt;
					c[t].oct[i] = tmpoct;
					c[t].acc[i] = tmpacc;
				}
			}
		}
	}
}

// Determine voices for each note

void  TabTrack::calcVoices()
{
	int t;
	// initialize all data
	for (t = 0; t < c.size(); t++) {
		for (int i = 0; i < strings; i++) {
			c[t].v[i] = -1;
		}
	}

	// if only one single voice, allocate all notes to voice 1
	if (!hasMultiVoices()) {
		t = 0;
		while (t < c.size()) {
			for (int i = 0; i < strings; i++) {
				if (c[t].a[i] != NULL_NOTE) {
					c[t].v[i] = 1;
				}
			}
			t++;
		}

	} else {

		// handle multiple voices

		// loop through track and allocate voice 0
		t = 0;
		while (t < c.size()) {
			// find all lowest notes of equal length in this column
			int  lntlen = 0;		// lowest note length
			for (int i = 0; i < strings; i++) {
				if (c[t].a[i] != NULL_NOTE) {
					if (lntlen == 0) {
						// lowest note in this column, allocate to voice 0
						c[t].v[i] = 0;
						lntlen = noteNrCols(t, i);
					} else {
						if (lntlen == noteNrCols(t, i)) {
							// same length as lowest note in this column,
							// allocate to voice 0
							c[t].v[i] = 0;
						}
					}
				}
			}
			// move to next note
			if (lntlen == 0) {
				t++;
			} else {
				t += lntlen;
			}
		}
		// loop through track again and allocate remaining notes to voice 1
		// LVIFIX: cannot handle more than two voices:
		// print error when more voices are found
		t = 0;
		while (t < c.size()) {
			for (int i = 0; i < strings; i++) {
				if ((c[t].a[i] != NULL_NOTE) && (c[t].v[i] == -1)) {
					c[t].v[i] = 1;
				}
			}
			t++;
		}
		// if a column contains more than one notes in voice 0,
		// but none in voice 1, then allocate all but lowest note
		// to voice 1
		t = 0;
		while (t < c.size()) {
			int v0 = 0;			// number of notes in voice 1
			int v1 = 0;			// number of notes in voice 1
			for (int i = 0; i < strings; i++) {
				if (c[t].v[i] == 0) {
					v0++;
				}
				if (c[t].v[i] == 1) {
					v1++;
				}
			}
			if ((v0 > 1) && (v1 == 0)) {
				int na = 0;		// number of allocated notes
				for (int i = 0; i < strings; i++) {
					if (c[t].a[i] != NULL_NOTE) {
						if (na == 0) {
							c[t].v[i] = 0;
						} else {
							c[t].v[i] = 1;
						}
						na++;
					}
				}
			}
			t++;
		} // while (t ...
	} // if (!hasMulti

	// if linked with previous column, then copy voices from previous
	for (t = 0; t < c.size(); t++) {
		if ((t > 0) && (c[t].flags & FLAG_ARC)) {
			for (int i = 0; i < strings; i++) {
				c[t].v[i] = c[t-1].v[i];
			}
		}
	}
}

// return true if track has multiple voices
// i.e. at least one note is ringing

bool TabTrack::hasMultiVoices()
{
	for (int t = 0; t < c.size(); t++) {
		for (int i = 0; i < strings; i++) {
			if (c[t].e[i] == EFFECT_LETRING) {
				return true;
			}
		}
	}
	return false;
}

// determine if d equals an exact note duration

bool TabTrack::isExactNoteDur(int d)
{
	switch (d) {
	case  15: return true;
	case  30: return true;
	case  60: return true;
	case 120: return true;
	case 240: return true;
	case 480: return true;
	}
	return false;
}

// get note head type tp, number of dots dt and triplet flag
// for column t in voice v
// if no note in voice v, then return false
// if note found but no valid type/dot combination, then return true,
// but tp = dt = 0, tr = false;

bool TabTrack::getNoteTypeAndDots(int t, int v, int & tp, int & dt, bool & tr)
{
	// defaults: no note, no dots, no triplet
	tp = 0;
	dt = 0;
	tr = false;

	// tie handling
	int  tt = t;				// t where tie starts
	if ((t > 0) && (c[t].flags & FLAG_ARC)) {
		tt = t - 1;				// LVIFIX: handle more than one tie
	}

	// find a note in voice v
	// if tied to previous column, use info from previous one
	int i;
	for (i = strings - 1; i >= 0; i--) {
		if ((c[tt].a[i] != NULL_NOTE)
			&& (c[tt].v[i] == v)) {
			break;
		}
	}
	if (i == -1) {
		// no note in this voice
		return false;
	}

	int dur = noteDuration(t, i);
	// try no dots
	tp = dur;
	dt = 0;
	if (isExactNoteDur(tp)) {
		return true;
	}
	// try one dot (duration = type * 3 / 2)
	tp = dur * 2 / 3;
	dt = 1;
	if (isExactNoteDur(tp)) {
		return true;
	}
	// try two dots (duration = type * 7 / 4)
	tp = dur * 4 / 7;
	dt = 2;
	if (isExactNoteDur(tp)) {
		return true;
	}
	// try triplet (duration = type * 2 / 3)
	tp = dur * 3 / 2;
	dt = 0;
	tr = true;
	if (isExactNoteDur(tp)) {
		return true;
	}

	// no valid note type / dot combination found
	tp = 0;
	dt = 0;
	tr = false;
	return true;
}

#ifdef WITH_TSE3
// These things are ugly stuff to somehow encode cursor tracking as
// pseudo MIDI events that don't play anything, but KGuitar will catch
// them and update cursor numbers accordingly. We encode absolute
// cursor position for a given track. That's bad, because currently we
// can have only 16 tracks and 16384 columns per track playable.

// *GREYFIX*

TSE3::MidiCommand TabTrack::encodeTimeTracking(int track, int x)
{
	// Channel - (0-15) - 4 bits - encode track number
	int pseudoChannel = track;

	// Data1 - 7 bits - low 7 bits of x
	int pseudoData1 = x & 0x7f;

	// Data2 - 7 bits - high 7 bits of x
	int pseudoData2 = x >> 7;

	return TSE3::MidiCommand(KGUITAR_MIDI_COMMAND,
							 pseudoChannel,
							 KGUITAR_MIDI_PORT,
							 pseudoData1, pseudoData2);
}

void TabTrack::decodeTimeTracking(TSE3::MidiCommand mc, int &track, int &x)
{
	track = mc.channel;
	x = (mc.data2 << 7) + mc.data1;
}
#endif
