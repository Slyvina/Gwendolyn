// License:
// 
// Gwendolyn
// Graphic User Interface
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
// Version: 24.12.07
// End License

#include "Gwen_GUI.hpp"
#include "Gwen_Assets.hpp"
#include "Gwen_Clock.hpp"
#include <TQSE.hpp>
#include <SlyvRoman.hpp>
#include <SlyvQCol.hpp>
#include <SlyvWindows.hpp>
#include <june19.hpp>
#include <SlyvTime.hpp>
#include "Gwen_Config.hpp"
#include <SlyvPhantasarTime.hpp>
#include "Gwen_Schedule.hpp"


using namespace Slyvina;
using namespace TQSE;
using namespace TQSG;
using namespace Units;
using namespace June19;

#define WinW 1000
#define WinH 600

namespace Slyvina {
	namespace Gwendolyn {

		j19gadget
			* ListSchedule{ nullptr },
			* SchIndexLabel{ nullptr },
			* SchIndexTime{ nullptr };


#pragma region ClockSpot
		ClockSpot DegSpot(double deg,double straal) {						
			auto r{ ((deg-90) * (PI / 180)) };
			//printf("Degrees: %f(%f) -> %f\n", 360 - deg, deg, r);
			ClockSpot ret{
				(int)floor(cos(r) * straal),
				(int)floor(sin(r) * straal)
			};
			return ret;
		}
#pragma endregion

#pragma region Panels
		static std::map<String, j19gadget*> _Panel{};
		static j19gadget
			* NormClock{ nullptr },
			* PhanClock{ nullptr },
			* NormCenter{ nullptr },
			* SchIndexGroup{ nullptr };
		static TImage GroteWijzer{ nullptr }, KleineWijzer{ nullptr };


		June19::j19gadget* Panel(std::string _PanID) {
			Trans2Upper(_PanID);
			if (!_Panel.count(_PanID)) { Gwen_Panic("Panel " + _PanID + " does not exist"); return CreatePanel(0, 0, 0, 0, WorkScreen()); }
			return _Panel[_PanID];
		}
		June19::j19gadget* NewPanel(std::string _PanID) {
			Trans2Upper(_PanID);
			_Panel[_PanID] = _Panel.count(_PanID) ? _Panel[_PanID] : CreatePanel(0, 0, WorkScreen()->W(), WorkScreen()->H(), WorkScreen());
			_Panel[_PanID]->SetBackground(18, 0, 25, 255);
			auto Img{ "GFX/Panel/" + _PanID + ".png" };
			if (Res()->EntryExists(Img)) {
				QCol->Doing("Loading image", Img);
				auto Pic{ LoadImage(Res(),Img) };
				auto GPC{ CreatePicture(_Panel[_PanID]->W() - Pic->Width(),_Panel[_PanID]->H() - Pic->Height(),Pic->Width(),Pic->Height(),_Panel[_PanID],Pic_FullStretch)};
				if (GPC->H() > _Panel[_PanID]->H()) {
					auto B{ (double) _Panel[_PanID]->H() / GPC->H() };
					GPC->W((int)floor((double)GPC->W() * B));
					GPC->H(_Panel[_PanID]->H());
					GPC->X(_Panel[_PanID]->W()-GPC->W());
					GPC->Y(0); 					
				}
				GPC->Image(Pic);
				printf("\x1b[91mDEBUG> Image: %s -> (%d,%d) %dx%d  (Pic: %d,%d)\n", Img.c_str(), GPC->X(), GPC->Y(), GPC->W(), GPC->H(),Pic->Width(),Pic->Height());
			} else { QCol->Doing("Skipping", Img); }
			return _Panel[_PanID];
		}
		void GoToPanel(std::string _PanID) {
			Trans2Upper(_PanID);
			for (auto& _p : _Panel) _p.second->Visible = _p.first == _PanID;
		}
#pragma endregion

#pragma region Clock
		static void ShowClock(j19gadget* g, j19action) {
			auto KDeg{(TotSec12()/ 43200.0)*360 };
			auto GDeg{ (TotSec() / 3600.0) * 360 };
			auto SDeg{ (Sec() / 60.0) * 360 };
			//auto SDeg{ DegSpot(((Sec() / 60.0) * 360),75 ) };
			SetScale(0.5, 5); SetColor(180, 0, 255); Rotate(SDeg); GroteWijzer->XDraw(g->X(), g->Y());
			SetScale(3, 4);			
			//SetColor(255, 255, 255); Line(g->X(), g->Y(), SDeg.X, SDeg.Y);
			SetColor(50, 36, 0); Rotate(KDeg); KleineWijzer->XDraw(g->X(), g->Y());
			SetColor(255, 180, 0); Rotate(GDeg); GroteWijzer->XDraw(g->X(), g->Y());
			Rotate(0);
			SetScale(1, 1);
			static String LastHour{ QTimeF("%H") }; 
			if (LastHour != QTimeF("%H") && ConfigSlaan()) { Slaan(ToInt(QTimeF("%I"))); LastHour = QTimeF("%H"); }
		}
		static void ShowPhanClock(j19gadget* g, j19action) {
			auto Mil{ Phan_TotalMillos() };
			auto KDeg{ (Mil/20000.0) * 360 };
			auto GDeg{ ((Mil%10000) / 1000.0) * 360 };
			SetScale(3, 4);
			SetColor(50, 36, 0); Rotate(360-KDeg); KleineWijzer->XDraw(g->X(), g->Y());
			SetColor(255, 180, 0); Rotate(360-GDeg); GroteWijzer->XDraw(g->X(), g->Y());
			Rotate(0);
			SetScale(1, 1);
			static int LastVecta{ Phan_Vectas() };		
			int Vecta{ Phan_Vectas() };
			if (LastVecta != Vecta && ConfigSlaan()) { Slaan(Vecta ? Vecta : 20); LastVecta = Vecta; }
		}
		
		static void NormDigiTime(j19gadget* g, j19action) { g->Caption = CurrentTime(); }
		static void PhanDigiTime(j19gadget* g, j19action) { g->Caption = TrSPrintF("%2d:%03d", Phan_Vectas(), Phan_Millos()); }
		static void IsEarth(j19gadget* g, j19action) { NormClock->Visible = g->checked; }
		static void IsPhan(j19gadget* g, j19action) { PhanClock->Visible = g->checked; }
		static void SetClockType(j19gadget* g, j19action) { Config()->Value("Clock", "Type", g->Caption); }
		static void SetTermRelease(j19gadget* g, j19action) { 
			static auto before{ false };
			Config()->Value("Windows", "TerminalRelease", uboolstring(g->checked)); 
			if (!before) { Notify("Please note that this setting only takes effect during the start up of Gwendolyn!"); }
			before = true;
		}
		static void SetBell(j19gadget* g, j19action) { Config()->Value("Bell", "Bell", uboolstring(g->checked)); }
		static void ActButton(j19gadget* g, j19action) { GoToPanel(g->Caption); }
		static void ActBack(j19gadget*, j19action) { GoToPanel("Clock"); }
		static void DrwBack(j19gadget* g, j19action) {
			auto p{ g->GetParent() };
			g->Y(p->Y() - g->H());
			g->FG ? g->FG-- : g->FG;
			g->FB = g->FG;
			g->BR < 25 ? g->BR++ : g->BR;
		}
		static void DrwWeekday(j19gadget* g, j19action) { g->Caption = QTimeF("%A"); }
		static void DrwData(j19gadget* g, j19action) { g->Caption = QTimeF("%B %d, %Y"); }
		static void DrwPhanDate(j19gadget* g, j19action) { g->Caption = PhanDate(); }
		static void ChkSchIndex(j19gadget* g, j19action) { 
			if (g->checked) {
				QCol->Doing("Schedule Index", g->Caption.substr(9));
				Config()->Value("Schedule","Index", g->Caption.substr(9));
			}
		}
		static void DrwSchedButtons(j19gadget* g, j19action) {
			auto Next{ (j19gadget*)g->bData };
			g->Y(Next ? Next->Y() - g->H() - 1 : WorkScreen()->Y() - 5 - g->H());
			g->X(std::min(g->X() + 1, SchIndexGroup->X()));
			g->Enabled = g->Caption == "Add" || ListSchedule->SelectedItem() >= 0;
		}

		static void ActSchedButtons(j19gadget*g,j19action){ Schedule(g->Caption == "Add" ? "*new" : ListSchedule->ItemText()); }
		static void ActSchedRemove(j19gadget* g, j19action) {  }
#pragma endregion


#pragma region Main

		void Gwen_Init() {
#ifdef SlyvWindows
			if (Upper(Config()->Value("Windows", "TerminalRelease")) == "TRUE") { 
				QCol->Doing("Releasing", "Console");
				ReleaseConsole(); 
			}
#endif
			Graphics(WinW, WinH, "Gwendolyn - (c) Jeroen P. Broks"); Cls(); Flip();
			j19gadget::SetDefaultFont(FntSys());
			// Clock Panel
			auto Clk{ NewPanel("Clock") };
			// Normal Earth Clock
			NormClock = CreateGroup(0, 0, Clk->W() / 2, Clk->W() / 2, Clk);
			auto NormDigi{ CreateLabel(".",(NormClock->W() / 2) - (FntRyanna()->Width("00:00:00") / 2),(int)floor(NormClock->H() * .65),FntRyanna()->Width("00:00:00") ,FntRyanna()->Height("00:00:00"),NormClock) };
			NormDigi->SetForeground(0, 255, 255, 255);
			NormDigi->SetBackground(0, 0, 0, 255);
			NormDigi->CBDraw = NormDigiTime;
			NormDigi->SetFont(FntRyanna());
			auto SatW = FntSys()->Width("Wednesday");
			auto DatW = FntSys()->Width("September 30th, 8888");
			auto DatY = (int)floor(NormClock->H() * .20);
			auto WeekDay{ CreateLabel("",(NormClock->W() / 2) - (SatW / 2),DatY,SatW,16,NormClock,2) };
			WeekDay->SetForeground(0, 255, 255, 255);
			WeekDay->SetBackground(0, 0, 0, 255);
			WeekDay->CBDraw = DrwWeekday;
			auto DatLab{ CreateLabel("",(NormClock->W() / 2) - (DatW / 2),DatY + 16,DatW,16,NormClock,2) };
			DatLab->SetForeground(0, 255, 255, 255);
			DatLab->SetBackground(0, 0, 0, 255);
			DatLab->CBDraw = DrwData;

			for (auto uur = 1; uur <= 12; uur++) {
				auto pos{ DegSpot((uur % 12) * 30,200) };
				auto midden{ NormClock->W() / 2 };
				auto uurl{ CreateLabel(ToRoman(uur),pos.X+midden,pos.Y+midden,0,0,NormClock,8) };
				uurl->SetFont(FntLiquid());				 
			}
			NormCenter = CreateGroup(NormClock->W() / 2, NormClock->W() / 2, 0, 0, NormClock);
			GroteWijzer = GetWijzer(true);
			KleineWijzer = GetWijzer(false);
			NormCenter->CBDraw = ShowClock;

			// Phantasar Clock
			PhanClock = CreateGroup(0, 0, Clk->W() / 2, Clk->W() / 2, Clk);
			auto PhanDigi{ CreateLabel(".",(PhanClock->W() / 2) - (FntRyanna()->Width("00:000") / 2),(int)floor(PhanClock->H() * .65),FntRyanna()->Width("00:000") ,FntRyanna()->Height("00:000"),PhanClock ) };
			PhanDigi->SetForeground(0, 255, 255, 255);
			PhanDigi->SetBackground(0, 0, 0, 255);
			PhanDigi->CBDraw = PhanDigiTime;
			PhanDigi->SetFont(FntRyanna());
			auto PhanDW{ FntRyanna()->Width("Dopplosephuh-99") };
			auto PhanDatum{ CreateLabel("",(PhanClock->W() / 2) - (PhanDW / 2),(int)(PhanClock->H() * .35),PhanDW,FntRyanna()->Height("WPwp"),PhanClock,2) };
			PhanDatum->SetBackground(0, 0, 0, 0xff);
			PhanDatum->SetForeground(0, 255, 255, 255);
			PhanDatum->CBDraw = DrwPhanDate;
			PhanDatum->SetFont(FntRyanna());
			for (auto vec = 1; vec <= 20; vec++) {
				auto pos{ DegSpot(vec * 18,220) };
				auto midden{ PhanClock->W() / 2 };
				auto uurl{ CreateLabel(std::to_string(20-vec),pos.X + midden,pos.Y + midden,0,0,PhanClock,8)};
				uurl->SetFont(FntLiquid());
			}
			auto PhanCenter = CreateGroup(PhanClock->W() / 2, PhanClock->W() / 2, 0, 0, PhanClock);
			PhanCenter->CBDraw = ShowPhanClock;
			// Select clock
			auto ClkGroup{ CreateGroup(NormClock->W(),0,0,60,Clk) };
			auto ClkEarth{ CreateRadioButton("Earth",0,0,0,0,ClkGroup,Config()->NewValue("Clock","Type","Earth") == "Earth") };
			auto ClkPhantasar{ CreateRadioButton("Phantasar",0,16,0,0,ClkGroup,Config()->NewValue("Clock","Type","Earth") == "Phantasar") };
			ClkEarth->SetForeground(255, 180, 0);
			ClkEarth->CBDraw = IsEarth;
			ClkEarth->CBAction = SetClockType;
			ClkPhantasar->SetForeground(180, 255, 0);
			ClkPhantasar->CBDraw = IsPhan;
			ClkPhantasar->CBAction = SetClockType;
			auto TermRelease{ CreateCheckBox("Release Terminal (Windows Only)",0,32,0,0,ClkGroup) };
#ifdef SlyvWindows
			TermRelease->SetForeground(180, 0, 255, 255);
			TermRelease->CBAction = SetTermRelease;
			TermRelease->checked = Upper(Config()->Value("Windows", "TerminalRelease")) == "TRUE";
#else
			TermRelease->Enabled = false;
#endif
			auto Bell{ CreateCheckBox("Bell",0,48,0,0,ClkGroup) };
			Bell->checked = ConfigSlaan();
			Bell->CBAction = SetBell;
			Bell->SetForeground(0, 180, 255, 255);
			auto Schedule{ CreateButton("Schedule",0,200,ClkGroup) };
			Schedule->SetForeground(255, 180, 0);
			Schedule->SetBackground(25, 18, 0);
			Schedule->CBAction = ActButton;
			auto CountDown{ CreateButton("Countdown",0,225,ClkGroup) };
			CountDown->SetForeground(255, 180, 0);
			CountDown->SetBackground(25, 18, 0);
			CountDown->CBAction = ActButton;
			// Schedule
			auto SchPanel{ NewPanel("Schedule") };
			auto SchBack{ CreateButton("Back",0,0,SchPanel) };
			SchBack->CBDraw = DrwBack;
			SchBack->CBAction = ActBack;
			ListSchedule = CreateListBox(5, 5, 400, SchPanel->H() - 40, SchPanel);
			ListSchedule->SetForeground(255, 180, 0);
			ListSchedule->SetBackground(90, 0, 100);
			auto LSW{ ListSchedule->W() + 60 };
			SchIndexGroup = CreateGroup(LSW, 0, SchPanel->W() - LSW, 32, SchPanel);
			SchIndexLabel = CreateRadioButton("Index by label", 0, 0, 0, 0, SchIndexGroup, Upper(Config()->NewValue("Schedule", "Index", "label")) == "LABEL");
			SchIndexTime = CreateRadioButton("Index by time", 0, 16, 0, 0, SchIndexGroup, Upper(Config()->NewValue("Schedule", "Index", "label")) == "TIME");
			SchIndexLabel->SetForeground(255, 180, 0);
			SchIndexLabel->CBAction = ChkSchIndex;
			SchIndexTime->CBAction = ChkSchIndex;
			SchIndexTime->SetForeground(255, 180, 0);
			auto SchIndexAdd{ CreateButton("Add",40,0,SchPanel) };
			auto SchIndexEdit{ CreateButton("Edit",20,0,SchPanel) };
			auto SchIndexRemove{ CreateButton("Remove",0,0,SchPanel) };
			SchIndexAdd->SetForeground(0, 255, 0);
			SchIndexAdd->SetBackground(0, 25, 0);
			SchIndexAdd->bData = SchIndexEdit;
			SchIndexAdd->SetFont(FntRyanna());
			SchIndexEdit->SetForeground(255, 255, 0);
			SchIndexEdit->SetBackground(25, 25, 0);
			SchIndexEdit->bData = SchIndexRemove;
			SchIndexEdit->SetFont(FntRyanna());
			SchIndexRemove->SetForeground(255, 0, 0);
			SchIndexRemove->SetBackground(25, 0, 0);
			SchIndexRemove->bData = nullptr;
			SchIndexRemove->SetFont(FntRyanna());
			SchIndexAdd->CBAction = ActSchedButtons;
			SchIndexEdit->CBAction = ActSchedButtons;
			SchIndexRemove->CBAction = ActSchedRemove;
			SchIndexAdd->CBDraw = DrwSchedButtons;
			SchIndexEdit->CBDraw = DrwSchedButtons;
			SchIndexRemove->CBDraw = DrwSchedButtons;
			



			// Countdown
			auto CntPanel{ NewPanel("Countdown") };
			auto CntBack{ CreateButton("Back",0,0,CntPanel) };
			CntBack->CBDraw = DrwBack;
			CntBack->CBAction = ActBack;

			// Must be last!
			TSchedule::RefreshScheduleList();
			GoToPanel("Clock");
		}

		void Gwen_Run() {
			while (true) {
				Cls(); Poll();
				if (AppTerminate()) return;
				Screen()->Draw();
				Flip();
			}
		}

		void Gwen_Done() {
			FreeJune19();
			CloseGraphics();
		}

		static void Gwen_Panic_Exit(j19gadget* g, j19action) { Gwen_Exit(ToInt(g->HData)); }
		void Gwen_Panic(std::string Error, int exitcode) {
			QCol->Error(Error);
			for (auto& P : _Panel) P.second->Visible = false;
			auto
				ErrPan{ CreatePanel(0, 0, ScreenWidth(), ScreenHeight(), WorkScreen()) },
				ErrDeath{ CreatePicture(0,0,0,ScreenHeight(),ErrPan,Pic_FullStretch) },
				ErrHead{ CreateLabel("Iskarderiu! We got a problem!", 0, 0, 0, 0, ErrPan) },
				ErrMsg{ CreateLabel(Error,0,30,0,0,ErrPan) },
				ErrButton{ CreateButton("Exit Gwendolyn",5,ScreenHeight() - 40,ErrPan) };
			auto Death{ LoadImage(Res(),"GFX/Death.png") };
			ErrPan->SetBackground(0, 0, 255, 255);
			ErrDeath->SetForeground(0, 180, 255, 255);			
			ErrHead->SetForeground(255, 0, 255);
			ErrMsg->SetForeground(255, 255, 0);
			ErrButton->SetForeground(255, 0, 0);
			ErrButton->SetBackground(25, 0, 0);
			ErrButton->HData = std::to_string(exitcode);
			ErrButton->CBAction = Gwen_Panic_Exit;
			if (Death) {
				ErrDeath->W((int)floor(((double)ErrDeath->H() / Death->Height()) * ErrDeath->W()));
			}
		}

		void Gwen_Exit(int code) {
			Gwen_Done();
			exit(code);
		}
#pragma endregion
	}
}
