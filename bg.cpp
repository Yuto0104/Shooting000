//=============================================================================
//
// �w�i�N���X(object.h)
// Author : �������l
// �T�v : �w�i�������s��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <assert.h>

#include "bg.h"
#include "renderer.h"
#include "application.h"

//=============================================================================
// �R���X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�������ɍs������
//=============================================================================
CBG::CBG()
{
	m_pVtxBuff = nullptr;								// ���_�o�b�t�@
	m_col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);				// �J���[
	m_typeTex = CTexture::TYPE_2DPLAYER;				// �e�N�X�`���̎��
}

//=============================================================================
// �f�X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�I�����ɍs������
//=============================================================================
CBG::~CBG()
{

}

//=============================================================================
// ������
// Author : �������l
// �T�v : ���_�o�b�t�@�𐶐����A�����o�ϐ��̏����l��ݒ�
//=============================================================================
HRESULT CBG::Init()
{// �����_���[�̃Q�b�g
	CRenderer *pRenderer = CApplication::GetRenderer();

	//���_�o�b�t�@�̐���
	pRenderer->GetDevice()->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,		// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,														// ���_�t�@�[�}�b�g
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// �|���S�����̐ݒ�
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �J���[
	m_typeTex = CTexture::TYPE_2DPOLYGON;			// �e�N�X�`���^�C�v

	// �e�N�X�`�����W�̐ݒ�
	SetTex(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));

	return S_OK;
}

//=============================================================================
// �I��
// Author : �������l
// �T�v : �e�N�X�`���̃|�C���^�ƒ��_�o�b�t�@�̉��
//=============================================================================
void CBG::Uninit()
{
	//���_�o�b�t�@��j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();

		m_pVtxBuff = nullptr;
	}

	// �w�i3D�̉��
	Release();
}

//=============================================================================
// �X�V
// Author : �������l
// �T�v : 2D�X�V���s��
//=============================================================================
void CBG::Update()
{

}

//=============================================================================
// �`��
// Author : �������l
// �T�v : 2D�`����s��
//=============================================================================
void CBG::Draw()
{

}

//=============================================================================
// ���_���W�Ȃǂ̐ݒ�
// Author : �������l
// �T�v : 2D�|���S���̒��_���W�Arhw�A���_�J���[��ݒ肷��
//=============================================================================
void CBG::SetVtx()
{
	//���_���ւ̃|�C���^�𐶐�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	pVtx[0].pos = D3DXVECTOR3();
	pVtx[1].pos = D3DXVECTOR3();
	pVtx[2].pos = D3DXVECTOR3();
	pVtx[3].pos = D3DXVECTOR3();

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
// Author : �������l
// �T�v : 2D�I�u�W�F�N�g�̃e�N�X�`�����W��ݒ肷��
//=============================================================================
void CBG::SetTex(const D3DXVECTOR2 &minTex, const D3DXVECTOR2 &maxTex)
{
	//���_���ւ̃|�C���^�𐶐�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(minTex.x, minTex.y);
	pVtx[1].tex = D3DXVECTOR2(maxTex.x, minTex.y);
	pVtx[2].tex = D3DXVECTOR2(minTex.x, maxTex.y);
	pVtx[3].tex = D3DXVECTOR2(maxTex.x, maxTex.y);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}