// License:
// 
// Gwendolyn
// Schedule Database
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

#include "Gwen_Schedule.hpp"
#include <SlyvDirry.hpp>
#include <SlyvQCol.hpp>
using namespace Slyvina::Units;

namespace Slyvina {
	namespace Gwendolyn {

		Units::GINIE TSchedule::_Data{ nullptr };
		std::map<String, TSchedule> TSchedule::_TrueDataBase{};
		std::map<String, String> TSchedule::_ByTime{};
		std::map<String, String> TSchedule::_ByLabel{};
		void TSchedule::_Load(bool force) {
			if ((!_Data) || force) _Data = LoadOptGINIE(ScheduleFile(), ScheduleFile(), "Gwendolyn schedule data");
		}
		void TSchedule::_Index() {
			_Load();
			auto cats{ _Data->Categories() };
			_ByTime.clear();
			for (auto c : *cats) {
				_ByTime[TrSPrintF("%02d%02d%02d", _Data->NewValue(c, "Hour", 0), _Data->NewValue(c, "Minute", 0), _Data->NewValue(c, "Second", 0))] = c;
				_ByLabel[_Data->Value(c, "Label").size() ? _Data->Value(c, "Label") : "!!!!!!!!!!!!!!!!!!!!!UNLABELED:" + c] = c;
			}
		}

		String TSchedule::ScheduleFile() {
			return Dirry("$AppSupport$/Gwendolyn.Schedule.ini");
		}

		TSchedule* TSchedule::NewRecord() {
			static uint64 cnt{ 0 };
			String NTag{ "" };
			_Load();
			do { NTag = TrSPrintF("Rec::%09x", ++cnt); } while (_Data->HasCat(NTag));
			QCol->Doing("New Schedule", NTag);
			_Data->Value(NTag, "Label", "Schedule: " + NTag);
			_TrueDataBase[NTag]._Record = NTag;
			_Index();
			return &_TrueDataBase[NTag];
		}

		TSchedule* TSchedule::GetRecord(String Tag) {
			if (!_Data->HasCat(Tag)) {
				QCol->Error("Non-existent record requested!");
				auto ret{ &_TrueDataBase["******FALSE"] };
				ret->_Record = "******FALSE";
				return ret; // Crash prevention!
			}
			_TrueDataBase[Tag]._Record = Tag;
			return &_TrueDataBase[Tag];
		}

		bool TSchedule::Valid() { return _Data->HasCat(_Record); }
		int TSchedule::Hour() { return _Data->IntValue(_Record, "Hour"); }
		int TSchedule::Hour12() { return Hour() % 12 == 0 ? 12 : Hour() % 12; }
		int TSchedule::Minute() { return _Data->IntValue(_Record, "Minute"); }
		int TSchedule::Second() { return _Data->IntValue(_Record, "Second"); }
		void TSchedule::Hour(int value) { _Data->Value(_Record, "Hour", value % 24); _Index(); }
		void TSchedule::Minute(int value) { _Data->Value(_Record, "Minute", value % 60); _Index(); }
		void TSchedule::Second(int value) { _Data->Value(_Record, "Second", value % 60); _Index(); }
		String TSchedule::Time(int h) { return h == 12 ? TrSPrintF("%2d:%2d:2d", Hour12(), Minute(), Second()) + ampm() : TrSPrintF("%2d:%2d:2d", Hour(), Minute(), Second()); }
	}
}
