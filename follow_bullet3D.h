//=============================================================================
//
// 3D�Ǐ]�o���b�g�N���X(follow_bullet3D.h)
// Author : �������l
// �T�v : �I�u�W�F�N�g�������s��
//
//=============================================================================
#ifndef _FOLLOW_BULLET3D_H_		// ���̃}�N����`������ĂȂ�������
#define _FOLLOW_BULLET3D_H_		// ��d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CObject;

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "bullet3D.h"
#include "texture.h"

//=============================================================================
// 3D�Ǐ]�o���b�g�N���X
// Author : �������l
// �T�v : 3D�Ǐ]�o���b�g�������s���N���X
//=============================================================================
class CFollowBullet3D : public CBullet3D
{
public:
	//--------------------------------------------------------------------
	// �ÓI�����o�֐�
	//--------------------------------------------------------------------
	static CFollowBullet3D *Create(void);				// 2D�I�u�W�F�N�g�̐���

	//--------------------------------------------------------------------
	// �R���X�g���N�^�ƃf�X�g���N�^
	//--------------------------------------------------------------------
	CFollowBullet3D();
	~CFollowBullet3D();

	//--------------------------------------------------------------------
	// �����o�֐�
	//--------------------------------------------------------------------
	HRESULT Init() override;																				// ������
	void Uninit() override;																					// �I��
	void Update() override;																					// �X�V
	void Draw() override;																					// �`��
	void SetTarget();																						// �^�[�Q�b�g�̐ݒ�
	void SetCoefficient(const float fCoefficient) { m_fCoefficient = fCoefficient; }						// �����W���̐ݒ�
	void SetAddCoefficient(const float fAddCoefficient) { m_fAddCoefficient = fAddCoefficient; }			// �����W���̉��Z�l�̐ݒ�

private:
	//--------------------------------------------------------------------
	// �����o�֐�
	//--------------------------------------------------------------------
	void Follow();				// �Ǐ]
	void AddCoefficient();		// �����W���̉��Z
	
	//--------------------------------------------------------------------
	// �����o�ϐ�
	//--------------------------------------------------------------------
	CObject *m_pTarget;				// �Ǐ]�̃^�[�Q�b�g
	float m_fCoefficient;			// �����W��
	float m_fAddCoefficient;		// �����W���̉��Z�l
};

#endif




