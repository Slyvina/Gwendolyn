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
// Version: 24.11.30 II
// End License
#include "Gwen_GUI.hpp"
#include "Gwen_Assets.hpp"
#include "Gwen_Clock.hpp"
#include <TQSE.hpp>
#include <SlyvRoman.hpp>
#include <SlyvQCol.hpp>
#include <june19_label.hpp>
#include <june19_picture.hpp>
#include <june19_button.hpp>
#include <SlyvTime.hpp>
#include "Gwen_Config.hpp"


using namespace Slyvina;
using namespace TQSE;
using namespace TQSG;
using namespace Units;
using namespace June19;

#define WinW 1000
#define WinH 600

namespace Slyvina {
	namespace Gwendolyn {

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
			* NormCenter{ nullptr };
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
		static void NormDigiTime(j19gadget* g, j19action) { g->Caption = CurrentTime(); }
#pragma endregion


#pragma region Main

		void Gwen_Init() {
			Graphics(WinW, WinH, "Gwendolyn - (c) Jeroen P. Broks"); Cls(); Flip();
			j19gadget::SetDefaultFont(FntSys());
			// Clock Panel
			auto Clk{ NewPanel("Clock") };
			NormClock = CreateGroup(0, 0, Clk->W() / 2, Clk->W() / 2, Clk);
			auto NormDigi{ CreateLabel(".",(NormClock->W() / 2) - (FntRyanna()->Width("00:00:00") / 2),(int)floor(NormClock->H() * .65),FntRyanna()->Width("00:00:00") ,FntRyanna()->Height("00:00:00"),NormClock) };
			NormDigi->SetForeground(0, 255, 255, 255);
			NormDigi->SetBackground(0, 0, 0, 255);
			NormDigi->CBDraw = NormDigiTime;
			NormDigi->SetFont(FntRyanna());
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
