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

#include "object.h"
#include "object2D.h"
#include "player2D.h"
#include "renderer.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
CObject *CObject::m_apObject[MAX_OBJECT] = {};		// �G�C���X�^���X

//=============================================================================
// �C���X�^���X�̉��
// Author : �������l
// �T�v : ���ׂẴC���X�^���X��������鏈��
//=============================================================================
void CObject::ReleaseAll(void)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (m_apObject[nCntObject] != nullptr)
		{// �C���X�^���X���g�p����Ă�
			// �I�u�W�F�N�g�I������
			m_apObject[nCntObject]->Uninit();
		}
	}
}

//=============================================================================
// ���ׂẴC���X�^���X�̍X�V
// Author : �������l
// �T�v : �g�p����Ă�C���X�^���X�̍X�V�������Ăяo��
//=============================================================================
void CObject::UpdateAll(void)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (m_apObject[nCntObject] != nullptr)
		{// �C���X�^���X���g�p����Ă�
			// �I�u�W�F�N�g�X�V����
			m_apObject[nCntObject]->Update();
		}
	}
}

//=============================================================================
// ���ׂẴC���X�^���X�̕`��
// Author : �������l
// �T�v : �g�p����Ă�C���X�^���X�̕`�揈�����Ăяo��
//=============================================================================
void CObject::DrawAll(void)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (m_apObject[nCntObject] != nullptr)
		{// �C���X�^���X���g�p����Ă�
			// �I�u�W�F�N�g�`�揈��
			m_apObject[nCntObject]->Draw();
		}
	}
}

//=============================================================================
// �R���X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�������ɍs������
//=============================================================================
CObject::CObject()
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (m_apObject[nCntObject] == nullptr)
		{// �C���X�^���X���g�p����ĂȂ�
			m_apObject[nCntObject] = this;
			m_nNumID = nCntObject;
			break;
		}
	}
}

//=============================================================================
// �f�X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�I�����ɍs������
//=============================================================================
CObject::~CObject()
{
	
}

//=============================================================================
// �C���X�^���X�̉��
// Author : �������l
// �T�v : ���ׂẴC���X�^���X��������鏈��
//=============================================================================
void CObject::Release(void)
{
	if (m_apObject[m_nNumID] != nullptr)
	{// �C���X�^���X���g�p����Ă�
		// �������̉��
		const int nID = m_nNumID;
		delete m_apObject[nID];
		m_apObject[nID] = nullptr;
	}
}

//=============================================================================
// �͈͓��̔���
// Author : �������l
// �T�v : �^�[�Q�b�g���͈͓��ɓ��������̔���
//=============================================================================
bool CObject::ColisonRange2D(CObject * target)
{// �����̏����擾����
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 size = GetSize();

	// �ڕW�̏��擾
	D3DXVECTOR3 posTarget = target->GetPos();
	D3DXVECTOR3 sizeTarget = target->GetSize();

	if (pos.x - size.x >= posTarget.x - sizeTarget.x
		&& pos.x + size.x <= posTarget.x + sizeTarget.x
		&& pos.y - size.y >= posTarget.y - sizeTarget.y
		&& pos.y + size.y <= posTarget.y + sizeTarget.y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//=============================================================================
// ��`�̔���
// Author : �������l
// �T�v : �^�[�Q�b�g�Ƃ̋�`����
//=============================================================================
bool CObject::ColisonRectangle2D(CObject *target, bool bExtrude)
{// �Ԃ�l�p�̕ϐ�
	bool bColison = false;

	// �����̏����擾����
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 posOld = GetPosOld();
	D3DXVECTOR3 size = GetSize() / 2.0f;

	// �ڕW�̏��擾
	D3DXVECTOR3 posTarget = target->GetPos();
	D3DXVECTOR3 sizeTarget = target->GetSize() / 2.0f;

	if (pos.y - size.y  < posTarget.y + sizeTarget.y 
		&& pos.y + size.y  > posTarget.y - sizeTarget.y )
	{// ���f�����ɂ���(Y��)
		if (posOld.x + size.y  <= posTarget.x - sizeTarget.x 
			&& pos.x + size.y  > posTarget.x - sizeTarget.x )
		{
			if (bExtrude)
			{// �����o�����g�p����
				pos.x = posTarget.x - sizeTarget.x  - size.y ;
			}

			bColison = true;
		}
		if (posOld.x - size.y  >= posTarget.x + sizeTarget.x 
			&& pos.x - size.y  < posTarget.x + sizeTarget.x )
		{
			if (bExtrude)
			{// �����o�����g�p����
				pos.x = posTarget.x + sizeTarget.x  + size.y ;
			}

			bColison = true;
		}
	}
	if (pos.x - size.x  < posTarget.x + sizeTarget.x 
		&& pos.x + size.x  > posTarget.x - sizeTarget.x )
	{// ���f�����ɂ���(X��)
		if (posOld.y + size.y  <= posTarget.y - sizeTarget.y 
			&& pos.y + size.y  > posTarget.y - sizeTarget.y )
		{
			if (bExtrude)
			{// �����o�����g�p����
				pos.y = posTarget.y - sizeTarget.y  - size.y ;
			}

			bColison = true;
		}
		if (posOld.y - size.y  >= posTarget.y + sizeTarget.y 
			&& pos.y - size.y  < posTarget.y + sizeTarget.y )
		{
			if (bExtrude)
			{// �����o�����g�p����
				pos.y = posTarget.y + sizeTarget.y  + size.y ;
			}

			bColison = true;
		}
	}

	// �ʒu�̐ݒ�
	SetPos(pos);
	return bColison;
}

//=============================================================================
// �~�̔���
// Author : �������l
// �T�v : �^�[�Q�b�g�Ƃ̉~����
//=============================================================================
bool CObject::ColisonCircle2D(CObject * target, bool bExtrude)
{
	// �ϐ��錾
	bool bCollision = false;

	// �����̏����擾����
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 size = GetSize() / 2.0f;

	// �ڕW�̏��擾
	D3DXVECTOR3 posTarget = target->GetPos();
	D3DXVECTOR3 sizeTarget = target->GetSize() / 2.0f;

	// ������s���������Z�o
	float fJudgeDistance = sqrtf((size.x * size.x) + (size.y * size.y));
	fJudgeDistance += sqrtf((sizeTarget.x * sizeTarget.x) + (sizeTarget.y * sizeTarget.y));

	// ���݂��̈ʒu�̍����Z�o
	D3DXVECTOR3 distance = posTarget - pos;
	float fDistance = sqrtf((distance.x * distance.x) + (distance.y * distance.y));

	if (fDistance <= fJudgeDistance)
	{// �ʒu�̍���������s���������Z���ꍇ
		bCollision = true;

		if (bExtrude)
		{
			float fRotDif = atan2f(distance.x, distance.y);
			pos.x = posTarget.x - (sinf(fRotDif) * fJudgeDistance);
			pos.y = posTarget.y - (cosf(fRotDif) * fJudgeDistance);
		}
	}

	// �ʒu�̐ݒ�
	SetPos(pos);
	return bCollision;
}
