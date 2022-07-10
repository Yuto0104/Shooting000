//=============================================================================
//
// �A�v���P�[�V�����N���X(application.h)
// Author : �������l
// �T�v : �e�N���X�̌Ăяo�����s���N���X
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "application.h"

#include <assert.h>
#include <stdio.h>

#include "renderer.h"
#include "keyboard.h"
#include "mouse.h"
#include "texture.h"
#include "sound.h"
#include "camera.h"
#include "light.h"

#include "object.h"
#include "object2D.h"
#include "player2D.h"
#include "enemy2D.h"
#include "object3D.h"
#include "model3D.h"
#include "motion_char3D.h"
#include "motion_player3D.h"
#include "mesh.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
CRenderer *CApplication::m_pRenderer = nullptr;					// �����_���[�C���X�^���X
CKeyboard *CApplication::m_pKeyboard = {};						// �L�[�{�[�h�C���X�^���X
CMouse *CApplication::m_pMouse = {};							// �}�E�X�C���X�^���X
CTexture *CApplication::m_pTexture = nullptr;					// �e�N�X�`���C���X�^���X
CSound *CApplication::m_pSound = nullptr;						// �T�E���h�C���X�^���X
CCamera *CApplication::m_pCamera = nullptr;						// �J�����C���X�^���X
CPlayer2D *CApplication::m_pPlayer2D = nullptr;					// �v���C���[�C���^���X

//=============================================================================
// �R���X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�������ɍs������
//=============================================================================
CApplication::CApplication()
{

}

//=============================================================================
// �f�X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�I�����ɍs������
//=============================================================================
CApplication::~CApplication()
{

}

//=============================================================================
// ������
// Author : �������l
// �T�v : �����o�ϐ���������A���N���X�̃N���G�C�g�֐����Ăяo��
//=============================================================================
HRESULT CApplication::Init(HINSTANCE hInstance, HWND hWnd)
{
	// �������̊m��	
	m_pRenderer = new CRenderer;
	m_pTexture = new CTexture;
	m_pSound = new CSound;
	m_pCamera = new CCamera;

	// ���̓f�o�C�X�̃������m��
	m_pKeyboard = new CKeyboard;
	m_pMouse = new CMouse;

	// ����������
	assert(m_pRenderer != nullptr);
	if (FAILED(m_pRenderer->Init(hWnd, TRUE)) != 0)
	{//���������������s�����ꍇ
		return-1;
	}

	// ����������
	assert(m_pTexture != nullptr);
	m_pTexture->Init();

	// ����������
	assert(m_pSound != nullptr);
	m_pSound->Init(hWnd);

	// ����������
	assert(m_pCamera != nullptr);
	m_pCamera->Init();
	//m_pCamera->SetViewType(CCamera::TYPE_PARALLEL);

	// ����������
	assert(m_pKeyboard != nullptr);
	if (FAILED(m_pKeyboard->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// ����������
	assert(m_pMouse != nullptr);
	if (FAILED(m_pMouse->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// ���C�g�̍쐬
	CLight::Create(D3DXVECTOR3(0.2f, -0.8f, -0.4f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	CLight::Create(D3DXVECTOR3(0.2f, 0.8f, -0.4f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	CLight::Create(D3DXVECTOR3(-0.2f, -0.5f, 0.7f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// �I�u�W�F�N�g�̐���
	//CObject2D::Create()->SetPos(D3DXVECTOR3(50.0f, 90.0f, 0.0f));
	//CObject2D::Create()->SetPos(D3DXVECTOR3(700.0f, 600.0f, 0.0f));
	//CEnemy2D::Create(D3DXVECTOR3(800.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f);
	//CEnemy2D::Create(D3DXVECTOR3(700.0f, 200.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f);
	//CEnemy2D::Create(D3DXVECTOR3(640.0f, 360.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f);

	CObject3D *pObject3D = CObject3D::Create();
	pObject3D->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pObject3D->SetSize(D3DXVECTOR3(100.0f, 100.0f, 0.0f));

	pObject3D = CObject3D::Create();
	pObject3D->SetPos(D3DXVECTOR3(-500.0f, 0.0f, 0.0f));
	pObject3D->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
	pObject3D->SetSize(D3DXVECTOR3(100.0f, 100.0f, 0.0f));
	pObject3D->SetBillboard(true);

	CModel3D *pModel3D = CModel3D::Create("data/MODEL/airplane000.x");
	pModel3D->SetPos(D3DXVECTOR3(100.0f, 0.0f, 0.0f));
	pModel3D->SetSize(D3DXVECTOR3(5.0f, 5.0f, 5.0f));

	CMotionChar3D *pMotionChar3D = CMotionChar3D::Create("data/MOTION/motion.txt");
	pMotionChar3D->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//pMotionChar3D = CMotionChar3D::Create("data/MOTION/motion.txt");
	//pMotionChar3D->SetPos(D3DXVECTOR3(0.0f, 0.0f, -300.0f));

	pMotionChar3D = CMotionChar3D::Create("data/MOTION/motion.txt");
	pMotionChar3D->SetPos(D3DXVECTOR3(0.0f, 100.0f, -150.0f));

	pMotionChar3D = CMotionChar3D::Create("data/MOTION/motion.txt");
	pMotionChar3D->SetPos(D3DXVECTOR3(0.0f, -100.0f, -150.0f));

	pMotionChar3D = CMotionChar3D::Create("data/MOTION/motion.txt");
	pMotionChar3D->SetPos(D3DXVECTOR3(-40.0f, 0.0f, 0.0f));

	pMotionChar3D = CMotionChar3D::Create("data/MOTION/motion.txt");
	pMotionChar3D->SetPos(D3DXVECTOR3(-80.0f, 0.0f, 0.0f));

	CMotionPlayer3D *pMotionPlayer3D = CMotionPlayer3D::Create("data/MOTION/motionShark.txt");
	pMotionPlayer3D->SetPos(D3DXVECTOR3(40.0f, 0.0f, -50.0f));
	//m_pPlayer2D = CPlayer2D::Create();
	//m_pPlayer2D->SetPos(D3DXVECTOR3(640.0f, 360.0f, 0.0f));

	CMesh3D *pMesh3D = CMesh3D::Create();
	pMesh3D->SetSize(D3DXVECTOR3(100.0f, 0, 100.0f));
	pMesh3D->SetBlock(CMesh3D::DOUBLE_INT(3, 3));

	return S_OK;
}

//=============================================================================
// �I��
// Author : �������l
// �T�v : �������̉���ƃ����[�X�֐����Ăяo��
//=============================================================================
void CApplication::Uninit()
{
	if (m_pRenderer != nullptr)
	{// �I������
		m_pRenderer->Uninit();

		// �������̉��
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	if (m_pKeyboard != nullptr)
	{// �I������
		m_pKeyboard->Uninit();

		// �������̉��
		delete m_pKeyboard;
		m_pKeyboard = nullptr;
	}

	if (m_pMouse != nullptr)
	{// �I������
		m_pMouse->Uninit();

		// �������̉��
		delete m_pMouse;
		m_pMouse = nullptr;
	}

	if (m_pTexture != nullptr)
	{// �I������
		m_pTexture->Uninit();

		// �������̉��
		delete m_pTexture;
		m_pTexture = nullptr;
	}

	if (m_pSound != nullptr)
	{// �I������
		m_pSound->Uninit();

		// �������̉��
		delete m_pSound;
		m_pSound = nullptr;
	}

	if (m_pCamera != nullptr)
	{// �I������
		m_pCamera->Uninit();

		// �������̉��
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	// ���C�g�̉��
	CLight::ReleaseAll();

	// �I�u�W�F�N�g�̉��
	CObject::ReleaseAll();
}

//=============================================================================
// �X�V
// Author : �������l
// �T�v : �X�V
//=============================================================================
void CApplication::Update()
{
	m_pKeyboard->Update();
	m_pMouse->Update();
	m_pCamera->Update();
	m_pRenderer->Update();
}

//=============================================================================
// �`��
// Author : �������l
// �T�v : �`��
//=============================================================================
void CApplication::Draw()
{
	m_pRenderer->Draw();
}
