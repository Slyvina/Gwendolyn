// License:
// 
// Gwendolyn
// Schedule Database (header)
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
// Version: 24.12.01
// End License
#pragma once
#include <Slyvina.hpp>
#include <SlyvGINIE.hpp>
namespace Slyvina {
	namespace Gwendolyn {

		class TSchedule;

		class TSchedule {
		private:
			static Units::GINIE _Data;
			String _Record;
			static std::map<String, TSchedule> _TrueDataBase;
			static std::map<String, String> _ByTime;
			static std::map<String, String> _ByLabel;
			static void _Load(bool force=false);
			static void _Index();
		public:
			inline String Record() { return _Record; }
			static String ScheduleFile();
			static TSchedule* NewRecord();
			static TSchedule* GetRecord(String Tag);
			bool Valid();
			int Hour();
			int Hour12();
			int Minute();
			int Second();
			void Hour(int value);
			void Minute(int value);
			void Second(int value);
			String Time(int h=24);
			String ampm() { return Hour() >= 12 ? "pm" : "am"; }
		};

	}
}