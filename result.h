//=============================================================================
//
// ���U���g�N���X(result.h)
// Author : �������l
// �T�v : ���U���g�N���X�̊Ǘ����s��
//
//=============================================================================
#ifndef _RESULT_H_		// ���̃}�N����`������ĂȂ�������
#define _RESULT_H_		// ��d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "scene_mode.h"

//=============================================================================
// ���U���g�N���X
// Author : �������l
// �T�v : ���U���g�������s���N���X
//=============================================================================
class CResult : public CSceneMode
{
public:
	//--------------------------------------------------------------------
	// �R���X�g���N�^�ƃf�X�g���N�^
	//--------------------------------------------------------------------
	CResult();
	~CResult() override;

	//--------------------------------------------------------------------
	// �����o�֐�
	//--------------------------------------------------------------------
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Update() override;		// �X�V
	void Draw() override;		// �`��

};

#endif



