#include <stdio.h>
#include <iostream>

#include <wx/init.h>
#include "../lwgamelib/scenemgr.h"
#include "../lwgamelib/reader.h"

#include <windows.h>

int main(int argc, char **argv)
{
	SceneManager scnMgr;
	GlobalResourceManager resMgr;
	
	//SetConsoleOutputCP(CP_UTF8);
	printf("Start testing lone wolf world\n");
	
	wxInitializer intializer(argc, argv);
	if (!intializer.IsOk())
	{
		wxPrintf(wxT("Failed to initialize the wxWidgets library, aborting."));
		return -1;
	}
	
	
	if(!LoneWolfXmlReader::LoadChapter(wxT("../../chapters/utokzetmy.xml"), scnMgr))
	{
		std::cout << "Chyba pri cteni souboru ../../chapters/utokzetmy.xml" << std::endl;
	}
	
	if(!LoneWolfXmlReader::LoadTextDefinitions(wxT("../../chapters/textdefs.xml"), resMgr))
	{
		std::cout << "Chyba pri cteni souboru ../../chapters/textdefs.xml" << std::endl;
	}
	
	std::cout << scnMgr << std::endl;	
	
	if(scnMgr.SceneMapTest())
	{
		std::cout << "Scene map test passed" << std::endl;
	} else {
		std::cout << "Scene map test failed" << std::endl;
	}
	
	return 0;
}
