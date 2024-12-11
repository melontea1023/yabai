#include "Object.h"

using namespace DirectX::SimpleMath;

//ƒRƒ“ƒXƒg
Object::Object(Camera* cam) :m_Camera(cam) {

}


Object::~Object() {

}

void Object::SetPosition(const Vector3& pos) {
	m_Position.x = pos.x;
	m_Position.y = pos.y;
	m_Position.z = pos.z;
}

Vector3 Object::GetPosition()const {
	return m_Position;
}

void Object::SetRotation(const Vector3& rot) {
	m_Rotation.x = rot.x;
	m_Rotation.y = rot.y;
	m_Rotation.z = rot.z;
}
Vector3 Object::GetRotation()const {
	return m_Rotation;
}