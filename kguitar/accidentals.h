/***************************************************************************
 * accidentals.h: definition of Accidentals class
 *
 * This file is part of KGuitar, a KDE tabulature editor
 *
 * copyright (C) 2002-2003 the KGuitar development team
 ***************************************************************************/

/***************************************************************************
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * See the file COPYING for more information.
 ***************************************************************************/

#ifndef ACCIDENTALS_H
#define ACCIDENTALS_H

#include "global.h"

class Accidentals
{
public:
	enum Accid { None, Natural, Sharp, Flat };
	Accidentals();
	void addPitch(int pitch);
	void calcChord();
	void countAccPrnt(string& stp, Accid& acc);
	int getAccPrnt(string& stp);
	int getKeySig();
	bool getNote(int pitch, string& stp, int& alt, int& oct, Accid& acc);
	void resetToKeySig();
	int  sao2Pitch(const string& stp, int alt, int oct);
	void setKeySig(int sig);
	void startChord();
private:
	void markInUse(int i, int nlh, Accid a);
	bool mustPrntAllAcc(int nn);
	bool naGet(const string& stp, int oct);
	void naReset(const string& stp, int oct);
	void naResetAll();
	void naSetAll(const string& stp);
	int naSo2i(const string& stp, int oct);
	int  normalize(int pitch);
	static const int stPerOct = 12;	// # semitones (half steps) per octave
	int accPrnt[7];					// # accidentals printed for each note
	int keySig;						// key signature in # sharps or flats
	bool notes_av[stPerOct];		// notes available
	bool notes_req[stPerOct];		// notes requested for this chord
	Accid old_acc_state[stPerOct];	// accidental state for all notes, before
	Accid new_acc_state[stPerOct];	// and after calcChord
	int out_root_note[stPerOct];	// notes to be printed
	Accid out_accidental[stPerOct];	// accidentals to be printed
	bool needs_acc[77];				// which notes need an accidental
	bool printAllAccInChrd;			// print acc on all notes in chord
	bool printAccAllInst;			// print acc for all instances same note
};

#endif
