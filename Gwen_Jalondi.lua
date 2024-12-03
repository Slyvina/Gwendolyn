-- License:
-- 
-- Gwendolyn
-- Jalondi Script Of Asset Packaging
-- 
-- 
-- 
-- 	(c) Jeroen P. Broks, 2024
-- 
-- 		This program is free software: you can redistribute it and/or modify
-- 		it under the terms of the GNU General Public License as published by
-- 		the Free Software Foundation, either version 3 of the License, or
-- 		(at your option) any later version.
-- 
-- 		This program is distributed in the hope that it will be useful,
-- 		but WITHOUT ANY WARRANTY; without even the implied warranty of
-- 		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- 		GNU General Public License for more details.
-- 		You should have received a copy of the GNU General Public License
-- 		along with this program.  If not, see <http://www.gnu.org/licenses/>.
-- 
-- 	Please note that some references to data like pictures or audio, do not automatically
-- 	fall under this licenses. Mostly this is noted in the respective files.
-- 
-- Version: 24.12.03
-- End License
SDL2Dir = "../../Libs/TQSL/SDL2/"

Jalondi.AddString(sprintf([[
# ID data
# (c) Jeroen P. Broks - GPL3
[ID]	
Tool=Gwendolyn
Author=Jeroen P. Broks
Updated=%s
]],os.date()),"ID/Identify.ini","Store","Jeroen P. Broks","GPL3")

Jalondi.Add("Assets","","zlib")
Jalondi.Add("../../../../Fonts/DosFont/DosFont.jfbf","Fonts/System.jfbf","zlib","","","System Font") -- Normally I would recommend AGAINST solid blocks for jfbf fonts.
Jalondi.Add("../../../../Fonts/jfbf/Ryanna","Fonts/Ryanna.jfbf","zlib","","","Ryanna Font")
Jalondi.Add("../../../../Fonts/jfbf/Mini","Fonts/Mini.jfbf","zlib","","","Mini Font");

SDL_Licenses = {"FLAC.txt", "modplug.txt", "mpg123.txt","ogg-vorbis.txt","opus.txt","opusfile.txt"}
for _,SDLL in pairs(SDL_Licenses) do
	Jalondi.Add(sprintf("%sLICENSE.%s",SDL2Dir,SDLL),sprintf("Licenses/SDL2/%s",SDLL),"zlib","See file content","License","License_SDL2")
end

Jalondi.Start("Exe/Windows/Gwendolyn.jcr")
