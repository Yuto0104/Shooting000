//=============================================================================
//
// モデルマネージャー処理(model_manager.cpp)
// Author : 唐﨑結斗
// 概要 : モデルマネージャーの設定を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>
#include <stdio.h>

#include "model_manager.h"
#include "renderer.h" 
#include "application.h"

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CModelManager::CModelManager()
{
	m_pModelMaterial = nullptr;		// モデルのマテリアル情報
	m_nMaxModelMaterial = 0;			// モデルの種別数
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CModelManager::~CModelManager()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 貼り付けるテクスチャ情報を格納する
//=============================================================================
void CModelManager::Init(void)
{// レンダラーのゲット
	CRenderer *pRenderer = CApplication::GetRenderer();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// ファイル読み込み
	LoadFile("data/FILE/data.txt");

	// 背景モデルの設置
	LoadFile("data/FILE/BG_model.txt");

	for (int nCnt = 0; nCnt < m_nMaxModelMaterial; nCnt++)
	{// Xファイルの読み込み
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
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャの解放
//=============================================================================
void CModelManager::Uninit(void)
{
	for (int nCnt = 0; nCnt < m_nMaxModelMaterial; nCnt++)
	{
		// メッシュの破棄
		if (m_pModelMaterial[nCnt].pMesh != nullptr)
		{
			m_pModelMaterial[nCnt].pMesh->Release();
			m_pModelMaterial[nCnt].pMesh = nullptr;
		}

		// マテリアルの破棄
		if (m_pModelMaterial[nCnt].pBuffer != nullptr)
		{
			m_pModelMaterial[nCnt].pBuffer->Release();
			m_pModelMaterial[nCnt].pBuffer = nullptr;
		}
	}

	// メモリの解放
	delete[] m_pModelMaterial;
	m_pModelMaterial = nullptr;
}

//=============================================================================
// モデルマテリアル情報のゲッター
// Author : 唐﨑結斗
// 概要 : モデルマテリアル情報のゲッター
//=============================================================================
void CModelManager::GetModelMateria(const int nNumModel, LPD3DXMESH &pMesh, LPD3DXBUFFER &pBuffer, DWORD &nNumMat)
{
	pMesh = m_pModelMaterial[nNumModel].pMesh;
	pBuffer = m_pModelMaterial[nNumModel].pBuffer;
	nNumMat = m_pModelMaterial[nNumModel].nNumMat;
}

//=============================================================================
// ファイルの読み込み
// Author : 唐﨑結斗
// 概要 : ファイルの読み込み
//=============================================================================
void CModelManager::LoadFile(const char *pFileName)
{
	// 変数宣言
	char aStr[128];
	int nCntModel = 0;
	int nCntSetModel = 0;

	// ファイルの読み込み
	FILE *pFile = fopen(pFileName, "r");

	if (pFile != nullptr)
	{
		while (fscanf(pFile, "%s", &aStr[0]) != EOF)
		{// "EOF"を読み込むまで 
			if (strncmp(&aStr[0], "#", 1) == 0)
			{// 一列読み込む
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
					{// 一列読み込む
						fgets(&aStr[0], sizeof(aStr), pFile);
					}

					if (strstr(&aStr[0], "POS") != NULL)
					{// モデルのファイル名の設定
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%f", &m_pObjectSeter[nCntSetModel].pos.x);
						fscanf(pFile, "%f", &m_pObjectSeter[nCntSetModel].pos.y);
						fscanf(pFile, "%f", &m_pObjectSeter[nCntSetModel].pos.z);
					}

					if (strstr(&aStr[0], "ROT") != NULL)
					{// モデルのファイル名の設定
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%f", &m_pObjectSeter[nCntSetModel].rot.x);
						fscanf(pFile, "%f", &m_pObjectSeter[nCntSetModel].rot.y);
						fscanf(pFile, "%f", &m_pObjectSeter[nCntSetModel].rot.z);
					}

					if (strstr(&aStr[0], "SCALE") != NULL)
					{// モデルのファイル名の設定
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%f", &m_pObjectSeter[nCntSetModel].size.x);
						fscanf(pFile, "%f", &m_pObjectSeter[nCntSetModel].size.y);
						fscanf(pFile, "%f", &m_pObjectSeter[nCntSetModel].size.z);
					}

					if (strcmp(&aStr[0], "TYPE") == 0)
					{// キー数の読み込み
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
// モデルの設定
// Author : 唐﨑結斗
// 概要 : モデルの設定を行う
//=============================================================================
void CModelManager::SetModelBG()
{
	CModel3D *pModel = nullptr;

	for (int nCntSetModel = 0; nCntSetModel < m_nMaxSetModelBG; nCntSetModel++)
	{
		// インスタン生成
		pModel = CModel3D::Create(m_pObjectSeter[nCntSetModel].nID);
		pModel->SetPos(m_pObjectSeter[nCntSetModel].pos);
		pModel->SetRot(m_pObjectSeter[nCntSetModel].rot);
		pModel->SetSize(m_pObjectSeter[nCntSetModel].size);
		pModel->SetObjectDrowType(CObject::DROWTYPE_BG);
	}
}

