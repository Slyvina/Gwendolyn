; License:
; 
; Gwendolyn
; Windows Install Script
; 
; 
; 
; 	(c) Jeroen P. Broks, 2024
; 
; 		This program is free software: you can redistribute it and/or modify
; 		it under the terms of the GNU General Public License as published by
; 		the Free Software Foundation, either version 3 of the License, or
; 		(at your option) any later version.
; 
; 		This program is distributed in the hope that it will be useful,
; 		but WITHOUT ANY WARRANTY; without even the implied warranty of
; 		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; 		GNU General Public License for more details.
; 		You should have received a copy of the GNU General Public License
; 		along with this program.  If not, see <http://www.gnu.org/licenses/>.
; 
; 	Please note that some references to data like pictures or audio, do not automatically
; 	fall under this licenses. Mostly this is noted in the respective files.
; 
; Version: 24.12.04
; End License


#define MyAppName "Gwendolyn"
#define MyAppVersion "Alpha"
#define MyAppPublisher "Jeroen P. Broks"
#define MyAppURL "https://github.com/Slyvina/Gwendolyn"
#define MyAppExeName "Gwendolyn.exe"

[Setup]
AppId={{B155FF43-F1FA-469D-9CFD-57D577CF6BEC}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DisableProgramGroupPage=yes
OutputDir=E:\Projects\Applications\Slyvina\Apps\Gwendolyn\Exe\New folder
OutputBaseFilename=Gwendolyn_Setup
SetupIconFile=E:\Projects\Applications\Slyvina\Apps\Gwendolyn\Icon\Gwendolyn.ico
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "E:\Projects\Applications\Slyvina\Apps\Gwendolyn\Exe\Windows\Gwendolyn.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gwendolyn\Exe\Windows\Gwendolyn.jcr"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gwendolyn\Exe\Windows\Gwendolyn.pdb"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gwendolyn\Exe\Windows\libFLAC-8.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gwendolyn\Exe\Windows\libjpeg-9.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gwendolyn\Exe\Windows\libmodplug-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gwendolyn\Exe\Windows\libmpg123-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gwendolyn\Exe\Windows\libogg-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gwendolyn\Exe\Windows\libopus-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gwendolyn\Exe\Windows\libopusfile-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gwendolyn\Exe\Windows\libpng16-16.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gwendolyn\Exe\Windows\libtiff-5.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gwendolyn\Exe\Windows\libvorbis-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gwendolyn\Exe\Windows\libvorbisfile-3.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gwendolyn\Exe\Windows\libwebp-7.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gwendolyn\Exe\Windows\SDL2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gwendolyn\Exe\Windows\SDL2_image.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gwendolyn\Exe\Windows\SDL2_mixer.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gwendolyn\Exe\Windows\zlib1.dll"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{commonprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

