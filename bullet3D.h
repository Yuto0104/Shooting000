//=============================================================================
//
// 3D�o���b�g�N���X(bullet3D.h)
// Author : �������l
// �T�v : �I�u�W�F�N�g�������s��
//
//=============================================================================
#ifndef _BULLET3D_H_		// ���̃}�N����`������ĂȂ�������
#define _BULLET3D_H_		// ��d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "object3D.h"
#include "texture.h"

//=============================================================================
// 3D�o���b�g�N���X
// Author : �������l
// �T�v : 3D�o���b�g�������s���N���X
//=============================================================================
class CBullet3D : public CObject3D
{
public:
	//--------------------------------------------------------------------
	// �ÓI�����o�֐�
	//--------------------------------------------------------------------
	static CBullet3D *Create(void);				// 2D�I�u�W�F�N�g�̐���

	//--------------------------------------------------------------------
	// �R���X�g���N�^�ƃf�X�g���N�^
	//--------------------------------------------------------------------
	CBullet3D();
	~CBullet3D();

	//--------------------------------------------------------------------
	// �����o�֐�
	//--------------------------------------------------------------------
	HRESULT Init() override;										// ������
	void Uninit() override;											// �I��
	void Update() override;											// �X�V
	void Draw() override;											// �`��
	void SetMoveVec(D3DXVECTOR3 moveVec) { m_moveVec = moveVec; }	// �ړ�����
	void SetSpeed(float fSpeed) { m_fSpeed = fSpeed; }				// ���x

private:
	//--------------------------------------------------------------------
	// �����o�ϐ�
	//--------------------------------------------------------------------
	D3DXVECTOR3		m_move;					// �ړ���
	D3DXVECTOR3		m_moveVec;				// �ړ�����
	float			m_fSpeed;				// ���x
};

#endif



