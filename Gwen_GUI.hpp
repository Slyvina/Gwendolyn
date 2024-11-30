// License:
// 
// Gwendolyn
// Graphic User Interface (header)
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
// Version: 24.11.30 I
// End License
#pragma once
#include <june19_core.hpp>
#include <TQSG.hpp>

namespace Slyvina {
	namespace Gwendolyn {

#pragma region ClockSpot
		struct ClockSpot { int X{ 0 }, Y{ 0 }; };
		ClockSpot DegSpot(double deg,double straal);
#pragma endregion

#pragma region Panels
		June19::j19gadget* Panel(std::string _PanID);
		June19::j19gadget* NewPanel(std::string _PanID);
		void GoToPanel(std::string _PanID);
#pragma endregion

#pragma region Main
		void Gwen_Init();
		void Gwen_Run();
		void Gwen_Done();
		void Gwen_Panic(std::string Error, int exitcode = 3);
		void Gwen_Exit(int code = 0);
#pragma endregion
	}
}
