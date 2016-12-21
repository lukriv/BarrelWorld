#ifndef __BARREL_WORLD_MAIN_MENU_H__
#define __BARREL_WORLD_MAIN_MENU_H__


#include <vector>
#include <Urho3D/Core/Context.h>
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
		Urho3D::SharedPtr<Urho3D::Context> m_spContext;
		Urho3D::SharedPtr<Urho3D::UI> m_spUI;
		Urho3D::SharedPtr<Urho3D::Text> m_spText;
		TMenuList m_menuButtons;
		bool m_visible;
	public:
		MainMenu(Urho3D::Context* pContext, Urho3D::UI* pUI);
		~MainMenu();
		
		void Visible(bool visible);
		bool IsVisible();

	};

}

#endif // __BARREL_WORLD_MAIN_MENU_H__
