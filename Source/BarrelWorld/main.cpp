/**
* A simple Urho3D example in one (big) file.
* Copyright 2014 Peter Gebauer, 2015 gawag.
* Released under the same permissive MIT-license as Urho3D.
* https://raw.githubusercontent.com/urho3d/Urho3D/master/License.txt
*
* Why?
* Because A first "simple" example tutorial shouldn't require additional
* frameworks or special toolchains. This file along with Urho3D and a C++
* compiler should do it. (you might have to change the prefix path in
* Game::Setup)
* Many (like me) want to learn themselves and just get an overview
* without the overhead of understanding how the example is built.
* I hope this file covers the basics and is of use to you.
*/
 


#include "Game.h"



/**
* This macro is expanded to (roughly, depending on OS) this:
*
* > int RunApplication()
* > {
* > Urho3D::SharedPtr<Urho3D::Context> context(new Urho3D::Context());
* > Urho3D::SharedPtr<className> application(new className(context));
* > return application->Run();
* > }
* >
* > int main(int argc, char** argv)
* > {
* > Urho3D::ParseArguments(argc, argv);
* > return function;
* > }
*/
URHO3D_DEFINE_APPLICATION_MAIN(BW::Game)

