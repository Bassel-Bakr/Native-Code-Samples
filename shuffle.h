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

Also add information on how to contact you by electronic and paper mail.

If the program is interactive, make it output a short notice like this
when it starts in an interactive mode:

    Gnomovision version 69, Copyright (C) year name of author
    Gnomovision comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
    This is free software, and you are welcome to redistribute it
    under certain conditions; type `show c' for details.

The hypothetical commands `show w' and `show c' should show the appropriate
parts of the General Public License.  Of course, the commands you use may
be called something other than `show w' and `show c'; they could even be
mouse-clicks or menu items--whatever suits your program.

You should also get your employer (if you work as a programmer) or your
school, if any, to sign a "copyright disclaimer" for the program, if
necessary.  Here is a sample; alter the names:

  Yoyodyne, Inc., hereby disclaims all copyright interest in the program
  `Gnomovision' (which makes passes at compilers) written by James Hacker.

  {signature of Ty Coon}, 1 April 1989
  Ty Coon, President of Vice

This General Public License does not permit incorporating your program into
proprietary programs.  If your program is a subroutine library, you may
consider it more useful to permit linking proprietary applications with the
library.  If this is what you want to do, use the GNU Lesser General
Public License instead of this License.
**/

#define null '\0'

int done = 0;
char *chars;

static char *NUMBERS = "0123456789";
static char *LOWER_CASE_ALPHABET = "abcdefghijklmnopqrstuvwxyz";
static char *UPPER_CASE_ALPHABET = "ABCDEFGHIJKLMNOPQESTUVWXYZ";

typedef struct IResultListener IResultListener;
typedef struct HarmonicInt HarmonicInt;

struct IResultListener
{
	void (*newResult) (char *);
};

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
	else if (chars[self->num + 1] == '\0')
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

void shuffle(char *chrs, int length, IResultListener * listener)
{
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
	for (int i = 0; i < length - 1; i++)
	{
		hi[i].next = &hi[i + 1];
	}

	int index;
	char *line = (char *)calloc(length + 1, sizeof(char));
	while (!done)
	{
		index = 0;
		for (int i = length - 1; i >= 0; i--)
		{
			line[index++] = chars[hi[i].num];
		}
		line[index] = '\0';
		if (listener != null)
			listener->newResult(line);
		increment(hi);
	}
	free(line);
	free(hi);
}