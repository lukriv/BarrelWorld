#include "mainmenu.h"

#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/UI/UI.h>

using namespace Urho3D;

BW::MainMenu::MainMenu(Context* pContext, UI* pUI) : m_spContext(pContext), m_spUI(pUI), m_visible(true)
{
	SharedPtr< Button > spButton(new Button(pContext));
	
	m_spText = new Text(pContext);
	m_spText->SetText("Main Menu");
    // If the engine cannot find the font, it comes with Urho3D.
    // Set the environment variables URHO3D_HOME, URHO3D_PREFIX_PATH or
    // change the engine parameter "ResourcePrefixPath" in the Setup method.
    m_spText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"),20);
    m_spText->SetColor(Color(.3,0,.3));
    m_spText->SetHorizontalAlignment(HA_CENTER);
    m_spText->SetVerticalAlignment(VA_TOP);
	
    pUI->GetRoot()->AddChild(m_spText.Get());
	
	spButton->SetName("New Game");
	spButton->SetStyle("Button");
	spButton->SetSize(32,32);
    spButton->SetPosition(16,16);
	
	pUI->GetRoot()->AddChild(spButton.Get());
	
	m_menuButtons.push_back(spButton);
}

BW::MainMenu::~MainMenu()
{
	for(auto &item : m_menuButtons)
	{
		m_spUI->GetRoot()->RemoveChild(item.Get());
	}
	m_menuButtons.clear();
}

void BW::MainMenu::Visible(bool visible)
{
}

bool BW::MainMenu::IsVisible()
{
	return m_visible;
}
