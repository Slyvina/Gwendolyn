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
-- Version: 24.12.04
-- End License
SDL2Dir = "../../Libs/TQSL/SDL2/"

function file_exists(name)
   local f=io.open(name,"r")
   if f~=nil then io.close(f) return true else return false end
end

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

-- None of these are present in my repository (copyright reasons)
-- You may need to adept this script in order to put in your own alarms.
XAssets = {
	{"Sound Effects/Pixabay/relaxing-guitar-loop-v5-245859.mp3","Guitar Loops.mp3"},
	{"Sound Effects/Pixabay/stab-f-01-brvhrtz-224599.mp3","Stab.mp3"},
	{"Sound Effects/Pixabay/ascent-braam-magma-brass-d-cinematic-trailer-sound-effect-222269.mp3","Braam.mp3"},
	{"Sound Effects/Pixabay/labyrinth-for-the-brain-190096.mp3","Labyrinth for the brain.mp3"}
}
for _,a in pairs(XAssets) do
	local fa="../../../../3rd Party Assets/"..a[1]
	if file_exists(fa) then
		Jalondi.Add(fa,"Assets/Audio/Alarm/"..a[2],"zlib")
		Jalondi.Doing("Alarm",a[1])
	else
		print("\x1b[91mError: \x1b[96mFile not found: \x1b[0m"..fa)
	end
end

Jalondi.Start("Exe/Windows/Gwendolyn.jcr")
