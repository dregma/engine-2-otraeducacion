#include "Scene2.h"
#include<sstream>
#include<string>
#include "Node.h"
#include<sstream>

void Juego::Scene2::frame(pGr::Renderer& renderer ,pGr::Importer& importer, pGr::Game& game, pGr::DirectInput& dInput){
	//input camera.
	float movementSpeed = 0.1f;
	float rotationSpeed = 0.1f;
	//Aceletar//
	if(dInput.keyDown(pGr::Input::KEY_LCONTROL)){
		movementSpeed = 0.9f;
	}
	////////////
	//******Atras y adelante************//
	if(dInput.keyDown(pGr::Input::KEY_W)){
		renderer.m_pkCamera->walk(movementSpeed);
	}
	if(dInput.keyDown(pGr::Input::KEY_S)){
		renderer.m_pkCamera->walk(-movementSpeed);
	}
	//******Atras y adelante************//

	//******Girar derecha/izquierda************//
	if(dInput.keyDown(pGr::Input::KEY_D)){
		renderer.m_pkCamera->roll(rotationSpeed);
	}if(dInput.keyDown(pGr::Input::KEY_A)){
		renderer.m_pkCamera->roll(-rotationSpeed);
	}
	//******Girar derecha/izquierda************//

	//******Arriba y abajo************//
	if(dInput.keyDown(pGr::Input::KEY_SPACE)){
		renderer.m_pkCamera->fly(movementSpeed);
	}
	if(dInput.keyDown(pGr::Input::KEY_LSHIFT)){
		renderer.m_pkCamera->fly(-movementSpeed);
	}
	//******Arriba y abajo************//

	//******Moverse para los costados************//
	if(dInput.keyDown(pGr::Input::KEY_E)){
		renderer.m_pkCamera->strafe(movementSpeed);
	}
	if(dInput.keyDown(pGr::Input::KEY_Q)){
		renderer.m_pkCamera->strafe(-movementSpeed);
	}
	//******Moverse para los costados************//
	//OutputDebugString("en el orto");
	renderer.m_pkCamera->DebugCamPos();
	//theMesh->draw();
	//std::stringstream ss;
	//ss << "Cuberto" << "x: " << theMesh->posX() << "y: " << theMesh->posY() << "z: " << theMesh->posZ() << std::endl;

	//std::string s( ss.str() );

	//OutputDebugString( s.c_str() );
}
bool Juego::Scene2::init(pGr::Renderer& renderer,pGr::Importer& importer){
	std::stringstream ss;
	//ss << "CARA DE CHOTA"<< std::endl;

	std::string s( ss.str() );

	OutputDebugString( s.c_str() );
	node = new pGr::Node();
	importer.import3DScene("Mesh.obj", *node);
	node->setPos(0,0,0);
   
	return true;
}
bool Juego::Scene2::deInit(){
	return true;
}