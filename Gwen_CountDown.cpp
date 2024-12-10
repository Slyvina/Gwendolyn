// License:
// 
// Gwendolyn
// CountDown Manager
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

#include "Gwen_CountDown.hpp"
#include "Gwen_Assets.hpp"
#include <SlyvDirry.hpp>
#include <SlyvQCol.hpp>
#include <SlyvTime.hpp>
#include <TQSA.hpp>
using namespace Slyvina::TQSA;

using namespace Slyvina::Units;
using namespace Slyvina::June19;

namespace Slyvina {
	namespace Gwendolyn {

		GINIE TCountDown::_Data{ nullptr };
		std::map<int, TCountDown> TCountDown::_Reg{};

		static j19gadget
			* ListCountDown{ nullptr },
			* DatPanel{ nullptr },
			* AddButton{ nullptr },
			* RemButton{ nullptr },
			* PlayButton{ nullptr },
			* PauseButton{ nullptr },
			* StopButton{ nullptr },
			* AlarmLabel{ nullptr },
			* edlabel{ nullptr },
			* edhour{ nullptr },
			* edminute{ nullptr },
			* edsecond{ nullptr };



		static void DrawLBCD(j19gadget* b, j19action) {
			static int hue{ 0 }; hue = (hue + 1) % 36000;
			b->SetBackgroundHSV(hue / 100, 1, .25);
			b->SetForeground(255, 180, 0);
			DatPanel->Visible = b->SelectedItem() >= 0;
		}

		static void DrawRemButton(j19gadget* g, j19action) {
			if (g->DrawY() < AddButton->DrawY() + AddButton->H() + 3) g->Y(g->Y() + 1);
		}

		static void ActAddButton(j19gadget*, j19action) {
			TCountDown::Create();
		}

		static void ShowLabel(j19gadget* g, j19action) {
			auto r{ TCountDown::Selected() };
			g->Caption = r ? r->Label() : "N/A";
		}
		static void ShowTime(j19gadget* g, j19action) {
			auto r{ TCountDown::Selected() };
			g->Caption = r ? r->RunTime() : "N/A";
			if (r != nullptr) { // This "if" was only required in order to avoid warnings.
				g->FR = r->running ? 255 : 0;
				g->FG = r->running ? (r->paused ? 0 : 180) : 255;
				g->FB = r->running ? 0 : 255;
			}
		}
		static void CDButtons(j19gadget*, j19action) {
			PauseButton->X((DatPanel->W() / 2) - (PauseButton->W() / 2));
			PlayButton->X(PauseButton->X() - (PlayButton->W() + 2));
			StopButton->X(PauseButton->X() + PauseButton->W() + 2);
			auto r{ TCountDown::Selected() };
			if (!r) return;
			PlayButton->Enabled = (!r->running) || r->paused;
			PauseButton->Enabled = r->running;
			StopButton->Enabled = r->running;
		}
		static void CDPlay(j19gadget*, j19action) {
			auto r{ TCountDown::Selected() };
			if (!r) return;
			r->running = true;
			r->paused = false;
		}
		static void CDPause(j19gadget*, j19action) {
			auto r{ TCountDown::Selected() };
			if (!r) return;
			r->paused = !r->paused;
		}

		static void CDStop(j19gadget*, j19action) {
			auto r{ TCountDown::Selected() };
			if (!r) return;
			r->running = false;
			r->paused = false;
			r->hours = r->DHours();
			r->minutes = r->DMinutes();
			r->seconds = r->DSeconds();
		}
		static void DrwAlarm(j19gadget* g, j19action a) {
			static auto t{ 0 };
			static auto Verdwijn{ 10u }, mdu{ 0u };
			auto smd{ (uint32)CurrentSecond() % 2u };
			switch (a) {
			case j19action::Activate:
				Verdwijn = 20u;
				mdu = smd;
				break;
			default:
				g->Visible = Verdwijn > 0;
				g->FA = mdu == smd ? 255 : 0;
				if (t != CurrentSecond()) Verdwijn--;
			}
			t = CurrentSecond();
		}
		static void NoRun(j19gadget* g, j19action) {
			auto r{ TCountDown::Selected() };
			if (!r) return;
			g->Enabled = !r->running;
		}

		static void ActLabel(j19gadget* g, j19action) {
			auto r{ TCountDown::Selected() };
			if (!r) return;
			r->Label(g->Text); 
			//TCountDown::ReIndex();
		}
#define DefA(fn,dt)\
		static void fn(j19gadget* g, j19action) { \
			auto r{ TCountDown::Selected() }; \
			if (!r) return; \
			r->dt(ToInt(g->Text)); \
		}
		DefA(ActHours, DHours);
		DefA(ActMinutes, DMinutes);
		DefA(ActSeconds, DSeconds);
#define DefATB(id,x,cbf)\
		id = CreateTextfield(x, y, 50, 16, DatPanel);\
		id->SetForeground(0, 180, 255);\
		id->SetBackground(0, 18, 255);\
		id->CBAction=cbf;\
		id->CBDraw=NoRun


		static void ActCDL(j19gadget* g, j19action) {
			auto r{ TCountDown::Selected() };
			if (!r) return;
			edlabel->Text = r->Label(true);
			edhour->Text = std::to_string(r->DHours());
			edminute->Text = std::to_string(r->DMinutes());
			edsecond->Text = std::to_string(r->DSeconds());
		}

		void InitCountDown(j19gadget* CntBack) {
			ListCountDown = CreateListBox(5, 5, 500, CntBack->H() - 175, CntBack);
			ListCountDown->CBDraw = DrawLBCD;
			ListCountDown->CBAction = ActCDL;
			DatPanel = CreateGroup(ListCountDown->DrawX() + ListCountDown->W() + 50, 5, CntBack->W() - (ListCountDown->DrawX() + ListCountDown->W() + 60), CntBack->H(), CntBack);
			AddButton = CreateButton("Add", DatPanel->DrawX(), DatPanel->DrawY() + DatPanel->H() - 100, CntBack); // Dirty, but assures me at least that it will always be visible.
			AddButton->SetForeground(0, 255, 0);
			AddButton->SetBackground(0, 25, 0);
			AddButton->SetFont(FntRyanna());
			AddButton->CBAction = ActAddButton;
			RemButton = CreateButton("Remove", 0, DatPanel->DrawY() + DatPanel->H() - 100 + 3, DatPanel);
			RemButton->SetFont(FntRyanna());
			RemButton->CBDraw = DrawRemButton;
			RemButton->SetBackgroundHSV(0, 1, .5);
			RemButton->SetForegroundHSV(0, 1, 1);
			auto CurLabel{ CreateLabel("--",0,10,DatPanel->W(),60,DatPanel,2) };
			CurLabel->CBDraw = ShowLabel;
			CurLabel->SetFont(FntRyanna());
			CurLabel->SetForeground(0, 255, 255);
			auto CurTime{ CreateLabel("Time",0,40,DatPanel->W(),30,DatPanel,2) };
			CurTime->SetFont(FntRyanna());
			CurTime->CBDraw = ShowTime;
			//CurLabel->SetBackground(0, 0, 0, 255); // debug
			PlayButton = CreateButton("A", 0, 75, DatPanel);
			StopButton = CreateButton("B", 0, 75, DatPanel);
			PauseButton = CreateButton("C", 0, 75, DatPanel);
			PlayButton->CBDraw = CDButtons;
			PlayButton->SetForeground(0, 255, 0);
			PauseButton->SetForeground(255, 140, 0);
			StopButton->SetForeground(255, 0, 0);
			PlayButton->SetBackground(0, 25, 0);
			PauseButton->SetBackground(25, 14, 0);
			StopButton->SetBackground(25, 0, 0);
			PlayButton->SetFont(FntCDI());
			StopButton->SetFont(FntCDI());
			PauseButton->SetFont(FntCDI());
			PlayButton->CBAction = CDPlay;
			PauseButton->CBAction = CDPause;
			StopButton->CBAction = CDStop;
			AlarmLabel = CreateLabel("Welcome to Gwendolyn!", 0, 0, WorkScreen()->W(), WorkScreen()->H(), WorkScreen(), 8);
			AlarmLabel->SetFont(FntRyanna()); 
			AlarmLabel->CBDraw = DrwAlarm;
			AlarmLabel->SetForeground(255, 255, 0, 255);
			AlarmLabel->SetBackground(50, 50, 50, 50);
			int y{ 200 };
			CreateLabel("Label:", 0, y, 200, 16, DatPanel);
			edlabel = CreateTextfield(205, y, DatPanel->W() - 210, 16, DatPanel);
			edlabel->SetBackground(18, 25, 0);
			edlabel->SetForeground(180, 255, 0);
			edlabel->CBAction = ActLabel;
			edlabel->CBDraw = NoRun;
			y += 16;
			CreateLabel("Time:", 0, y, 200, 16, DatPanel);
			DefATB(edhour, 205, ActHours);
			DefATB(edminute, 256, ActMinutes);
			DefATB(edsecond, 307, ActSeconds);
			TCountDown::ReIndex();
		}
		void TCountDown::CheckCountDown() {
			static auto ot{ CurrentSecond() };
			static auto tring{ LoadAudio(Res(),"Audio/Tring.mp3") };
			auto nt{ CurrentSecond() };
			if (nt == ot) return;
			_Load();
			for (auto& cdi : _Reg) {
				auto cd{ &_Reg[cdi.first] };
				if (cd->running && (!cd->paused)) {
					cd->seconds--;
					if (cd->seconds < 0 && (cd->minutes>0 || cd->hours>0)) { cd->minutes--; cd->seconds += 60; }
					if (cd->minutes < 0 && cd->hours>0) { cd->hours = std::max(cd->hours - 1, 0); cd->minutes += 60; }
					if (cd->seconds == 0 && cd->minutes == 0 && cd->hours == 0) {
						cd->running = false;
						AlarmLabel->Caption = cd->Label();
						AlarmLabel->Visible = true;
						DrwAlarm(AlarmLabel, j19action::Activate);
						tring->Play();
					}
				}
			}
			ot = nt;
		}

		String TCountDown::V(String Key) { return _Data->Value("REC::" + Record(), Key); }
		void TCountDown::V(String Key, String Value) { _Data->Value("REC::" + Record(), Key, Value); }
		int TCountDown::I(String Key) { return _Data->IntValue("REC::" + Record(), Key); }
		void TCountDown::I(String key, int Value) { _Data->Value("REC::" + Record(), key, Value); }
		void TCountDown::_Load() {
			if (!_Data) {
				auto f{ Dirry("$AppSupport$/Gwendolyn.CountDown.ini") };
				_Data = LoadOptGINIE(f, f, "Gwendolyn\nCountDown data");
			}
		}
		TCountDown::TCountDown() {
			hours = 0;   //= _Data->NewValue("REC::" + Record(),"Time_Hours", 0);
			minutes = 5; //= _Data->NewValue("REC::" + Record(), "Time_Minutes", 5);
			seconds = 0; //= _Data->NewValue("REC::" + Record(), "Time_Seconds", 0);
		}
		void TCountDown::ReIndex() {
			_Load();
			_Reg.clear();
			ListCountDown->ClearItems();
			auto c{ _Data->Categories() };
			for (auto i : *c) {
				auto idns{ i.substr(5) };
				auto idn{ ToInt(idns) };
				_Reg[idn]._ID = idn;
				ListCountDown->AddItem(_Reg[idn].Record() + "::" + _Reg[idn].Label());
			}
		}
		TCountDown* TCountDown::Create() {
			_Load();
			int nid{ 0 };
			while (hasID(++nid));
			QCol->Doing("Creating CD", nid);
			_Reg[nid]._ID = nid;
			_Data->Value(TrSPrintF("REC::%09d", nid), "Created", CurrentDate() + "; " + CurrentTime());
			ReIndex();
			return &_Reg[nid];
		}
		TCountDown* TCountDown::Get(int ID) {
			return hasID(ID) ? &_Reg[ID] : nullptr;
		}

		TCountDown* TCountDown::Selected() {
			return ListCountDown->SelectedItem() >= 0 ? Get(ToInt(ListCountDown->ItemText().substr(0, 9))) : nullptr;
		}

		bool TCountDown::hasID(int ID) { _Load(); return _Data->HasCat(TrSPrintF("REC::%09d", ID)) || _Reg.count(ID); }
		String TCountDown::Record() { return TrSPrintF("%09d", _ID); }
		String TCountDown::Label(bool actual) {
			auto r{ V("Label") };
			return actual || r.size() ? r : TrSPrintF("Record #%d", _ID);
		}
		String TCountDown::RunTime() {
			if (!running) {
				hours = _Data->NewValue("REC::" + Record(), "Time_Hours", 0);
				minutes = _Data->NewValue("REC::" + Record(), "Time_Minutes", 5);
				seconds = _Data->NewValue("REC::" + Record(), "Time_Seconds", 0);
			}
			return TrSPrintF("%2d:%02d:%02d", hours, minutes, seconds);
		}
		void TCountDown::Label(String nv) { V("Label", nv); }
		int TCountDown::DSeconds() { return I("Time_Seconds"); }
		int TCountDown::DMinutes() { return I("Time_Minutes"); }
		int TCountDown::DHours() { return I("Time_Hours"); }
		void TCountDown::DHours(int v) { I("Time_Hours", v); }
		void TCountDown::DMinutes(int v) { I("Time_Minutes", v); }
		void TCountDown::DSeconds(int v) { I("Time_Seconds", v); }
	}
}

