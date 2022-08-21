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
#include "enemy3D.h"
#include "motion_char3D.h"
#include "motion_player3D.h"
#include "mesh.h"
#include "sphere.h"
#include "circle_polygon3D.h"
#include "number.h"
#include "score.h"
#include "life.h"
#include "life_manager.h"
#include "gauge2D.h"
#include "energy_gage.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
CRenderer *CApplication::m_pRenderer = nullptr;					// �����_���[�C���X�^���X
CKeyboard *CApplication::m_pKeyboard = {};						// �L�[�{�[�h�C���X�^���X
CMouse *CApplication::m_pMouse = {};							// �}�E�X�C���X�^���X
CTexture *CApplication::m_pTexture = nullptr;					// �e�N�X�`���C���X�^���X
CSound *CApplication::m_pSound = nullptr;						// �T�E���h�C���X�^���X
CCamera *CApplication::m_pCamera = nullptr;						// �J�����C���X�^���X
CCamera *CApplication::m_pCameraBG = nullptr;					// �J�����C���X�^���X
CPlayer2D *CApplication::m_pPlayer2D = nullptr;					// �v���C���[�C���^���X
CMotionPlayer3D *CApplication::m_MotionPlayer3D = nullptr;		// ���[�V�����v���C���[�C���X�^���X
CScore *CApplication::m_pScore = nullptr;						// �X�R�A�C���X�^���X
CLifeManager *CApplication::m_pLifeManager = nullptr;			// ���C�t�}�l�[�W���[�C���X�^���X
CGauge2D *CApplication::m_pGauge2D = nullptr;					// �Q�[�W�}�l�[�W���[
CEnergyGage *CApplication::m_pEnergyGage = nullptr;				// �G�l���M�[�Q�[�W�}�l�[�W���[

//=============================================================================
// �X�N���[�����W�����[���h���W�ɃL���X�g����
// Author : �������l
// �T�v : 
//=============================================================================
D3DXVECTOR3 CApplication::ScreenCastWorld(const D3DXVECTOR3 &pos)
{
	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtx, mtxTrans, mtxView, mtxPrj, mtxViewPort;

	// �s��ړ��֐� (��������X,Y,Z�����̈ړ��s����쐬)
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);

	// �J�����̃r���[�}�g���b�N�X�̎擾
	mtxView = m_pCamera->GetMtxView();

	// �J�����̃v���W�F�N�V�����}�g���b�N�X�̎擾
	mtxPrj = m_pCamera->GetMtxProj();

	// �}�g���b�N�X�̏�Z
	mtx = mtxTrans * mtxView * mtxPrj;

	// �r���[�|�[�g�s��i�X�N���[���s��j�̍쐬
	float w = (float)CRenderer::SCREEN_WIDTH / 2.0f;
	float h = (float)CRenderer::SCREEN_HEIGHT / 2.0f;

	mtxViewPort = {
		w , 0 , 0 , 0 ,
		0 ,-h , 0 , 0 ,
		0 , 0 , 1 , 0 ,
		w , h , 0 , 1
	};

	// �}�g���b�N�X��XYZ
	D3DXVECTOR3 vec = D3DXVECTOR3(mtx._41, mtx._42, mtx._43);

	D3DXVec3TransformCoord(&vec, &vec, &mtxViewPort);

	return vec;
}

//=============================================================================
// ���[���h���W���X�N���[�����W�ɃL���X�g����
// Author : �������l
// �T�v : 
//=============================================================================
D3DXVECTOR3 CApplication::WorldCastScreen(const D3DXVECTOR3 &pos)
{
	// �v�Z�p�x�N�g��
	D3DXVECTOR3 vec = pos;

	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtx, mtxTrans, mtxView, mtxPrj, mtxViewPort;

	// �s��ړ��֐� (��������X,Y,Z�����̈ړ��s����쐬)
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);

	// �J�����̃r���[�}�g���b�N�X�̎擾
	mtxView = m_pCamera->GetMtxView();

	// �J�����̃v���W�F�N�V�����}�g���b�N�X�̎擾
	mtxPrj = m_pCamera->GetMtxProj();
	
	// �r���[�|�[�g�s��i�X�N���[���s��j�̍쐬
	D3DXMatrixIdentity(&mtxViewPort);
	float w = (float)CRenderer::SCREEN_WIDTH / 2.0f;
	float h = (float)CRenderer::SCREEN_HEIGHT / 2.0f;
	mtxViewPort = {
		w , 0 , 0 , 0 ,
		0 ,-h , 0 , 0 ,
		0 , 0 , 1 , 0 ,
		w , h , 0 , 1
	};

	// �t�s��̎Z�o
	D3DXMatrixInverse(&mtxView, NULL, &mtxView);
	D3DXMatrixInverse(&mtxPrj, NULL, &mtxPrj);
	D3DXMatrixInverse(&mtxViewPort, NULL, &mtxViewPort);

	// �t�ϊ�
	mtx = mtxViewPort * mtxPrj * mtxView;
	D3DXVec3TransformCoord(&vec, &D3DXVECTOR3(vec.x, vec.y, vec.z), &mtx);

	return vec;
}

//=============================================================================
// �p�x�̐��K������
// Author : �������l
// �T�v : �p�x���~������2�{�𒴂����Ƃ��ɔ͈͓��ɖ߂�
//=============================================================================
float CApplication::RotNormalization(float fRot)
{
	if (fRot >= D3DX_PI)
	{// �ړ������̐��K��
		fRot -= D3DX_PI * 2;
	}
	else if (fRot <= -D3DX_PI)
	{// �ړ������̐��K��
		fRot += D3DX_PI * 2;
	}

	return fRot;
}

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
	m_pCameraBG = new CCamera;
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
	m_pCamera->SetViewType(CCamera::TYPE_PARALLEL);

	// ����������
	assert(m_pCameraBG != nullptr);
	m_pCameraBG->Init();

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

	/*CObject3D *pObject3D = CObject3D::Create();
	pObject3D->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pObject3D->SetSize(D3DXVECTOR3(100.0f, 100.0f, 0.0f));

	pObject3D = CObject3D::Create();
	pObject3D->SetPos(D3DXVECTOR3(-500.0f, 0.0f, 0.0f));
	pObject3D->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
	pObject3D->SetSize(D3DXVECTOR3(100.0f, 100.0f, 0.0f));
	pObject3D->SetBillboard(true);*/

	/*CModel3D *pModel3D = CModel3D::Create("data/MODEL/airplane000.x");
	pModel3D->SetPos(D3DXVECTOR3(100.0f, 0.0f, 0.0f));
	pModel3D->SetSize(D3DXVECTOR3(5.0f, 5.0f, 5.0f));*/

	CEnemy3D *pEnemy = CEnemy3D::Create("data/MODEL/enemy_white_000.x");
	pEnemy->SetPos(D3DXVECTOR3(-200.0f, 0.0f, 200.0f));
	pEnemy->SetSize(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	pEnemy->SetColorType(CObject::TYPE_WHITE);

	pEnemy = CEnemy3D::Create("data/MODEL/enemy_white_000.x");
	pEnemy->SetPos(D3DXVECTOR3(-150.0f, 0.0f, 200.0f));
	pEnemy->SetSize(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	pEnemy->SetColorType(CObject::TYPE_WHITE);

	pEnemy = CEnemy3D::Create("data/MODEL/enemy_white_000.x");
	pEnemy->SetPos(D3DXVECTOR3(-100.0f, 0.0f, 200.0f));
	pEnemy->SetSize(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	pEnemy->SetColorType(CObject::TYPE_WHITE);

	pEnemy = CEnemy3D::Create("data/MODEL/enemy_black_000.x");
	pEnemy->SetPos(D3DXVECTOR3(200.0f, 0.0f, 200.0f));
	pEnemy->SetSize(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	pEnemy->SetColorType(CObject::TYPE_BLACK);

	pEnemy = CEnemy3D::Create("data/MODEL/enemy_black_000.x");
	pEnemy->SetPos(D3DXVECTOR3(100.0f, 0.0f, 200.0f));
	pEnemy->SetSize(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	pEnemy->SetColorType(CObject::TYPE_BLACK);

	pEnemy = CEnemy3D::Create("data/MODEL/enemy_black_000.x");
	pEnemy->SetPos(D3DXVECTOR3(150.0f, 0.0f, 200.0f));
	pEnemy->SetSize(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	pEnemy->SetColorType(CObject::TYPE_BLACK);

	CMotionChar3D *pMotionChar3D = CMotionChar3D::Create("data/MOTION/motion.txt");
	pMotionChar3D->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pMotionChar3D->SetObjectDrowType(CObject::DROWTYPE_BG);

	m_MotionPlayer3D = CMotionPlayer3D::Create();
	m_MotionPlayer3D->SetPos(D3DXVECTOR3(40.0f, 0.0f, -50.0f));
	m_MotionPlayer3D->SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));

	CMesh3D *pMesh3D = CMesh3D::Create();
	pMesh3D->SetSize(D3DXVECTOR3(2000.0f, 0, 2000.0f));
	pMesh3D->SetBlock(CMesh3D::DOUBLE_INT(10, 10));
	pMesh3D->SetSplitTex(true);
	pMesh3D->SetObjectDrowType(CObject::DROWTYPE_BG);

	CSphere *pSphere = CSphere::Create();
	pSphere->SetRot(D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f));
	pSphere->SetSize(D3DXVECTOR3(100.0f, 0, 100.0f));
	pSphere->SetBlock(CMesh3D::DOUBLE_INT(100, 100));
	//pSphere->SetSplitTex(true);
	pSphere->SetRadius(1000.0f);
	pSphere->SetSphereRange(D3DXVECTOR2(D3DX_PI * 2.0f, D3DX_PI * -0.35f));
	pSphere->SetObjectDrowType(CObject::DROWTYPE_BG);

	/*CCirclePolygon3D *pCirclePolygon3D = CCirclePolygon3D::Create();
	pCirclePolygon3D->SetPos(D3DXVECTOR3(0.0f, 10.0f, 0.0f));
	pCirclePolygon3D->SetObjectDrowType(CObject::DROWTYPE_BG);*/

	m_pScore = CScore::Create(10, true);
	m_pScore->SetScore(0);
	m_pScore->SetPos(D3DXVECTOR3(1280.0f, m_pScore->GetSize().y / 2.0f, 0.0f));

	m_pLifeManager = CLifeManager::Create();
	m_pLifeManager->SetSize(D3DXVECTOR3(50.0f, 50.0f, 0.0f));
	m_pLifeManager->SetPos(D3DXVECTOR3(0.0f, m_pLifeManager->GetSize().y / 2.0f, 0.0f));

	m_pGauge2D = CGauge2D::Create();
	m_pGauge2D->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pGauge2D->SetSize(D3DXVECTOR3(50.0f, 600.0f, 0.0f));
	m_pGauge2D->SetPos(D3DXVECTOR3(50.0f, 700, 0.0f));
	m_pGauge2D->SetCol(D3DXCOLOR(0.2f, 0.9f, 1.0f, 1.0f));
	m_pGauge2D->SetMaxNumber((float)CMotionPlayer3D::MAX_ENERGY);
	m_pGauge2D->SetCoefficient(0.06f);

	m_pEnergyGage = CEnergyGage::Create();

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

	if (m_pCameraBG != nullptr)
	{// �I������
		m_pCameraBG->Uninit();

		// �������̉��
		delete m_pCameraBG;
		m_pCameraBG = nullptr;
	}

	if (m_pEnergyGage != nullptr)
	{// �I������
		m_pEnergyGage->Uninit();

		// �������̉��
		delete m_pEnergyGage;
		m_pEnergyGage = nullptr;
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
	m_pCameraBG->Update();
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
