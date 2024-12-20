// License:
// 
// Gwendolyn
// Assets Management (header)
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
#pragma once
#include <JCR6_Core.hpp>
#include <TQSG.hpp>
#include "Gwen_Schedule.hpp"

namespace Slyvina {
	namespace Gwendolyn {
		String ResFile(String A0="");
		JCR6::JT_Dir Res(String A0="");
		void JCR6Assert();
		void CheckAssets(String A0);


		// Fonts
		TQSG::TImageFont GetFont(std::string Name, std::string Tag);
		inline TQSG::TImageFont GetFont(std::string Name) { return GetFont("Fonts/" + Name + ".jfbf", Name); }
		inline TQSG::TImageFont FntSys() { return GetFont("System"); }
		inline TQSG::TImageFont FntLiquid() { return GetFont("Liquid");}
		inline TQSG::TImageFont FntRyanna() { return GetFont("Ryanna"); }
		inline TQSG::TImageFont FntMini() { return GetFont("Mini"); }
		inline TQSG::TImageFont FntCDI() { return GetFont("CDI"); }

		TQSG::TImage GetWijzer(bool groot);

		void PlayAlarm(bool intern, String File, bool loop = false);
		void PlayAlarm(AlarmRef r, bool loop=false);

		void Slaan(int times);
	}
}
