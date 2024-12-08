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
// Version: 24.12.08 I
// End License

#include "Gwen_CountDown.hpp"
#include "Gwen_Assets.hpp"
#include <SlyvDirry.hpp>

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
			* RemButton{ nullptr };

		

		static void DrawLBCD(j19gadget* b, j19action) {
			static int hue{ 0 }; hue = (hue + 1) % 36000;
			b->SetBackgroundHSV(hue / 100, 1, .25);
			b->SetForeground(255, 180, 0);
			//DatPanel->Visible = b->SelectedItem() >= 0;
		}

		static void DrawRemButton(j19gadget* g, j19action) {
			if (g->DrawY() < AddButton->DrawY() + AddButton->H() + 3) g->Y(g->Y() + 1);
		}
		

		void InitCountDown(j19gadget* CntBack) {
			ListCountDown = CreateListBox(5, 5, 500, CntBack->H() - 175, CntBack);
			ListCountDown->CBDraw = DrawLBCD;
			DatPanel = CreateGroup(ListCountDown->DrawX()+ListCountDown->W() + 50, 5, CntBack->H(), CntBack->W() - (ListCountDown->W() + 15), CntBack);
			AddButton = CreateButton("Add", DatPanel->DrawX(),DatPanel->DrawY()+DatPanel->H()-100,CntBack); // Dirty, but assures me at least that it will always be visible.
			AddButton->SetForeground(0, 255, 0);
			AddButton->SetBackground(0, 25, 0);
			AddButton->SetFont(FntRyanna());
			RemButton = CreateButton("Remove", 0, DatPanel->DrawY() + DatPanel->H() - 100 + 3, DatPanel);
			RemButton->SetFont(FntRyanna());
			RemButton->CBDraw = DrawRemButton;
			RemButton->SetBackgroundHSV(0, 1, .5);
			RemButton->SetForegroundHSV(0, 1, 1);
			
		}
		String TCountDown::V(String Key) { return _Data->Value("REC::" + Record(), Key); }
		void TCountDown::V(String Key, String Value) { _Data->Value("REC::" + Record(), Key, Value); }
		void TCountDown::_Load() {
			if (!_Data) {
				auto f{ Dirry("$AppSupport$/Gwendolyn.CountDown.ini") };
				_Data = LoadOptGINIE(f, f, "Gwendolyn\nCountDown data");
			}
		}
		void TCountDown::ReIndex() {
			_Load();
			_Reg.clear();
			auto c{ _Data->Categories() };
			for (auto i : *c) {
				auto idns{ i.substr(5) };
				auto idn{ ToInt(idns) };
				_Reg[idn]._ID = idn;

			}			
		}
		String TCountDown::Record() { return TrSPrintF("%9d", _ID); }
		String TCountDown::Label(bool actual) {
			auto r{ V("Label") };
			return actual || r.size() ? r : TrSPrintF("Record #%d", _ID);
		}
	}
}
