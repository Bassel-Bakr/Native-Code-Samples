/**
    Native Code Examples
    Copyright (C) 2014  Bassel Bakr

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
**/

#define null '\0'

int done = 0;
char *chars;

static char *NUMBERS = "0123456789";
static char *LOWER_CASE_ALPHABET = "abcdefghijklmnopqrstuvwxyz";
static char *UPPER_CASE_ALPHABET = "ABCDEFGHIJKLMNOPQESTUVWXYZ";

typedef struct IResultListener IResultListener;
typedef struct HarmonicInt HarmonicInt;

// We are mimiciking OOP style a bit

// An interface-like struct that holds a function pointer which points to callback function memory address
struct IResultListener
{
	void (*newResult) (char *);
};

// A struct that increments another @param next if it was defined
struct HarmonicInt
{
	int num;
	int reset;
	HarmonicInt *next;
};

void increment(HarmonicInt *);
void incrementReset(HarmonicInt *);

void increment(HarmonicInt * self)
{
	self->num++;
	if (self->reset)
	{
		incrementReset(self);
	}
	else if (chars[self->num + 1] == null)
	{
		self->reset++;
	}
}

void incrementReset(HarmonicInt * self)
{
	self->num = 0;
	self->reset = 0;
	if (self->next != null)
		increment(self->next);
	else
		done++;
}

// Here goes the real thing read carefully or get lost in translation!
void shuffle(char *chrs, int length, IResultListener * listener)
{
	// If @param chrs holds just one char
	if (chrs[1] == null)
	{
		char *l = (char *) calloc(1, length + 1);
		for (int i = 0; i < length; i++)
			l[i] = chrs[0];
		l[length] = null;
		if (listener != null)
			listener->newResult(l);
		free(l);
		return;
	}
	
	chars = chrs;
	HarmonicInt *hi = (HarmonicInt *) calloc(length, sizeof(HarmonicInt));
	
	// Last HarmonicInt shouldn't define its @param next pointer, that's why we used (length - 1)
	for (int i = 0; i < length - 1; i++)
	{
		// hi[i].next = i[hi].next = (hi + i)->next = (&hi[i])->next
		hi[i].next = &hi[i + 1];
	}

	int index;
	
	// Time to allocate some memory (equal to length, the +1 is just for terminating null char)
	char *line = (char *)calloc(length + 1, sizeof(char));
	
	// Loop until done's value != 0
	while (!done)
	{
		index = 0;
		for (int i = length - 1; i >= 0; i--)
		{
			line[index++] = chars[hi[i].num];
		}
		line[index] = null;
		
		// Is our callback defined? If so, pass it our results one after another
		if (listener != null)
			listener->newResult(line);
		increment(hi);
	}
	
	// Not used anymore
	free(line);
	free(hi);
}
