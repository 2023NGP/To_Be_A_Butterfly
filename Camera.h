#pragma once
class Camera
{
public:
	VECTOR2			m_vLookAt = POINT{ 0, MEM_HEIGHT - (FRAME_HEIGHT) };		// ī�޶� ���� ��ġ
	VECTOR2			m_vCurLookAt;		// ���� ��ġ�� ���� ��ġ ������ ������ ��ġ
	VECTOR2			m_vPrevLookAt;		// ī�޶� ���� ���������� ��ġ

	VECTOR2			m_vDiff;			// �ػ� �߽���ġ�� ī�޶� LookAT ���� ���� ��

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

