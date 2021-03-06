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
 *  The Jade Empire main menu.
 */

#include "src/common/util.h"

#include "src/engines/jade/module.h"

#include "src/engines/jade/gui/main/main.h"

namespace Engines {

namespace Jade {

MainMenu::MainMenu(Module &module, ::Engines::Console *console) : ::Engines::KotOR::GUI(console),
	_module(&module) {

	load("maingame");
}

MainMenu::~MainMenu() {
}

void MainMenu::callbackActive(Widget &UNUSED(widget)) {
	try {
		_module->load("j01_town");
	} catch (Common::Exception &e) {
		Common::printException(e, "WARNING: ");
		return;
	}
}

} // End of namespace Jade

} // End of namespace Engines
