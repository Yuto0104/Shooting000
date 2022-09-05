//=============================================================================
//
// ���f���}�l�[�W���[����(model_manager.cpp)
// Author : �������l
// �T�v : ���f���}�l�[�W���[�̐ݒ���s��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <assert.h>
#include <stdio.h>

#include "model_manager.h"
#include "renderer.h" 
#include "application.h"

//=============================================================================
// �R���X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�������ɍs������
//=============================================================================
CModelManager::CModelManager()
{
	m_pModelMaterial = nullptr;		// ���f���̃}�e���A�����
	m_nMaxModelMaterial = 0;			// ���f���̎�ʐ�
}

//=============================================================================
// �f�X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�I�����ɍs������
//=============================================================================
CModelManager::~CModelManager()
{

}

//=============================================================================
// ������
// Author : �������l
// �T�v : �\��t����e�N�X�`�������i�[����
//=============================================================================
void CModelManager::Init(void)
{// �����_���[�̃Q�b�g
	CRenderer *pRenderer = CApplication::GetRenderer();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// �t�@�C���ǂݍ���
	LoadFile("data/FILE/data.txt");

	// �w�i���f���̐ݒu
	LoadFile("data/FILE/BG_model.txt");

	for (int nCnt = 0; nCnt < m_nMaxModelMaterial; nCnt++)
	{// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(&m_pModelMaterial[nCnt].aFileName[0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&m_pModelMaterial[nCnt].pBuffer,
			NULL,
			&m_pModelMaterial[nCnt].nNumMat,
			&m_pModelMaterial[nCnt].pMesh);
	}
}

//=============================================================================
// �I��
// Author : �������l
// �T�v : �e�N�X�`���̉��
//=============================================================================
void CModelManager::Uninit(void)
{
	for (int nCnt = 0; nCnt < m_nMaxModelMaterial; nCnt++)
	{
		// ���b�V���̔j��
		if (m_pModelMaterial[nCnt].pMesh != nullptr)
		{
			m_pModelMaterial[nCnt].pMesh->Release();
			m_pModelMaterial[nCnt].pMesh = nullptr;
		}

		// �}�e���A���̔j��
		if (m_pModelMaterial[nCnt].pBuffer != nullptr)
		{
			m_pModelMaterial[nCnt].pBuffer->Release();
			m_pModelMaterial[nCnt].pBuffer = nullptr;
		}
	}

	// �������̉��
	delete[] m_pModelMaterial;
	m_pModelMaterial = nullptr;
}

//=============================================================================
// ���f���}�e���A�����̃Q�b�^�[
// Author : �������l
// �T�v : ���f���}�e���A�����̃Q�b�^�[
//=============================================================================
void CModelManager::GetModelMateria(const int nNumModel, LPD3DXMESH &pMesh, LPD3DXBUFFER &pBuffer, DWORD &nNumMat)
{
	pMesh = m_pModelMaterial[nNumModel].pMesh;
	pBuffer = m_pModelMaterial[nNumModel].pBuffer;
	nNumMat = m_pModelMaterial[nNumModel].nNumMat;
}

//=============================================================================
// �t�@�C���̓ǂݍ���
// Author : �������l
// �T�v : �t�@�C���̓ǂݍ���
//=============================================================================
void CModelManager::LoadFile(const char *pFileName)
{
	// �ϐ��錾
	char aStr[128];
	int nCntModel = 0;
	int nCntSetModel = 0;

	// �t�@�C���̓ǂݍ���
	FILE *pFile = fopen(pFileName, "r");

	if (pFile != nullptr)
	{
		while (fscanf(pFile, "%s", &aStr[0]) != EOF)
		{// "EOF"��ǂݍ��ނ܂� 
			if (strncmp(&aStr[0], "#", 1) == 0)
			{// ���ǂݍ���
				fgets(&aStr[0], sizeof(aStr), pFile);
			}

			if (strstr(&aStr[0], "MAX_TYPE") != NULL)
			{
				fscanf(pFile, "%s", &aStr[0]);
				fscanf(pFile, "%d", &m_nMaxModelMaterial);
				m_pModelMaterial = new MODEL_MATERIAL[m_nMaxModelMaterial];
				assert(m_pModelMaterial != nullptr);
				memset(&m_pModelMaterial[0], 0, sizeof(MODEL_MATERIAL));
			}

			if (strstr(&aStr[0], "NUM_MODEL") != NULL)
			{
				fscanf(pFile, "%s", &aStr[0]);
				fscanf(pFile, "%d", &m_nMaxSetModelBG);
				m_pObjectSeter = new OBJECT_SETER[m_nMaxSetModelBG];
				assert(m_pObjectSeter != nullptr);
				memset(&m_pObjectSeter[0], 0, sizeof(OBJECT_SETER));
			}

			if (strstr(&aStr[0], "MODEL_FILENAME") != NULL)
			{
				fscanf(pFile, "%s", &aStr[0]);
				fscanf(pFile, "%s", &m_pModelMaterial[nCntModel].aFileName[0]);
				nCntModel++;
			}

			if (strstr(&aStr[0], "MODELSET") != NULL)
			{
				while (strstr(&aStr[0], "END_MODELSET") == NULL)
				{
					fscanf(pFile, "%s", &aStr[0]);

					if (strncmp(&aStr[0], "#", 1) == 0)
					{// ���ǂݍ���
						fgets(&aStr[0], sizeof(aStr), pFile);
					}

					if (strstr(&aStr[0], "POS") != NULL)
					{// ���f���̃t�@�C�����̐ݒ�
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%f", &m_pObjectSeter[nCntSetModel].pos.x);
						fscanf(pFile, "%f", &m_pObjectSeter[nCntSetModel].pos.y);
						fscanf(pFile, "%f", &m_pObjectSeter[nCntSetModel].pos.z);
					}

					if (strstr(&aStr[0], "ROT") != NULL)
					{// ���f���̃t�@�C�����̐ݒ�
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%f", &m_pObjectSeter[nCntSetModel].rot.x);
						fscanf(pFile, "%f", &m_pObjectSeter[nCntSetModel].rot.y);
						fscanf(pFile, "%f", &m_pObjectSeter[nCntSetModel].rot.z);
					}

					if (strstr(&aStr[0], "SCALE") != NULL)
					{// ���f���̃t�@�C�����̐ݒ�
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%f", &m_pObjectSeter[nCntSetModel].size.x);
						fscanf(pFile, "%f", &m_pObjectSeter[nCntSetModel].size.y);
						fscanf(pFile, "%f", &m_pObjectSeter[nCntSetModel].size.z);
					}

					if (strcmp(&aStr[0], "TYPE") == 0)
					{// �L�[���̓ǂݍ���
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%d", &m_pObjectSeter[nCntSetModel].nID);
					}
				}

				nCntSetModel++;
			}
		}
	}
	else
	{
		assert(false);
	}
}

//=============================================================================
// ���f���̐ݒ�
// Author : �������l
// �T�v : ���f���̐ݒ���s��
//=============================================================================
void CModelManager::SetModelBG()
{
	CModel3D *pModel = nullptr;

	for (int nCntSetModel = 0; nCntSetModel < m_nMaxSetModelBG; nCntSetModel++)
	{
		// �C���X�^������
		pModel = CModel3D::Create(m_pObjectSeter[nCntSetModel].nID);
		pModel->SetPos(m_pObjectSeter[nCntSetModel].pos);
		pModel->SetRot(m_pObjectSeter[nCntSetModel].rot);
		pModel->SetSize(m_pObjectSeter[nCntSetModel].size);
		pModel->SetObjectDrowType(CObject::DROWTYPE_BG);
	}
}

