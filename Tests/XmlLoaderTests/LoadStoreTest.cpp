#include <UnitTest++.h>

#include <wx/string.h>
#include <wx/vector.h>
#include <wx/filefn.h>
#include <wx/file.h>

#include <GameSystem/refobjectsmptr.h>
#include <GameSystem/glog.h>

#include "testdata.h"

#include <GameXmlDefinitions/gxmlloader.h>
#include <OIS/OISKeyboard.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// To add a test, simply put the following code in the a .cpp file of your choice:
//
// =================================
// Simple Test
// =================================
//
//  TEST(YourTestName)
//  {
//  }
//
// The TEST macro contains enough machinery to turn this slightly odd-looking syntax into legal C++, and automatically register the test in a global list. 
// This test list forms the basis of what is executed by RunAllTests().
//
// If you want to re-use a set of test data for more than one test, or provide setup/teardown for tests, 
// you can use the TEST_FIXTURE macro instead. The macro requires that you pass it a class name that it will instantiate, so any setup and teardown code should be in its constructor and destructor.
//
//  struct SomeFixture
//  {
//    SomeFixture() { /* some setup */ }
//    ~SomeFixture() { /* some teardown */ }
//
//    int testData;
//  };
// 
//  TEST_FIXTURE(SomeFixture, YourTestName)
//  {
//    int temp = testData;
//  }
//
// =================================
// Test Suites
// =================================
// 
// Tests can be grouped into suites, using the SUITE macro. A suite serves as a namespace for test names, so that the same test name can be used in two difference contexts.
//
//  SUITE(YourSuiteName)
//  {
//    TEST(YourTestName)
//    {
//    }
//
//    TEST(YourOtherTestName)
//    {
//    }
//  }
//
// This will place the tests into a C++ namespace called YourSuiteName, and make the suite name available to UnitTest++. 
// RunAllTests() can be called for a specific suite name, so you can use this to build named groups of tests to be run together.
// Note how members of the fixture are used as if they are a part of the test, since the macro-generated test class derives from the provided fixture class.
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct LoadFixture
{
private:
  wxDword m_fileCounter;
  wxVector<wxString> m_createdFiles;
public:
  RefObjSmPtr<GameLogger> m_spLogger;
public:
  LoadFixture() : m_fileCounter(1) 
  {
	if (FWG_FAILED(GameLoggerCreator::CreateLogger(m_spLogger.OutRef(),wxT("default"))))
	{
		printf("Failed to initialize the logger, aborting.");
	}  
  }
  ~LoadFixture() 
  { 
	if ( !m_createdFiles.empty() )
	{
		wxVector<wxString>::iterator iter;
		for ( iter = m_createdFiles.begin(); iter != m_createdFiles.end(); iter++ )
		{
			if( !wxRemoveFile(*iter))
			{
				wxPrintf(wxT("File %s cannot be removed"), iter->GetData().AsInternal());
			}
		}
		
		m_createdFiles.clear();
	}
  }
  
  bool CreateTestFile(const wxChar* fileData, wxString &createdFileName)
  {
	wxFile file;
	wxString fileName(wxT("TestFile"));
	fileName.Append(wxString::Format(wxT("%d"), m_fileCounter));
	
	if( !file.Open(fileName, wxFile::write))
	{
		return false;
	}
	
	if ( !file.Write(wxString(fileData)) )
	{
		return false;
	}
	
	if ( !file.Close() )
	{
		return false;
	}
	
	m_createdFiles.push_back(fileName);
	m_fileCounter++;
	createdFileName.assign(fileName);
	
	return true;
  }

};

SUITE(LoadXml)
{
	TEST_FIXTURE(LoadFixture, InitializeTest)
	{
		wxString testFile;
		GameXmlResourceLoader testLoader;
		
		CHECK(CreateTestFile(LOADMESHTEST_DEFINITION_FILE, testFile));
		CHECK(FWG_SUCCEDED(testLoader.Initialize(testFile)));
		
	}
	
	TEST_FIXTURE(LoadFixture, LoadMeshTest)
	{
		// prep
		wxString testFile;
		GameXmlResourceLoader testLoader;
		GameDefinitionHolder defHolder;
		RefObjSmPtr<NameDef> *spTestMesh = nullptr;
		
		CHECK(CreateTestFile(LOADMESHTEST_DEFINITION_FILE, testFile));
		CHECK(FWG_SUCCEDED(testLoader.Initialize(testFile, nullptr, m_spLogger)));
		
		// test
		CHECK(FWG_SUCCEDED(testLoader.Load(defHolder)));
		
		// check
		CHECK_EQUAL( 1, defHolder.m_meshDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_materialDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_renderEntityDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_cameraDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_renderDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_inputDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_logicDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_entityDefs.Size());
		CHECK_EQUAL( true, defHolder.m_spTerrain.IsEmpty());
		
		spTestMesh = defHolder.m_meshDefs.FindValue(TEST_MESH_NAME);
		CHECK(spTestMesh != nullptr);
		
		if(spTestMesh)
		{	
			CHECK(!(*spTestMesh).IsEmpty());
			if(!(*spTestMesh).IsEmpty())
			{
				CHECK((*spTestMesh)->m_name == wxString(TEST_MESH_RESOURCENAME));
				//CHECK(testMesh->m_group == wxString(wxT("")));
			}
		}
	}
	
	TEST_FIXTURE(LoadFixture, LoadMaterialTest)
	{
		// prep
		wxString testFile;
		GameXmlResourceLoader testLoader;
		GameDefinitionHolder defHolder;
		RefObjSmPtr<NameDef> *spTestRenderEnt = nullptr;
		
		CHECK(CreateTestFile(LOADMATERIALTEST_DEFINITION_FILE, testFile));
		CHECK(FWG_SUCCEDED(testLoader.Initialize(testFile, nullptr, m_spLogger)));
		
		// test
		CHECK(FWG_SUCCEDED(testLoader.Load(defHolder)));
		
		// check
		CHECK_EQUAL( 0, defHolder.m_meshDefs.Size());
		CHECK_EQUAL( 1, defHolder.m_materialDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_renderEntityDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_cameraDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_renderDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_inputDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_logicDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_entityDefs.Size());
		CHECK_EQUAL( true, defHolder.m_spTerrain.IsEmpty());
		
		spTestRenderEnt = defHolder.m_materialDefs.FindValue(TEST_MATERIAL_NAME);
		CHECK(spTestRenderEnt != nullptr);
		if(spTestRenderEnt)
		{	
			CHECK(!(*spTestRenderEnt).IsEmpty());
			if(!(*spTestRenderEnt).IsEmpty())
			{
				CHECK((*spTestRenderEnt)->m_name == wxString(TEST_MATERIAL_RESOURCENAME));
			}
			//CHECK(testMesh->m_group == wxString(wxT("")));
		}
	}
	
	TEST_FIXTURE(LoadFixture, LoadRenderEntTest1)
	{
		// prep
		wxString testFile;
		GameXmlResourceLoader testLoader;
		GameDefinitionHolder defHolder;
		RefObjSmPtr<RenderEntityDef> *spTestRenderEnt = nullptr;
		
		CHECK(CreateTestFile(LOADRENDERENTTEST_DEFINITION_FILE, testFile));
		CHECK(FWG_SUCCEDED(testLoader.Initialize(testFile, nullptr, m_spLogger)));
		
		// test
		CHECK(FWG_SUCCEDED(testLoader.Load(defHolder)));
		
		// check
		CHECK_EQUAL( 1, defHolder.m_meshDefs.Size());
		CHECK_EQUAL( 1, defHolder.m_materialDefs.Size());
		CHECK_EQUAL( 1, defHolder.m_renderEntityDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_cameraDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_renderDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_inputDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_logicDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_entityDefs.Size());
		CHECK_EQUAL( true, defHolder.m_spTerrain.IsEmpty());
		
		spTestRenderEnt = defHolder.m_renderEntityDefs.FindValue(TEST_RENDER_ENT1_NAME);
		CHECK(spTestRenderEnt != nullptr);
		if(spTestRenderEnt)
		{	
			CHECK(!(*spTestRenderEnt).IsEmpty());
			if(!(*spTestRenderEnt).IsEmpty())
			{
				CHECK(!(*spTestRenderEnt)->m_mesh.IsEmpty());
				CHECK((*spTestRenderEnt)->m_mesh->m_name == wxString(TEST_MESH_RESOURCENAME));
				
				CHECK(!(*spTestRenderEnt)->m_material.IsEmpty());
				CHECK((*spTestRenderEnt)->m_material->m_name == wxString(TEST_MATERIAL_RESOURCENAME));
				
				CHECK((*spTestRenderEnt)->m_position == TEST_RENDER_ENT1_POSITION);
				
			}
		}
	}
	
	TEST_FIXTURE(LoadFixture, LoadRenderEntTest2)
	{
		// prep
		wxString testFile;
		GameXmlResourceLoader testLoader;
		GameDefinitionHolder defHolder;
		RefObjSmPtr<RenderEntityDef> *spTestRenderEnt = nullptr;
		
		CHECK(CreateTestFile(LOADRENDERENTTEST2_DEFINITION_FILE, testFile));
		CHECK(FWG_SUCCEDED(testLoader.Initialize(testFile, nullptr, m_spLogger)));
		
		// test
		CHECK(FWG_SUCCEDED(testLoader.Load(defHolder)));
		
		// check
		CHECK_EQUAL( 0, defHolder.m_meshDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_materialDefs.Size());
		CHECK_EQUAL( 1, defHolder.m_renderEntityDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_cameraDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_renderDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_inputDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_logicDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_entityDefs.Size());
		CHECK_EQUAL( true, defHolder.m_spTerrain.IsEmpty());
		
		spTestRenderEnt = defHolder.m_renderEntityDefs.FindValue(TEST_RENDER_ENT2_NAME);
		CHECK(spTestRenderEnt != nullptr);
		if(spTestRenderEnt)
		{	
			CHECK(!(*spTestRenderEnt).IsEmpty());
			if(!(*spTestRenderEnt).IsEmpty())
			{
				CHECK(!(*spTestRenderEnt)->m_mesh.IsEmpty()&&(*spTestRenderEnt)->m_mesh->m_name == wxString(TEST_MESH_RESOURCENAME));
				CHECK(!(*spTestRenderEnt)->m_material.IsEmpty()&&(*spTestRenderEnt)->m_material->m_name == wxString(TEST_MATERIAL_RESOURCENAME));
				CHECK((*spTestRenderEnt)->m_position == TEST_RENDER_ENT2_POSITION);
			}
		}
	}
	
	TEST_FIXTURE(LoadFixture, LoadCameraTest)
	{
		// prep
		wxString testFile;
		GameXmlResourceLoader testLoader;
		GameDefinitionHolder defHolder;
		RefObjSmPtr<CameraDef> *spCamera = nullptr;
		
		CHECK(CreateTestFile(LOADCAMERA_DEFINITION_FILE, testFile));
		CHECK(FWG_SUCCEDED(testLoader.Initialize(testFile, nullptr, m_spLogger)));
		
		// test
		CHECK(FWG_SUCCEDED(testLoader.Load(defHolder)));
		
		// check
		CHECK_EQUAL( 0, defHolder.m_meshDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_materialDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_renderEntityDefs.Size());
		CHECK_EQUAL( 1, defHolder.m_cameraDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_renderDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_inputDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_logicDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_entityDefs.Size());
		CHECK_EQUAL( true, defHolder.m_spTerrain.IsEmpty());
		
		spCamera = defHolder.m_cameraDefs.FindValue(TEST_CAMERA_NAME);
		CHECK(spCamera != nullptr);
		if(spCamera)
		{	
			CHECK(!(*spCamera).IsEmpty());
			if(!(*spCamera).IsEmpty())
			{
				CHECK((*spCamera)->m_position == TEST_CAMERA_POSITION);
				CHECK((*spCamera)->m_direction == TEST_CAMERA_DIRECTION);
			}
		}
	}
	
	TEST_FIXTURE(LoadFixture, LoadRenderTest)
	{
		// prep
		wxString testFile;
		GameXmlResourceLoader testLoader;
		GameDefinitionHolder defHolder;
		RefObjSmPtr<RenderDef> *spRender = nullptr;
		
		CHECK(CreateTestFile(LOADRENDER_DEFINITION_FILE, testFile));
		CHECK(FWG_SUCCEDED(testLoader.Initialize(testFile, nullptr, m_spLogger)));
		
		// test
		CHECK(FWG_SUCCEDED(testLoader.Load(defHolder)));
		
		// check
		CHECK_EQUAL( 0, defHolder.m_meshDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_materialDefs.Size());
		CHECK_EQUAL( 2, defHolder.m_renderEntityDefs.Size());
		CHECK_EQUAL( 1, defHolder.m_cameraDefs.Size());
		CHECK_EQUAL( 2, defHolder.m_renderDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_inputDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_logicDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_entityDefs.Size());
		CHECK_EQUAL( true, defHolder.m_spTerrain.IsEmpty());
		
		spRender = defHolder.m_renderDefs.FindValue(TEST_RENDER1_NAME);
		CHECK(spRender != nullptr);
		if(spRender)
		{	
			CHECK(!(*spRender).IsEmpty());
			if(!(*spRender).IsEmpty())
			{
				CHECK((*spRender)->m_cameras.size() == 0);
				CHECK((*spRender)->m_lights.size() == 0);
				CHECK((*spRender)->m_entities.size() == 2);
				if((*spRender)->m_entities.size() == 2)
				{
					CHECK(!(*spRender)->m_entities[0]->m_mesh.IsEmpty()
							&&(*spRender)->m_entities[0]->m_mesh->m_name == wxString(TEST_MESH_RESOURCENAME));
					CHECK(!(*spRender)->m_entities[0]->m_material.IsEmpty()	
							&&(*spRender)->m_entities[0]->m_material->m_name == wxString(TEST_MATERIAL_RESOURCENAME));
					CHECK((*spRender)->m_entities[0]->m_position == TEST_RENDER_ENT1_POSITION);
					
					CHECK(!(*spRender)->m_entities[1]->m_mesh.IsEmpty()
							&&(*spRender)->m_entities[1]->m_mesh->m_name == wxString(TEST_MESH_RESOURCENAME));
					CHECK(!(*spRender)->m_entities[1]->m_material.IsEmpty()
							&&(*spRender)->m_entities[1]->m_material->m_name == wxString(TEST_MATERIAL_RESOURCENAME));
					CHECK((*spRender)->m_entities[1]->m_position == TEST_RENDER_ENT2_POSITION);
					
				}
			}
		}
		
		spRender = defHolder.m_renderDefs.FindValue(TEST_RENDER2_NAME);
		CHECK(spRender != nullptr);
		if(spRender)
		{	
			CHECK(!(*spRender).IsEmpty());
			if(!(*spRender).IsEmpty())
			{
				CHECK((*spRender)->m_cameras.size() == 1);
				CHECK((*spRender)->m_lights.size() == 0);
				CHECK((*spRender)->m_entities.size() == 0);
				
				if((*spRender)->m_cameras.size() == 1)
				{
					CHECK(!(*spRender)->m_cameras[0].IsEmpty());
					if(!(*spRender)->m_cameras[0].IsEmpty())
					{
						CHECK((*spRender)->m_cameras[0]->m_position == TEST_CAMERA_POSITION);
						CHECK((*spRender)->m_cameras[0]->m_direction == TEST_CAMERA_DIRECTION);
					}
				}
			}
		}
	}
	
	TEST_FIXTURE(LoadFixture, LoadInputTest)
	{
		// prep
		wxString testFile;
		GameXmlResourceLoader testLoader;
		GameDefinitionHolder defHolder;
		RefObjSmPtr<InputDef> *spInput = nullptr;
		
		CHECK(CreateTestFile(LOADINPUT_DEFINITION_FILE, testFile));
		CHECK(FWG_SUCCEDED(testLoader.Initialize(testFile, nullptr, m_spLogger)));
		
		// test
		CHECK(FWG_SUCCEDED(testLoader.Load(defHolder)));
		
		// check
		CHECK_EQUAL( 0, defHolder.m_meshDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_materialDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_renderEntityDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_cameraDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_renderDefs.Size());
		CHECK_EQUAL( 2, defHolder.m_inputDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_logicDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_entityDefs.Size());
		CHECK_EQUAL( true, defHolder.m_spTerrain.IsEmpty());
		
		spInput = defHolder.m_inputDefs.FindValue(TEST_INPUT1_NAME);
		CHECK(spInput != nullptr);
		if(spInput)
		{	
			CHECK(!(*spInput).IsEmpty());
			if(!(*spInput).IsEmpty())
			{
				CHECK((*spInput)->m_inputMap.Size() == 4);
				if((*spInput)->m_inputMap.Size() == 4)
				{
					CHECK((*spInput)->m_inputMap.Exists(wxT("up")) && (*(*spInput)->m_inputMap.FindValue(wxT("up")) == OIS::KC_W));
					CHECK((*spInput)->m_inputMap.Exists(wxT("down")) && (*(*spInput)->m_inputMap.FindValue(wxT("down")) == OIS::KC_S));
					CHECK((*spInput)->m_inputMap.Exists(wxT("left")) && (*(*spInput)->m_inputMap.FindValue(wxT("left")) == OIS::KC_A));
					CHECK((*spInput)->m_inputMap.Exists(wxT("right")) && (*(*spInput)->m_inputMap.FindValue(wxT("right")) == OIS::KC_D));
				}
			}
		}
		
		spInput = defHolder.m_inputDefs.FindValue(TEST_INPUT2_NAME);
		CHECK(spInput != nullptr);
		if(spInput)
		{	
			CHECK(!(*spInput).IsEmpty());
			if(!(*spInput).IsEmpty())
			{
				CHECK((*spInput)->m_inputMap.Size() == 2);
				if((*spInput)->m_inputMap.Size() == 2)
				{
					CHECK((*spInput)->m_inputMap.Exists(wxT("forward")) && (*(*spInput)->m_inputMap.FindValue(wxT("forward")) == OIS::KC_J));
					CHECK((*spInput)->m_inputMap.Exists(wxT("backward")) && (*(*spInput)->m_inputMap.FindValue(wxT("backward")) == OIS::KC_L));
				}
			}
		}
	}
	
	TEST_FIXTURE(LoadFixture, LoadLogicTest)
	{
		// prep
		wxString testFile;
		GameXmlResourceLoader testLoader;
		GameDefinitionHolder defHolder;
		RefObjSmPtr<LogicDef> *spLogic = nullptr;
		
		CHECK(CreateTestFile(LOADLOGIC_DEFINITION_FILE, testFile));
		CHECK(FWG_SUCCEDED(testLoader.Initialize(testFile, nullptr, m_spLogger)));
		
		// test
		CHECK(FWG_SUCCEDED(testLoader.Load(defHolder)));
		
		// check
		CHECK_EQUAL( 0, defHolder.m_meshDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_materialDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_renderEntityDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_cameraDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_renderDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_inputDefs.Size());
		CHECK_EQUAL( 1, defHolder.m_logicDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_entityDefs.Size());
		CHECK_EQUAL( true, defHolder.m_spTerrain.IsEmpty());
		
		spLogic = defHolder.m_logicDefs.FindValue(TEST_LOGIC_NAME);
		CHECK(spLogic != nullptr);
		if(spLogic)
		{	
			CHECK(!(*spLogic).IsEmpty());
			if(!(*spLogic).IsEmpty())
			{
				CHECK((*spLogic)->m_logicType.Cmp(TEST_LOGIC_TYPE) == 0);
			}
		}
	}
	
	TEST_FIXTURE(LoadFixture, LoadEntityTest1)
	{
		// prep
		wxString testFile;
		GameXmlResourceLoader testLoader;
		GameDefinitionHolder defHolder;
		RefObjSmPtr<EntityDef> *spEntity = nullptr;
		
		CHECK(CreateTestFile(LOADENTITYTEST1_DEFINITION_FILE, testFile));
		CHECK(FWG_SUCCEDED(testLoader.Initialize(testFile, nullptr, m_spLogger)));
		
		// test
		CHECK(FWG_SUCCEDED(testLoader.Load(defHolder)));
		
		// check
		CHECK_EQUAL( 1, defHolder.m_meshDefs.Size());
		CHECK_EQUAL( 1, defHolder.m_materialDefs.Size());
		CHECK_EQUAL( 2, defHolder.m_renderEntityDefs.Size());
		CHECK_EQUAL( 1, defHolder.m_cameraDefs.Size());
		CHECK_EQUAL( 2, defHolder.m_renderDefs.Size());
		CHECK_EQUAL( 2, defHolder.m_inputDefs.Size());
		CHECK_EQUAL( 1, defHolder.m_logicDefs.Size());
		CHECK_EQUAL( 2, defHolder.m_entityDefs.Size());
		CHECK_EQUAL( true, defHolder.m_spTerrain.IsEmpty());
		
		spEntity = defHolder.m_entityDefs.FindValue(TEST_ENTITY1_NAME);
		CHECK(spEntity != nullptr);
		if(spEntity)
		{	
			CHECK(!(*spEntity).IsEmpty());
			if(!(*spEntity).IsEmpty())
			{
				CHECK(!(*spEntity)->m_transformation.IsEmpty());
				if(!(*spEntity)->m_transformation.IsEmpty())
				{
					CHECK((*spEntity)->m_transformation->m_position == TEST_TRANSFORM_POSITION);
					CHECK((*spEntity)->m_transformation->m_scale == TEST_TRANSFORM_SCALE);
					CHECK((*spEntity)->m_transformation->m_rotation == TEST_TRANSFORM_ROTATION);
				}
				CHECK(!(*spEntity)->m_renderDef.IsEmpty());
				CHECK(!(*spEntity)->m_logicDef.IsEmpty());
				CHECK(!(*spEntity)->m_inputDef.IsEmpty());
			}
		}
		
		spEntity = defHolder.m_entityDefs.FindValue(TEST_ENTITY2_NAME);
		CHECK(spEntity != nullptr);
		if(spEntity)
		{	
			CHECK(!(*spEntity).IsEmpty());
			if(!(*spEntity).IsEmpty())
			{
				CHECK(!(*spEntity)->m_transformation.IsEmpty());
				if(!(*spEntity)->m_transformation.IsEmpty())
				{
					CHECK((*spEntity)->m_transformation->m_position == TEST_TRANSFORM_POSITION);
					CHECK((*spEntity)->m_transformation->m_scale == TEST_TRANSFORM_SCALE);
					CHECK((*spEntity)->m_transformation->m_rotation == TEST_TRANSFORM_ROTATION);
				}
				CHECK(!(*spEntity)->m_renderDef.IsEmpty());
				CHECK(!(*spEntity)->m_logicDef.IsEmpty());
				CHECK(!(*spEntity)->m_inputDef.IsEmpty());
			}
		}
	}
	
	TEST_FIXTURE(LoadFixture, LoadPhysics)
	{
		// prep
		wxString testFile;
		GameXmlResourceLoader testLoader;
		GameDefinitionHolder defHolder;
		RefObjSmPtr<EntityDef> *spEntity = nullptr;
		
		CHECK(CreateTestFile(LOADPHYSICSTEST_DEFINITION_FILE, testFile));
		CHECK(FWG_SUCCEDED(testLoader.Initialize(testFile, nullptr, m_spLogger)));
		
		// test
		CHECK(FWG_SUCCEDED(testLoader.Load(defHolder)));
		
		// check
		CHECK_EQUAL( 0, defHolder.m_meshDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_materialDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_renderEntityDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_cameraDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_renderDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_inputDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_logicDefs.Size());
		CHECK_EQUAL( 1, defHolder.m_entityDefs.Size());
		CHECK_EQUAL( true, defHolder.m_spTerrain.IsEmpty());
		
		spEntity = defHolder.m_entityDefs.FindValue(TEST_ENTITY1_NAME);
		CHECK(spEntity != nullptr);
		if(spEntity)
		{	
			CHECK(!(*spEntity).IsEmpty());
			if(!(*spEntity).IsEmpty())
			{
				CHECK((*spEntity)->m_transformation.IsEmpty());
				CHECK((*spEntity)->m_renderDef.IsEmpty());
				CHECK((*spEntity)->m_logicDef.IsEmpty());
				CHECK((*spEntity)->m_inputDef.IsEmpty());
				CHECK(!(*spEntity)->m_physDef.IsEmpty());
				if(!(*spEntity)->m_physDef.IsEmpty())
				{
					CHECK(!(*spEntity)->m_physDef->m_shape.IsEmpty());
					if(!(*spEntity)->m_physDef->m_shape.IsEmpty())
					{
						CHECK((*spEntity)->m_physDef->m_shape->m_shapeType == TEST_PHYSICS_SHAPE_BOXTYPE);
						CHECK((*spEntity)->m_physDef->m_shape->m_boxHalfSize == TEST_PHYSICS_BOX_BOXHALFSIZE);
					}
					CHECK((*spEntity)->m_physDef->m_mass == TEST_PHYSICS_MASS);
					CHECK((*spEntity)->m_physDef->m_inertiaVector == TEST_PHYSICS_INERTIA);
					CHECK((*spEntity)->m_physDef->m_physType == PhysCompDef::PHYS_TYPE_COLLISION);
				}
			}
		}
	}
	
	TEST_FIXTURE(LoadFixture, LoadTerrain)
	{
		// prep
		wxString testFile;
		GameXmlResourceLoader testLoader;
		GameDefinitionHolder defHolder;
		
		CHECK(CreateTestFile(LOADTERRAINTEST_DEFINITION_FILE, testFile));
		CHECK(FWG_SUCCEDED(testLoader.Initialize(testFile, nullptr, m_spLogger)));
		
		// test
		CHECK(FWG_SUCCEDED(testLoader.Load(defHolder)));
		
		// check
		CHECK_EQUAL( 0, defHolder.m_meshDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_materialDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_renderEntityDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_cameraDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_renderDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_inputDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_logicDefs.Size());
		CHECK_EQUAL( 0, defHolder.m_entityDefs.Size());
		CHECK_EQUAL( false, defHolder.m_spTerrain.IsEmpty());
		
		if(!defHolder.m_spTerrain.IsEmpty())
		{
			TerrainDef *pTerrainDef = defHolder.m_spTerrain.In();
			CHECK_EQUAL( TEST_TERRAIN_MAPSIZE, pTerrainDef->m_mapSize);
			CHECK_EQUAL( TEST_TERRAIN_WORLDSIZE, pTerrainDef->m_worldSize);
			CHECK_EQUAL( 1, pTerrainDef->m_terrainPages.size());
			if(pTerrainDef->m_terrainPages.size() == 1)
			{
				TerrainPage *pTerrainPage = pTerrainDef->m_terrainPages[0].In();
				CHECK_EQUAL( 0, wxString(pTerrainPage->m_filename).Cmp(TEST_TERRAINPAGE_FILENAME));
				CHECK_EQUAL( TEST_TERRAINPAGE_PAGEX, pTerrainPage->m_pageX);
				CHECK_EQUAL( TEST_TERRAINPAGE_PAGEY, pTerrainPage->m_pageY);
			}
		}
	}

}