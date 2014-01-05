#ifndef __WX_UTF8_CS_TEXT_CONVERSION_TO_NONDIACRITICAL_ASCII_CS_TEXT_H__
#define __WX_UTF8_CS_TEXT_CONVERSION_TO_NONDIACRITICAL_ASCII_CS_TEXT_H__

//česká: á č ď é ě í ň ó ř š ť ú ů ý ž
#include <wx/string.h>

void ConvertToNonDiacriticsCsText(wxString &text)
{
	text.Replace(wxString(wxT("á")), wxString(wxT("a")), true);
	text.Replace(wxString(wxT("č")), wxString(wxT("c")), true);
	text.Replace(wxString(wxT("ď")), wxString(wxT("d")), true);
	text.Replace(wxString(wxT("é")), wxString(wxT("e")), true);
	text.Replace(wxString(wxT("ě")), wxString(wxT("e")), true);
	text.Replace(wxString(wxT("í")), wxString(wxT("i")), true);
	text.Replace(wxString(wxT("ň")), wxString(wxT("n")), true);
	text.Replace(wxString(wxT("ó")), wxString(wxT("o")), true);
	text.Replace(wxString(wxT("ř")), wxString(wxT("r")), true);
	text.Replace(wxString(wxT("š")), wxString(wxT("s")), true);
	text.Replace(wxString(wxT("ť")), wxString(wxT("t")), true);
	text.Replace(wxString(wxT("ú")), wxString(wxT("u")), true);
	text.Replace(wxString(wxT("ů")), wxString(wxT("u")), true);
	text.Replace(wxString(wxT("ý")), wxString(wxT("y")), true);
	text.Replace(wxString(wxT("ž")), wxString(wxT("z")), true);
	
	text.Replace(wxString(wxT("Á")), wxString(wxT("A")), true);
	text.Replace(wxString(wxT("Č")), wxString(wxT("C")), true);
	text.Replace(wxString(wxT("Ď")), wxString(wxT("D")), true);
	text.Replace(wxString(wxT("É")), wxString(wxT("E")), true);
	text.Replace(wxString(wxT("Ě")), wxString(wxT("E")), true);
	text.Replace(wxString(wxT("Í")), wxString(wxT("I")), true);
	text.Replace(wxString(wxT("Ň")), wxString(wxT("N")), true);
	text.Replace(wxString(wxT("Ó")), wxString(wxT("O")), true);
	text.Replace(wxString(wxT("Ř")), wxString(wxT("R")), true);
	text.Replace(wxString(wxT("Š")), wxString(wxT("S")), true);
	text.Replace(wxString(wxT("Ť")), wxString(wxT("T")), true);
	text.Replace(wxString(wxT("Ú")), wxString(wxT("U")), true);
	text.Replace(wxString(wxT("Ů")), wxString(wxT("U")), true);
	text.Replace(wxString(wxT("Ý")), wxString(wxT("Y")), true);
	text.Replace(wxString(wxT("Ž")), wxString(wxT("Z")), true);
}

void ConvertToNonBreakText(wxString &text)
{
	text.Replace(wxString(wxT("\n")), wxString(wxT(" ")), true);
	text.Replace(wxString(wxT("\t\t\t")), wxString(wxT(" ")), true);
	text.Replace(wxString(wxT("\t\t")), wxString(wxT(" ")), true);
	text.Replace(wxString(wxT("\t")), wxString(wxT(" ")), true);
}

void ConvertToBasicFormatedText(wxString &text)
{
	text.Replace(wxString(wxT("<p>")), wxString(wxT(" ")), true);
	text.Replace(wxString(wxT("</p>")), wxString(wxT("\n")), true);
}



#endif //__WX_UTF8_CS_TEXT_CONVERSION_TO_NONDIACRITICAL_ASCII_CS_TEXT_H__