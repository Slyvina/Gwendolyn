// License:
// 
// Gwendolyn
// Assets management
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
// Version: 24.11.30
// End License
#include "Gwen_Assets.hpp"
#include <SlyvStream.hpp>
#include <SlyvQCol.hpp>
#include <SlyvGINIE.hpp>
using namespace Slyvina::JCR6;
using namespace Slyvina::Units;

namespace Slyvina {
	namespace Gwendolyn {
		static String _ResFile{ "" };
		static JT_Dir _Res{ nullptr };
		static GINIE _ResID{ nullptr };

		String ResFile(String A0) {
			if (_ResFile.size()) return _ResFile;
			static String Candidates[2]{
#ifndef SlyvWindows
				"/self/proc/exe",
#else
				A0,
#endif
				StripExt(A0) + ".jcr" // In Windows all safe, in Linux and other Unix based systems, this option should only be used for debugging.
			};
			for (auto& rf : Candidates) {
				//QCol->LMagenta(rf + "\n"); // Debug
				if (JCR6_Recognize(rf)!="NONE") { _ResFile = rf; return rf; }
			}
			QCol->Error("No resource present!"); exit(1);
			return _ResFile;
		}

		JCR6::JT_Dir Res(String A0) {
			if (!_Res) { 				
				QCol->Doing("Analysing", ResFile(A0)); _Res = JCR6_Dir(ResFile(A0)); 
			}
			JCR6Assert();
			return _Res;
		}
		void JCR6Assert() {
			if (Last()->Error) {
				QCol->Error(Last()->ErrorMessage);
				QCol->Doing("- Main", Last()->MainFile);
				QCol->Doing("- Entry", Last()->Entry);
				exit(2);
			} else if (!_Res) {
				QCol->Error("JCR6 resource list is null for some silly reason!");
			}
		}
		void CheckAssets(String A0) {
#ifdef SlyvWindows
			A0 = ChReplace(A0, '\\', '/');
#endif
			auto IDS{ Res(A0)->GetString("ID/Identify.ini") };
			_ResID = ParseGINIE(IDS);
			if (_ResID->Value("ID", "Tool") != "Gwendolyn") { QCol->Error("Resource does not appear to be long to Gwendolyn"); exit(3); }
			QCol->Doing("ResBuild", _ResID->Value("ID", "Updated"));
		}

	}
}
