/* xoreos - A reimplementation of BioWare's Aurora engine
 *
 * xoreos is the legal property of its developers, whose names
 * can be found in the AUTHORS file distributed with this source
 * distribution.
 *
 * xoreos is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * xoreos is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with xoreos. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file
 *  Handling BioWare's GFF'd talk tables.
 */

#include "src/common/util.h"
#include "src/common/error.h"

#include "src/aurora/talktable_gff.h"
#include "src/aurora/talkman.h"
#include "src/aurora/gff4file.h"

static const uint32 kTLKID     = MKTAG('T', 'L', 'K', ' ');
static const uint32 kVersion02 = MKTAG('V', '0', '.', '2');
static const uint32 kVersion05 = MKTAG('V', '0', '.', '5');

namespace Aurora {

TalkTable_GFF::TalkTable_GFF(Common::SeekableReadStream *tlk, uint32 languageID) :
	_gff(0), _languageID(languageID) {

	load(tlk);
}

TalkTable_GFF::~TalkTable_GFF() {
	delete _gff;
}

uint32 TalkTable_GFF::getLanguageID() const {
	return _languageID;
}

bool TalkTable_GFF::hasEntry(uint32 strRef) const {
	return _entries.find(strRef) != _entries.end();
}

static const Common::UString kEmptyString = "";
const Common::UString &TalkTable_GFF::getString(uint32 strRef) const {
	Entries::iterator e = _entries.find(strRef);
	if (e == _entries.end())
		return kEmptyString;

	readString(*e->second);

	return e->second->text;
}

const Common::UString &TalkTable_GFF::getSoundResRef(uint32 UNUSED(strRef)) const {
	return kEmptyString;
}

void TalkTable_GFF::load(Common::SeekableReadStream *tlk) {
	assert(tlk);

	try {
		_gff = new GFF4File(tlk, kTLKID);

		const GFF4Struct &top = _gff->getTopLevel();

		if      (_gff->getTypeVersion() == kVersion02)
			load02(top);
		else if (_gff->getTypeVersion() == kVersion05)
			load05(top);
		else
			throw Common::Exception("Unsupported GFF TLK file version %08X", _gff->getTypeVersion());

	} catch (Common::Exception &e) {
		delete _gff;

		e.add("Unable to load GFF TLK");
		throw;
	}
}

void TalkTable_GFF::load02(const GFF4Struct &top) {
	if (top.hasField(kGFF4TalkStringList)) {
		const GFF4List &strings = top.getList(kGFF4TalkStringList);

		for (GFF4List::const_iterator s = strings.begin(); s != strings.end(); ++s) {
			if (!*s)
				continue;

			uint32 strRef = (*s)->getUint(kGFF4TalkStringID, 0xFFFFFFFF);
			if (strRef == 0xFFFFFFFF)
				continue;

			_entries[strRef] = new Entry(*s);
		}
	}
}

void TalkTable_GFF::load05(const GFF4Struct &UNUSED(top)) {
	throw Common::Exception("TODO: GFF4 TLK V0.5");
}

void TalkTable_GFF::readString(Entry &entry) const {
	if (!entry.strct)
		return;

	Common::Encoding encoding = Common::kEncodingUTF16LE;
	if (_languageID != 0xFFFFFFFF)
		encoding = TalkMan.getEncoding(_languageID);

	if (encoding != Common::kEncodingInvalid)
		entry.text = entry.strct->getString(kGFF4TalkString, encoding);
	else
		entry.text = "[???]";

	entry.strct = 0;
}

} // End of namespace Aurora
