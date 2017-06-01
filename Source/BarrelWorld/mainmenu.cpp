#include "mainmenu.h"

#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/UI/Font.h>


using namespace Urho3D;


//static const int32_t MENU_ITEM_NUMBER = 2;
static const int32_t MENU_ITEM_WIDTH = 320;
static const int32_t MENU_ITEM_HEIGHT = 32;
static const int32_t MENU_ITEM_FONT_SIZE = 20;
static const int32_t MENU_ITEM_BORDER = 5;
static const int32_t MENU_BORDER = 10;

struct MenuItemStruct {
	String name;
	String label;
};

static const MenuItemStruct MenuItems[] = { { "new", "New Game"},
											//{ "load", "Load" },
											{ "store", "Save" }, 
											{ "exit", "Exit" } };

BW::MainMenu::MainMenu( Application *pApp, int32_t width, int32_t height ) : m_pApp( pApp )
	, m_width(width)
	, m_height(height)
{
	ResourceCache* cache = pApp->GetSubsystem<ResourceCache>();
	m_spWindow = (new Window(pApp->GetContext()));
	
	int32_t positionY = MENU_ITEM_HEIGHT + MENU_ITEM_BORDER;
	
	int32_t MENU_ITEM_NUMBER = sizeof(MenuItems)/sizeof(MenuItems[0]);
	
	
	m_spWindow->SetMovable(false);
	m_spWindow->SetResizable(false);
	m_spWindow->SetSize( MENU_ITEM_WIDTH + 2*MENU_BORDER, (MENU_ITEM_HEIGHT + MENU_ITEM_BORDER) * (MENU_ITEM_NUMBER + 1) + 2*MENU_BORDER );
	m_spWindow->SetPosition( (m_width - MENU_ITEM_WIDTH ) / 2 , 200 );
	m_spWindow->SetStyle("Window");
	
	m_spText = new Text(pApp->GetContext());
	m_spText->SetText("Main Menu");
    // If the engine cannot find the font, it comes with Urho3D.
    // Set the environment variables URHO3D_HOME, URHO3D_PREFIX_PATH or
    // change the engine parameter "ResourcePrefixPath" in the Setup method.
    m_spText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"),20);
    m_spText->SetColor(Color(.3,0,.3));
    m_spText->SetHorizontalAlignment(HA_CENTER);
    m_spText->SetVerticalAlignment(VA_TOP);
	
    m_spWindow->AddChild(m_spText.Get());
	
	for( int32_t i = 0; i < MENU_ITEM_NUMBER; ++i )
	{
		SharedPtr< Button > spButton(new Button(pApp->GetContext()));
		SharedPtr< Text > spText(new Text(pApp->GetContext()));
		
		spButton->SetName(MenuItems[i].name);
		spButton->SetStyle("Button");
		spButton->SetSize( MENU_ITEM_WIDTH, MENU_ITEM_HEIGHT );
		spButton->SetPosition( MENU_BORDER , positionY );
		
		positionY += MENU_ITEM_HEIGHT + MENU_ITEM_BORDER;
		
		spText->SetText(MenuItems[i].label);
		
		spButton.Get()->AddChild(spText.Get());
		
		spText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"),20);
		spText->SetColor(Color(1,1,1));
		spText->SetHorizontalAlignment(HA_CENTER);
		spText->SetVerticalAlignment(VA_CENTER);
		
		m_spWindow.Get()->AddChild(spButton);
	}
	
	
	pApp->GetSubsystem<UI>()->GetRoot()->AddChild(m_spWindow.Get());
	
}

BW::MainMenu::~MainMenu()
{
}

void BW::MainMenu::Visible(bool visible)
{
	if(visible != m_spWindow->IsVisible())
	{
		m_spWindow->SetVisible(visible);
	}
}

bool BW::MainMenu::IsVisible()
{
	return m_spWindow->IsVisible();
}
