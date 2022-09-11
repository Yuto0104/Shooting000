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
#include "move.h"
#include "renderer.h"
#include "application.h"
#include "keyboard.h"
#include "mouse.h"
#include "game.h"

#include "calculation.h"
#include "bullet3D.h"
#include "follow_bullet3D.h"
#include "camera.h"
#include "life_manager.h"
#include "gauge2D.h"
#include "score.h"
#include "energy_gage.h"

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
	m_nLife = 0;									// ����
	m_nInvalidLife = 0;								// �ǉ����C�t
	m_nEnergy = 0;									// �G�l���M�[
	m_bPressShot = false;							// �������e���g�p���Ă邩�ǂ���
	m_bLockShot = false;							// �e���˂��\���ǂ���
	m_bFollowShot = false;							// �Ǐ]�e�𔭎˂��Ă��邩��
	// �I�u�W�F�N�g�̎�ʐݒ�
	SetObjType(CObject::OBJTYPE_3DPLAYER);
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
	m_pMotion[0] = new CMotion("data/MOTION/white.txt");
	assert(m_pMotion[0] != nullptr);
	m_pMotion[1] = new CMotion("data/MOTION/black.txt");
	assert(m_pMotion[1] != nullptr);

	// �ړ�
	m_pMove = new CMove;
	assert(m_pMove != nullptr);

	// �ړ����̏�����
	m_pMove->SetMoving(1.0f, 15.0f, 0.0f, 0.1f);

	// �����o�ϐ��̏�����
	m_nNumMotion = 0;
	m_nNumMotionOld = m_nNumMotion;
	m_bMotion = false;
	m_bMotionBlend = false;
	SetColorType(CModel3D::TYPE_WHITE);
	SetColisonSize(D3DXVECTOR3(50.0f, 50.0f, 50.0f));
	m_nLife = 5;
	m_nInvalidLife = 0;
	SetSize(D3DXVECTOR3(1.2f, 1.2f, 1.2f));

	return E_NOTIMPL;
}

//=============================================================================
// �I��
// Author : �������l
// �T�v : �e�N�X�`���̃|�C���^�ƒ��_�o�b�t�@�̉��
//=============================================================================
void CMotionPlayer3D::Uninit()
{
	if (m_pMove != nullptr)
	{// �I������
		// �������̉��
		delete m_pMove;
		m_pMove = nullptr;
	}

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
	// ���̎擾
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	if (!m_bFollowShot)
	{// �j���[�g�������[�V�����̓���
		m_nNumMotion = TYPE_NEUTRAL;
	}

	// �ړ�
	pos += Move();

	// ��]
	Rotate();

	// �e�̔���
	Shot();

	// �G�l���M�[����
	Consumption();

	// �F�̕ύX
	ChangeColor();

	// ���[�V�����̐ݒ�
	MotionSet();

	// �ړ����̐ݒ�
	SetPos(pos);

	// �X�N���[���̂����蔻��
	CollisionScreen();

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
D3DXVECTOR3 CMotionPlayer3D::Move()
{
	// ���͏��̎擾
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	// �����o�ϐ��̎擾
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (pKeyboard->GetPress(DIK_W)
		|| pKeyboard->GetPress(DIK_A)
		|| pKeyboard->GetPress(DIK_D)
		|| pKeyboard->GetPress(DIK_S))
	{// �ړ��L�[�������ꂽ
		if (!m_bFollowShot)
		{// �ړ����[�V����
			m_nNumMotion = TYPE_MOVE;
		}

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
		move = D3DXVECTOR3(sinf(m_rotDest.y), 0.0f, cosf(m_rotDest.y));

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

	// ���C�W�����l�������ړ�
	m_pMove->Moving(move);

	return m_pMove->GetMove();
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
		pBullet3D->SetParent(CObject::OBJTYPE_3DPLAYER);

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
			pBullet3D->SetParent(CObject::OBJTYPE_3DPLAYER);

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
			pBullet3D->SetParent(CObject::OBJTYPE_3DPLAYER);

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
	// �ʒu�̎擾
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 worldPos;

	// �����蔻��̎擾
	D3DXVECTOR3 collision = CObject::GetColisonSize();

	// �X�N���[�����W�ɃL���X�g
	D3DXVECTOR3 screenPos = CApplication::ScreenCastWorld(pos);
	D3DXVECTOR3 screenPosUp = CApplication::ScreenCastWorld(pos + collision);
	D3DXVECTOR3 screenPosUnder = CApplication::ScreenCastWorld(pos - collision);

	if (screenPosUnder.x < 0.0f)
	{
		worldPos = CApplication::WorldCastScreen(D3DXVECTOR3(collision.x, screenPos.y, screenPos.z));
		SetPos(D3DXVECTOR3(worldPos.x, pos.y, worldPos.z));
		screenPos = CApplication::ScreenCastWorld(GetPos());
	}
	else if (screenPosUp.x > (float)CRenderer::SCREEN_WIDTH)
	{
		worldPos = CApplication::WorldCastScreen(D3DXVECTOR3((float)CRenderer::SCREEN_WIDTH - collision.x, screenPos.y, screenPos.z));
		SetPos(D3DXVECTOR3(worldPos.x, pos.y, worldPos.z));
		screenPos = CApplication::ScreenCastWorld(GetPos());
	}

	if (screenPosUp.y < 0.0f)
	{
		worldPos = CApplication::WorldCastScreen(D3DXVECTOR3(screenPos.x, collision.y, screenPos.z));
		SetPos(D3DXVECTOR3(worldPos.x, pos.y, worldPos.z));
		screenPos = CApplication::ScreenCastWorld(GetPos());
	}
	else if (screenPosUnder.y > (float)CRenderer::SCREEN_HEIGHT)
	{
		worldPos = CApplication::WorldCastScreen(D3DXVECTOR3(screenPos.x, (float)CRenderer::SCREEN_HEIGHT - collision.y, screenPos.z));
		SetPos(D3DXVECTOR3(worldPos.x, pos.y, worldPos.z));
		screenPos = CApplication::ScreenCastWorld(GetPos());
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
		m_bFollowShot = false;
	}
}

//=============================================================================
// ��
// Author : �������l
// �T�v : ���C�t�̉񕜂��s��
//=============================================================================
void CMotionPlayer3D::Recovery()
{
	if (m_nLife < MAX_LIFE
		&& m_nEnergy >= ENERGY_RECOVERY)
	{
		int nRecoveryEnergy = (int)ENERGY_RECOVERY;

		// �G�l���M�[�̏���
		Charge(-nRecoveryEnergy);

		m_nLife++;

		if (m_nLife >= MAX_LIFE)
		{
			m_nLife = MAX_LIFE;
		}

		// ���C�t�̐ݒ�
		CGame::GetLifeManager()->SetLife();

		m_bRecovery = true;
	}
}

//=============================================================================
// �Ǐ]�e�̔���
// Author : �������l
// �T�v : �Ǐ]�e�𔭎˂���
//=============================================================================
void CMotionPlayer3D::FollowShot()
{
	if (m_nEnergy >= ENERGY_FOLLOW_SHOT)
	{
		// �U�����[�V����
		m_nNumMotion = TYPE_ATTACK;
		m_bFollowShot = true;

		int nFollowShotEnergy = (int)ENERGY_FOLLOW_SHOT;

		// �G�l���M�[�̏���
		Charge(-nFollowShotEnergy);

		// Object2D�̃����o�ϐ��̎擾
		D3DXVECTOR3 pos = GetPos();
		D3DXVECTOR3 rot = GetRot();

		// �ϐ��錾
		D3DXVECTOR3 bulletPos;					// �e�̔��ˈʒu
		CFollowBullet3D *pFollowBullet3D;		// �Ǐ]�e�̐���
		D3DXCOLOR bulletColor;					// �e�̐F

		if (GetColorType() == CObject::TYPE_WHITE)
		{// �e�̐F�̐ݒ�
			bulletColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else if (GetColorType() == CObject::TYPE_BLACK)
		{// �e�̐F�̐ݒ�
			bulletColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		}

		// �e�̔��ˈʒu
		bulletPos = D3DXVECTOR3(0.0f, 18.0f, 45.0f);

		// ���[���h���W�ɃL���X�g
		bulletPos = WorldCastVtx(bulletPos, pos, rot);

		// �e�̐���
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			float fAttRot = 0.5f / 4 * nCnt;
			pFollowBullet3D = CFollowBullet3D::Create();
			pFollowBullet3D->SetPos(bulletPos);
			pFollowBullet3D->SetSize(D3DXVECTOR3(10.0f, 10.0f, 0.0f));
			pFollowBullet3D->SetMoveVec(D3DXVECTOR3(rot.x + D3DX_PI * -0.5f, rot.y - D3DX_PI * (0.5f - fAttRot), 0.0f));
			pFollowBullet3D->SetSpeed(30.0f);
			pFollowBullet3D->SetColor(bulletColor);
			pFollowBullet3D->SetColorType(GetColorType());
			pFollowBullet3D->SetCoefficient(0.01f);
			pFollowBullet3D->SetAddCoefficient(0.01f);
			pFollowBullet3D->SetParent(CObject::OBJTYPE_3DPLAYER);

			pFollowBullet3D = CFollowBullet3D::Create();
			pFollowBullet3D->SetPos(bulletPos);
			pFollowBullet3D->SetSize(D3DXVECTOR3(10.0f, 10.0f, 0.0f));
			pFollowBullet3D->SetMoveVec(D3DXVECTOR3(rot.x + D3DX_PI * -0.5f, rot.y - D3DX_PI * (-0.5f + fAttRot), 0.0f));
			pFollowBullet3D->SetSpeed(30.0f);
			pFollowBullet3D->SetColor(bulletColor);
			pFollowBullet3D->SetColorType(GetColorType());
			pFollowBullet3D->SetCoefficient(0.01f);
			pFollowBullet3D->SetAddCoefficient(0.01f);
			pFollowBullet3D->SetParent(CObject::OBJTYPE_3DPLAYER);
		}
	}
}

//=============================================================================
// �G�l���M�[����
// Author : �������l
// �T�v : �G�l���M�[������A�������s��
//=============================================================================
void CMotionPlayer3D::Consumption()
{
	// ���͏��̎擾
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	if (pKeyboard->GetPress(DIK_RETURN)
		&& !m_bPressConsumption)
	{// �e�̔���
		// �J�E���g�̏�����
		m_nCntConsumption = 0;

		// �v���X�I��
		m_bPressConsumption = true;
	}

	if (pKeyboard->GetRelease(DIK_RETURN))
	{// �v���X�I�t
		m_bPressConsumption = false;

		if (m_bRecovery)
		{
			m_bRecovery = false;
		}
		else
		{
			FollowShot();
		}

		m_nCntConsumption = 0;
	}

	if (m_bPressConsumption)
	{// �v���X�I��
		m_nCntConsumption++;

		if (m_nCntConsumption >= MAX_CNT_RECOVERY)
		{// �J�E���g���e���˂܂ł̃J�E���g�ɒB����
			// �J�E���g�̏�����
			m_nCntConsumption = 0;

			// ��
			Recovery();
		}
	}
}

//=============================================================================
// �q�b�g
// Author : �������l
// �T�v : �_���[�W��^����
//=============================================================================
void CMotionPlayer3D::Hit()
{
	if (m_nInvalidLife > 0)
	{// �ǉ����C�t�̏���
		m_nInvalidLife--;
		
		if (m_nInvalidLife <= 0)
		{
			m_nInvalidLife = 0;
		}
	}
	else
	{// ���C�t�̏���
		m_nLife--;

		if (m_nLife <= 0)
		{
			m_nLife = 0;
		}
	}

	// ���C�t�̐ݒ�
	CGame::GetLifeManager()->SetLife();

	if (m_nLife == 0)
	{// �I��
		Uninit();
		CApplication::SetNextMode(CApplication::MODE_RESULT);
	}
}

//=============================================================================
// �`���[�W
// Author : �������l
// �T�v : �G�l���M�[���z������
//=============================================================================
void CMotionPlayer3D::Charge(const int nEnergy)
{
	CEnergyGage *pEnergyGage = CGame::GetEnergyGage();
	CGauge2D *pEnergyGauge2D = pEnergyGage->GetGauge2D();
	CScore *pEnergy = pEnergyGage->GetScore();

	// �G�l���M�[�̃C���N�������g
	int nEnergyPoint = pEnergy->GetScore();
	int nEnergyPointOld = nEnergyPoint;
	int nDiffEP = 0;

	// �G�l���M�[�̉��Z
	nEnergyPoint += nEnergy;

	// ���Z�l�̍����̎Z�o
	nDiffEP = nEnergyPoint - nEnergyPointOld;

	// ���l�̐ݒ�
	pEnergy->AddScore(nDiffEP);

	if (nEnergyPoint >= MAX_ENERGY)
	{
		nDiffEP = MAX_ENERGY - nEnergyPoint;
		nEnergyPoint = MAX_ENERGY;
		pEnergy->AddScore(nDiffEP);
	}
	else if (nEnergyPoint <= 0)
	{
		nDiffEP = 0 - nEnergyPoint;
		nEnergyPoint = 0;
		pEnergy->AddScore(nDiffEP);
	}

	// �Q�[�W�̐ݒ�
	pEnergyGauge2D->SetNumber((float)nEnergyPoint);

	// �G�l���M�[�Q�[�W�̐ݒ�
	m_nEnergy = nEnergyPoint;
}


