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
// Version: 24.12.07 XI
// End License

#include "Gwen_Schedule.hpp"
#include <SlyvDirry.hpp>
#include <SlyvQCol.hpp>
#include <june19.hpp>
#include "Gwen_GUI.hpp"
#include "Gwen_Assets.hpp"
#include <SlyvTime.hpp>
#include <TQSA.hpp>
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
		VecString TSchedule::_Cats{ nullptr };
		void TSchedule::_Load(bool force) {
			if ((!_Data) || force) _Data = LoadOptGINIE(ScheduleFile(), ScheduleFile(), "Gwendolyn schedule data");
		}
		void TSchedule::_Index(bool dont) {
			_Load();
			auto cats{ _Data->Categories() };
			_Cats = cats;
			_ByTime.clear();
			_ByLabel.clear();
			for (auto c : *cats) {
				_ByTime[TrSPrintF("%02d%02d%02d", _Data->NewValue(c, "Hour", 0), _Data->NewValue(c, "Minute", 0), _Data->NewValue(c, "Second", 0))] = c;
				_ByLabel[_Data->Value(c, "Label").size() ? _Data->Value(c, "Label") : "!!!!!!!!!!!!!!!!!!!!!UNLABELED:" + c] = c;
			}
			if (!dont) RefreshScheduleList(true);
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
			Tag = Prefixed(Upper(Tag), "REC::") ? Tag : "REC::" + Tag;
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
		String TSchedule::Label() { return _Data->Value(_Record, "Label"); }
		void TSchedule::Label(String v) { _Data->Value(_Record, "Label", v); }
		String TSchedule::Repeat() { return _Data->Value(_Record, "Repeat"); }
		void TSchedule::Repeat(String V) { _Data->Value(_Record, "Repeat", V); }
		bool TSchedule::Destroy() { return _Data->BoolValue(_Record, "Destroy"); }
		void TSchedule::Destroy(bool v) { _Data->BoolValue(_Record, "Destroy", v); }
		void TSchedule::WeekDay(String wd, bool v) { _Data->BoolValue(_Record, "Weekday::" + wd, v); }
		bool TSchedule::WeekDay(String wd) { return _Data->BoolValue(_Record, "Weekday::" + wd); }
		void TSchedule::MonthDay(int day, bool v) { _Data->BoolValue(_Record, TrSPrintF("MonthDay_%02d", day), v); }
		bool TSchedule::MonthDay(int day) { return _Data->BoolValue(_Record, TrSPrintF("MonthDay_%02d", day)); }
		void TSchedule::Month(String mn, bool v) { _Data->BoolValue(_Record, "Month_" + mn, v); }
		bool TSchedule::Month(String mn) { return _Data->BoolValue(_Record, "Motn_" + mn); }
		bool TSchedule::MonthSet() {
			for (int i = 1; i <= 12; ++i) if (Month(Months[i])) return true;
			return false;
		}

		bool TSchedule::MonthDaySet() {
			for (int i = 1; i <= 31; ++i) if (MonthDay(i)) return true;
			return false;
		}

		bool TSchedule::WeekDaySet() {
			for (auto wd : WeekDayNames) if (WeekDay(wd)) return true;
			return false;
		}

		void TSchedule::Alarm(bool intern, String File) {
			_Data->BoolValue(_Record, "Alarm_Where", intern);
			_Data->Value(_Record, "Alarm_File", File);
		}

		AlarmRef TSchedule::Alarm() {
			return AlarmRef{ _Data->BoolValue(_Record, "Alarm_Where"), _Data->Value(_Record, "Alarm_File") };
		}

		void TSchedule::Active(bool value) { _Data->BoolValue(_Record, "Active",value); }
		bool TSchedule::Active() { return _Data->BoolValue(_Record, "Active"); }

		void TSchedule::RefreshScheduleList(bool dont) {
			auto& _idx{ SchIndexLabel->checked ? _ByLabel : _ByTime };
			if (!dont) _Index(true);
			ListSchedule->ClearItems();
			for (auto iidx : _idx) {
				QCol->Doing(iidx.first, iidx.second);
				auto Rec{ GetRecord(iidx.second) };
				ListSchedule->AddUniqueItem(Rec->Record().substr(5) + ": " + Rec->Label());
			}
		}

		void TSchedule::Kill(String Tag) {
			Tag = Prefixed(Upper(Tag), "REC::") ? Tag : "REC::" + Tag;
			_Data->Kill(Tag);
			QCol->Doing("Killed SCH", Tag);
			_Index();
		}



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
			* ChkActive,
			* AlarmIntern, * AlarmExtern,
			* IntAlarmList, * ExtAlarmFld, *ExtAlarmButton;
			//* LstWeekDay;
		static std::map<String, j19gadget*> gRepeat{};
		static j19gadget* WeekDayCheck[7];
		static j19gadget* MonthDayCheck[32]; // By exception starting at 1 and ignoring 0. For month and month days that'll work handier.
		static j19gadget* MonthCheck[13];
		

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
		static void DrwHueGadget(j19gadget* g, j19action) {
			static int Hue{ 0 }; 
			/*
			if (gRepeat["Weekly"]->checked) {
				g->Enabled = true;
			} else { g->Enabled = false; }
			//*/
			Hue = (Hue + 1) % 3600;
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
		static void DrwCancelButton(j19gadget* g, j19action) { g->Y(g->GetParent()->H() + 2); }
		static void ActCancelButton(j19gadget*, j19action) { GoToPanel("Schedule"); }
		static void DrwAlarm(j19gadget* g, j19action) {
			Fade(g->FR, g->checked ? 180 : 250);
			Fade(g->FG, g->checked ? 255 : 0);
			Fade(g->FB, 0);
			Fade(g->BG, g->checked ? 25 : 0);
			Fade(g->BA, g->checked ? 255 : 0);
			g->BR = 0;
			g->BB = 0;
		}
		static void AlarmShow(j19gadget*, j19action) {
			IntAlarmList->Visible = AlarmIntern->checked;
			ExtAlarmFld->Visible = AlarmExtern->checked;
			ExtAlarmButton->Visible = AlarmExtern->checked;
#ifndef  SlyvWindows
			ExtAlarmButton->Enabled = false; // No Linux support YET!
#endif // ! SlyvWindows
		}
		static void PreviewAlarm(j19gadget*, j19action) {
			PlayAlarm(AlarmIntern->checked, AlarmIntern->checked ? IntAlarmList->ItemText() : ExtAlarmFld->Text);
		}

		static void ActOkay(j19gadget*,j19action) {
			auto Rec{ RecordLabel->Caption[0] == '*' ? TSchedule::NewRecord() : TSchedule::GetRecord(RecordLabel->Caption) };
			Rec->Label(LabelText->Text.size() ? LabelText->Text : Rec->Label());
			Rec->Hour(ToInt(TimHr->Text));
			Rec->Minute(ToInt(TimMn->Text));
			Rec->Second(ToInt(TimSc->Text));
			String Rep{ "Don't" };
			for (auto gt : gRepeat) { 
				auto g{ gt.second };
				if (g->checked) Rep = g->Caption;
			}
			Rec->Repeat(Rep);
			Rec->Destroy(ChkDestroy->checked);
			for (int wi = 0; wi < 7; wi++) Rec->WeekDay(WeekDayCheck[wi]->Caption, WeekDayCheck[wi]->checked);
			Rec->Alarm(AlarmIntern->checked, AlarmIntern->checked ? IntAlarmList->ItemText() : ExtAlarmFld->Text);
			Rec->Active(ChkActive->checked);
			GoToPanel("Schedule");
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
				MonthCheck[i] = CreateCheckBox(mnt, mdx, y, 0, 0, ret);
				mdx += mdw; 
				MonthCheck[i]->SetFont(FntMini());
				MonthCheck[i]->CBDraw = DrawMonthDay;
			}
			y += (FntMini()->Height("The quick brown fox jumps over the lazy dog") + 3)*2;
			CreateLabel("Alarm:", 2, y, 250, 16, ret);
			auto Alarm{ CreateGroup(252, y, 500, 16, ret) };
			AlarmIntern = CreateRadioButton("Intern", 0, 0,  200,16, Alarm);
			AlarmExtern = CreateRadioButton("Extern", 200, 0, 200, 16, Alarm);
			AlarmIntern->CBDraw = DrwAlarm;
			AlarmExtern->CBDraw = DrwAlarm;
			y += 16;
			IntAlarmList = CreateListBox(252, y, 300, 50,ret);
			schStrField(ExtAlarmFld, "Alarm File: ");
			ExtAlarmButton = CreateButton("...", ExtAlarmFld->DrawX() + ExtAlarmFld->W(),ExtAlarmFld->Y(), ret);
			ExtAlarmButton->SetFont(FntMini());
			ExtAlarmButton->SetForeground(250, 180, 0);
			ExtAlarmButton->SetBackground(25, 18, 0);
			Alarm->CBDraw = AlarmShow;
			IntAlarmList->CBDraw = DrwHueGadget;
			auto Preview{ CreateButton("Play",IntAlarmList->DrawX() + IntAlarmList->W() + 75,y,ret) };
			Preview->SetForeground(255, 255, 0);
			Preview->SetBackground(0, 0, 255);
			Preview->CBAction = PreviewAlarm;
			y += 50;
			// Assets/Audio/Alarm /
			for (auto& e : Res()->_Entries) {
				if (ExtractDir(e.first) == "ASSETS/AUDIO/ALARM") IntAlarmList->AddItem(StripAll(e.second->Name()));
			}
			schChkField(ChkActive, "Active");
			auto Ok{ CreateButton("Ok",2,y,ret) };
			Ok->SetBackground(0, 25, 0);
			Ok->SetForeground(0, 255, 0);
			Ok->SetFont(FntRyanna());
			auto Cancel{ CreateButton("Cancel",2,y,Ok) };
			Cancel->SetBackground(25, 0, 0);
			Cancel->SetForeground(255, 0, 0);
			Cancel->CBDraw = DrwCancelButton;
			Cancel->SetFont(FntRyanna());
			Cancel->CBAction = ActCancelButton;
			Ok->CBAction = ActOkay;
			return ret;
		}

		void Schedule(String rec) {
			static auto SchedulePanel{ CreateSchedulePanel() };
			GoToPanel("ScheduleEdit");
			QCol->Doing("Record", rec);
			if (rec == "*new") {
				RecordLabel->Caption = "*New record*";
				LabelText->Text = "";
				TimHr->Text = std::to_string((CurrentHour() + 2) % 24);
				TimMn->Text = std::to_string(CurrentMinute());
				TimSc->Text = std::to_string(CurrentSecond());
				for (auto wd : gRepeat) wd.second->checked = wd.first == "Don't";
				ChkDestroy->Enabled = false;
				for (int i = 0; i <= 6; ++i) WeekDayCheck[i]->checked = false;
				for (int i = 1; i <= 31; ++i) MonthDayCheck[i]->checked = false;
				for (int i = 1; i <= 12; ++i) MonthCheck[i]->checked = false;
				ChkActive->checked = true;
				AlarmIntern->checked = true;
				AlarmExtern->checked = false;
				IntAlarmList->SelectItem(0);
				ExtAlarmFld->Caption = "";
			} else {
				//QCol->Warn("Fetching record '" + rec + "' not yet possible");
				auto GR{ TSchedule::GetRecord(rec) };
				RecordLabel->Caption = rec;
				LabelText->Text = GR->Label();
				TimHr->Text = std::to_string(GR->Hour());
				TimMn->Text = std::to_string(GR->Minute());
				TimSc->Text = std::to_string(GR->Second());
				for (auto wd : gRepeat) wd.second->checked = GR->Repeat() == wd.first;
				ChkDestroy->checked = GR->Destroy();
				for (int i = 0; i <= 6; ++i) WeekDayCheck[i]->checked = GR->WeekDay(WeekDayCheck[i]->Caption);
				for (int i = 1; i <= 31; ++i) MonthDayCheck[i]->checked = GR->MonthDay(i);
				for (int i = 1; i <= 12; ++i) MonthCheck[i]->checked = GR->Month(MonthCheck[i]->Caption);
				ChkActive->checked = GR->Active();
				auto A{ GR->Alarm() };
				AlarmIntern->checked = A.intern;
				AlarmExtern->checked = !A.intern;
				if (A.intern) {
					IntAlarmList->SelectItem(A.File);
					ExtAlarmFld->Caption = "";
				} else {
					ExtAlarmFld->Caption = A.File;
					IntAlarmList->SelectItem(0);
				}
				ChkActive->checked = GR->Active();
			}
		}

		static j19gadget
			* BarSchedule{ nullptr },
			* BarLabel{ nullptr },
			* BarSnooze{ nullptr },
			* BarKill{ nullptr };
		static TSchedule* Active{ nullptr };
		void HideScheduleAlarm() {
			if (BarSchedule) BarSchedule->Visible = false;
			Active = nullptr;
			Mix_HaltChannel(2);
		}
		static void DrawBarSchedule(j19gadget* g, j19action) {
			if ((!Active) || (Active->SecAlarmCountDown<=0)) { HideScheduleAlarm(); return; }
			if (Active->SecAlarmSnooze > 0) {
				g->SetBackground(0, 100, 0);
			} else {
				static int d{ 0 }; d = (d + 1) % 360;
				static double v{ abs(sin(((double)d * PI) * 180)) };
				g->SetBackgroundHSV(0, 1, v);
			}
			BarLabel->Caption = Active->Label();
		}
		static void DrawSnooze(j19gadget* g, j19action) { g->Enabled = Active && Active->SecAlarmSnooze <= 0; g->Y(BarKill->Y() - g->H() - 2); }
		static void DrawKill(j19gadget* g, j19action) {
			if (!Active) return;
			g->Caption = Active->Destroy() ? "Terminate and delete" : "Terminate";
			g->Y(BarSchedule->H() - g->H());				
			Mix_HaltChannel(2);
		}
		static void ActSnooze(j19gadget*, j19action) {
			if (!Active) return;
			Active->SecAlarmSnooze = 300;
		}
		static void ActKill(j19gadget*, j19action) { 
			if (!Active) return;
			if (Active->Destroy()) { TSchedule::Kill(Active->Record()); }
			HideScheduleAlarm(); 
		}


		static void InitBarSchedule() {
			if (BarSchedule) return;
			BarSchedule = CreatePanel(1, WorkScreen()->H() - 201, WorkScreen()->W()-2, 200, WorkScreen());
			BarLabel = CreateLabel("N/A", 5, 5, WorkScreen()->W(), 40, BarSchedule);
			BarLabel->SetFont(FntRyanna());
			BarLabel->SetForeground(255, 255, 0);
			BarLabel->CBDraw = DrawBarSchedule;
			BarSnooze = CreateButton("Snooze", 0, 0, BarSchedule);
			BarKill = CreateButton("End", 0, 0, BarSchedule);
			BarSnooze->CBDraw = DrawSnooze;
			BarKill->CBDraw = DrawKill;
			BarSnooze->CBAction = ActSnooze;
			BarKill->CBAction = ActKill;
		}

		#define BrAct  Active = rec; break
		void CheckScheduleAlarm() {
			static auto oldtime{ CurrentTime() };
			auto newtime(CurrentTime());
			if (newtime == oldtime) return;
			//QCol->Doing("Debug", "Schedule Check");
			oldtime = newtime;
			if (Active) {
				if (Active->SecAlarmSnooze > 0) {
					if ((--Active->SecAlarmSnooze) <= 0) PlayAlarm(Active->Alarm(), true);
				}
				if (--Active->SecAlarmCountDown <= 0) {
					HideScheduleAlarm();
					Active = nullptr;
					return; // safety pre-caution, in case I may need to add code later that could bump into a null pointer.
				}
			}
			auto H{ CurrentHour() }, M(CurrentMinute()), S{ CurrentSecond() };
			auto recs{ TSchedule::Records() };
			for (auto rid : *recs) {
				auto rec{ TSchedule::GetRecord(rid) };
				//QCol->Doing("Debug", "Check: " + rid+ " Active: "+boolstring(rec->Active()));
				if (!rec->Active()) continue;
				//QCol->Doing("Debug", TrSPrintF("Time check %02d:%02d:%02d -> %02d:%02d:%02d", H, M, S, rec->Hour(), rec->Minute(), rec->Second()));
				if (H != rec->Hour() || M != rec->Minute() || S != rec->Second()) continue;
				if (rec->Repeat() == "Daily") { BrAct; }
				else if (rec->Repeat() == "Weekly") {
					 if (rec->WeekDay(WeekDay()))  { BrAct; }
				}
				else if (rec->Repeat() == "Monthly") { if (rec->MonthDay(CurrentDay())) { BrAct; } }
				else if (rec->Repeat() == "Annual") { if (rec->MonthDay(CurrentDay()) && rec->Month(CurrentMonthName())) { BrAct; } }
				else if (rec->Repeat() == "Don't") {
					auto Ok{ true };
					if (rec->WeekDaySet()) Ok = rec->WeekDay(WeekDay());
					if (Ok && rec->MonthDaySet()) Ok = rec->MonthDay(CurrentDay());
					if (Ok && rec->MonthSet()) Ok = rec->Month(CurrentMonthName());
					if (Ok) { BrAct; }
				} else {
					QCol->Error("Unknown repeat value \"" + rec->Repeat() + "\" in record \"" + rec->Record() + "\".\7");
				}
			}
			if (Active) {
				QCol->Doing("Alarm:", Active->Record());
				InitBarSchedule();
				Active->SecAlarmCountDown = 60 * 60;
				Active->SecAlarmSnooze = 0;
				auto A{ Active->Alarm() };
				PlayAlarm(A.intern, A.File, true);
				BarSchedule->Visible = true;
				if (Active->Repeat() != "Don't") Active->Destroy(false);
			}
		}
	}
}
