echo "I'll try to install Gwendolyn for you!"
echo "Notes:"
echo "- Before this script is run you must have build Gwendolyn first with SCons"
echo "- This script MUST be run with the 'sudo' command or it won't work!"
echo "Installing: Gwendolyn"
cat Exe/Linux/Gwendolyn Exe/Linux/Gwendolyn.jcr > /usr/bin/gwendolyn
chmod +x /usr/bin/gwendolyn
echo "Ok"
