#ifndef __LONE_WOLF_XML_READER_H__
#define __LONE_WOLF_XML_READER_H__




class LoneWolfXmlReader {
public:
	static bool CreateGameFromXmlFile(const wxChar* xmlfilepath, SceneManager &sceneMgr);
	
};




#endif //__LONE_WOLF_XML_READER_H__