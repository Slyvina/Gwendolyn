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
// Version: 24.12.10
// End License

#include <math.h>
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
			return S + (M * 60) + (H * 60 * 60);
		}
		int TotSec12() {
			auto
				H{ ToInt(QTimeF("%I")) },
				M{ ToInt(QTimeF("%M")) },
				S{ ToInt(QTimeF("%S")) };
			return S + (M * 60) + (H * 60 * 60);
		}
		int TotSec() {
			auto
				M{ ToInt(QTimeF("%M")) },
				S{ ToInt(QTimeF("%S")) };
			return S + (M * 60);
		}

		int Sec() { return ToInt(QTimeF("%S")); }

		int Phan_TotalMillos() {
			return (int)floor(
				(TotSec24() / 86400.0)
				* 20000);
		}

		int Phan_Vectas() {
			return Phan_TotalMillos() / 1000;
		}

		int Phan_Millos() {
			return Phan_TotalMillos() % 1000;;
		}

	}

}
