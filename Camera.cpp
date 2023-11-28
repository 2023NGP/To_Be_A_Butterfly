#include "stdafx.h"
#include "Camera.h"

Camera::Camera()
	: m_fTime(0.5f)
	, m_fAccTime(0.5f)
	, m_fSpeed(0.f)
{

}
Camera::~Camera() {

}
void Camera::calDiff(const float frameTime)
{
	// ���� LookAt�� ���� LookAt�� ���̰��� �����ؼ� ������ LookAt�� ���Ѵ�.

	m_fAccTime += frameTime;

	if (m_fTime <= m_fAccTime) {
		m_vCurLookAt = m_vLookAt;
	}
	else {
		VECTOR2 vLookDir = m_vLookAt - m_vPrevLookAt;
		m_vCurLookAt = m_vPrevLookAt + vLookDir.Normalize() * m_fSpeed * frameTime;
	}

	VECTOR2 vResolution = POINT{ 1280, 768 };
	VECTOR2	vCenter = vResolution / 2;

	m_vDiff = m_vCurLookAt - vCenter;
	m_vPrevLookAt = m_vCurLookAt;
}

void Camera::setLookAt(POINT p)
{
	m_vLookAt.x = (float)p.x;
	m_vLookAt.y = (float)p.y;
}

void Camera::Update(const float frameTime)
{	
	if (m_vLookAt.y <= 0) {
		m_vLookAt.y = 0.f;
	}
	else {
		m_vLookAt.y -= 50.f * frameTime;

		// ȭ�� �߾���ǥ�� ī�޶� LookAt ��ǥ���� ���̰� ���
		calDiff(frameTime);
	}
}
