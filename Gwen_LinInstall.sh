# License:
# 
# Gwendolyn
# Quick Linux Install
# 
# 
# 
# 	(c) Jeroen P. Broks, 2024
# 
# 		This program is free software: you can redistribute it and/or modify
# 		it under the terms of the GNU General Public License as published by
# 		the Free Software Foundation, either version 3 of the License, or
# 		(at your option) any later version.
# 
# 		This program is distributed in the hope that it will be useful,
# 		but WITHOUT ANY WARRANTY; without even the implied warranty of
# 		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# 		GNU General Public License for more details.
# 		You should have received a copy of the GNU General Public License
# 		along with this program.  If not, see <http://www.gnu.org/licenses/>.
# 
# 	Please note that some references to data like pictures or audio, do not automatically
# 	fall under this licenses. Mostly this is noted in the respective files.
# 
# Version: 24.12.10
# End License
echo "I'll try to install Gwendolyn for you!"
echo "Notes:"
echo "- Before this script is run you must have build Gwendolyn first with SCons"
echo "- This script MUST be run with the 'sudo' command or it won't work!"
echo "Installing: Gwendolyn"
cat Exe/Linux/Gwendolyn Exe/Linux/Gwendolyn.jcr > /usr/bin/gwendolyn
chmod +x /usr/bin/gwendolyn
echo "Ok"
