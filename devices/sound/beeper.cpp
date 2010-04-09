/*
Portable ZX-Spectrum emulator.
Copyright (C) 2001-2010 SMT, Dexus, Alone Coder, deathsoft, djdron, scor

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "../../std.h"
#include "beeper.h"

//=============================================================================
//	eBeeper::IoWrite
//-----------------------------------------------------------------------------
void eBeeper::IoWrite(word port, byte v, int tact)
{
	if(port & 1)
		return;
	const short vol = 8192;
	short spk = (v & 0x10) ? vol : 0;
	short mic = (v & 0x08) ? vol : 0;
	short mono = spk + mic;
	Update(tact, mono, mono);
}
