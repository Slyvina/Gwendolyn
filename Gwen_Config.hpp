// License:
// 
// Gwendolyn
// Configuration (header)
// 
// 
// 
// 	(c) Jeroen P. Broks, 2024
// 
// 		This program is free software: you can redistribute it and/or modify
// 		it under the terms of the GNU General Public License as published by
// 		the Free Software Foundation, either version 3 of the License, or
// 		(at your option) any later version.
// 
// 		This program is distributed in the hope that it will be useful,
// 		but WITHOUT ANY WARRANTY; without even the implied warranty of
// 		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// 		GNU General Public License for more details.
// 		You should have received a copy of the GNU General Public License
// 		along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// 	Please note that some references to data like pictures or audio, do not automatically
// 	fall under this licenses. Mostly this is noted in the respective files.
// 
// Version: 24.12.02
// End License
#pragma once
#include <SlyvGINIE.hpp>
#include <SlyvDirry.hpp>

namespace Slyvina {
	namespace Gwendolyn {
		inline String ConfigFile() { return Units::Dirry("$AppSupport$/Gwendolyn.ini"); }
		Units::GINIE Config();
		inline bool ConfigSlaan() { return Config()->NewValue("Bell", "Bell", "TRUE") == "TRUE"; }
		inline bool ConfigBool(String C, String V, bool dv = false) { return Units::Upper(Config()->NewValue(C, V, uboolstring(dv))) == "TRUE"; }
	}
}
