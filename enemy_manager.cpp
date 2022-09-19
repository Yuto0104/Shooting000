//=============================================================================
//
// エネミーマネージャー処理(enemy_manager.h)
// Author : 唐﨑結斗
// 概要 : エネミーマネージャーを行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <stdio.h>
#include <assert.h>

#include "enemy_manager.h"
#include "renderer.h"
#include "application.h"
#include "game.h"

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : スコアを生成する
//=============================================================================
CEnemyManager *CEnemyManager::Create()
{
	// オブジェクトインスタンス
	CEnemyManager *pEnemyManager = nullptr;

	pEnemyManager = new CEnemyManager;

	// メモリの確保ができなかった
	assert(pEnemyManager != nullptr);

	// 初期化
	pEnemyManager->Init();

	// インスタンスを返す
	return pEnemyManager;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CEnemyManager::CEnemyManager()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 過去の位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 向き
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 大きさ
	m_status = nullptr;									// 敵のステータス
	m_arrangement = nullptr;							// 敵の配置
	m_move = nullptr;									// 移動情報
	m_nMaxType = 0;										// 種類の最大数
	m_nMaxInstall = 0;									// 最大設置数
	m_nMaxMove = 0;										// 移動情報数
	m_nCntFrame = 0;									// フレームカウント
	m_nGameEndFrame = 0;								// ゲームの終了時間
	m_nCntSetEnemy = 0;									// 敵の設置カウント
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CEnemyManager::~CEnemyManager()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CEnemyManager::Init()
{
	// ファイルの読み込み
	LoadFile("data/FILE/enemy.txt");

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CEnemyManager::Uninit()
{
	if (m_status != nullptr)
	{// 終了処理
	 // メモリの解放
		delete m_status;
		m_status = nullptr;
	}

	if (m_arrangement != nullptr)
	{// 終了処理
	 // メモリの解放
		delete m_arrangement;
		m_arrangement = nullptr;
	}

	if (m_move != nullptr)
	{// 終了処理
	 // メモリの解放
		delete m_move;
		m_move = nullptr;
	}

	// スコアの解放
	Release();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 更新を行う
//=============================================================================
void CEnemyManager::Update()
{
	if (m_arrangement != nullptr)
	{// フレームカウントのインクリメント
		m_nCntFrame++;

		// 敵の配置
		SetEnemy();

		// ゲーム終了
		if (m_nCntFrame >= m_nGameEndFrame)
		{
			CGame::SetGame(false);
		}
	}	
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 描画を行う
//=============================================================================
void CEnemyManager::Draw()
{

}

//=============================================================================
// ファイルの読み込み
// Author : 唐﨑結斗
// 概要 : ファイルを読み込む
//=============================================================================
void CEnemyManager::LoadFile(char *pFileName)
{
	// 変数宣言
	char aStr[128];
	int nCntStatus = 0;
	int nCntArrangement = 0;
	int nCntMove = 0;
	int nCntMoveKey = 0;

	// ファイルの読み込み
	FILE *pFile = fopen(pFileName, "r");

	if (pFile != nullptr)
	{
		while (fscanf(pFile, "%s", &aStr[0]) != EOF)
		{// "EOF"を読み込むまで 

			if (strstr(&aStr[0], "MAX_TYPE") != NULL)
			{
				fscanf(pFile, "%s", &aStr[0]);
				fscanf(pFile, "%d", &m_nMaxType);
				m_status = new ENEMY_STATUS[m_nMaxType];
				assert(m_status != nullptr);
				memset(&m_status[0], 0, sizeof(m_status));
			}

			if (strstr(&aStr[0], "GAME_END_FRAME") != NULL)
			{
				fscanf(pFile, "%s", &aStr[0]);
				fscanf(pFile, "%d", &m_nGameEndFrame);
			}

			if (strstr(&aStr[0], "MAX_INSTALL") != NULL)
			{
				fscanf(pFile, "%s", &aStr[0]);
				fscanf(pFile, "%d", &m_nMaxInstall);

				if (m_arrangement != nullptr)
				{// 終了処理
				 // メモリの解放
					delete m_arrangement;
					m_arrangement = nullptr;
				}

				m_arrangement = new ENEMY_ARRANGEMENT[m_nMaxInstall];
				assert(m_arrangement != nullptr);
				memset(&m_arrangement[0], 0, sizeof(m_arrangement));
			}

			if (strstr(&aStr[0], "MAX_MOVE") != NULL)
			{
				fscanf(pFile, "%s", &aStr[0]);
				fscanf(pFile, "%d", &m_nMaxMove);
				m_move = new CEnemy3D::ENEMY_MOVE[m_nMaxMove];
				assert(m_move != nullptr);
				memset(&m_move[0], 0, sizeof(m_move));
			}

			if (strstr(&aStr[0], "STATUS_SET") != NULL)
			{
				while (strstr(&aStr[0], "STATUS_END") == NULL)
				{
					fscanf(pFile, "%s", &aStr[0]);

					if (strstr(&aStr[0], "SCORE") != NULL)
					{// モデルのファイル名の設定
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%d", &m_status[nCntStatus].nScore);
					}

					if (strstr(&aStr[0], "COLLISION_SIZE") != NULL)
					{// モデルのファイル名の設定
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%f", &m_status[nCntStatus].collisionSize.x);
						fscanf(pFile, "%f", &m_status[nCntStatus].collisionSize.y);
						fscanf(pFile, "%f", &m_status[nCntStatus].collisionSize.z);
					}

					if (strstr(&aStr[0], "SIZE") != NULL)
					{// モデルのファイル名の設定
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%f", &m_status[nCntStatus].size.x);
						fscanf(pFile, "%f", &m_status[nCntStatus].size.y);
						fscanf(pFile, "%f", &m_status[nCntStatus].size.z);
					}

					if (strstr(&aStr[0], "COLOR") != NULL)
					{// カラータイプの設定
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%s", &aStr[0]);

						if (strstr(&aStr[0], "BLACK") != NULL)
						{
							m_status[nCntStatus].colorType = CObject::TYPE_BLACK;
						}
						else if(strstr(&aStr[0], "WHITE") != NULL)
						{
							m_status[nCntStatus].colorType = CObject::TYPE_WHITE;
						}
					}

					if (strstr(&aStr[0], "MODEL_ID") != NULL)
					{// モデルのファイル名の設定
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%d", &m_status[nCntStatus].nModelID);
					}

					if (strstr(&aStr[0], "LIFE") != NULL)
					{// モデルのファイル名の設定
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%d", &m_status[nCntStatus].nLife);
					}

					if (strstr(&aStr[0], "SCORE") != NULL)
					{// モデルのファイル名の設定
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%d", &m_status[nCntStatus].nScore);
					}

					if (strstr(&aStr[0], "CNTSHOT") != NULL)
					{// モデルのファイル名の設定
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%d", &m_status[nCntStatus].nMaxShot);
					}

					if (strstr(&aStr[0], "SHOT_ID") != NULL)
					{// モデルのファイル名の設定
						int nShotID = 0;
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%d", &nShotID);
						m_status[nCntStatus].shotMode = (CEnemy3D::SHOT_MODE)nShotID;
					}

					if (strstr(&aStr[0], "SHOT_SPEED") != NULL)
					{// モデルのファイル名の設定
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%f", &m_status[nCntStatus].fBulletSpeed);
					}

					if (strstr(&aStr[0], "SHOT_ROT") != NULL)
					{// モデルのファイル名の設定
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%f", &m_status[nCntStatus].fBulletRot);
					}

					if (strstr(&aStr[0], "DIFFUSIONWIDTH") != NULL)
					{// モデルのファイル名の設定
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%f", &m_status[nCntStatus].fDiffusionWidth);
					}

					if (strstr(&aStr[0], "MAXBULLET") != NULL)
					{// モデルのファイル名の設定
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%d", &m_status[nCntStatus].nMaxBullet);
					}

					if (strstr(&aStr[0], "COEFFBULLET") != NULL)
					{// モデルのファイル名の設定
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%f", &m_status[nCntStatus].fCoeffBullet);
					}
				}

				nCntStatus++;
			}

			if (strstr(&aStr[0], "ARRANGEMENT_SET") != NULL)
			{
				while (strstr(&aStr[0], "ARRANGEMENT_END") == NULL)
				{
					fscanf(pFile, "%s", &aStr[0]);

					if (strstr(&aStr[0], "POS") != NULL)
					{// カラータイプの設定
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%f", &m_arrangement[nCntArrangement].pos.x);
						fscanf(pFile, "%f", &m_arrangement[nCntArrangement].pos.y);
						fscanf(pFile, "%f", &m_arrangement[nCntArrangement].pos.z);
					}

					if (strstr(&aStr[0], "MOVE_ID") != NULL)
					{// モデルのファイル名の設定
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%d", &m_arrangement[nCntArrangement].nMoveID);
					}

					if (strstr(&aStr[0], "FRAME") != NULL)
					{// モデルのファイル名の設定
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%d", &m_arrangement[nCntArrangement].nFrame);
					}

					if (strstr(&aStr[0], "ID") != NULL)
					{// モデルのファイル名の設定
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%d", &m_arrangement[nCntArrangement].nID);
					}

					if (strstr(&aStr[0], "DRAW_TYPE") != NULL)
					{// モデルのファイル名の設定
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%d", &m_arrangement[nCntArrangement].nDrawType);
					}
				}

				nCntArrangement++;
			}

			if (strstr(&aStr[0], "MOVE_SET") != NULL)
			{
				while (strstr(&aStr[0], "MOVE_END") == NULL)
				{
					fscanf(pFile, "%s", &aStr[0]);

					if (strstr(&aStr[0], "MAX_KEY") != NULL)
					{// モデルのファイル名の設定
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%d", &m_move[nCntMove].nMaxKey);

						m_move[nCntMove].moveKey = new CEnemy3D::ENEMY_MOVE_KEY[m_move[nCntMove].nMaxKey];
						assert(m_move[nCntMove].moveKey != nullptr);
						memset(&m_move[nCntMove].moveKey[0], 0, sizeof(m_move[nCntMove].moveKey));
					}

					if (strstr(&aStr[0], "LOOP") != NULL)
					{// モデルのファイル名の設定
						int nBool = 0;
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%d", &nBool);

						if (nBool == 0)
						{
							m_move[nCntMove].bLoop = false;
						}
						else
						{
							m_move[nCntMove].bLoop = true;
						}
					}
					
					if (strstr(&aStr[0], "KEY_SET") != NULL)
					{
						while (strstr(&aStr[0], "KEY_END") == NULL)
						{
							fscanf(pFile, "%s", &aStr[0]);

							if (strstr(&aStr[0], "FRAME") != NULL)
							{// モデルのファイル名の設定
								fscanf(pFile, "%s", &aStr[0]);
								fscanf(pFile, "%d", &m_move[nCntMove].moveKey[nCntMoveKey].nFrame);
							}

							if (strstr(&aStr[0], "MOVE_MODE") != NULL)
							{// モデルのファイル名の設定
								fscanf(pFile, "%s", &aStr[0]);
								fscanf(pFile, "%d", &m_move[nCntMove].moveKey[nCntMoveKey].moveMode);
							}
							
							if (strcmp(&aStr[0], "MOVE_VEC") == 0)
							{// キー数の読み込み
								fscanf(pFile, "%s", &aStr[0]);
								fscanf(pFile, "%f", &m_move[nCntMove].moveKey[nCntMoveKey].fMoveVec);
							}

							if (strstr(&aStr[0], "SPEED") != NULL)
							{// モデルのファイル名の設定
								fscanf(pFile, "%s", &aStr[0]);
								fscanf(pFile, "%f", &m_move[nCntMove].moveKey[nCntMoveKey].fSpeed);
							}

							if (strstr(&aStr[0], "WAVE_LENGTH") != NULL)
							{// モデルのファイル名の設定
								fscanf(pFile, "%s", &aStr[0]);
								fscanf(pFile, "%f", &m_move[nCntMove].moveKey[nCntMoveKey].fWaveLength);
							}

							if (strstr(&aStr[0], "WAVE_SIZE") != NULL)
							{// モデルのファイル名の設定
								fscanf(pFile, "%s", &aStr[0]);
								fscanf(pFile, "%f", &m_move[nCntMove].moveKey[nCntMoveKey].fWaveSize);
							}

							if (strcmp(&aStr[0], "ADD_MOVE_VEC") == 0)
							{// キー数の読み込み
								fscanf(pFile, "%s", &aStr[0]);
								fscanf(pFile, "%f", &m_move[nCntMove].moveKey[nCntMoveKey].fAddMoveVec);
							}
						}

						nCntMoveKey++;
					}
				}

				nCntMove++;
				nCntMoveKey = 0;
			}
		}
	}
	else
	{
		assert(false);
	}
}

//=============================================================================
// 敵の設置
// Author : 唐﨑結斗
// 概要 : 敵の設置を行う
//=============================================================================
void CEnemyManager::SetEnemy()
{
	CEnemy3D* pEnemy = nullptr;

	for (int nCntEnemy = 0; nCntEnemy < m_nMaxInstall; nCntEnemy++)
	{
		if (m_arrangement[nCntEnemy].nFrame == m_nCntFrame)
		{
			int nID = m_arrangement[nCntEnemy].nID;

			pEnemy = CEnemy3D::Create(m_status[nID].nModelID);
			pEnemy->SetColisonSize(m_status[nID].collisionSize);
			pEnemy->SetColorType(m_status[nID].colorType);
			pEnemy->SetSize(m_status[nID].size);
			pEnemy->SetLife(m_status[nID].nLife);
			pEnemy->SetScore(m_status[nID].nScore);
			pEnemy->SetMaxShot(m_status[nID].nMaxShot);
			pEnemy->SetShotMode(m_status[nID].shotMode);
			pEnemy->SetBulletSpeed(m_status[nID].fBulletSpeed);
			pEnemy->SetBulletRot(m_status[nID].fBulletRot);
			pEnemy->SetDiffusionWidth(m_status[nID].fDiffusionWidth);
			pEnemy->SetCoeffBullet(m_status[nID].fCoeffBullet);
			pEnemy->SetMaxBullet(m_status[nID].nMaxBullet);
			pEnemy->SetPos(m_arrangement[nCntEnemy].pos);
			pEnemy->SetObjectDrowType((EObjectDrowType)m_arrangement[nCntEnemy].nDrawType);
			pEnemy->SetMoveData(m_move[m_arrangement[nCntEnemy].nMoveID]);
			m_nCntSetEnemy++;
		}
	}	
}

