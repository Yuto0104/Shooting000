//=============================================================================
//
// 3D���[�V�����L�����N�^�[�N���X(motion_char3D.h)
// Author : �������l
// �T�v : 3D�v���C���[�������s��
//
//=============================================================================
#ifndef _MOTION_CHAR3D_H_			// ���̃}�N����`������ĂȂ�������
#define _MOTION_CHAR3D_H_			// ��d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "model3D.h"
#include "main.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CMotion;

//=============================================================================
// 3D�v���C���[�N���X
// Author : �������l
// �T�v : 3D�v���C���[�������s���N���X
//=============================================================================
class CMotionChar3D : public CModel3D
{
public:
	//--------------------------------------------------------------------
	// �ÓI�����o�֐�
	//--------------------------------------------------------------------
	static CMotionChar3D *Create(const char *pName);			// 3D�v���C���[�̐���

	//--------------------------------------------------------------------
	// �R���X�g���N�^�ƃf�X�g���N�^
	//--------------------------------------------------------------------
	CMotionChar3D();
	~CMotionChar3D();

	//--------------------------------------------------------------------
	// �����o�֐�
	//--------------------------------------------------------------------
	virtual HRESULT Init(const char *pName);									// ������
	void Uninit() override;														// �I��
	void Update() override;														// �X�V
	void Draw() override;														// �`��
	void ReloadMotion(const char *pName);										// ���[�V�����̍ēǂݍ���
	void SetNumMotion(int nNumMotion) { m_nNumMotion = nNumMotion; }			// ���[�V�����i���o�[�̐ݒ�
	int GetNumMotion() { return m_nNumMotion; }									// ���[�V�����i���o�[�̎擾
	
private:
	//--------------------------------------------------------------------
	// �����o�ϐ�
	//--------------------------------------------------------------------
	CMotion		*m_pMotion;				// ���[�V�����C���X�^���X
	int			m_nNumMotion;			// ���݂̃��[�V�����ԍ�
	int			m_nNumMotionOld;		// �ߋ��̃��[�V�����ԍ�
	bool		m_bMotion;				// ���[�V�����̎g�p��
	bool		m_bMotionBlend;			// ���[�V�����u�����h�̎g�p��
};

#endif