#ifndef __BARREL_WORLD_MAIN_MENU_H__
#define __BARREL_WORLD_MAIN_MENU_H__


#include <cstdint>
#include <vector>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Container/Ptr.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/Text.h>


namespace BW
{

	class MainMenu : public Urho3D::RefCounted
	{
		typedef std::vector< Urho3D::SharedPtr<Urho3D::Button> > TMenuList;
	private:
		Urho3D::Application *m_pApp;
		Urho3D::SharedPtr<Urho3D::Text> m_spText;
		TMenuList m_menuButtons;
		int32_t m_width;
		int32_t m_height;
		bool m_visible;
	public:
		MainMenu( Urho3D::Application *pApp, int32_t width, int32_t height );
		~MainMenu();
		
		void Visible(bool visible);
		bool IsVisible();

	};

}

#endif // __BARREL_WORLD_MAIN_MENU_H__
