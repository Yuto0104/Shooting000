//=============================================================================
//
// �I�u�W�F�N�g�N���X(object.h)
// Author : �������l
// �T�v : �I�u�W�F�N�g�������s��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <assert.h>

#include "bullet3D.h"
#include "enemy3D.h"
#include "renderer.h"
#include "application.h"

#include "score.h"

//=============================================================================
// �C���X�^���X����
// Author : �������l
// �T�v : 3D�o���b�g�𐶐�����
//=============================================================================
CBullet3D * CBullet3D::Create(void)
{
	// �I�u�W�F�N�g�C���X�^���X
	CBullet3D *pBullet3D = nullptr;

	// �������̉��
	pBullet3D = new CBullet3D;

	// �������̊m�ۂ��ł��Ȃ�����
	assert(pBullet3D != nullptr);

	// ���l�̏�����
	pBullet3D->Init();

	// �C���X�^���X��Ԃ�
	return pBullet3D;
}

//=============================================================================
// �R���X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�������ɍs������
//=============================================================================
CBullet3D::CBullet3D()
{
	m_move = D3DXVECTOR3(0.0f,0.0f,0.0f);					// �ړ���
	m_moveVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �ړ�����
	m_fSpeed = 0.0f;										// ���x
	m_nAttack = 0;											// �U����

	// �I�u�W�F�N�g�̎�ʐݒ�
	SetObjType(CObject::OBJTYPE_3DBULLET);
}

//=============================================================================
// �f�X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�I�����ɍs������
//=============================================================================
CBullet3D::~CBullet3D()
{

}

//=============================================================================
// ������
// Author : �������l
// �T�v : ���_�o�b�t�@�𐶐����A�����o�ϐ��̏����l��ݒ�
//=============================================================================
HRESULT CBullet3D::Init()
{
	// ������
	CObject3D::Init();

	// �e�N�X�`���̐ݒ�
	LoadTex(CTexture::TYPE_2DPOLYGON);

	// �r���{�[�h�I��
	SetBillboard(true);

	// �U��
	m_nAttack = 20;

	return S_OK;
}

//=============================================================================
// �I��
// Author : �������l
// �T�v : �e�N�X�`���̃|�C���^�ƒ��_�o�b�t�@�̉��
//=============================================================================
void CBullet3D::Uninit()
{
	// �I��
	CObject3D::Uninit();
}

//=============================================================================
// �X�V
// Author : �������l
// �T�v : 2D�X�V���s��
//=============================================================================
void CBullet3D::Update()
{
	// �ʒu�̎擾
	D3DXVECTOR3 pos = GetPos();

	// �ړ��ʂ̎Z�o
	m_move.z = sinf(m_moveVec.x) * cosf(m_moveVec.y) * m_fSpeed;
	m_move.x = sinf(m_moveVec.x) * sinf(m_moveVec.y) * m_fSpeed;
	m_move.y = cosf(m_moveVec.x) * m_fSpeed;

	// �ʒu�̍X�V
	pos += m_move;

	// �ʒu�̍X�V
	SetPos(pos);

	// �X�V
	CObject3D::Update();

	// �I�u�W�F�N�g�C���X�^���X�̎擾
	CObject **apObject = CObject::GetObjectAll();

	for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	{
		if (apObject[nCntObj] != nullptr)
		{
			if ((apObject[nCntObj]->GetObjType() == CObject::OBJTYPE_3DENEMY
				|| apObject[nCntObj]->GetObjType() == CObject::OBJTYPE_3DPLAYER)
				&& apObject[nCntObj]->GetObjType() != CObject::OBJTYPE_3DBULLET)
			{// �^�C�v����v�����ꍇ
				if (ColisonSphere3D(apObject[nCntObj], D3DXVECTOR3(GetSize().x, GetSize().y, GetSize().x), apObject[nCntObj]->GetColisonSize(), true))
				{
					if (apObject[nCntObj]->GetObjType() == CObject::OBJTYPE_3DENEMY
						&& m_parent != TYPE_ENEMY)
					{
						CEnemy3D *pEnemy3D = (CEnemy3D*)apObject[nCntObj];

						if (pEnemy3D->GetColorType() == GetColorType())
						{
							pEnemy3D->Hit(m_nAttack * 2);
						}
						else
						{
							pEnemy3D->Hit(m_nAttack);
						}

						Uninit();
						break;
					}
					else if (apObject[nCntObj]->GetObjType() == CObject::OBJTYPE_3DPLAYER
						&& m_parent != TYPE_PLAYER)
					{

					}
				}
			}
		}
	}
}

//=============================================================================
// �`��
// Author : �������l
// �T�v : 2D�`����s��
//=============================================================================
void CBullet3D::Draw()
{
	// �`��
	CObject3D::Draw();
}
