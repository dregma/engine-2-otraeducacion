#include "RigidBody.h"
#include "Mathjavi.h"
#include "Collider.h"
#include <d3dx9.h>
#include <Common/Base/hkBase.h>
#include <Physics2012/Dynamics/Entity/hkpRigidBody.h>
#include <Physics2012/Collide/Shape/Convex/Box/hkpBoxShape.h>
using namespace pGr;

hkpMotion::MotionType s_HavokMType [RigidBody::HavokMotionCount] = {
	hkpMotion::MOTION_FIXED,
	hkpMotion::MOTION_DYNAMIC
};

RigidBody::RigidBody() :	m_pTransformation (new D3DXMATRIX()), m_pkRigidBody(NULL), m_HMotion(Dynamic){
        hkpBoxShape* pkBox = new hkpBoxShape( hkVector4(1.0f, 1.0f, 1.0f) );

        hkpRigidBodyCinfo kRigidBodyInfo;

        kRigidBodyInfo.m_shape = pkBox;
        kRigidBodyInfo.m_position = hkVector4(0, 0, 0);
        kRigidBodyInfo.m_motionType = hkpMotion::MOTION_DYNAMIC;
		kRigidBodyInfo.m_mass = 5.0f;
		kRigidBodyInfo.m_restitution = 2.5f;
        m_pkRigidBody = new hkpRigidBody(kRigidBodyInfo);
}

RigidBody::~RigidBody(){
	m_pkRigidBody->removeReference();
	m_pkRigidBody = NULL;
}

void RigidBody::setPosition(float x, float y, float z)
{
	m_pkRigidBody->markForWrite();
	m_pkRigidBody->setPosition( hkVector4(-x, y, z) );
	m_pkRigidBody->unmarkForWrite();
}

float RigidBody::getPosX() const
{
	    m_pkRigidBody->markForRead();
        float fResult = -m_pkRigidBody->getPosition().getComponent(0);
        m_pkRigidBody->unmarkForRead();
        return fResult;
}

float RigidBody::getPosY() const
{
	    m_pkRigidBody->markForRead();
        float fResult = m_pkRigidBody->getPosition().getComponent(1);
        m_pkRigidBody->unmarkForRead();
        return fResult;
}

float RigidBody::getPosZ() const
{
	    m_pkRigidBody->markForRead();
        float fResult = m_pkRigidBody->getPosition().getComponent(2);
        m_pkRigidBody->unmarkForRead();
        return fResult;
}

void RigidBody::setRotation(float px, float py, float pz)
{
	    m_pkRigidBody->markForWrite();

        float x, y, z, w;
        MATHJAVI::eulerToQuaternion(px, py, pz, x, y, z, w);
        hkQuaternion kRotation(x, y, z, w);
        m_pkRigidBody->setRotation(kRotation);

        m_pkRigidBody->unmarkForWrite();
}

float RigidBody::getRotationX () const
{
        m_pkRigidBody->markForRead();

        float fRotX, fRotY, fRotZ;
		MATHJAVI::quaternionToEuler( m_pkRigidBody->getRotation()(0), 
                                                                   m_pkRigidBody->getRotation()(1), 
                                                                   m_pkRigidBody->getRotation()(2), 
                                                                   m_pkRigidBody->getRotation()(3), 
                                                                   fRotX, fRotY, fRotZ
        );

        m_pkRigidBody->unmarkForRead();

        return fRotX;
}

float RigidBody::getRotationY () const
{
        m_pkRigidBody->markForRead();

        float fRotX, fRotY, fRotZ;
        MATHJAVI::quaternionToEuler( m_pkRigidBody->getRotation()(0), 
                                                                   m_pkRigidBody->getRotation()(1), 
                                                                   m_pkRigidBody->getRotation()(2), 
                                                                   m_pkRigidBody->getRotation()(3), 
                                                                   fRotX, fRotY, fRotZ
        );

        m_pkRigidBody->unmarkForRead();

        return fRotY;
}

float RigidBody::getRotationZ () const
{
        m_pkRigidBody->markForRead();

        float fRotX, fRotY, fRotZ;
		MATHJAVI::quaternionToEuler( m_pkRigidBody->getRotation()(0), 
                                                                   m_pkRigidBody->getRotation()(1), 
                                                                   m_pkRigidBody->getRotation()(2), 
                                                                   m_pkRigidBody->getRotation()(3), 
                                                                   fRotX, fRotY, fRotZ
        );

        m_pkRigidBody->unmarkForRead();

        return fRotZ;
}

void RigidBody::setCollider(Collider* pkCollider)
{
	m_pkCollider = pkCollider;
	m_pkRigidBody->markForWrite();
	m_pkRigidBody->setShape( m_pkCollider->shape() );
	m_pkRigidBody->unmarkForWrite();
}

void RigidBody::setHavokMotion(RigidBody::HavokMotion type)
{
	m_HMotion = type;
	m_pkRigidBody->markForWrite();
	m_pkRigidBody->setMotionType( s_HavokMType[type] );
	m_pkRigidBody->unmarkForWrite();
}