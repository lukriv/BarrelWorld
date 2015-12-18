#include "gphysdbgdraw.h"

enum {
  POSITION_BINDING,
  COLOUR_BINDING
};

#ifndef M_PI
#define M_PI    3.14159265358979323846f
#endif

CDynamicLineDrawer::CDynamicLineDrawer(void)
{
	Initialize();
	Dirty = true;
}

CDynamicLineDrawer::~CDynamicLineDrawer(void)
{
	delete mRenderOp.vertexData;
	delete mRenderOp.indexData;
}

void CDynamicLineDrawer::Initialize()
{
	mRenderOp.operationType = Ogre::RenderOperation::OT_LINE_LIST;
	mRenderOp.useIndexes = false;
	mRenderOp.vertexData = new Ogre::VertexData;
	
	// Reset buffer capacities
	VertexBufferCapacity = 0;
	
	// Create vertex declaration
	CreateVertexDeclaration();
}

Ogre::Real CDynamicLineDrawer::getBoundingRadius() const
{
	return Ogre::Math::Sqrt(std::max(mBox.getMaximum().squaredLength(), mBox.getMinimum().squaredLength()));
}

Ogre::Real CDynamicLineDrawer::getSquaredViewDepth(const Ogre::Camera* Cam) const
{
	Ogre::Vector3 VMin, VMax, VMid, VDist;
	VMin = mBox.getMinimum();
	VMax = mBox.getMaximum();
	VMid = ((VMax - VMin) * 0.5f) + VMin;
	VDist = Cam->getDerivedPosition() - VMid;

	return VDist.squaredLength();
}

void CDynamicLineDrawer::AddPoint(const Ogre::Vector3 &Point, const Ogre::ColourValue &Colour)
{
	Points.push_back(Point);
	Colours.push_back(Colour);
	Dirty = true;
}

void CDynamicLineDrawer::AddPoint(Ogre::Real X, Ogre::Real Y, Ogre::Real Z, Ogre::Real R, Ogre::Real G, Ogre::Real B)
{
	Points.push_back(Ogre::Vector3(X, Y, Z));
	Colours.push_back(Ogre::ColourValue(R, G, B));
	Dirty = true;
}

void CDynamicLineDrawer::SetPoint(unsigned short Index, const Ogre::Vector3 &Value, const Ogre::ColourValue &ColourValue)
{
	assert(Index < Points.size() && "Point index is out of bounds!!");

	Points[Index] = Value;
	Colours[Index] = ColourValue;
	Dirty = true;
}

const Ogre::Vector3& CDynamicLineDrawer::GetPoint(unsigned short Index) const
{
	assert(Index < Points.size() && "Point index is out of bounds!!");
	return Points[Index];
}

unsigned short CDynamicLineDrawer::GetNumPoints() const
{
	return (unsigned short) Points.size();
}

void CDynamicLineDrawer::Clear()
{
	Points.clear();
	Colours.clear();
	Dirty = true;
}

void CDynamicLineDrawer::Update()
{
	if ( Dirty )
		FillHardwareBuffers();
}

void CDynamicLineDrawer::CreateVertexDeclaration()
{
	// Points
	Ogre::VertexDeclaration *Decl = mRenderOp.vertexData->vertexDeclaration;
	Decl->addElement(POSITION_BINDING, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
	
	// Colour
	Decl->addElement(COLOUR_BINDING, 0, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);
}

void CDynamicLineDrawer::PrepareHardwareBuffers(size_t VertexCount)
{
	// Prepare vertex buffer
	size_t NewVertCapacity = VertexBufferCapacity;
	if ( ( VertexCount > VertexBufferCapacity ) || !VertexBufferCapacity )
	{
		// vertexCount exceeds current capacity!
		// It is necessary to reallocate the buffer.

		// Check if this is the first call
		if (!NewVertCapacity)
			NewVertCapacity = 1;

		// Make capacity the next power of two
		while (NewVertCapacity < VertexCount)
			NewVertCapacity <<= 1;
	}
	else if ( VertexCount < VertexBufferCapacity >> 1 ) 
	{
		// Make capacity the previous power of two
		while ( VertexCount < NewVertCapacity >> 1 )
			NewVertCapacity >>= 1;
	}
	if ( NewVertCapacity != VertexBufferCapacity )
	{
		VertexBufferCapacity = NewVertCapacity;
		// Create new vertex buffer
		Ogre::HardwareVertexBufferSharedPtr VBuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
			mRenderOp.vertexData->vertexDeclaration->getVertexSize(0),
			VertexBufferCapacity,
			Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY );

		// Bind buffer
		mRenderOp.vertexData->vertexBufferBinding->setBinding(0, VBuf);

		// Create new colour buffer
		size_t offset = Ogre::VertexElement::getTypeSize(Ogre::VET_COLOUR);
		VBuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
			offset,
			VertexBufferCapacity,
			Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY );

		// Bind buffer
		mRenderOp.vertexData->vertexBufferBinding->setBinding(1, VBuf);
	}
	// Update vertex count in the render operation
	mRenderOp.vertexData->vertexCount = VertexCount;
}

void CDynamicLineDrawer::FillHardwareBuffers()
{
	int Size = int(Points.size());

	PrepareHardwareBuffers(Size);

	if (!Size) { 
		mBox.setExtents( Ogre::Vector3::ZERO, Ogre::Vector3::ZERO );
		Dirty = false;
		return;
	}
  
	Ogre::Vector3 AABMin = Points[0];
	Ogre::Vector3 AABMax = Points[0];

	Ogre::HardwareVertexBufferSharedPtr VBuf =
		mRenderOp.vertexData->vertexBufferBinding->getBuffer(0);

	Ogre::HardwareVertexBufferSharedPtr CBuf = 
		mRenderOp.vertexData->vertexBufferBinding->getBuffer(1);

	// get rendersystem to pack colours
	Ogre::GameRenderSystem* RS = Ogre::Root::getSingleton().getRenderSystem();

	Ogre::Real* VPrPos = static_cast<Ogre::Real*>(VBuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));
	Ogre::RGBA* CPrPos = static_cast<Ogre::RGBA*>(CBuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));
	for(int i = 0; i < Size; i++)
	{
		*VPrPos++ = Points[i].x;
		*VPrPos++ = Points[i].y;
		*VPrPos++ = Points[i].z;

		Ogre::RGBA color;
		RS->convertColourValue(Colours[i], &color);
		*CPrPos++ = color;
		//*CPrPos++ = unsigned int(Colours[i].g);
		//*CPrPos++ = unsigned int(Colours[i].b);

		if(Points[i].x < AABMin.x)
			AABMin.x = Points[i].x;
		if(Points[i].y < AABMin.y)
			AABMin.y = Points[i].y;
		if(Points[i].z < AABMin.z)
			AABMin.z = Points[i].z;

		if(Points[i].x > AABMax.x)
			AABMax.x = Points[i].x;
		if(Points[i].y > AABMax.y)
			AABMax.y = Points[i].y;
		if(Points[i].z > AABMax.z)
			AABMax.z = Points[i].z;
	}
	VBuf->unlock();
	CBuf->unlock();

	mBox.setExtents(AABMin, AABMax);

	Dirty = false;
}

#ifndef M_PI
#define M_PI    3.14159265358979323846f
#endif
CDebugDraw::CDebugDraw( Ogre::SceneManager* Scene, btDynamicsWorld* World )
: World( World ), IsActive( true )
{

	// generate a unit sphere
	std::size_t CircleSubdivs = 12;
	UnitCircle.reserve( CircleSubdivs+1 );

	for ( std::size_t i = 0; i < CircleSubdivs; ++i )
	{
		float Angle = (M_PI * 2.f * i) / CircleSubdivs;
		UnitCircle.push_back( Ogre::Vector2( std::cos( Angle ), std::sin( Angle ) ) );
	}
	UnitCircle.push_back( UnitCircle.front() );

	// setup rendering properties
	DebugLineDrawer = new CDynamicLineDrawer();
	if ( !Scene->hasSceneNode("DebugDrawer") )
		DebugDrawerNode = Scene->getRootSceneNode()->createChildSceneNode("DebugDrawer");
	else
	{
		DebugDrawerNode = Scene->getSceneNode("DebugDrawer");
		Ogre::SceneNode::ObjectIterator Iter = DebugDrawerNode->getAttachedObjectIterator();
		while( Iter.hasMoreElements() )
			Scene->destroyEntity( dynamic_cast<Ogre::Entity*>( Iter.getNext() ) );
	}
	DebugDrawerNode->attachObject( DebugLineDrawer );

	DebugLineDrawer->setMaterial("DebugLines");

	World->setDebugDrawer( this );
}

CDebugDraw::~CDebugDraw()
{
	delete DebugLineDrawer;
	World->setDebugDrawer(NULL);
}

void CDebugDraw::Update()
{
	if ( IsActive )
	{
		World->debugDrawWorld();
		DebugLineDrawer->Update();
		DebugDrawerNode->needUpdate();
		DebugLineDrawer->Clear();
	}
	else
	{
		DebugLineDrawer->Clear();
		DebugLineDrawer->Update();
		DebugDrawerNode->needUpdate();		
	}
}

void CDebugDraw::setDebugMode( int DebugMode )
{
	this->IsActive = DebugMode!=0;

	if ( !IsActive )
	{
		DebugLineDrawer->Clear();
	}
}

int CDebugDraw::getDebugMode() const
{
	return IsActive ? 1 : 0;
}

void CDebugDraw::drawSphere( const btVector3& p, btScalar radius, const btVector3& BColor )
{
	/*Ogre::ColourValue Color( BColor[0], BColor[1], BColor[2] );

	ManualObject.begin( "", Ogre::RenderOperation::OT_LINE_LIST );
	for ( std::size_t i = 1; i < UnitCircle.size(); ++i )
	{
		Ogre::Vector2& c0 = UnitCircle[i-1];
		ManualObject.position( p[0]+c0[0]*radius, p[1]+c0[1]*radius, p[2] );
		ManualObject.colour( Color );
		Ogre::Vector2& c1 = UnitCircle[i];
		ManualObject.position( p[0]+c1[0]*radius, p[1]+c1[1]*radius, p[2] );
		ManualObject.colour( Color );
	}
	
	for ( std::size_t i = 1; i < UnitCircle.size(); ++i )
	{
		Ogre::Vector2& c0 = UnitCircle[i-1];
		ManualObject.position( p[0]+c0[0]*radius, p[1], p[2]+c0[1]*radius );
		ManualObject.colour( Color );
		Ogre::Vector2& c1 = UnitCircle[i];
		ManualObject.position( p[0]+c1[0]*radius, p[1], p[2]+c1[1]*radius );
		ManualObject.colour( Color );
	}

	for ( std::size_t i = 1; i < UnitCircle.size(); ++i )
	{
		Ogre::Vector2& c0 = UnitCircle[i-1];
		ManualObject.position( p[0], p[1]+c0[1]*radius, p[2]+c0[0]*radius );
		ManualObject.colour( Color );
		Ogre::Vector2& c1 = UnitCircle[i];
		ManualObject.position( p[0], p[1]+c1[1]*radius, p[2]+c1[0]*radius );
		ManualObject.colour( Color );
	}
	ManualObject.end();*/
}

void CDebugDraw::drawLine( const btVector3& from, const btVector3& to,
						  const btVector3& color )
{
	DebugLineDrawer->AddPoint( Ogre::Vector3(from.getX(), from.getY(), from.getZ()), Ogre::ColourValue(color.getX(), color.getY(), color.getZ()) );
	DebugLineDrawer->AddPoint( Ogre::Vector3(to.getX(), to.getY(), to.getZ()), Ogre::ColourValue(color.getX(), color.getY(), color.getZ()) );
}


void CDebugDraw::drawTriangle( const btVector3& v0, const btVector3& v1, const btVector3& v2,
					   const btVector3& color, btScalar alpha )
{
	/*Ogre::ColourValue OgreColor( color.x(), color.y(), color.z(), alpha );

	ManualObject.begin( "", Ogre::RenderOperation::OT_LINE_LIST );
		ManualObject.position( Ogre::Vector3( v0 ) );
		ManualObject.colour( OgreColor );
		ManualObject.position( Ogre::Vector3( v1 ) );
		ManualObject.colour( OgreColor );
		ManualObject.position( Ogre::Vector3( v1 ) );
		ManualObject.colour( OgreColor );
		ManualObject.position( Ogre::Vector3( v2 ) );
		ManualObject.colour( OgreColor );
		ManualObject.position( Ogre::Vector3( v2 ) );
		ManualObject.colour( OgreColor );
		ManualObject.position( Ogre::Vector3( v0 ) );
		ManualObject.colour( OgreColor );
	ManualObject.end();*/
}

void CDebugDraw::drawContactPoint( const btVector3& PointOnB, const btVector3& normalOnB,
	btScalar distance, int lifeTime, const btVector3& color )
{
}

void CDebugDraw::reportErrorWarning( const char* WarningString )
{
	std::cout << WarningString << std::flush;
}

void CDebugDraw::draw3dText( const btVector3& location, const char* textString )
{
}