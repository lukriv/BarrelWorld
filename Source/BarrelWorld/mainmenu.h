#ifndef __BARRELWORD_MAIN_MENU_H__
#define __BARRELWORD_MAIN_MENU_H__


namespace Urho3D
{

	class Context;
	class RefCounted;
	
}


namespace BW
{

	class MainMenu : public RefCounted
	{
		typedef std::vector< SharedPtr<Text> > TMenuList;
	private:
		Context *m_pContext;
		
		
		
	public:
		MainMenu(Context* context);
		~MainMenu();
		
		void Visible(bool visible);
		bool IsVisible();

	};

}

#endif // __BARRELWORD_MAIN_MENU_H__
