//=============================================================================
//
// �J�����}�l�[�W���[����(camera_manager.h)
// Author : �������l
// �T�v : �J�����̃A�N�V�����ݒ���s��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <assert.h>
#include <stdio.h>

#include "camera_manager.h"

//=============================================================================
// �R���X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�������ɍs������
//=============================================================================
CCameraManager::CCameraManager()
{
	m_pCameraAction = nullptr;						// �J�����̃A�N�V����
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ���_
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �����_
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ����
	m_nMaxAction = 0;								// �A�N�V������
}

//=============================================================================
// �f�X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�I�����ɍs������
//=============================================================================
CCameraManager::~CCameraManager()
{

}

//=============================================================================
// ������
// Author : �������l
// �T�v : �\��t����e�N�X�`�������i�[����
//=============================================================================
void CCameraManager::Init(void)
{
	LoadFile("data/FILE/camera_action.txt");
}

//=============================================================================
// �I��
// Author : �������l
// �T�v : �e�N�X�`���̉��
//=============================================================================
void CCameraManager::Uninit(void)
{
	// �������̉��
	delete[] m_pCameraAction;
	m_pCameraAction = nullptr;
}

//=============================================================================
// �t�@�C���̓ǂݍ���
// Author : �������l
// �T�v : �t�@�C���̓ǂݍ���
//=============================================================================
void CCameraManager::LoadFile(const char *pFileName)
{
	// �ϐ��錾
	char aStr[128];
	int nCntSetAction = 0;

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

			if (strstr(&aStr[0], "MAX_ACTION") != NULL)
			{
				fscanf(pFile, "%s", &aStr[0]);
				fscanf(pFile, "%d", &m_nMaxAction);
				m_pCameraAction = new CCamera::CAMERA_ACTION[m_nMaxAction];
				assert(m_pCameraAction != nullptr);
				memset(&m_pCameraAction[0], 0, sizeof(CCamera::CAMERA_ACTION));
			}

			if (strstr(&aStr[0], "SET_CAMERA") != NULL)
			{
				while (strstr(&aStr[0], "END_SET_CAMERA") == NULL)
				{
					fscanf(pFile, "%s", &aStr[0]);

					if (strncmp(&aStr[0], "#", 1) == 0)
					{// ���ǂݍ���
						fgets(&aStr[0], sizeof(aStr), pFile);
					}

					if (strstr(&aStr[0], "POSV") != NULL)
					{// ���f���̃t�@�C�����̐ݒ�
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%f", &m_posV.x);
						fscanf(pFile, "%f", &m_posV.y);
						fscanf(pFile, "%f", &m_posV.z);
					}

					if (strstr(&aStr[0], "POSR") != NULL)
					{// ���f���̃t�@�C�����̐ݒ�
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%f", &m_posR.x);
						fscanf(pFile, "%f", &m_posR.y);
						fscanf(pFile, "%f", &m_posR.z);
					}

					if (strstr(&aStr[0], "ROT") != NULL)
					{// ���f���̃t�@�C�����̐ݒ�
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%f", &m_rot.x);
						fscanf(pFile, "%f", &m_rot.y);
						fscanf(pFile, "%f", &m_rot.z);
					}
				}
			}

			if (strstr(&aStr[0], "ACTIONSET") != NULL)
			{
				while (strstr(&aStr[0], "END_ACTIONSET") == NULL)
				{
					fscanf(pFile, "%s", &aStr[0]);

					if (strncmp(&aStr[0], "#", 1) == 0)
					{// ���ǂݍ���
						fgets(&aStr[0], sizeof(aStr), pFile);
					}

					if (strstr(&aStr[0], "POSV") != NULL)
					{// ���f���̃t�@�C�����̐ݒ�
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%f", &m_pCameraAction[nCntSetAction].posVDest.x);
						fscanf(pFile, "%f", &m_pCameraAction[nCntSetAction].posVDest.y);
						fscanf(pFile, "%f", &m_pCameraAction[nCntSetAction].posVDest.z);
					}

					if (strstr(&aStr[0], "POSR") != NULL)
					{// ���f���̃t�@�C�����̐ݒ�
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%f", &m_pCameraAction[nCntSetAction].posRDest.x);
						fscanf(pFile, "%f", &m_pCameraAction[nCntSetAction].posRDest.y);
						fscanf(pFile, "%f", &m_pCameraAction[nCntSetAction].posRDest.z);
					}

					if (strcmp(&aStr[0], "FRAME") == 0)
					{// �L�[���̓ǂݍ���
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%d", &m_pCameraAction[nCntSetAction].nFrame);
					}
				}

				nCntSetAction++;
			}
		}
	}
	else
	{
		assert(false);
	}
}

