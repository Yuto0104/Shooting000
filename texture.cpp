//=============================================================================
//
// �e�N�X�`���ݒ菈��(texture.cpp)
// Author : �������l
// �T�v : �e�N�X�`���ݒ���s��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <string.h>
#include <assert.h>

#include "texture.h"
#include "renderer.h" 
#include "application.h"

//=============================================================================
// �R���X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�������ɍs������
//=============================================================================
CTexture::CTexture()
{
	
}

//=============================================================================
// �f�X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�I�����ɍs������
//=============================================================================
CTexture::~CTexture()
{

}

//=============================================================================
// ������
// Author : �������l
// �T�v : �\��t����e�N�X�`�������i�[����
//=============================================================================
void CTexture::Init(void)
{// �����_���[�̃Q�b�g
	CRenderer *pRenderer = CApplication::GetRenderer();

	// �t�@�C�����̐ݒ�
	const char *pFileName[] =
	{
		"data/TEXTURE/bullet000.png",				// 2D�|���S��
		"data/TEXTURE/effect000.jpg" ,				// 2D�v���C���[
		"data/TEXTURE/695783_VvJuOyc1.png",			// 2D�e
		"data/TEXTURE/explosion000.png",			// 2D����
		"data/TEXTURE/�{�X.png",					// 2D�G�l�~�[
		"data/TEXTURE/sky002.jpg",					// ��(0)
		"data/TEXTURE/number003.png",				// �i���o�[(0)
		"data/TEXTURE/�n�[�g.png",					// �n�[�g(0)
	};

	for (int nCnt = 0; nCnt < MAX_TYPE; nCnt++)
	{//�|���S���ɓ\��t����e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pRenderer->GetDevice(),
			pFileName[nCnt],
			&m_pTexture[nCnt]);
	}
}

//=============================================================================
// �I��
// Author : �������l
// �T�v : �e�N�X�`���̉��
//=============================================================================
void CTexture::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_TYPE; nCnt++)
	{//�e�N�X�`���̔j��	  
		if (m_pTexture[nCnt] != nullptr)
		{
			m_pTexture[nCnt]->Release();

			m_pTexture[nCnt] = nullptr;
		}
	}
}

//=============================================================================
// �e�N�X�`���|�C���^�̃Q�b�^�[	
// Author : �������l
// �T�v : �e�N�X�`���̃Q�b�^�[
//=============================================================================
LPDIRECT3DTEXTURE9 CTexture::GetTexture(TEXTURE_TYPE type)
{
	LPDIRECT3DTEXTURE9 pTexture = nullptr;

	if (type != TYPE_NULL)
	{// �^�C�v���ݒ肳��Ă�
		pTexture = m_pTexture[type];
	}

	return pTexture;
}
