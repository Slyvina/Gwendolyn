Jalondi.AddString(sprintf([[
# ID data
# (c) Jeroen P. Broks - GPL3
[ID]	
Tool=Gwendolyn
Author=Jeroen P. Broks
Updated=%s
]],os.date()),"ID/Identify.ini","Store","Jeroen P. Broks","GPL3")

Jalondi.Add("Assets","","zlib")
Jalondi.Start("Exe/Windows/Gwendolyn.jcr")