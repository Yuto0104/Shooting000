//**************************************************************************************************
//
// ���[�V��������(motion.cpp)
// Auther�F�������l
//
//**************************************************************************************************

//***************************************************************************
// �C���N���[�h
//***************************************************************************
#include <stdio.h>
#include <assert.h>

#include "application.h"
#include "motion.h"	
#include "calculation.h"

//=============================================================================
// �R���X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�������ɍs������
//=============================================================================
CMotion::CMotion(char * pFileName)
{
	// �p�[�c���̏�����
	memset(&m_partsFile, 0, sizeof(m_partsFile));

	// ���[�V�����̏�����
	memset(&m_motion, 0, sizeof(m_motion));

	// �p�[�c�̏�����
	memset(&m_parts, 0, sizeof(m_parts));

	// �p�[�c���̏�����
	m_nMaxParts = 0;

	// ���[�V�����̓ǂݍ���
	LoodSetMotion(pFileName);
}

//=============================================================================
// �f�X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�I�����ɍs������
//=============================================================================
CMotion::~CMotion()
{
}

//=============================================================================
// ������
// Author : �������l
// �T�v : �p�[�c����X�t�@�C���ǂݍ���
//=============================================================================
void CMotion::Init(void)
{
	for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
	{// �J�E���g�̃��Z�b�g
		CntReset(nCntMotion);
	}

	for (int i = 0; i < m_nMaxParts; i++)
	{
		// �ʒu�ƌ����̏����l��ۑ�
		(m_parts + i)->posOrigin = (m_parts + i)->pos;
		(m_parts + i)->rotOrigin = (m_parts + i)->rot;

		// �p�[�c���̏�����
		(m_parts + i)->mtxWorld = {};												// ���[���h�}�g���b�N�X

		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(m_partsFile[(m_parts + i)->nType].aName,
			D3DXMESH_SYSTEMMEM,
			CApplication::GetRenderer()->GetDevice(),
			NULL,
			&(m_parts + i)->pBuffer,
			NULL,
			&(m_parts + i)->nNumMat,
			&(m_parts + i)->pMesh);
	}
}

//=============================================================================
// �p�[�c�̐ݒ�
// Author : �������l
// �T�v : �s��𗘗p���āA�p�[�c�̐e�q�֌W�ƕ`��ݒ���s��
//=============================================================================
void CMotion::SetParts(D3DXMATRIX mtxWorld)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat = {};

	for (int i = 0; i < m_nMaxParts; i++)
	{// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&(m_parts + i)->mtxWorld);			// �s�񏉊����֐�

		// �����̔��f
		D3DXMatrixRotationYawPitchRoll(&mtxRot,
			(m_parts + i)->rot.y,
			(m_parts + i)->rot.x,
			(m_parts + i)->rot.z);								// �s���]�֐�

		D3DXMatrixMultiply(&(m_parts + i)->mtxWorld,
			&(m_parts + i)->mtxWorld,
			&mtxRot);											// �s��|���Z�֐� 

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans,
			(m_parts + i)->pos.x,
			(m_parts + i)->pos.y,
			(m_parts + i)->pos.z);								// �s��ړ��֐�

		D3DXMatrixMultiply(&(m_parts + i)->mtxWorld,
			&(m_parts + i)->mtxWorld,
			&mtxTrans);											// �s��|���Z�֐�

		// �e�p�[�c�̃��[���h�}�g���b�N�X��ێ�
		D3DXMATRIX mtxParent;

		if ((m_parts + i)->nIdxModelParent == -1)
		{// �e���f���̃C���f�b�N�X����-1�̎�
		 // �V�K�[�x�l��Z�o�b�t�@�̐[�x�l�������l�Ȃ�e�X�g�����ɂ���
		 //pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
			mtxParent = mtxWorld;
		}
		else
		{// �V�K�[�x�l��Z�o�b�t�@�̐[�x�l�������l�Ȃ�e�X�g�����ɂ���
			//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATEREQUAL);
			mtxParent = (m_parts + (m_parts + i)->nIdxModelParent)->mtxWorld;
		}

		// �����̐e�}�g���b�N�X�Ƃ̊|���Z
		D3DXMatrixMultiply(&(m_parts + i)->mtxWorld, &(m_parts + i)->mtxWorld, &mtxParent);

		//// �T�C�Y�̔��f
		//D3DXMatrixScaling()

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &(m_parts + i)->mtxWorld);

		// ���݂̃}�e���A����ێ�
		pDevice->GetMaterial(&matDef);

		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)(m_parts + i)->pBuffer->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)(m_parts + i)->nNumMat; nCntMat++)
		{
			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// �v���C���[�p�[�c�̕`��
			(m_parts + i)->pMesh->DrawSubset(nCntMat);
		}

		// �ۂ��Ă����}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}

	// �V�K�[�x�l��Z�o�b�t�@�̐[�x�l�������l�Ȃ�e�X�g�����ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

//=============================================================================
// ���[�V�����̍Đ�
// Author : �������l
// �T�v : �ړI�̈ʒu�܂œ���̃t���[�����œ������鏈�����p�[�c���Ƃɍs��
//=============================================================================
bool CMotion::PlayMotion(const int nCntMotionSet)
{
	// �ϐ��錾
	bool bMotion = true;

	for (int nCntParts = 0; nCntParts < m_nMaxParts; nCntParts++)
	{
		if (m_motion[nCntMotionSet].nCntFrame == 0)
		{// �t���[���J�E���g��0�̎�
			// �ړI�̈ʒu�ƌ����̎Z�o
			(m_parts + nCntParts)->posDest = ((m_parts + nCntParts)->posOrigin + m_motion[nCntMotionSet].keySet[m_motion[nCntMotionSet].nCntKeySet].key[nCntParts].pos) - (m_parts + nCntParts)->pos;
			(m_parts + nCntParts)->rotDest = ((m_parts + nCntParts)->rotOrigin + m_motion[nCntMotionSet].keySet[m_motion[nCntMotionSet].nCntKeySet].key[nCntParts].rot) - (m_parts + nCntParts)->rot;

			// �p�x�̐��K��
			(m_parts + nCntParts)->rotDest.x = RotNormalization((m_parts + nCntParts)->rotDest.x);
			(m_parts + nCntParts)->rotDest.y = RotNormalization((m_parts + nCntParts)->rotDest.y);
			(m_parts + nCntParts)->rotDest.z = RotNormalization((m_parts + nCntParts)->rotDest.z);
		}

		// �ϐ��錾
		D3DXVECTOR3 addPos = D3DXVECTOR3((m_parts + nCntParts)->posDest / (float)(m_motion[nCntMotionSet].keySet[m_motion[nCntMotionSet].nCntKeySet].nFrame));
		D3DXVECTOR3 addRot = D3DXVECTOR3((m_parts + nCntParts)->rotDest / (float)(m_motion[nCntMotionSet].keySet[m_motion[nCntMotionSet].nCntKeySet].nFrame));

		// �ʒu�̉��Z
		(m_parts + nCntParts)->pos += addPos;

		//	�����̉��Z
		(m_parts + nCntParts)->rot += addRot;

		// �p�x�̐��K��
		(m_parts + nCntParts)->rotDest.x = RotNormalization((m_parts + nCntParts)->rotDest.x);
		(m_parts + nCntParts)->rotDest.y = RotNormalization((m_parts + nCntParts)->rotDest.y);
		(m_parts + nCntParts)->rotDest.z = RotNormalization((m_parts + nCntParts)->rotDest.z);
	}

	// �t���[���J�E���g�̉��Z
	m_motion[nCntMotionSet].nCntFrame++;

	if (m_motion[nCntMotionSet].nCntFrame >= m_motion[nCntMotionSet].keySet[m_motion[nCntMotionSet].nCntKeySet].nFrame)
	{// �t���[���J�E���g���w��̃t���[�����𒴂����ꍇ
		// �t���[�����̏�����
		m_motion[nCntMotionSet].nCntFrame = 0;

		// �Đ����̃L�[�ԍ����̉��Z
		m_motion[nCntMotionSet].nCntKeySet++;

		if (m_motion[nCntMotionSet].nCntKeySet >= m_motion[nCntMotionSet].nNumKey && m_motion[nCntMotionSet].bLoop)
		{// �Đ����̃L�[���J�E���g���L�[���̍ő�l�𒴂����Ƃ��A���̃��[�V���������[�v���g�p���Ă���
			// �Đ����̃L�[���J�E���g��������
			m_motion[nCntMotionSet].nCntKeySet = 0;

		}
		else if (m_motion[nCntMotionSet].nCntKeySet >= m_motion[nCntMotionSet].nNumKey)
		{
			m_motion[nCntMotionSet].nCntKeySet = 0;
			bMotion = false;
		}
	}

	return bMotion;
}

//=============================================================================
// ���[�V�����u�����h
// Author : �������l
// �T�v : ���[�V�����ƃ��[�V�����̂Ȃ��ڂ𒲐�����
//=============================================================================
bool CMotion::MotionBlend(const int nCntMotionSet)
{
	// �ϐ��錾
	bool bMotionBlend = true;

	for (int nCntParts = 0; nCntParts < m_nMaxParts; nCntParts++)
	{
		if ((m_motion + nCntMotionSet)->nCntFrame == 0)
		{// �t���[���J�E���g��0�̎�
		 // �ړI�̈ʒu�ƌ����̎Z�o
			(m_parts + nCntParts)->posDest = ((m_parts + nCntParts)->posOrigin + (m_motion + nCntMotionSet)->keySet[(m_motion + nCntMotionSet)->nCntKeySet].key[nCntParts].pos) - (m_parts + nCntParts)->pos;
			(m_parts + nCntParts)->rotDest = ((m_parts + nCntParts)->rotOrigin + (m_motion + nCntMotionSet)->keySet[(m_motion + nCntMotionSet)->nCntKeySet].key[nCntParts].rot) - (m_parts + nCntParts)->rot;

			// �p�x�̐��K��
			(m_parts + nCntParts)->rotDest.x = RotNormalization((m_parts + nCntParts)->rotDest.x);
			(m_parts + nCntParts)->rotDest.y = RotNormalization((m_parts + nCntParts)->rotDest.y);
			(m_parts + nCntParts)->rotDest.z = RotNormalization((m_parts + nCntParts)->rotDest.z);
		}

		// �ϐ��錾
		D3DXVECTOR3 addPos = D3DXVECTOR3((m_parts + nCntParts)->posDest / (float)(MOTION_BLEND_FRAM));
		D3DXVECTOR3 addRot = D3DXVECTOR3((m_parts + nCntParts)->rotDest / (float)(MOTION_BLEND_FRAM));

		// �ʒu�̉��Z
		(m_parts + nCntParts)->pos += addPos;

		//	�����̉��Z
		(m_parts + nCntParts)->rot += addRot;

		// �p�x�̐��K��
		(m_parts + nCntParts)->rotDest.x = RotNormalization((m_parts + nCntParts)->rotDest.x);
		(m_parts + nCntParts)->rotDest.y = RotNormalization((m_parts + nCntParts)->rotDest.y);
		(m_parts + nCntParts)->rotDest.z = RotNormalization((m_parts + nCntParts)->rotDest.z);
	}

	// �t���[���J�E���g�̉��Z
	(m_motion + nCntMotionSet)->nCntFrame++;

	if ((m_motion + nCntMotionSet)->nCntFrame >= MOTION_BLEND_FRAM)
	{// �t���[���J�E���g���w��̃t���[�����𒴂����ꍇ
		// �t���[�����̏�����
		(m_motion + nCntMotionSet)->nCntFrame = 0;

		// �Đ����̃L�[�ԍ����̉��Z
		(m_motion + nCntMotionSet)->nCntKeySet++;

		bMotionBlend = false;
	}

	return bMotionBlend;
}

//=============================================================================
// �p�[�c�ƃ��[�V�����̓ǂݍ���
// Author : �������l
// �T�v : �p�[�c�ƃ��[�V�����̓ǂݍ��݁A���������Ăяo��
//=============================================================================
void CMotion::LoodSetMotion(char * pFileName)
{
	// �ϐ��錾
	char aString[128] = {};			// �������r�p�̕ϐ�
	char g_aEqual[128] = {};		// ���ǂݍ��ݗp�ϐ�
	int	nCntKeySet = 0;				// KeySet�J�E���g
	int	nCntKey = 0;				// Key�J�E���g
	int nCntFileName = 0;			// �t�@�C����
	int nCntParts = 0;				// �p�[�c���̃J�E���g
	int nCntMotion = 0;				// ���[�V�������̃J�E���g

	// �t�@�C���|�C���^�̐錾
	FILE * pFile;

	//�t�@�C�����J��
	pFile = fopen(pFileName, "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		fscanf(pFile, "%s", &aString);

		while (strncmp(&aString[0], "SCRIPT", 6) != 0)
		{// �����񂪈�v�����ꍇ
			aString[0] = {};
			// ������̓ǂݍ���
			fscanf(pFile, "%s", &aString[0]);
		}
		while (strncmp(&aString[0], "END_SCRIPT", 10) != 0)
		{
			fscanf(pFile, "%s", &aString[0]);

			if (strncmp(&aString[0], "#", 1) == 0)
			{// ���ǂݍ���
				fgets(&aString[0], sizeof(aString), pFile);
			}

			if (strcmp(&aString[0], "MODEL_FILENAME") == 0)
			{// �t�@�C�����̓ǂݍ���
				fscanf(pFile, "%s", &g_aEqual[0]);
				fscanf(pFile, "%s", &m_partsFile[nCntFileName].aName[0]);
				nCntFileName++;
			}

			if (strcmp(&aString[0], "CHARACTERSET") == 0)
			{// �L�����N�^�[�̓ǂݍ���
				while (strcmp(&aString[0], "END_CHARACTERSET") != 0)
				{
					fscanf(pFile, "%s", &aString[0]);

					if (strncmp(&aString[0], "#", 1) == 0)
					{// ���ǂݍ���
						fgets(&aString[0], sizeof(aString), pFile);
					}

					if (strcmp(&aString[0], "NUM_PARTS") == 0)
					{// �ǂݍ��ރp�[�c��
						fscanf(pFile, "%s", &g_aEqual[0]);
						fscanf(pFile, "%d", &m_nMaxParts);

						// �������̉��
						m_parts = new Parts[m_nMaxParts];
						m_motion = new MyMotion[MAX_MOTION];
						assert(m_parts != nullptr && m_motion != nullptr);
					}

					if (strcmp(&aString[0], "PARTSSET") == 0)
					{// �p�[�c�̓ǂݍ���
						while (strcmp(&aString[0], "END_PARTSSET") != 0)
						{
							fscanf(pFile, "%s", &aString[0]);

							if (strncmp(&aString[0], "#", 1) == 0)
							{// ���ǂݍ���
								fgets(&aString[0], sizeof(aString), pFile);
							}

							if (strcmp(&aString[0], "INDEX") == 0)
							{// �^�C�v�̓ǂݍ���
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%d", &m_parts[nCntParts].nType);
							}
							if (strcmp(&aString[0], "PARENT") == 0)
							{// �e�̓ǂݍ���
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%d", &m_parts[nCntParts].nIdxModelParent);
							}
							if (strcmp(&aString[0], "POS") == 0)
							{// �ʒu�̓ǂݍ���
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%f", &m_parts[nCntParts].pos.x);
								fscanf(pFile, "%f", &m_parts[nCntParts].pos.y);
								fscanf(pFile, "%f", &m_parts[nCntParts].pos.z);
							}
							if (strcmp(&aString[0], "ROT") == 0)
							{// �����̓ǂݍ���
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%f", &m_parts[nCntParts].rot.x);
								fscanf(pFile, "%f", &m_parts[nCntParts].rot.y);
								fscanf(pFile, "%f", &m_parts[nCntParts].rot.z);
							}
						}

						// �p�[�c�J�E���g�̃C���N�������g
						nCntParts++;
					}
				}
			}
			if (strcmp(&aString[0], "MOTIONSET") == 0)
			{// ���[�V�����̓ǂݍ���
				while (strcmp(&aString[0], "END_MOTIONSET") != 0)
				{
					fscanf(pFile, "%s", &aString[0]);

					if (strncmp(&aString[0], "#", 1) == 0)
					{// ���ǂݍ���
						fgets(&aString[0], sizeof(aString), pFile);
					}

					if (strcmp(&aString[0], "LOOP") == 0)
					{// ���[�v�L���̓ǂݍ���
						fscanf(pFile, "%s", &g_aEqual[0]);
						fscanf(pFile, "%d", (int*)(&m_motion[nCntMotion].bLoop));
					}
					if (strcmp(&aString[0], "NUM_KEY") == 0)
					{// �L�[���̓ǂݍ���
						fscanf(pFile, "%s", &g_aEqual[0]);
						fscanf(pFile, "%d", &m_motion[nCntMotion].nNumKey);
					}
					if (strcmp(&aString[0], "KEYSET") == 0)
					{// �L�[�Z�b�g�̓ǂݍ���
						while (strcmp(&aString[0], "END_KEYSET") != 0)
						{
							fscanf(pFile, "%s", &aString[0]);

							if (strncmp(&aString[0], "#", 1) == 0)
							{// ���ǂݍ���
								fgets(&aString[0], sizeof(aString), pFile);
							}

							if (strcmp(&aString[0], "FRAME") == 0)
							{// �t���[�����̓ǂݍ���
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%d", &m_motion[nCntMotion].keySet[nCntKeySet].nFrame);
							}
							if (strcmp(&aString[0], "KEY") == 0)
							{// �L�[�̓ǂݍ���
								while (strcmp(&aString[0], "END_KEY") != 0)
								{
									fscanf(pFile, "%s", &aString[0]);

									if (strncmp(&aString[0], "#", 1) == 0)
									{// ���ǂݍ���
										fgets(&aString[0], sizeof(aString), pFile);
									}

									if (strcmp(&aString[0], "POS") == 0)
									{// �ʒu�̓ǂݍ���
										fscanf(pFile, "%s", &g_aEqual[0]);
										fscanf(pFile, "%f", &m_motion[nCntMotion].keySet[nCntKeySet].key[nCntKey].pos.x);
										fscanf(pFile, "%f", &m_motion[nCntMotion].keySet[nCntKeySet].key[nCntKey].pos.y);
										fscanf(pFile, "%f", &m_motion[nCntMotion].keySet[nCntKeySet].key[nCntKey].pos.z);
									}
									if (strcmp(&aString[0], "ROT") == 0)
									{// �����̓ǂݍ���
										fscanf(pFile, "%s", &g_aEqual[0]);
										fscanf(pFile, "%f", &m_motion[nCntMotion].keySet[nCntKeySet].key[nCntKey].rot.x);
										fscanf(pFile, "%f", &m_motion[nCntMotion].keySet[nCntKeySet].key[nCntKey].rot.y);
										fscanf(pFile, "%f", &m_motion[nCntMotion].keySet[nCntKeySet].key[nCntKey].rot.z);
									}
								}

								// �L�[�J�E���g�̃C���N�������g
								nCntKey++;
							}
						}

						// �L�[�J�E���g�̏�����
						nCntKey = 0;

						// �L�[�Z�b�g�J�E���g�̃C���N�������g
						nCntKeySet++;
					}
				}
				// �L�[�Z�b�g�J�E���g�̏�����
				nCntKeySet = 0;

				// �p�[�c���̃C���N�������g
				nCntMotion++;
			}
		}

		//�t�@�C�������
		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ��ꍇ
		assert(false);
	}

	// ������
	Init();
}

//=============================================================================
// �I��
// Author : �������l
// �T�v : �������̉��
//=============================================================================
void CMotion::Uninit(void)
{
	for (int i = 0; i < m_nMaxParts; i++)
	{
		if (m_parts[i].pBuffer != NULL)
		{// ���_�o�b�t�@�[�̉��
			m_parts[i].pBuffer->Release();
			m_parts[i].pBuffer = NULL;
		}

		if (m_parts[i].pMesh != NULL)
		{// ���b�V���̉��
			m_parts[i].pMesh->Release();
			m_parts[i].pMesh = NULL;
		}
	}

	if (m_parts != nullptr
		&& m_motion != nullptr)
	{// �������̉��
		delete[] m_parts;
		delete[] m_motion;
		m_parts = nullptr;
	}
}

//=============================================================================
// �J�E���g�̃��Z�b�g
// Author : �������l
// �T�v : �J�E���g�̃��Z�b�g
//=============================================================================
void CMotion::CntReset(const int nNumMotionOld)
{
	m_motion[nNumMotionOld].nCntFrame = 0;
	m_motion[nNumMotionOld].nCntKeySet = 0;
}


