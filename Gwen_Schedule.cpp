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
// Version: 24.12.03 I
// End License

#include "Gwen_Schedule.hpp"
#include <SlyvDirry.hpp>
#include <SlyvQCol.hpp>
#include <june19.hpp>
#include "Gwen_GUI.hpp"
#include "Gwen_Assets.hpp"
using namespace Slyvina::Units;
using namespace Slyvina::June19;

namespace Slyvina {
	namespace Gwendolyn {

		static std::map<eRepeat,String> _mRepeat{
			{eRepeat::Dont,"Don't"},
			{eRepeat::Daily,"Daily"},
			{eRepeat::Weekly,"Weekly"},
			{eRepeat::Monthly,"Monthly"},
			{eRepeat::Annual,"Annual"}
		};
		static std::map<String, int> MaxDays{
			{"January",31},
			{"February",29}, // Please note, since the year is not given, there's no way the system can tell if it's a leap year or not!
			{"March",31},
			{"April",30},
			{"May",31},
			{"June",30},
			{"July",31},
			{"August",31},
			{"September",30},
			{"October",31},
			{"November",30},
			{"December",31}
		};
		static const char Months[13][20]{ "--", "January","February","March","April","May","June","July","August","September","October","November","December" };
		static const char WeekDayNames[7][10]{ "Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday" };

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
		String sRepeat(eRepeat n) { return _mRepeat[n]; }
		eRepeat sRepeat(String n) {
			Trans2Upper(n);
			for (auto f : _mRepeat) {
				if (n == Upper(f.second)) return f.first;
			}
			QCol->Error("Unknown repeat type! Reverting to basic value!");
			return eRepeat::Dont;
		}

		static j19gadget
			* RecordLabel,
			* LabelText,
			* TimHr, * TimMn, * TimSc,
			* ChkDestroy,
			* ChkActive;
			//* LstWeekDay;
		static std::map<String, j19gadget*> gRepeat{};
		static j19gadget* WeekDayCheck[7];
		static j19gadget* MonthDayCheck[32]; // By exception starting at 1 and ignoring 0. For month and month days that'll work handier.
		

		static void TmCorrect(j19gadget*, j19action) {
			int
				hr{ ToInt(TimHr->Text) },
				mn{ ToInt(TimMn->Text) },
				sc{ ToInt(TimSc->Text) };
			TimHr->Text = hr > 0 ? std::to_string(std::min(23, hr)) : "";
			TimMn->Text = mn > 0 ? std::to_string(std::min(59, mn)) : "";
			TimSc->Text = sc > 0 ? std::to_string(std::min(59, sc)) : "";
			ChkDestroy->Enabled = gRepeat["Don't"]->checked;
			ChkDestroy->checked = ChkDestroy->checked && gRepeat["Don't"]->checked;
		}
		static void RepCol(j19gadget* g, j19action) {
			if (g->checked) {
				g->FR = g->FR > 180 ? --g->FR : g->FR;
				g->FG = g->FG < 255 ? ++g->FG : g->FG;
			} else {
				g->FG = g->FG > 180 ? --g->FG : g->FG;
				g->FR = g->FR < 255 ? ++g->FR : g->FR;
			}
			g->FB = g->FB > 0 ? --g->FB : g->FB;
		}
		static void ColChk(j19gadget* g, j19action) {
			g->FR = g->checked ? 0 : 255;
			g->FG = g->checked ? 255 : 0;
			g->FB = g->FB > 0 ? g->FB - 1 : 0;
			g->Caption = g->checked ? "Yes" : "No";
		}
		static void DrwWeek(j19gadget* g, j19action) {
			static int Hue{ 0 }; 
			if (gRepeat["Weekly"]->checked) {
				g->Enabled = true;
				Hue = (Hue + 1) % 3600;
			} else { g->Enabled = false; }
			g->SetForegroundHSV(Hue / 10, 1, 1);
			g->SetBackgroundHSV(360 - (Hue / 10), 1, .25);
		}
		void Fade(Byte& B, Byte To) { B = B < To ? B + 1 : (B > To ? B - 1 : B); }
		static void DrwWeekDay(j19gadget* g, j19action) {
			g->Enabled = gRepeat["Don't"]->checked || gRepeat["Weekly"]->checked;
			if (g->Enabled) {
				Fade(g->FR, g->checked ? 180 : 255);
				Fade(g->FG, g->checked ? 255 : 0);
				Fade(g->FB, 0);
			} else {
				Fade(g->FR, 127);
				Fade(g->FG, 127);
				Fade(g->FB, 127);
			}
		}
		static void DrawMonthDay(j19gadget* g, j19action) {
			g->Enabled = g->Caption.size() == 2 ? gRepeat["Don't"]->checked || gRepeat["Monthly"]->checked || gRepeat["Annual"]->checked : gRepeat["Don't"]->checked || gRepeat["Annual"]->checked;
			if (g->Enabled) {
				Fade(g->FR, g->checked ? 180 : 255);
				Fade(g->FG, g->checked ? 255 : 0);
				Fade(g->FB, 0);
			} else {
				Fade(g->FR, 127);
				Fade(g->FG, 127);
				Fade(g->FB, 127);
			}
		}


#define schLabField(var,desc) var = CreateLabel("",252,y,ret->W()-300,16,ret); var->SetForeground(255,255,255,255); CreateLabel(desc,2,y,250,16,ret); y+=16;
#define schStrField(var,desc) var = CreateTextfield(252,y,ret->W()-300,16,ret); var->SetForeground(180,255,0,255); var->SetBackground(18,25,0,255); CreateLabel(desc,2,y,250,16,ret); y+=16;
#define schChkField(var,desc) var = CreateCheckBox("",252,y,ret->W()-300,16,ret); var->CBDraw=ColChk; CreateLabel(desc,2,y,250,16,ret); y+=16;
		static j19gadget* CreateSchedulePanel() {
			QCol->Doing("Initizing", "Schedule Edit UI");
			auto ret{ NewPanel("ScheduleEdit") };
			int y{ 2 };
			schLabField(RecordLabel, "Record:");
			schStrField(LabelText, "Label:");
			CreateLabel("Time:", 2, y, 250, 16, ret);
			TimHr = CreateTextfield(252, y, 48, ret);
			TimMn = CreateTextfield(302, y, 48, ret);
			TimSc = CreateTextfield(352, y, 48, ret); y += 20;
			TimHr->SetForeground(0, 180, 255);
			TimMn->SetForeground(0, 180, 255);
			TimSc->SetForeground(0, 180, 255);
			TimHr->SetBackground(0, 18, 25);
			TimMn->SetBackground(0, 18, 25);
			TimSc->SetBackground(0, 18, 25);
			TimHr->CBDraw = TmCorrect;
			CreateLabel("Repeat:", 2, y, 250, 16, ret);			
			for (auto& R : _mRepeat) {
				gRepeat[R.second] = CreateRadioButton(R.second, 252, y, 200, 16, ret); y += 16;
				gRepeat[R.second]->CBDraw = RepCol;
			}
			schChkField(ChkDestroy, "Destroy:");
			CreateLabel("Day of week:", 2, y, 250, 16, ret);
			/*
			LstWeekDay = CreateListBox(252, y, 300, 50, ret);
			LstWeekDay->AddItem("Sunday");
			LstWeekDay->AddItem("Monday");
			LstWeekDay->AddItem("Tuesday");
			LstWeekDay->AddItem("Wednesday");
			LstWeekDay->AddItem("Thursday");
			LstWeekDay->AddItem("Friday");
			LstWeekDay->AddItem("Saturday");
			LstWeekDay->CBDraw = DrwWeek; 
			y += 50;
			*/
			for (int wi = 0; wi < 7; wi++) {
				WeekDayCheck[wi] = CreateCheckBox(WeekDayNames[wi], 252, y, 200, 16, ret); y += 16;
				WeekDayCheck[wi]->CBDraw = DrwWeekDay;
			}
			CreateLabel("Day of Month:", 2, y, 250, 16, ret);
			for (int i = 1; i <= 31; i++) {
				static auto mdx{ 252 };
				static auto mdw{ FntMini()->Width("1234") };
				MonthDayCheck[i] = CreateCheckBox(TrSPrintF("%2d", i), mdx, y,0,0, ret);
				mdx += mdw; if (mdx + mdw > ret->W() - 20) { mdx = 252; y += FntMini()->Height("The quick brown fox jumps over the lazy dog")+3; }
				MonthDayCheck[i]->SetFont(FntMini());
				MonthDayCheck[i]->CBDraw = DrawMonthDay;
			}
			y += FntMini()->Height("The quick brown fox jumps over the lazy dog") + 3;
			CreateLabel("Month:", 2, y, 250, 16, ret);
			for (int i = 1; i <= 12; i++) {
				static auto mdx{ 252 };
				auto mnt{ Months[i] };
				auto mdw{ FntMini()->Width(mnt) + 25 };
				if (mdx + mdw > ret->W() - 20) { mdx = 252; y += FntMini()->Height("The quick brown fox jumps over the lazy dog") + 3; }
				MonthDayCheck[i] = CreateCheckBox(mnt, mdx, y, 0, 0, ret);
				mdx += mdw; 
				MonthDayCheck[i]->SetFont(FntMini());
				MonthDayCheck[i]->CBDraw = DrawMonthDay;
			}
			y += FntMini()->Height("The quick brown fox jumps over the lazy dog") + 3;
			schChkField(ChkActive, "Active");
			return ret;
		}

		void Schedule(String rec) {
			static auto SchedulePanel{ CreateSchedulePanel() };
			GoToPanel("ScheduleEdit");
		}
	}
}
