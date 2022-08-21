//=============================================================================
//
// �X�R�A�N���X(score.cpp)
// Author : �������l
// �T�v : �X�R�A�������s��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <assert.h>
#include <math.h>

#include "energy_gage.h"
#include "object2D.h"
#include "score.h"
#include "gauge2D.h"
#include "motion_player3D.h"
#include "renderer.h"
#include "application.h"
#include "texture.h"

//=============================================================================
// �C���X�^���X����
// Author : �������l
// �T�v : �X�R�A�𐶐�����
//=============================================================================
CEnergyGage *CEnergyGage::Create()
{
	// �I�u�W�F�N�g�C���X�^���X
	CEnergyGage *pEnergyGage = nullptr;

	pEnergyGage = new CEnergyGage;

	// �������̊m�ۂ��ł��Ȃ�����
	assert(pEnergyGage != nullptr);

	// ���l�̏�����
	pEnergyGage->Init();

	// �C���X�^���X��Ԃ�
	return pEnergyGage;
}

//=============================================================================
// �R���X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�������ɍs������
//=============================================================================
CEnergyGage::CEnergyGage(int nPriority /*= CObject::PRIORITY_LEVEL3*/) : CObject(nPriority)
{
	m_pObject2D = nullptr;								// 2D�I�u�W�F�N�g
	m_pScore = nullptr;									// �X�R�A
	m_pGauge2D = nullptr;								// �Q�[�W
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �ʒu
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ߋ��̈ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ����
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �傫��
}

//=============================================================================
// �f�X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�I�����ɍs������
//=============================================================================
CEnergyGage::~CEnergyGage()
{

}

//=============================================================================
// ������
// Author : �������l
// �T�v : ���_�o�b�t�@�𐶐����A�����o�ϐ��̏����l��ݒ�
//=============================================================================
HRESULT CEnergyGage::Init()
{
	// 2D�I�u�W�F�N�g�̐���
	m_pObject2D = CObject2D::Create();
	assert(m_pObject2D != nullptr);
	m_pObject2D->LoadTex(CTexture::TYPE_NULL);
	m_pObject2D->SetCol(D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f));

	// �X�R�A�̐���
	m_pScore = CScore::Create(3, true);
	assert(m_pScore != nullptr);

	//// �Q�[�W�̐���
	//m_pGauge2D = CGauge2D::Create();
	//assert(m_pGauge2D != nullptr);

	// �ʒu�̐ݒ�
	SetPos(D3DXVECTOR3(640.0f, 360.0f, 0.0f));

	// �����̐ݒ�
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	
	// �傫���̐ݒ�
	SetSize(D3DXVECTOR3(70.0f, 500.0f, 0.0f));

	return S_OK;
}

//=============================================================================
// �I��
// Author : �������l
// �T�v : �e�N�X�`���̃|�C���^�ƒ��_�o�b�t�@�̉��
//=============================================================================
void CEnergyGage::Uninit()
{
	if (m_pObject2D != nullptr)
	{// �I������
		m_pObject2D->Uninit();

		// �������̉��
		delete m_pObject2D;
		m_pObject2D = nullptr;
	}

	if (m_pScore != nullptr)
	{// �I������
		m_pScore->Uninit();

		// �������̉��
		delete m_pScore;
		m_pScore = nullptr;
	}

	if (m_pGauge2D != nullptr)
	{// �I������
		m_pGauge2D->Uninit();

		// �������̉��
		delete m_pGauge2D;
		m_pGauge2D = nullptr;
	}

	// �C���X�^���X�̉��
	Release();
}

//=============================================================================
// �X�V
// Author : �������l
// �T�v : �X�V���s��
//=============================================================================
void CEnergyGage::Update()
{

}

//=============================================================================
// �`��
// Author : �������l
// �T�v : �`����s��
//=============================================================================
void CEnergyGage::Draw()
{

}

//=============================================================================
// �ʒu�̃Z�b�^�[
// Author : �������l
// �T�v : �ʒu�̃����o�ϐ��Ɉ�������
//=============================================================================
void CEnergyGage::SetPos(const D3DXVECTOR3 &pos)
{
	m_pos = pos;

	m_pObject2D->SetPos(m_pos);
}

//=============================================================================
// �����̃Z�b�^�[
// Author : �������l
// �T�v : �����̃����o�ϐ��Ɉ�������
//=============================================================================
void CEnergyGage::SetRot(const D3DXVECTOR3 &rot)
{
	m_rot = rot;

	m_pObject2D->SetRot(m_rot);
}

//=============================================================================
// �傫���̃Z�b�^�[
// Author : �������l
// �T�v : �傫���̃����o�ϐ��Ɉ�������
//=============================================================================
void CEnergyGage::SetSize(const D3DXVECTOR3 & size)
{
	m_size = size;

	m_pObject2D->SetSize(m_size);
}