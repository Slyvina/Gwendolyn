#include <JCR6_zlib.hpp>
#include <SlyvQCol.hpp>
#include "Gwen_Assets.hpp"
using namespace Slyvina;
using namespace Slyvina::JCR6;
using namespace Slyvina::Gwendolyn;


int main(int ca, char** args) {
	JCR_InitZlib();
	QCol->LMagenta("Gwendolyn\n");
	QCol->Doing("Coded by", "Jeroen P. Broks");
	CheckAssets(args[0]);
	QCol->Reset();
}