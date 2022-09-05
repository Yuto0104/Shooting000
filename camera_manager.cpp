//=============================================================================
//
// カメラマネージャー処理(camera_manager.h)
// Author : 唐﨑結斗
// 概要 : カメラのアクション設定を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>
#include <stdio.h>

#include "camera_manager.h"

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CCameraManager::CCameraManager()
{
	m_pCameraAction = nullptr;						// カメラのアクション
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 視点
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 注視点
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 向き
	m_nMaxAction = 0;								// アクション数
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CCameraManager::~CCameraManager()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 貼り付けるテクスチャ情報を格納する
//=============================================================================
void CCameraManager::Init(void)
{
	LoadFile("data/FILE/camera_action.txt");
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャの解放
//=============================================================================
void CCameraManager::Uninit(void)
{
	// メモリの解放
	delete[] m_pCameraAction;
	m_pCameraAction = nullptr;
}

//=============================================================================
// ファイルの読み込み
// Author : 唐﨑結斗
// 概要 : ファイルの読み込み
//=============================================================================
void CCameraManager::LoadFile(const char *pFileName)
{
	// 変数宣言
	char aStr[128];
	int nCntSetAction = 0;

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
					{// 一列読み込む
						fgets(&aStr[0], sizeof(aStr), pFile);
					}

					if (strstr(&aStr[0], "POSV") != NULL)
					{// モデルのファイル名の設定
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%f", &m_posV.x);
						fscanf(pFile, "%f", &m_posV.y);
						fscanf(pFile, "%f", &m_posV.z);
					}

					if (strstr(&aStr[0], "POSR") != NULL)
					{// モデルのファイル名の設定
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%f", &m_posR.x);
						fscanf(pFile, "%f", &m_posR.y);
						fscanf(pFile, "%f", &m_posR.z);
					}

					if (strstr(&aStr[0], "ROT") != NULL)
					{// モデルのファイル名の設定
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
					{// 一列読み込む
						fgets(&aStr[0], sizeof(aStr), pFile);
					}

					if (strstr(&aStr[0], "POSV") != NULL)
					{// モデルのファイル名の設定
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%f", &m_pCameraAction[nCntSetAction].posVDest.x);
						fscanf(pFile, "%f", &m_pCameraAction[nCntSetAction].posVDest.y);
						fscanf(pFile, "%f", &m_pCameraAction[nCntSetAction].posVDest.z);
					}

					if (strstr(&aStr[0], "POSR") != NULL)
					{// モデルのファイル名の設定
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%f", &m_pCameraAction[nCntSetAction].posRDest.x);
						fscanf(pFile, "%f", &m_pCameraAction[nCntSetAction].posRDest.y);
						fscanf(pFile, "%f", &m_pCameraAction[nCntSetAction].posRDest.z);
					}

					if (strcmp(&aStr[0], "FRAME") == 0)
					{// キー数の読み込み
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

