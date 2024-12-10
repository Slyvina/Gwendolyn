import platform
from datetime import date
from os import system
from os import environ

Files = []
Libs = []
Allowed = ["Linux"]
def IsAllowed():
    for a in Allowed: 
        # print("%s == %s -> %s"%(a,platform.system(),a==platform.system())) # debug only!
        if a==platform.system(): 
            return True
    return False

Today = date.today().strftime("%b %d %y")

print("\x1b[32mGwendolyn\x1b[33m\tBuilder script for the run-time environment\x1b[0m\n(c) Jeroen P. Broks\n")
def Doing(a,b): print("\x1b[93m%s: \x1b[96m%s\x1b[0m"%(a,b))
Doing("Platform",platform.system())
print("\n\n")
if not IsAllowed():
    print("\x1b[31mERROR!\x1b[0mThis script is NOT compatible with %s!"%platform.system())
    quit(1)

IncludeDirs = [
    "../../Libs/JCR6/Headers",
    "../../Libs/TQSL/Headers",
    "../../Libs/Units/Headers",
    "../../Libs/JCR6/3rdParty/zlib/src",
    "../../Libs/June19/head",
    ]

def Add(cat,files):
    for f in files: 
        Doing("Code "+cat,f)
        Files.append(f)

def AddLib(cat,files):
    for f in files: 
        Doing("Lib  "+cat,f)
        Libs.append(f)
        
# zlib        
if platform.system()=="Linux":
    Add("zlib",Glob("../../Libs/JCR6/3rdParty/zlib/src/*.c"))
else:
    AddLib("zlib",["../../Libs/JCR6/3rdParty/3rdParty/zlib/zlib"])
    
# project
#Environment(ENV = {'PATH' : environ['PATH']})
#Environment(ENV = {'PATH' : "/usr/local/lib"})
AddLib("SDL2",["SDL2","SDL2_mixer","SDL2_image","SDL2main"])

Add("JCR6",[    
    "../../Libs/JCR6/Source/JCR6_Core.cpp",
#    "../../Libs/JCR6/Source/JCR6_JQL.cpp",
#    "../../Libs/JCR6/Source/JCR6_RealDir.cpp",
#    "../../Libs/JCR6/Source/JCR6_Write.cpp",
#    "../../Libs/JCR6/Source/JCR6_WAD.cpp",
#    "../../Libs/JCR6/Source/JCR6_QuakePAK.cpp",
#    "../../Libs/JCR6/Source/JCR6_TAR.cpp",
    "../../Libs/JCR6/Source/JCR6_zlib.cpp"])
Add("TQSL",[
    "../../Libs/TQSL/Source/TQSG.cpp",
    "../../Libs/TQSL/Source/TQSA.cpp",
    "../../Libs/TQSL/Source/TQSE.cpp"]
    )
Add("June 19", Glob("../../Libs/June19/src/*.cpp"))
Add("Units",[
#    "../../Libs/Units/Source/SlyvAsk.cpp",
    "../../Libs/Units/Source/SlyvBank.cpp",
    "../../Libs/Units/Source/SlyvDir.cpp",
    "../../Libs/Units/Source/SlyvDirry.cpp",
    "../../Libs/Units/Source/SlyvMD5.cpp",
#    "../../Libs/Units/Source/SlyvOpenURL.cpp",
    "../../Libs/Units/Source/SlyvQCol.cpp",
    "../../Libs/Units/Source/SlyvRequestFile.cpp",
    "../../Libs/Units/Source/SlyvRoman.cpp",
    "../../Libs/Units/Source/SlyvSharedArray.cpp",
    "../../Libs/Units/Source/SlyvSTOI.cpp",
    "../../Libs/Units/Source/SlyvStream.cpp",
    "../../Libs/Units/Source/SlyvString.cpp",
    "../../Libs/Units/Source/SlyvTime.cpp",
    "../../Libs/Units/Source/SlyvVolumes.cpp",
    "../../Libs/Units/Source/SlyvPhantasarTime.cpp"
])

Add("Gwendolyn",Glob("*.cpp"))
Doing("Jalondi","Gwen_Jalondi.lua")
exc = system("jalondi script Gwen_Jalondi.lua")

Doing("Compiling","Gwendolyn")
Program("Exe/%s/Gwendolyn"%platform.system(),Files,CPPPATH=IncludeDirs,LIBS=Libs)
