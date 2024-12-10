// License:
// 
// Gwendolyn
// Main
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
// Version: 24.12.10
// End License

#include <JCR6_zlib.hpp>
#include <SlyvQCol.hpp>
#include "Gwen_Assets.hpp"
#include "Gwen_GUI.hpp"
#include "Gwen_Version.hpp"

using namespace Slyvina;
using namespace Slyvina::JCR6;
using namespace Slyvina::Gwendolyn;


int main(int ca, char** args) {
	JCR_InitZlib();
	QCol->LMagenta("Gwendolyn\n");
	QCol->Doing("Version", QVersion.Version());
	QCol->Doing("Coded by", "Jeroen P. Broks");
	CheckAssets(args[0]);
	Gwen_Init();
	Gwen_Run();
	Gwen_Done();
	QCol->Reset();
	return 0;
}
