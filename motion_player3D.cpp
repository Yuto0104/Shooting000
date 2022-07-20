//=============================================================================
//
// 3D���[�V�����v���C���[�N���X(model3D.h)
// Author : �������l
// �T�v : 3D���[�V�����v���C���[�������s��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <assert.h>

#include "motion_player3D.h"
#include "motion.h"
#include "renderer.h"
#include "application.h"
#include "keyboard.h"
#include "mouse.h"

#include "calculation.h"
#include "bullet3D.h"
#include "camera.h"

//*****************************************************************************
// �萔��`
//*****************************************************************************
const float CMotionPlayer3D::ROTATE_SPEED = (0.1f);

//=============================================================================
// �C���X�^���X����
// Author : �������l
// �T�v : 3D���[�V�����v���C���[�𐶐�����
//=============================================================================
CMotionPlayer3D * CMotionPlayer3D::Create()
{
	// �I�u�W�F�N�g�C���X�^���X
	CMotionPlayer3D *pMotionPlayer3D = nullptr;

	// �������̉��
	pMotionPlayer3D = new CMotionPlayer3D;

	// �������̊m�ۂ��ł��Ȃ�����
	assert(pMotionPlayer3D != nullptr);

	// ���l�̏�����
	pMotionPlayer3D->Init();

	// �C���X�^���X��Ԃ�
	return pMotionPlayer3D;
}

//=============================================================================
// �R���X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�������ɍs������
//=============================================================================
CMotionPlayer3D::CMotionPlayer3D()
{
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړI�̌���
	m_nNumMotion = TYPE_NEUTRAL;					// ���[�V�����^�C�v
	m_nCntShot = 0;									// �e���˂܂ł̃J�E���g
	m_bPressShot = false;							// �������e���g�p���Ă邩�ǂ���
	m_bLockShot = false;							// �e���˂��\���ǂ���

	// �I�u�W�F�N�g�̎�ʐݒ�
	SetObjType(CObject::OBJTYPE_3DENEMY);
}

//=============================================================================
// �f�X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�I�����ɍs������
//=============================================================================
CMotionPlayer3D::~CMotionPlayer3D()
{

}

//=============================================================================
// ������
// Author : �������l
// �T�v : ���_�o�b�t�@�𐶐����A�����o�ϐ��̏����l��ݒ�
//=============================================================================
HRESULT CMotionPlayer3D::Init()
{
	// ������
	CModel3D::Init();

	// ���[�V�������
	m_pMotion[0] = new CMotion("data/MOTION/motion.txt");
	assert(m_pMotion[0] != nullptr);
	m_pMotion[1] = new CMotion("data/MOTION/motionShark.txt");
	assert(m_pMotion[1] != nullptr);

	// �����o�ϐ��̏�����
	m_nNumMotion = 0;
	m_nNumMotionOld = m_nNumMotion;
	m_bMotion = false;
	m_bMotionBlend = false;
	SetColorType(CModel3D::TYPE_WHITE);
	SetColisonSize(D3DXVECTOR3(50.0f, 50.0f, 50.0f));

	return E_NOTIMPL;
}

//=============================================================================
// �I��
// Author : �������l
// �T�v : �e�N�X�`���̃|�C���^�ƒ��_�o�b�t�@�̉��
//=============================================================================
void CMotionPlayer3D::Uninit()
{
	for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
	{
		if (m_pMotion[nCntMotion] != nullptr)
		{// �I������
			m_pMotion[nCntMotion]->Uninit();

			// �������̉��
			delete m_pMotion[nCntMotion];
			m_pMotion[nCntMotion] = nullptr;
		}
	}	

	// �I��
	CModel3D::Uninit();
}

//=============================================================================
// �X�V
// Author : �������l
// �T�v : �X�V���s��
//=============================================================================
void CMotionPlayer3D::Update()
{
	// �j���[�g�������[�V�����̓���
	m_nNumMotion = TYPE_NEUTRAL;

	// �ړ�
	Move();

	// ��]
	Rotate();

	// �e�̔���
	Shot();

	// �X�N���[���̂����蔻��
	CollisionScreen();

	// �F�̕ύX
	ChangeColor();

	// ���[�V�����̐ݒ�
	MotionSet();

	// �X�V
	CModel3D::Update();
}

//=============================================================================
// �`��
// Author : �������l
// �T�v : �`����s��
//=============================================================================
void CMotionPlayer3D::Draw()
{
	// �`��
	CModel3D::Draw();

	// ���[���h�}�g���b�N�X�̎擾
	D3DXMATRIX mtxWorld = GetMtxWorld();

	// �p�[�c�̕`��ݒ�
	m_pMotion[GetColorType() - 1]->SetParts(mtxWorld);
}

//=============================================================================
// ��]
// Author : �������l
// �T�v : ��]���s��
//=============================================================================
void CMotionPlayer3D::Rotate()
{
	// �����̎擾
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	// �����̍X�V
	rot.y += (m_rotDest.y - rot.y) * ROTATE_SPEED;

	if (rot.y >= D3DX_PI)
	{// �ړ������̐��K��
		rot.y -= D3DX_PI * 2;
	}
	else if (rot.y <= -D3DX_PI)
	{// �ړ������̐��K��
		rot.y += D3DX_PI * 2;
	}

	// �����̐ݒ�
	//SetRot(rot);
}

//=============================================================================
// �ړ�
// Author : �������l
// �T�v : �ړ����s��
//=============================================================================
void CMotionPlayer3D::Move()
{
	// ���͏��̎擾
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	// �����o�ϐ��̎擾
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	if (pKeyboard->GetPress(DIK_W)
		|| pKeyboard->GetPress(DIK_A)
		|| pKeyboard->GetPress(DIK_D)
		|| pKeyboard->GetPress(DIK_S))
	{// �ړ��L�[�������ꂽ
		// �ړ����[�V����
		m_nNumMotion = TYPE_MOVE;

		if (pKeyboard->GetPress(DIK_W))
		{// [��]�L�[�������ꂽ��
			if (pKeyboard->GetPress(DIK_A))
			{// [��]�L�[�������ꂽ��
			 // �ړ������̍X�V
				m_rotDest.y = D3DX_PI * -0.25f;
			}
			else if (pKeyboard->GetPress(DIK_D))
			{// [��]�L�[�������ꂽ��
			 // �ړ������̍X�V
				m_rotDest.y = D3DX_PI * 0.25f;
			}
			else
			{// �ړ������̍X�V
				m_rotDest.y = D3DX_PI * 0.0f;
			}
		}
		else if (pKeyboard->GetPress(DIK_S))
		{// [��]�L�[�������ꂽ��
			if (pKeyboard->GetPress(DIK_A))
			{// [��]�L�[�������ꂽ��
			 // �ړ������̍X�V
				m_rotDest.y = D3DX_PI * -0.75f;
			}
			else if (pKeyboard->GetPress(DIK_D))
			{// [��]�L�[�������ꂽ��
			 // �ړ������̍X�V
				m_rotDest.y = D3DX_PI * 0.75f;
			}
			else
			{// �ړ������̍X�Vq
				m_rotDest.y = D3DX_PI;
			}
		}
		else if (pKeyboard->GetPress(DIK_A))
		{// [��]�L�[�������ꂽ��
		 // �ړ������̍X�V
			m_rotDest.y = D3DX_PI * -0.5f;
		}
		else if (pKeyboard->GetPress(DIK_D))
		{// [��]�L�[�������ꂽ��
		 // �ړ������̍X�V
			m_rotDest.y = D3DX_PI * 0.5f;
		}

		if (m_rotDest.y > D3DX_PI)
		{// �ړ������̐��K��
			m_rotDest.y -= D3DX_PI * 2;
		}
		else if (m_rotDest.y < -D3DX_PI)
		{// �ړ������̐��K��
			m_rotDest.y += D3DX_PI * 2;
		}

		// �ړ��ʂ̌v�Z
		D3DXVECTOR3 move = D3DXVECTOR3(sinf(m_rotDest.y), 0.0f, cosf(m_rotDest.y));

		// �ړ�
		pos += move * 10.0f;

		// �ړI�̌����̔��]
		m_rotDest.y -= D3DX_PI;
	}

	// �ړI�̌����̕␳
	if (m_rotDest.y - rot.y >= D3DX_PI)
	{// �ړ������̐��K��
		m_rotDest.y -= D3DX_PI * 2;
	}
	else if (m_rotDest.y - rot.y <= -D3DX_PI)
	{// �ړ������̐��K��
		m_rotDest.y += D3DX_PI * 2;
	}

	// �ړ����̐ݒ�
	SetPos(pos);
}

//=============================================================================
// �e�̔���
// Author : �������l
// �T�v : �L�[���͂��s��ꂽ�ꍇ�A�e�𔭎˂���
//=============================================================================
void CMotionPlayer3D::Shot()
{
	// ���͏��̎擾
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	// Object2D�̃����o�ϐ��̎擾
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 size = GetSize();

	// �ϐ��錾
	D3DXVECTOR3 bulletPos;		// �e�̔��ˈʒu
	CBullet3D *pBullet3D;		// 3D�e�̐���
	D3DXCOLOR bulletColor;		// �e�̐F

	if (GetColorType() == CObject::TYPE_WHITE)
	{// �e�̐F�̐ݒ�
		bulletColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (GetColorType() == CObject::TYPE_BLACK)
	{// �e�̐F�̐ݒ�
		bulletColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	}

	if (pKeyboard->GetPress(DIK_SPACE)
		&& !m_bLockShot)
	{// �e�̔���
		// �e�̔��ˈʒu
		bulletPos = D3DXVECTOR3(0.0f, 18.0f, -45.0f);

		// ���[���h���W�ɃL���X�g
		bulletPos = WorldCastVtx(bulletPos, pos, rot);

		// �e�̐���
		pBullet3D = CBullet3D::Create();
		pBullet3D->SetPos(bulletPos);
		pBullet3D->SetSize(D3DXVECTOR3(10.0f, 10.0f, 0.0f));
		pBullet3D->SetMoveVec(D3DXVECTOR3(rot.x + D3DX_PI * -0.5f, rot.y, 0.0f));
		pBullet3D->SetSpeed(10.0f);
		pBullet3D->SetColor(bulletColor);
		pBullet3D->SetColorType(GetColorType());

		// �J�E���g�̏�����
		m_nCntShot = 0;

		// �v���X�I��
		m_bPressShot = true;

		// �e�����˂��ł��邩�ǂ���
		m_bLockShot = true;
	}

	if (pKeyboard->GetRelease(DIK_SPACE))
	{// �v���X�I�t
		m_bPressShot = false;
	}

	if (m_bPressShot)
	{// �v���X�I��
		m_nCntShot++;

		if (m_nCntShot >= MAX_CNT_SHOT)
		{// �J�E���g���e���˂܂ł̃J�E���g�ɒB����
			// �e�̔��ˈʒu
			bulletPos = D3DXVECTOR3(20.0f, 18.0f, -45.0f);

			// ���[���h���W�ɃL���X�g
			bulletPos = WorldCastVtx(bulletPos, pos, rot);

			// �e�̐���
			pBullet3D = CBullet3D::Create();
			pBullet3D->SetPos(bulletPos);
			pBullet3D->SetSize(D3DXVECTOR3(10.0f, 10.0f, 0.0f));
			pBullet3D->SetMoveVec(D3DXVECTOR3(rot.x + D3DX_PI * -0.5f, rot.y, 0.0f));
			pBullet3D->SetSpeed(10.0f);
			pBullet3D->SetColor(bulletColor);
			pBullet3D->SetColorType(GetColorType());

			// �e�̍��W�̎Z�o
			bulletPos = D3DXVECTOR3(-20.0f, 18.0f, -45.0f);

			// ���[���h���W�ɃL���X�g
			bulletPos = WorldCastVtx(bulletPos, pos, rot);

			// �e�̐���
			pBullet3D = CBullet3D::Create();
			pBullet3D->SetPos(bulletPos);
			pBullet3D->SetSize(D3DXVECTOR3(10.0f, 10.0f, 0.0f));
			pBullet3D->SetMoveVec(D3DXVECTOR3(rot.x + D3DX_PI * -0.5f, rot.y, 0.0f));
			pBullet3D->SetSpeed(10.0f);
			pBullet3D->SetColor(bulletColor);
			pBullet3D->SetColorType(GetColorType());

			// �J�E���g�̏�����
			m_nCntShot = 0;
		}
	}
	else if (m_bLockShot)
	{// ���b�N������Ă�ꍇ
		m_nCntShot++;

		if (m_nCntShot >= MAX_CNT_SHOT)
		{// �J�E���g�̏�����
			m_nCntShot = 0;
			m_bLockShot = false;
		}
	}
}

//=============================================================================
// �X�N���[���̂����蔻��
// Author : �������l
// �T�v : �X�N���[���ƃv���C���[�̂����蔻����s��
//=============================================================================
void CMotionPlayer3D::CollisionScreen()
{
	// �J�����̎擾
	CCamera *pCamera = CApplication::GetCamera();

	// �ʒu�̎擾
	D3DXVECTOR3 pos = GetPos();

	// �X�N���[���̍��W���擾
	D3DXVECTOR3 minScreen = WorldCastScreen(&D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1280.0f, 720.0f, 0.0f), &pCamera->GetMtxView(), &pCamera->GetMtxProj());
	D3DXVECTOR3 maxScreen = WorldCastScreen(&D3DXVECTOR3(1280.0f, 720.0f, 0.0f), D3DXVECTOR3(1280.0f, 720.0f, 0.0f), &pCamera->GetMtxView(), &pCamera->GetMtxProj());

	if (pos.x < minScreen.x)
	{
		pos.x = minScreen.x;
		SetPos(pos);
	}
	else if (pos.x > maxScreen.x)
	{
		pos.x = maxScreen.x;
		SetPos(pos);
	}
}

//=============================================================================
// �F�̕ύX
// Author : �������l
// �T�v : �L�[���������ƐF��ύX����
//=============================================================================
void CMotionPlayer3D::ChangeColor()
{
	// ���͏��̎擾
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	if (pKeyboard->GetTrigger(DIK_Q))
	{
		if (GetColorType() == CObject::TYPE_WHITE)
		{
			SetColorType(CObject::TYPE_BLACK);
		}
		else if (GetColorType() == CObject::TYPE_BLACK)
		{
			SetColorType(CObject::TYPE_WHITE);
		}
	}
}

//=============================================================================
// ���[�V�����̐ݒ�
// Author : �������l
// �T�v : ���[�V�����̕ύX�⃂�[�V�����u�����h�̐ݒ���s��
//=============================================================================
void CMotionPlayer3D::MotionSet()
{
	// ���݂̃��[�V�����ԍ��̕ۊ�
	m_nNumMotionOld = m_nNumMotion;

	if (!m_bMotion)
	{// �g�p���Ă郂�[�V�������Ȃ��ꍇ
		m_nNumMotion = TYPE_NEUTRAL;
	}

	if (m_nNumMotionOld != m_nNumMotion)
	{// ���[�V�����^�C�v���ύX���ꂽ��
		m_pMotion[GetColorType() - 1]->CntReset((int)(m_nNumMotionOld));
		m_bMotionBlend = true;
	}

	if (m_bMotionBlend)
	{// ���[�V�����u�����h���g�p���Ă�
		m_bMotionBlend = m_pMotion[GetColorType() - 1]->MotionBlend((int)(m_nNumMotion));
	}
	else if (!m_bMotionBlend)
	{// ���[�V�����u�����h���g�p���ĂȂ��ꍇ
		m_bMotion = m_pMotion[GetColorType() - 1]->PlayMotion((int)(m_nNumMotion));
	}
}


