// License:
// 
// Gwendolyn
// Assets management
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
// Version: 24.12.07 II
// End License
#include "Gwen_Assets.hpp"
#include <SlyvStream.hpp>
#include <SlyvQCol.hpp>
#include <SlyvGINIE.hpp>
#include "Gwen_GUI.hpp"
#include <TQSA.hpp>
#include "Gwen_Schedule.hpp"
using namespace Slyvina::TQSA;
using namespace Slyvina::TQSG;

#define WIJZER_HOT_X 3
#define WIJZER_HOT_Y 42

using namespace Slyvina::JCR6;
using namespace Slyvina::Units;

namespace Slyvina {
	namespace Gwendolyn {
		static String _ResFile{ "" };
		static JT_Dir _Res{ nullptr };
		static GINIE _ResID{ nullptr };
		static std::map<String, TQSG::TImageFont> _Font;
		static TAudio LastAlarm{ nullptr };
		static String LastAlarmFile = "";

		String ResFile(String A0) {
			if (_ResFile.size()) return _ResFile;
			static String Candidates[2]{
#ifndef SlyvWindows
				"/self/proc/exe",
#else
				A0,
#endif
				StripExt(A0) + ".jcr" // In Windows all safe, in Linux and other Unix based systems, this option should only be used for debugging.
			};
			for (auto& rf : Candidates) {
				//QCol->LMagenta(rf + "\n"); // Debug
				if (JCR6_Recognize(rf)!="NONE") { _ResFile = rf; return rf; }
			}
			QCol->Error("No resource present!"); exit(1);
			return _ResFile;
		}

		JCR6::JT_Dir Res(String A0) {
			if (!_Res) { 				
				QCol->Doing("Analysing", ResFile(A0)); _Res = JCR6_Dir(ResFile(A0)); 
			}
			JCR6Assert();
			return _Res;
		}
		void JCR6Assert() {
			if (Last()->Error) {
				QCol->Error(Last()->ErrorMessage);
				QCol->Doing("- Main", Last()->MainFile);
				QCol->Doing("- Entry", Last()->Entry);
				exit(2);
			} else if (!_Res) {
				QCol->Error("JCR6 resource list is null for some silly reason!");
			}
		}
		void CheckAssets(String A0) {
#ifdef SlyvWindows
			A0 = ChReplace(A0, '\\', '/');
#endif
			auto IDS{ Res(A0)->GetString("ID/Identify.ini") };
			_ResID = ParseGINIE(IDS);
			if (_ResID->Value("ID", "Tool") != "Gwendolyn") { QCol->Error("Resource does not appear to be long to Gwendolyn"); exit(3); }
			QCol->Doing("ResBuild", _ResID->Value("ID", "Updated"));
		}

		TQSG::TImageFont GetFont(std::string Name, std::string Tag) {
			Trans2Upper(Tag);
			if (!_Font.count(Tag)) {
				QCol->Doing("Linking font", Name);
				if (!Res()->DirectoryExists(Name)) { Gwen_Panic("Font not found"); return nullptr; }
				_Font[Tag] = TQSG::LoadImageFont(Res(), Name);
			}
			return _Font[Tag];
		}


		TQSG::TImage GetWijzer(bool groot) {
			auto ret{ LoadImage(Res(), groot ? "GFX/Grote Wijzer.png" : "GFX/Kleine Wijzer.png") };
			ret->Hot(WIJZER_HOT_X, WIJZER_HOT_Y);
			return ret;
		}

		void PlayAlarm(bool intern, String File, bool loop) {
			auto Tag = boolstring(intern) + "::" + File;
			if (Tag != LastAlarmFile || (!LastAlarm)) {
				if (intern) {
					auto FFile{ "Assets/Audio/Alarm/" + File + ".mp3" };
					QCol->Doing("Getting intern", FFile);
					if (!Res()->EntryExists(FFile)) { QCol->Error("Internal resource doesn't have that file: "+FFile); return; }
					LastAlarm = LoadAudio(Res(), FFile);
				} else {
					if (!FileExists(File)) {
						QCol->Error("File not found: " + File);
						return;
					}
					QCol->Doing("Getting extern", File);
					LastAlarm = LoadAudio(File);
				}
				if (!LastAlarm) {
					QCol->Error("Alarm sound didn't load: " + ((intern) ? "Internal:" + File : File));
					return;
				}
			}
			QCol->Doing("Playing", "Alarm sound");
			LastAlarm->ChPlay(2) //, loop ? -1 : 0);
		}

		void PlayAlarm(AlarmRef r, bool loop) { PlayAlarm(r.intern, r.File,loop); }

		void Slaan(int times) {
			if (Res()->EntryExists("Audio/Pixabay_Bell.mp3")) {
				static auto Bell{ LoadAudio(Res(),"Audio/Pixabay_Bell.mp3") };
				if (times <= 0) times = 12;
				Bell->ChPlay(0, times-1);
			}
		}

	}
}
