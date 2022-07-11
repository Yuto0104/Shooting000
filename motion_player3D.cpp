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
#include "renderer.h"
#include "application.h"
#include "keyboard.h"

#include "bullet3D.h"

//*****************************************************************************
// �萔��`
//*****************************************************************************
const float CMotionPlayer3D::ROTATE_SPEED = (0.1f);

//=============================================================================
// �C���X�^���X����
// Author : �������l
// �T�v : 3D���[�V�����v���C���[�𐶐�����
//=============================================================================
CMotionPlayer3D * CMotionPlayer3D::Create(char * pName)
{
	// �I�u�W�F�N�g�C���X�^���X
	CMotionPlayer3D *pMotionPlayer3D = nullptr;

	// �������̉��
	pMotionPlayer3D = new CMotionPlayer3D;

	// �������̊m�ۂ��ł��Ȃ�����
	assert(pMotionPlayer3D != nullptr);

	// ���l�̏�����
	pMotionPlayer3D->Init(pName);

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
	m_motionType = TYPE_NEUTRAL;					// ���[�V�����^�C�v
	m_nCntShot = 0;									// �e���˂܂ł̃J�E���g
	m_bPressShot = false;							// �������e���g�p���Ă邩�ǂ���
	m_bLockShot = false;							// �e���˂��\���ǂ���
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
HRESULT CMotionPlayer3D::Init(const char *pMotionName)
{
	// ������
	CMotionChar3D::Init(pMotionName);

	return E_NOTIMPL;
}

//=============================================================================
// �I��
// Author : �������l
// �T�v : �e�N�X�`���̃|�C���^�ƒ��_�o�b�t�@�̉��
//=============================================================================
void CMotionPlayer3D::Uninit()
{
	// �I��
	CMotionChar3D::Uninit();
}

//=============================================================================
// �X�V
// Author : �������l
// �T�v : �X�V���s��
//=============================================================================
void CMotionPlayer3D::Update()
{
	// �j���[�g�������[�V�����̓���
	m_motionType = TYPE_NEUTRAL;

	// �ړ�
	Move();

	// ��]
	Rotate();

	// �e�̔���
	Shot();

	// ���[�V�����ԍ��̐ݒ�
	SetNumMotion(m_motionType);

	// �X�V
	CMotionChar3D::Update();
}

//=============================================================================
// �`��
// Author : �������l
// �T�v : �`����s��
//=============================================================================
void CMotionPlayer3D::Draw()
{
	// �`��
	CMotionChar3D::Draw();
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
	SetRot(rot);
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
		m_motionType = TYPE_MOVE;

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
		pos += move * 1.0f;

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
void CMotionPlayer3D::Shot(void)
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

			bulletPos = D3DXVECTOR3(-20.0f, 18.0f, -45.0f);

			// ���[���h���W�ɃL���X�g
			bulletPos = WorldCastVtx(bulletPos, pos, rot);

			// �e�̐���
			pBullet3D = CBullet3D::Create();
			pBullet3D->SetPos(bulletPos);
			pBullet3D->SetSize(D3DXVECTOR3(10.0f, 10.0f, 0.0f));
			pBullet3D->SetMoveVec(D3DXVECTOR3(rot.x + D3DX_PI * -0.5f, rot.y, 0.0f));
			pBullet3D->SetSpeed(10.0f);

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


