/***************************************************************************
 * accidentals.cpp: implementation of Accidentals class
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

#include <string.h>
#include "accidentals.h"

// class Accidentals -- given note pitch, handle all accidentals
//
// use as follows:
// setKeySig()
// foreach measure
//   resetToKeySig()
//   foreach chord in measure:
//     startChord()
//     foreach note in chord:
//       addPitch()
//     calcChord()
//     foreach note in chord:
//       getNote()
//
// Note: two opinions about accidental handling can be found:
// - accidentals apply to a single octave
// - accidentals apply to all octaves
// Class Accidentals assumes accidentals apply to all octaves

static const string notes_flat[12]  = {"C",  "Db", "D",  "Eb", "E",  "F",
                                        "Gb", "G",  "Ab", "A",  "Bb", "B"};
static const string notes_sharp[12] = {"C",  "C#", "D",  "D#", "E",  "F",
                                        "F#", "G",  "G#", "A",  "A#", "B"};

// Key signature table
// if keySig > 0, start at F and work to the right, notes are sharpened
// if keySig < 0, start at B and work to the left, notes are flattened
// Examples:
// keySig =  1 (G major / E minor): F#
// keySig = -2 (Bb major / G minor): Bb, Eb

//                               F  C  G  D  A  E   B
static const int keySigTab[7] = {5, 0, 7, 2, 9, 4, 11};

// Note about accidental handling options:
// (currently set to fixed values in the class constructor)

// printAllAccInChrd determines if an accidental must be printed
// for all instances of a given note in a chord.
// E.g. if a chord contains three F# then:
// printAllAccInCh = false prints one F# and two F's
// printAllAccInCh = true prints three F#'s
// LVIFIX: make configurable (i.e.global option)

// printAccAllPitch determines if an accidental must be printed
// for all instances of a given note.
// E.g. if a chord contains two F# and a later chord contains
// a different F# then:
// printAccAllInst = false print accidental on first chord
// printAccAllInst = true also print accidental on second chord
// LVIFIX: make configurable (i.e.global option)

// Accidentals constructor

Accidentals::Accidentals()
{
	keySig = 0;
	printAllAccInChrd = true;
//	printAllAccInChrd = false;
	printAccAllInst = true;
//	printAccAllInst = false;

	for (int i=0; i<stPerOct; i++) {
		notes_av[i]       = false;
		notes_req[i]      = false;
		old_acc_state[i]  = Natural;
		new_acc_state[i]  = Natural;
		out_root_note[i]  = 0;
		out_accidental[i] = None;
	}
}

// add pitch to chord

void Accidentals::addPitch(int pitch)
{
	int noteNumber = normalize(pitch);
	notes_req[noteNumber] = true;
}

// do the work

void Accidentals::calcChord()
{
	// init
	for (int i=0; i<stPerOct; i++) {
		// only naturals are available
		if (notes_sharp[i].length() == 1) {
			notes_av[i] = true;
		} else {
			notes_av[i] = false;
		}
		// default new_acc_state = old_acc_state
		new_acc_state[i] = old_acc_state[i];
		// init root note (dummy value)
		out_root_note[i] = 0;
		// init accidentals
		out_accidental[i] = Natural;
	}
	// pass 1: handle naturals
	// loop over all requested notes
	for (int i=0; i<stPerOct; i++) {
		// if note needed
		if (notes_req[i]) {
			// if note is a natural
			if (notes_sharp[i].length() == 1) {
				markInUse(i, i, Natural);
			}
		}
	}
	// pass 2: handle accidentals
	// loop over all requested notes
	for (int i=0; i<stPerOct; i++) {
		// if note needed
		if (notes_req[i]) {
			// if note is not a natural
			if (notes_sharp[i].length() != 1) {
				int nl = normalize(i - 1);	// lower note to try
				int nh = normalize(i + 1);	// higher note to try
				// first check of this pitch is already available
				// i.e. lower note available and acc_state is Sharp
				// or higher note available and acc_state is Flat
				if (notes_av[nl] && (old_acc_state[nl] == Sharp)) {
					// lower note: F# requested, F# used in previous chord
					markInUse(i, nl, Sharp);
				} else if (notes_av[nh] && (old_acc_state[nh] == Flat)) {
					// higher note: F# requested, Gb used in previous chord
					markInUse(i, nh, Flat);
				// pitch is not already available, make it
				} else if (notes_av[nl]) {
					// lower note: if F# req, if F av. use F + #
					markInUse(i, nl, Sharp);
				} else if (notes_av[nh]) {
					// higher note: if F# req, if G av. use G + b
					markInUse(i, nh, Flat);
				} else {
					// special case:
					// F# req, both F and G already used
					// for F use F + natural, for F# use F + #
					// new_acc_state is natural
					out_accidental[nl] = Natural;	// explicit natural
					out_root_note[i]   = nl;		// note name to use
					out_accidental[i]  = Sharp;
					new_acc_state[nl]  = Natural;
		        }
			}
		}
	}

	// copy new_acc_state into old_acc_state
	// set required "need accidental" flags
	for (int i=0; i<stPerOct; i++) {
		old_acc_state[i] = new_acc_state[i];
		if (notes_req[i] && (out_accidental[i] != None)) {
			naSetAll(notes_sharp[out_root_note[i]]);
		}
	}
}

// count # accidentals "printed" in this chord
// includes suppressed accidentals

void Accidentals::countAccPrnt(string& stp, Accid& acc)
{
	char a = 'A';
	int i = stp[0] - a;
	if ((i < 0) || (i >= 7)) {
		return;
	}
	if (acc != None) {
		accPrnt[i]++;
	}
}

// return # accidentals printed in this chord

int Accidentals::getAccPrnt(string& stp)
{
	char a = 'A';
	int i = stp[0] - a;
	if ((i < 0) || (i >= 7)) {
		return 0;
	}
	return accPrnt[i];
}

// return the key signature

int Accidentals::getKeySig()
{
	return keySig;
}

// get note info for given pitch
// note pitch=40 returns stp=E, alt=0, oct=2
// MusicXML tutorial 0.7a:
// guitar tuning is E2 A2 D3 G3 B3 E4
// which corresponds to midi note numbers 40 45 50 55 59 64

bool Accidentals::getNote(int pitch, string& stp,
							int& alt, int& oct, Accid& acc)
{
	int noteNumber = normalize(pitch);
	if (!notes_req[noteNumber]) {
		return false;
	}
	stp = notes_sharp[out_root_note[noteNumber]].substr(0,1);
	oct = pitch / stPerOct;
	alt = pitch - (oct * stPerOct + out_root_note[noteNumber]);
	acc = out_accidental[noteNumber];
	oct--;						// correct oct (as 40 / stPerOct = 3 i.s.o. 2)
	if ((acc != None) && (!mustPrntAllAcc(noteNumber))) {
		naReset(stp, oct);
	}
	// if requested, suppress printing of second and following accidentals
	// but make sure that if both F natural and F accidental are printed,
	// all F's in the chord get an accidental even if printAllAccInCh = false
	if ((getAccPrnt(stp) > 0) && !printAllAccInChrd
		&& !mustPrntAllAcc(noteNumber)
		&& !printAccAllInst) {
		acc = None;
	}
	countAccPrnt(stp, acc);
	// if no accidental printed, check pending accidentals
	if ((acc == None) && naGet(stp, oct)) {
		acc = new_acc_state[out_root_note[noteNumber]];
		naReset(stp, oct);
	}
	return true;
}

// mark pitch i as composed of pitch nlh and accidental a

void Accidentals::markInUse(int i, int nlh, Accid a)
{
	notes_av[nlh]      = false;
	new_acc_state[nlh] = a;
	out_root_note[i]   = nlh;
	if (old_acc_state[nlh] == new_acc_state[nlh]) {
		out_accidental[i] = None;
	} else {
		out_accidental[i] = a;
	}
}

// determine if all accidentals must be printed for note nn
// this is the case when both nn with natural and nn with sharp are printed

// if one char notename (without accidental):
// - next note also requested and same root note
// if two char notename (with accidental):
// - previous note also requested (must be a natural) and same root note

bool Accidentals::mustPrntAllAcc(int nn)
{
	int on = 0;					// the other (previous or next) note
	bool res = true;
	if (notes_sharp[nn].length() == 1) {
		// nn is a single char note, compare with next
		on = normalize(nn + 1);
	} else {
		// nn must be double char note, compare with previous
		on = normalize(nn - 1);
	}
	res = notes_req[on] && (out_root_note[nn] == out_root_note[on]);
	return res;;
}

// get the "needs accidental" flag for note stp in octave oct

bool Accidentals::naGet(const string& stp, int oct)
{
	return needs_acc[naSo2i(stp, oct)];
}

// reset the "needs accidental" flag for note stp in octave oct

void Accidentals::naReset(const string& stp, int oct)
{
	needs_acc[naSo2i(stp, oct)] = false;
}

// reset all "needs accidental" flags

void Accidentals::naResetAll()
{
	int k = 0;
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 7; j++) {
			needs_acc[k] = false;
			k++;
		}
	}
}

// set "needs accidental" flags for note stp in all octaves

void Accidentals::naSetAll(const string& stp)
{
	int j = naSo2i(stp, 0);
	for (int i = 0; i < 11; i++) {
		needs_acc[j] = true;
		j += 7;
	}
}

// convert stp/oct to index

int Accidentals::naSo2i(const string& stp, int oct)
{
	char a = 'A';
	int i = stp[0] - a;
	if ((i < 0) || (i >= 7)) {
		return 0;
	}
	if ((oct < 0) || (oct >= 11)) {
		return 0;
	}
	return 7 * oct + i;
}

// make sure note number is in range 0..11

int Accidentals::normalize(int pitch)
{
	int noteNumber = pitch % stPerOct;
	if (noteNumber < 0) {
		noteNumber += stPerOct;
	}
	return noteNumber;
}

// reset to key signature
// clear all "needs accidental" flags

void Accidentals::resetToKeySig()
{
	for (int i=0; i<stPerOct; i++) {
		old_acc_state[i] = Natural;
	}
	if (keySig > 0) {
		for (int i = 0; i < keySig; i++) {
			old_acc_state[keySigTab[i]] = Sharp;
		}
	} else if (keySig < 0) {
		for (int i = 0; i > keySig; i--) {
			old_acc_state[keySigTab[i + 6]] = Flat;
		}
	}
	naResetAll();
}

// convert step (note name), alter (flat/sharp) and octave to pitch
// return -1 on failure
// note: MusicXML tutorial 0.7a: A-string = A2 = pitch 45

int Accidentals::sao2Pitch(const string& stp, int alt = 0, int oct = 0)
{
    int cn = -1;

	// search step in note name table
    for (int i = 0; i < 12; i++) {
		if (notes_sharp[i] == stp) {
			cn = i;
		}
		if (notes_flat[i] == stp) {
			cn = i;
		}
	}
	if (cn == -1) {
		return -1;
	}
	return (oct + 1) * 12 + cn + alt;
}

// set the key signature

void Accidentals::setKeySig(int sig)
{
	if ((-8 < sig) && (sig < 8)) {
		keySig = sig;
	} else {
		keySig = 0;
	}
}

// start a new chord

void Accidentals::startChord()
{
	for (int i=0; i < stPerOct; i++) {
		notes_req[i] = false;
		out_root_note[i] = 0;
	}
	for (int i=0; i < 7; i++) {
		accPrnt[i] = 0;
	}
}
