#pragma once
class Camera
{
public:
	VECTOR2			m_vLookAt = POINT{ 0, MEM_HEIGHT - (FRAME_HEIGHT) };		// 카메라가 보는 위치
	VECTOR2			m_vCurLookAt;		// 이전 위치와 현재 위치 사이의 보정된 위치
	VECTOR2			m_vPrevLookAt;		// 카메라가 보는 이전프레임 위치

	VECTOR2			m_vDiff;			// 해상도 중심위치와 카메라 LookAT 간의 차이 값

	float			m_fTime;
	float			m_fSpeed;
	float			m_fAccTime;

	void calDiff(const float frameTime);

public:
	Camera();
	~Camera();

	void setLookAt(VECTOR2 _vLook) {
		m_vLookAt = _vLook;
		float fMoveDist = (m_vLookAt - m_vPrevLookAt).Length();
		m_fSpeed = fMoveDist / m_fTime;
		m_fAccTime = 0.f;
	}

	VECTOR2 getLookAt() { return m_vCurLookAt; }
	VECTOR2 getRenderPos(VECTOR2 _vObjPos) { return _vObjPos - m_vDiff; }
	VECTOR2 getRealPos(VECTOR2 _vRenderPos) { return _vRenderPos + m_vDiff; }

	void Update(const float frameTime);
};

