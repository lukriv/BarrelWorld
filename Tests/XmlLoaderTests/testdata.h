#ifndef __GAME_XML_LOADER_TEST_DATA_H__
#define __GAME_XML_LOADER_TEST_DATA_H__

#include <wx/chartype.h>
#include <OGRE/OgreVector3.h>

static const wxChar* LOADMESHTEST_DEFINITION_FILE = 
	wxT("<world>\n")
	wxT(" <definitions>\n")
	wxT("  <meshdefs>\n")
	wxT("   <mesh name=\"mesh1\">\n")
	wxT("    <param name=\"meshname\" value=\"TestingCube\" />\n")
	wxT("   </mesh>\n")
	wxT("  </meshdefs>\n")		
	wxT(" </definitions>\n")
	wxT("</world>");
static const wxChar* TEST_MESH_NAME = wxT("mesh1");
static const wxChar* TEST_MESH_RESOURCENAME = wxT("TestingCube");
	
static const wxChar* LOADMATERIALTEST_DEFINITION_FILE = 
	wxT("<world>\n")
	wxT(" <definitions>\n")
	wxT("  <materialdefs>\n")
	wxT("   <material name=\"material1\">\n")
	wxT("    <param name=\"materialname\" value=\"Test/ColourTest\" />\n")
	wxT("   </material>\n")
	wxT("  </materialdefs>\n")		
	wxT(" </definitions>\n")
	wxT("</world>");
static const wxChar* TEST_MATERIAL_NAME = wxT("material1");
static const wxChar* TEST_MATERIAL_RESOURCENAME = wxT("Test/ColourTest");


static const wxChar* LOADRENDERENTTEST_DEFINITION_FILE = 
	wxT("<world>\n")
	wxT(" <definitions>\n")
	wxT("  <meshdefs>\n")
	wxT("   <mesh name=\"mesh1\">\n")
	wxT("    <param name=\"meshname\" value=\"TestingCube\" />\n")
	wxT("   </mesh>\n")
	wxT("  </meshdefs>\n")	
	wxT("  <materialdefs>\n")
	wxT("   <material name=\"material1\">\n")
	wxT("    <param name=\"materialname\" value=\"Test/ColourTest\" />\n")
	wxT("   </material>\n")
	wxT("  </materialdefs>\n")
	wxT("  <renderentdefs>\n")
	wxT("   <renderent name=\"renderEnt1\">\n")
	wxT("    <meshref value=\"mesh1\" />\n")
	wxT("	 <materialref value=\"material1\" />\n")
	wxT("    <param name=\"position\" value=\"1.0;2.0;3.0\" />\n")
	wxT("   </renderent>\n")
	wxT("  </renderentdefs>\n")	
	wxT(" </definitions>\n")
	wxT("</world>");
static const wxChar* TEST_RENDER_ENT1_NAME = wxT("renderEnt1");
static const Ogre::Vector3 TEST_RENDER_ENT1_POSITION = { 1.0, 2.0, 3.0 };
	
static const wxChar* LOADRENDERENTTEST2_DEFINITION_FILE = 
	wxT("<world>\n")
	wxT(" <definitions>\n")
	wxT("  <renderentdefs>\n")
	wxT("   <renderent name=\"renderEnt2\">\n")
	wxT("    <mesh>\n")
	wxT("     <param name=\"meshname\" value=\"TestingCube\" />\n")
	wxT("    </mesh>\n")
	wxT("    <material>\n")
	wxT("     <param name=\"materialname\" value=\"Test/ColourTest\" />\n")
	wxT("    </material>\n")
	wxT("    <param name=\"position\" value=\"-3.0;-4.0;-5.0\" />\n")
	wxT("   </renderent>\n")
	wxT("  </renderentdefs>\n")	
	wxT(" </definitions>\n")
	wxT("</world>");
static const wxChar* TEST_RENDER_ENT2_NAME = wxT("renderEnt2");
static const Ogre::Vector3 TEST_RENDER_ENT2_POSITION = { -3.0, -4.0, -5.0 };

static const wxChar* LOADCAMERA_DEFINITION_FILE = 
	wxT("<world>\n")
	wxT(" <definitions>\n")
	wxT("   <cameradefs>\n")
	wxT("    <camera name=\"MainCamera\">\n")
	wxT("     <param name=\"position\" value=\"0.0, 10.0, -5.0\" />\n")
	wxT("     <param name=\"direction\" value=\"0, -10, 5\" />\n")
	wxT("    </camera>\n")
	wxT("   </cameradefs>\n")
	wxT(" </definitions>\n")
	wxT("</world>");
static const wxChar* TEST_CAMERA_NAME = wxT("MainCamera");
static const Ogre::Vector3 TEST_CAMERA_POSITION = { 0.0, 10.0, -5.0 };
static const Ogre::Vector3 TEST_CAMERA_DIRECTION = { 0.0, -10.0, 5.0 };

static const wxChar* LOADRENDER_DEFINITION_FILE = 
	wxT("<world>\n")
	wxT(" <definitions>\n")
	wxT("  <renderentdefs>\n")
	wxT("   <renderent name=\"renderEnt1\">\n")
	wxT("    <mesh>\n")
	wxT("     <param name=\"meshname\" value=\"TestingCube\" />\n")
	wxT("    </mesh>\n")
	wxT("    <material>\n")
	wxT("     <param name=\"materialname\" value=\"Test/ColourTest\" />\n")
	wxT("    </material>\n")
	wxT("    <param name=\"position\" value=\"1.0;2.0;3.0\" />\n")
	wxT("   </renderent>\n")
	wxT("  </renderentdefs>\n")	
	wxT("  <cameradefs>\n")
	wxT("   <camera name=\"MainCamera\">\n")
	wxT("    <param name=\"position\" value=\"0.0, 10.0, -5.0\" />\n")
	wxT("    <param name=\"direction\" value=\"0, -10, 5\" />\n")
	wxT("   </camera>\n")
	wxT("  </cameradefs>\n")
	wxT("  <renderdefs>\n")
	wxT("   <render name=\"render1\">\n")
	wxT("    <renderentref value=\"renderEnt1\" />\n")
	wxT("    <renderent name=\"renderEnt2\">\n")
	wxT("     <mesh>\n")
	wxT("      <param name=\"meshname\" value=\"TestingCube\" />\n")
	wxT("     </mesh>\n")
	wxT("     <material>\n")
	wxT("      <param name=\"materialname\" value=\"Test/ColourTest\" />\n")
	wxT("     </material>\n")
	wxT("     <param name=\"position\" value=\"-3.0;-4.0;-5.0\" />\n")
	wxT("    </renderent>\n")
	wxT("   </render>\n")
	wxT("   <render name=\"render2\">\n")
	wxT("    <cameraref value=\"MainCamera\" />\n")
	wxT("   </render>\n")
	wxT("  </renderdefs>\n")
	wxT(" </definitions>\n")
	wxT("</world>");
static const wxChar* TEST_RENDER1_NAME = wxT("render1");
static const wxChar* TEST_RENDER2_NAME = wxT("render2");

static const wxChar* LOADINPUT_DEFINITION_FILE = 
	wxT("<world>\n")
	wxT(" <definitions>\n")
	wxT("  <inputdefs>\n")
	wxT("   <input name=\"testInput\">\n")
	wxT("    <key action=\"up\" value=\"key_w\" />\n")
	wxT("    <key action=\"down\" value=\"key_s\" />\n")
	wxT("    <key action=\"left\" value=\"key_a\" />\n")
	wxT("    <key action=\"right\" value=\"key_d\" />\n")
	wxT("   </input>\n")
	wxT("   <input name=\"cameraInput\">\n")
	wxT("    <key action=\"forward\" value=\"key_j\" />\n")
	wxT("    <key action=\"backward\" value=\"key_l\" />\n")
	wxT("   </input>\n")
	wxT("  </inputdefs>\n")
	wxT(" </definitions>\n")
	wxT("</world>");
	
static const wxChar* TEST_INPUT1_NAME = wxT("testInput");
static const wxChar* TEST_INPUT2_NAME = wxT("cameraInput");

static const wxChar* LOADLOGIC_DEFINITION_FILE = 
	wxT("<world>\n")
	wxT(" <definitions>\n")
	wxT(" <logicdefs>\n")
	wxT("  <logic name=\"manualLogic\">\n")
	wxT("   <param name=\"logictype\" value=\"MANUAL_TEST\" />\n")
	wxT("  </logic>\n")
	wxT(" </logicdefs>\n")
	wxT(" </definitions>\n")
	wxT("</world>");
	
static const wxChar* TEST_LOGIC_NAME = wxT("manualLogic");
static const wxChar* TEST_LOGIC_TYPE = wxT("MANUAL_TEST");

static const wxChar* LOADENTITYTEST1_DEFINITION_FILE = 
	wxT("<world>\n")
	wxT(" <definitions>\n")
	wxT("  <meshdefs>\n")
	wxT("   <mesh name=\"mesh1\">\n")
	wxT("    <param name=\"meshname\" value=\"TestingCube\" />\n")
	wxT("   </mesh>\n")
	wxT("  </meshdefs>\n")
	wxT("  <materialdefs>\n")
	wxT("   <material name=\"material1\">\n")
	wxT("    <param name=\"materialname\" value=\"Test/ColourTest\" />\n")
	wxT("   </material>\n")
	wxT("  </materialdefs>\n")
	wxT("  <renderentdefs>\n")
	wxT("   <renderent name=\"renderEnt1\">\n")
	wxT("    <meshref value=\"mesh1\" />\n")
	wxT("	 <materialref value=\"material1\" />\n")
	wxT("    <param name=\"position\" value=\"1.0;2.0;3.0\" />\n")
	wxT("   </renderent>\n")
	wxT("  </renderentdefs>\n")
	wxT("  <cameradefs>\n")
	wxT("   <camera name=\"MainCamera\">\n")
	wxT("    <param name=\"position\" value=\"0.0, 10.0, -5.0\" />\n")
	wxT("    <param name=\"direction\" value=\"0, -10, 5\" />\n")
	wxT("   </camera>\n")
	wxT("  </cameradefs>\n")
	wxT("  <renderdefs>\n")
	wxT("   <render name=\"render1\">\n")
	wxT("    <renderentref value=\"renderEnt1\" />\n")
	wxT("    <renderent name=\"renderEnt2\">\n")
	wxT("     <mesh>\n")
	wxT("      <param name=\"meshname\" value=\"TestingCube\" />\n")
	wxT("     </mesh>\n")
	wxT("     <material>\n")
	wxT("      <param name=\"materialname\" value=\"Test/ColourTest\" />\n")
	wxT("     </material>\n")
	wxT("     <param name=\"position\" value=\"-3.0;-4.0;-5.0\" />\n")
	wxT("    </renderent>\n")
	wxT("   </render>\n")
	wxT("   <render name=\"render2\">\n")
	wxT("    <cameraref value=\"MainCamera\" />\n")
	wxT("   </render>\n")
	wxT("  </renderdefs>\n")
	wxT("  <inputdefs>\n")
	wxT("   <input name=\"testInput\">\n")
	wxT("    <key action=\"up\" value=\"key_w\" />\n")
	wxT("    <key action=\"down\" value=\"key_s\" />\n")
	wxT("    <key action=\"left\" value=\"key_a\" />\n")
	wxT("    <key action=\"right\" value=\"key_d\" />\n")
	wxT("   </input>\n")
	wxT("   <input name=\"cameraInput\">\n")
	wxT("    <key action=\"forward\" value=\"key_j\" />\n")
	wxT("    <key action=\"backward\" value=\"key_l\" />\n")
	wxT("   </input>\n")
	wxT("  </inputdefs>\n")
	wxT("  <logicdefs>\n")
	wxT("   <logic name=\"manualLogic\">\n")
	wxT("    <param name=\"logictype\" value=\"MANUAL_TEST\" />\n")
	wxT("   </logic>\n")
	wxT("  </logicdefs>\n")	
	wxT(" </definitions>\n")
	wxT(" <scene>\n")
	wxT("  <entity name=\"entity1\">\n")
	wxT("   <transform>\n")
	wxT("    <param name=\"position\" value=\"0.0, 0.0, 0.0\" />\n")
	wxT("    <param name=\"scale\" value=\"1.0, 1.0, 1.0\" />\n")
	wxT("    <param name=\"rotation\" value=\"1.0, 0.5, 0.3, 0.1\" />\n")		
	wxT("   </transform>\n")
	wxT("   <renderref value=\"render1\" />\n")
	wxT("   <inputref value=\"testInput\" />\n")
	wxT("   <logicref value=\"manualLogic\" />\n")
	wxT("  </entity>\n")
	wxT("  <entity name=\"mainCameraEntity\">\n")
	wxT("   <transform>\n")
	wxT("    <param name=\"position\" value=\"0.0, 0.0, 0.0\" />\n")
	wxT("    <param name=\"scale\" value=\"1.0, 1.0, 1.0\" />\n")
	wxT("    <param name=\"rotation\" value=\"1.0, 0.5, 0.3, 0.1\" />\n")
	wxT("   </transform>\n")
	wxT("   <renderref value=\"render2\" />\n")
	wxT("   <inputref value=\"cameraInput\" />\n")
	wxT("   <logicref value=\"manualLogic\" />\n")
	wxT("  </entity>\n")
	wxT(" </scene>\n")
	wxT("</world>\n");
		
static const wxChar* TEST_ENTITY1_NAME = wxT("entity1");
static const wxChar* TEST_ENTITY2_NAME = wxT("mainCameraEntity");
static const Ogre::Vector3 TEST_TRANSFORM_POSITION = { 0.0, 0.0, 0.0 };
static const Ogre::Vector3 TEST_TRANSFORM_SCALE 	= {1.0, 1.0, 1.0 };
static const Ogre::Quaternion TEST_TRANSFORM_ROTATION = { 1.0, 0.5, 0.3, 0.1 };


static const wxChar* LOADPHYSICSTEST_DEFINITION_FILE = 
	wxT("<world>\n")
	wxT(" <definitions>\n")
	wxT(" </definitions>\n")
	wxT(" <scene>\n")
	wxT("  <entity name=\"entity1\">\n")
	wxT("   <physics>\n")
	wxT("    <shape type=\"box\">\n")
	wxT("     <param name=\"boxHalfSize\" value=\"0.5, 0.5, 0.5\" />\n")
	wxT("	 </shape>\n")
	wxT("    <param name=\"mass\" value=\"1.0\" />\n")
	wxT("    <param name=\"inertia\" value=\"0.0,0.0,0.0\" />\n")
	wxT("   </physics>\n")
	wxT("  </entity>\n")
	wxT(" </scene>\n")
	wxT("</world>\n");
	
static const float TEST_PHYSICS_MASS = 1.0;
static const wxInt32 TEST_PHYSICS_SHAPE_BOXTYPE = 1;
static const Ogre::Vector3 TEST_PHYSICS_BOX_BOXHALFSIZE = { 0.5, 0.5, 0.5 };
static const Ogre::Vector3 TEST_PHYSICS_INERTIA = { 0.0, 0.0, 0.0 };


#endif // __GAME_XML_LOADER_TEST_DATA_H__