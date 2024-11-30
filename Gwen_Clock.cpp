// License:
// 
// Gwendolyn
// Clock manager
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
#include <SlyvTime.hpp>
#include "Gwen_Clock.hpp"
#include <SlyvSTOI.hpp>

using namespace Slyvina::Units;

namespace Slyvina {

	namespace Gwendolyn {
		int TotSec24() {
			auto
				H{ ToInt(QTimeF("%H")) },
				M{ ToInt(QTimeF("%M")) },
				S{ ToInt(QTimeF("%S")) };
			return 60 + (M * 60) + (H * 60 * 60);
		}
		int TotSec12() {
			auto
				H{ ToInt(QTimeF("%I")) },
				M{ ToInt(QTimeF("%M")) },
				S{ ToInt(QTimeF("%S")) };
			return 60 + (M * 60) + (H * 60 * 60);
		}
		int TotSec() {
			auto				
				M{ ToInt(QTimeF("%M")) },
				S{ ToInt(QTimeF("%S")) };
			return 60 + (M * 60);
		}

		int Sec() { return ToInt(QTimeF("%S")); }

	}

}
