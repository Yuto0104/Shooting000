//=============================================================================
//
// 3D�G�l�~�[�N���X(enemy3D.cpp)
// Author : �������l
// �T�v : 3D�G�l�~�[�������s��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <assert.h>
#include <time.h>

#include "enemy3D.h"
#include "renderer.h"
#include "application.h"
#include "score.h"
#include "bullet3D.h"

//*****************************************************************************
// �ÓI�����o�ϐ��̒�`
//*****************************************************************************
const float CEnemy3D::LINTERN_BULLET_SPAWN_RANGE = 30.0f;				// �Ԃ��e�̐����͈�
const float CEnemy3D::LINTERN_BULLET_MOVE_VEC = 0.15f;					// �Ԃ��e�̈ړ�����

//=============================================================================
// �C���X�^���X����
// Author : �������l
// �T�v : 3D�G�l�~�[�𐶐�����
//=============================================================================
CEnemy3D * CEnemy3D::Create(const char *pName)
{
	// �I�u�W�F�N�g�C���X�^���X
	CEnemy3D *pEnemy3D = nullptr;

	// �������̉��
	pEnemy3D = new CEnemy3D;

	// �������̊m�ۂ��ł��Ȃ�����
	assert(pEnemy3D != nullptr);

	// ���l�̏�����
	pEnemy3D->Init(pName);

	// �C���X�^���X��Ԃ�
	return pEnemy3D;
}

//=============================================================================
// �R���X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�������ɍs������
//=============================================================================
CEnemy3D::CEnemy3D()
{// �I�u�W�F�N�g�̎�ʐݒ�
	SetObjType(CObject::OBJTYPE_3DENEMY);

	m_nLife = 0;		// �̗�
	m_nScore = 0;		// �X�R�A
	m_nCntShot = 0;		// �e���˂܂ł̃J�E���g

	// �^�������̏�����
	srand((unsigned int)time(NULL));
}

//=============================================================================
// �f�X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�I�����ɍs������
//=============================================================================
CEnemy3D::~CEnemy3D()
{

}

//=============================================================================
// ������
// Author : �������l
// �T�v : ���_�o�b�t�@�𐶐����A�����o�ϐ��̏����l��ݒ�
//=============================================================================
HRESULT CEnemy3D::Init(const char * pName)
{
	// ���f���̏�����
	CModel3D::Init(pName);

	// �����蔻��̐ݒ�
	SetColisonSize(D3DXVECTOR3(50.0f, 50.0f, 50.0f));

	// �̗�
	m_nLife = 100;

	// �X�R�A
	m_nScore = 100;

	return S_OK;
}

//=============================================================================
// �I��
// Author : �������l
// �T�v : �e�N�X�`���̃|�C���^�ƒ��_�o�b�t�@�̉��
//=============================================================================
void CEnemy3D::Uninit()
{
	// ���f���̏I��
	CModel3D::Uninit();
}

//=============================================================================
// �X�V
// Author : �������l
// �T�v : 2D�X�V���s��
//=============================================================================
void CEnemy3D::Update()
{
	D3DXVECTOR3 rot = GetRot();
	D3DXCOLOR bulletColor;					// �e�̐F

	if (GetColorType() == CObject::TYPE_WHITE)
	{// �e�̐F�̐ݒ�
		bulletColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (GetColorType() == CObject::TYPE_BLACK)
	{// �e�̐F�̐ݒ�
		bulletColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	}

	m_nCntShot++;

	if (m_nCntShot % 180 == 0)
	{
		// �e�̐���
		CBullet3D * pBullet3D = CBullet3D::Create();
		pBullet3D->SetPos(GetPos());
		pBullet3D->SetSize(D3DXVECTOR3(10.0f, 10.0f, 0.0f));
		pBullet3D->SetMoveVec(D3DXVECTOR3(rot.x + D3DX_PI * -0.5f, rot.y, 0.0f));
		pBullet3D->SetSpeed(10.0f);
		pBullet3D->SetColor(bulletColor);
		pBullet3D->SetColorType(GetColorType());
		pBullet3D->SetParent(CObject::OBJTYPE_3DENEMY);
	}

	// ���f���̍X�V
	CModel3D::Update();
}

//=============================================================================
// �`��
// Author : �������l
// �T�v : 2D�`����s��
//=============================================================================
void CEnemy3D::Draw()
{
	// ���f���̕`��
	CModel3D::Draw();
}

//=============================================================================
// �q�b�g
// Author : �������l
// �T�v : �������̃_���[�W��^����
//=============================================================================
void CEnemy3D::Hit(COLOR_TYPE colorType, int nAttack)
{// �ϐ��錾
	COLOR_TYPE MyColorType = GetColorType();
	int nMyAttack = nAttack;

	if (MyColorType == colorType)
	{// �F�̃^�C�v������̏ꍇ
		nMyAttack *= 2;
	}

	// �̗͂̌��Z
	m_nLife -= nMyAttack;

	if (m_nLife <= 0)
	{// �̗͂�0�̏ꍇ
		m_nLife = 0;
		CScore *pScore = CApplication::GetScore();
		pScore->AddScore(m_nScore);

		// �f�[�^�i�[�p�ϐ�
		CBullet3D * pBullet3D;
		D3DXVECTOR3 pos = GetPos();
		D3DXVECTOR3 rot = GetRot();
		D3DXVECTOR3 randLinternBulletSpawnRange = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		float fRandMoveVec = 0.0f;

		// �e�̐F
		D3DXCOLOR bulletColor;

		if (MyColorType == CObject::TYPE_WHITE)
		{// �e�̐F�̐ݒ�
			bulletColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else if (MyColorType == CObject::TYPE_BLACK)
		{// �e�̐F�̐ݒ�
			bulletColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		}

		for (int nCntBullet = 0; nCntBullet < MAX_LINTERN_BULLET; nCntBullet++)
		{// �����̐ݒ�
			randLinternBulletSpawnRange.x = pos.x - LINTERN_BULLET_SPAWN_RANGE + ((float)(rand() % (int)(LINTERN_BULLET_SPAWN_RANGE * 200.0f) / 100));
			randLinternBulletSpawnRange.z = pos.z - LINTERN_BULLET_SPAWN_RANGE + ((float)(rand() % (int)(LINTERN_BULLET_SPAWN_RANGE * 200.0f) / 100));
			fRandMoveVec = rot.y - D3DX_PI * LINTERN_BULLET_MOVE_VEC + D3DX_PI * ((float)(rand() % (int)(LINTERN_BULLET_MOVE_VEC * 200.0f)) / 100);

			// �e�̐���
			pBullet3D = CBullet3D::Create();
			pBullet3D->SetPos(D3DXVECTOR3(randLinternBulletSpawnRange.x, pos.y, randLinternBulletSpawnRange.z));
			pBullet3D->SetSize(D3DXVECTOR3(5.0f, 5.0f, 0.0f));
			pBullet3D->SetMoveVec(D3DXVECTOR3(rot.x + D3DX_PI * -0.5f, fRandMoveVec, 0.0f));
			pBullet3D->SetSpeed(5.0f);
			pBullet3D->SetColor(bulletColor);
			pBullet3D->SetColorType(MyColorType);
			pBullet3D->SetParent(CObject::OBJTYPE_3DENEMY);
		}

		Uninit();
	}
}
