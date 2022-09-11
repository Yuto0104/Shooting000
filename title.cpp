//=============================================================================
//
// �^�C�g���N���X(title.cpp)
// Author : �������l
// �T�v : �^�C�g���N���X�̊Ǘ����s��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <assert.h>

#include "title.h"
#include "application.h"
#include "keyboard.h"
#include "object2D.h"
#include "bg.h"

//=============================================================================
// �R���X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�������ɍs������
//=============================================================================
CTitle::CTitle()
{
	m_pTitleLogo = nullptr;				// �^�C�g�����S�I�u�W�F�N�g
	m_pPressEnter = nullptr;			// �v���X�G���^�[�I�u�W�F�N�g
	m_nCntFrame = 0;					// �t���[���J�E���g
	m_bPressEnter = true;				// �G���^�[�L�[�������邩
}

//=============================================================================
// �f�X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�I�����ɍs������
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
// ������
// Author : �������l
// �T�v : ���_�o�b�t�@�𐶐����A�����o�ϐ��̏����l��ݒ�
//=============================================================================
HRESULT CTitle::Init()
{
	m_pTitleLogo = CObject2D::Create();
	m_pTitleLogo->SetPos(D3DXVECTOR3(640.0f, 300.0f, 0.0f));
	m_pTitleLogo->SetSize(D3DXVECTOR3(200.0f, 400.0f, 0.0f));
	m_pTitleLogo->SetCol(D3DXCOLOR(0.25f, 0.1f, 0.8f, 1.0f));
	m_pTitleLogo->LoadTex(16);

	m_pPressEnter = CObject2D::Create();
	m_pPressEnter->SetPos(D3DXVECTOR3(640.0f, 580.0f, 0.0f));
	m_pPressEnter->SetSize(D3DXVECTOR3(400.0f, 80.0f, 0.0f));
	m_pPressEnter->SetCol(D3DXCOLOR(0.25f, 0.1f, 0.8f, 1.0f));
	m_pPressEnter->LoadTex(15);
	
	CBG *pBG = CBG::Create();
	pBG->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
	return S_OK;
}

//=============================================================================
// �I��
// Author : �������l
// �T�v : �e�N�X�`���̃|�C���^�ƒ��_�o�b�t�@�̉��
//=============================================================================
void CTitle::Uninit()
{
	// �X�R�A�̉��
	Release();
}

//=============================================================================
// �X�V
// Author : �������l
// �T�v : �X�V���s��
//=============================================================================
void CTitle::Update()
{
	// ���͏��̎擾
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	FlashPE();

	if (m_bPressEnter
		&& pKeyboard->GetTrigger(DIK_RETURN))
	{
		m_bPressEnter = false;
	}

	if (m_nCntFrame >= 40)
	{
		CApplication::SetNextMode(CApplication::MODE_GAME);
	}
}

//=============================================================================
// �`��
// Author : �������l
// �T�v : �`����s��
//=============================================================================
void CTitle::Draw()
{

}

//=============================================================================
// �v���X�G���^�[�̓_�ŏ���
// Author : �������l
// �T�v : �v���X�G���^�[�̓_�ł�����
//=============================================================================
void CTitle::FlashPE()
{
	if (m_bPressEnter)
	{
		m_fFrame += 0.07f;
	}
	else if (!m_bPressEnter)
	{
		m_fFrame += 0.5f;
		m_nCntFrame++;
	}
	
	m_pPressEnter->SetCol(D3DXCOLOR(0.25f, 0.1f, 0.8f, sinf(m_fFrame) * 3.0f));
}
