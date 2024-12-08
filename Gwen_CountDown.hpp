// License:
// 
// Gwendolyn
// CountDown Manager (header)
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
// Version: 24.12.08 I
// End License

#pragma once
#include <june19.hpp>
#include <SlyvGINIE.hpp>

namespace Slyvina {
	namespace Gwendolyn {

		class TCountDown {
		private:
			static Units::GINIE _Data;
			static std::map<int, TCountDown> _Reg;
			int _ID{ 0 };
			String V(String Key);
			void V(String Key, String Value);
			static void _Load();
		public:
			static void ReIndex();
			String Record();
			String Label(bool actual = false);
			
		};

		void InitCountDown(June19::j19gadget* CntBack);
	}
}
