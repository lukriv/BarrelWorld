#ifndef __WX_UTF8_CS_TEXT_CONVERSION_TO_NONDIACRITICAL_ASCII_CS_TEXT_H__
#define __WX_UTF8_CS_TEXT_CONVERSION_TO_NONDIACRITICAL_ASCII_CS_TEXT_H__

//česká: á č ď é ě í ň ó ř š ť ú ů ý ž
#include <wx/string.h>

void ConvertToNonDiacriticsCsText(wxString &text);

void ConvertToNonBreakText(wxString &text);

void ConvertToBasicFormatedText(wxString &text);



#endif //__WX_UTF8_CS_TEXT_CONVERSION_TO_NONDIACRITICAL_ASCII_CS_TEXT_H__