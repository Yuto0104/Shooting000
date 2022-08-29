//=============================================================================
//
// 3Dエネミークラス(enemy3D.cpp)
// Author : 唐﨑結斗
// 概要 : 3Dエネミー生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>
#include <time.h>

#include "enemy3D.h"
#include "renderer.h"
#include "application.h"
#include "score.h"
#include "bullet3D.h"
#include "game.h"

//*****************************************************************************
// 静的メンバ変数の定義
//*****************************************************************************
const float CEnemy3D::LINTERN_BULLET_SPAWN_RANGE = 30.0f;				// 返し弾の生成範囲
const float CEnemy3D::LINTERN_BULLET_MOVE_VEC = 0.15f;					// 返し弾の移動方向

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : 3Dエネミーを生成する
//=============================================================================
CEnemy3D * CEnemy3D::Create(const char *pName)
{
	// オブジェクトインスタンス
	CEnemy3D *pEnemy3D = nullptr;

	// メモリの解放
	pEnemy3D = new CEnemy3D;

	// メモリの確保ができなかった
	assert(pEnemy3D != nullptr);

	// 数値の初期化
	pEnemy3D->Init(pName);

	// インスタンスを返す
	return pEnemy3D;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CEnemy3D::CEnemy3D()
{// オブジェクトの種別設定
	SetObjType(CObject::OBJTYPE_3DENEMY);

	m_moveMode = MODE_NONE;								// 移動モード
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 移動量
	m_moveData = {};									// 移動情報
	m_fSpeed = 0.0f;									// 移動速度
	m_fMoveVec = 0.0f;									// 移動方向
	m_fAddMoveVec = 0.0f;								// 移動方向の加算値
	m_fWave = 0.0f;										// 波
	m_fWaveLength = 0.0f;								// 波長
	m_fWaveSize = 0.0f;									// 波の大きさ
	m_CntKey = 0;										// キーカウント
	m_nCntFrame = 0;									// フレームカウント
	m_nLife = 0;										// 体力
	m_nScore = 0;										// スコア
	m_nCntShot = 0;										// 弾発射までのカウント
	m_bMove = false;									// 移動を行っている

	// 疑似乱数の初期化
	srand((unsigned int)time(NULL));
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CEnemy3D::~CEnemy3D()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CEnemy3D::Init(const char * pName)
{
	// モデルの初期化
	CModel3D::Init(pName);

	// あたり判定の設定
	SetColisonSize(D3DXVECTOR3(50.0f, 50.0f, 50.0f));

	// 体力
	m_nLife = 100;

	// スコア
	m_nScore = 100;

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CEnemy3D::Uninit()
{
	// モデルの終了
	CModel3D::Uninit();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 2D更新を行う
//=============================================================================
void CEnemy3D::Update()
{
	// 変数宣言
	D3DXVECTOR3 pos = GetPos();

	if (m_bMove)
	{
		// 移動情報の設定
		Move();

		// 移動情報のコピー
		SetMoveCopy();

		// 移動量の設定
		SetMove();
	}
	else
	{
		m_moveMode = MODE_NONE;
	}

	// 弾の発射
	Shot();

	// 移動
	pos += m_move;

	// 位置の設定
	SetPos(pos);

	// モデルの更新
	CModel3D::Update();
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 2D描画を行う
//=============================================================================
void CEnemy3D::Draw()
{
	// モデルの描画
	CModel3D::Draw();
}

//=============================================================================
// ヒット
// Author : 唐﨑結斗
// 概要 : 引数分のダメージを与える
//=============================================================================
void CEnemy3D::Hit(COLOR_TYPE colorType, int nAttack)
{// 変数宣言
	COLOR_TYPE MyColorType = GetColorType();
	int nMyAttack = nAttack;

	if (MyColorType == colorType)
	{// 色のタイプが同一の場合
		nMyAttack *= 2;
	}

	// 体力の減算
	m_nLife -= nMyAttack;

	if (m_nLife <= 0)
	{// 体力が0の場合
		m_nLife = 0;
		CScore *pScore = CGame::GetScore();
		pScore->AddScore(m_nScore);

		// データ格納用変数
		CBullet3D * pBullet3D;
		D3DXVECTOR3 pos = GetPos();
		D3DXVECTOR3 rot = GetRot();
		D3DXVECTOR3 randLinternBulletSpawnRange = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		float fRandMoveVec = 0.0f;

		// 弾の色
		D3DXCOLOR bulletColor;

		if (MyColorType == CObject::TYPE_WHITE)
		{// 弾の色の設定
			bulletColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else if (MyColorType == CObject::TYPE_BLACK)
		{// 弾の色の設定
			bulletColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		}

		for (int nCntBullet = 0; nCntBullet < MAX_LINTERN_BULLET; nCntBullet++)
		{// 乱数の設定
			randLinternBulletSpawnRange.x = pos.x - LINTERN_BULLET_SPAWN_RANGE + ((float)(rand() % (int)(LINTERN_BULLET_SPAWN_RANGE * 200.0f) / 100));
			randLinternBulletSpawnRange.z = pos.z - LINTERN_BULLET_SPAWN_RANGE + ((float)(rand() % (int)(LINTERN_BULLET_SPAWN_RANGE * 200.0f) / 100));
			fRandMoveVec = rot.y - D3DX_PI * LINTERN_BULLET_MOVE_VEC + D3DX_PI * ((float)(rand() % (int)(LINTERN_BULLET_MOVE_VEC * 200.0f)) / 100);

			// 弾の生成
			pBullet3D = CBullet3D::Create();
			pBullet3D->SetPos(D3DXVECTOR3(randLinternBulletSpawnRange.x, pos.y, randLinternBulletSpawnRange.z));
			pBullet3D->SetSize(D3DXVECTOR3(5.0f, 5.0f, 0.0f));
			pBullet3D->SetMoveVec(D3DXVECTOR3(rot.x + D3DX_PI * -0.5f, fRandMoveVec, 0.0f));
			pBullet3D->SetSpeed(5.0f);
			pBullet3D->SetColor(bulletColor);
			pBullet3D->SetColorType(MyColorType);
			pBullet3D->SetParent(CObject::OBJTYPE_3DENEMY);
		}

		Uninit();
	}
}

//=============================================================================
// 移動情報のセッター
// Author : 唐﨑結斗
// 概要 : 移動情報のセッター
//=============================================================================
void CEnemy3D::SetMoveData(ENEMY_MOVE moveData)
{
	m_moveData = moveData;
	m_bMove = true;
}

//=============================================================================
// 弾の発射
// Author : 唐﨑結斗
// 概要 : 弾を発射する
//=============================================================================
void CEnemy3D::Shot()
{
	D3DXVECTOR3 rot = GetRot();
	D3DXCOLOR bulletColor;					// 弾の色

	if (GetColorType() == CObject::TYPE_WHITE)
	{// 弾の色の設定
		bulletColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (GetColorType() == CObject::TYPE_BLACK)
	{// 弾の色の設定
		bulletColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	}

	m_nCntShot++;

	if (m_nCntShot % 180 == 0)
	{
		// 弾の生成
		CBullet3D * pBullet3D = CBullet3D::Create();
		pBullet3D->SetPos(GetPos());
		pBullet3D->SetSize(D3DXVECTOR3(10.0f, 10.0f, 0.0f));
		pBullet3D->SetMoveVec(D3DXVECTOR3(rot.x + D3DX_PI * -0.5f, rot.y, 0.0f));
		pBullet3D->SetSpeed(10.0f);
		pBullet3D->SetColor(bulletColor);
		pBullet3D->SetColorType(GetColorType());
		pBullet3D->SetParent(CObject::OBJTYPE_3DENEMY);
	}
}

//=============================================================================
// 移動量の設定
// Author : 唐﨑結斗
// 概要 : 移動量の設定
//=============================================================================
void CEnemy3D::SetMove()
{
	switch (m_moveMode)
	{
	case CEnemy3D::MODE_NONE:
		m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		break;

	case CEnemy3D::MODE_NORMAL:
		m_move.x = sinf(m_fMoveVec) * m_fSpeed;
		m_move.z = cosf(m_fMoveVec) * m_fSpeed;
		break;

	case CEnemy3D::MODE_SIN:
		m_fWave += m_fWaveLength;
		m_move.x = sinf(m_fMoveVec) * m_fSpeed;
		m_move.z = sinf(m_fWave) * m_fWaveSize;
		break;

	case CEnemy3D::MODE_COS:
		m_fWave += m_fWaveLength;
		m_move.x = cosf(m_fWave) * m_fWaveSize;
		m_move.z = cosf(m_fMoveVec) * m_fSpeed;
		break;

	case CEnemy3D::MODE_CIRCLE:
		m_fMoveVec += m_fAddMoveVec;
		m_fMoveVec = CApplication::RotNormalization(m_fMoveVec);
		m_move.x = sinf(m_fMoveVec) * m_fSpeed;
		m_move.z = cosf(m_fMoveVec) * m_fSpeed;
		break;

	default:
		break;
	}
}

//=============================================================================
// 移動情報の設定
// Author : 唐﨑結斗
// 概要 : 移動情報の設定
//=============================================================================
void CEnemy3D::Move()
{
	// 移動フレームのインクリメント
	m_nCntFrame++;

	if (m_nCntFrame >= m_moveData.moveKey[m_CntKey].nFrame)
	{
		m_nCntFrame = 0;
		m_CntKey++;

		if (m_CntKey >= m_moveData.nMaxKey)
		{
			if (m_moveData.bLoop)
			{
				m_CntKey = 0;
			}
			else
			{
				m_bMove = false;
			}
		}
	}
}

//=============================================================================
// 移動情報のコピー
// Author : 唐﨑結斗
// 概要 : 移動情報のコピー
//=============================================================================
void CEnemy3D::SetMoveCopy()
{
	m_moveMode = m_moveData.moveKey[m_CntKey].moveMode;				// 移動モード
	m_fMoveVec = m_moveData.moveKey[m_CntKey].fMoveVec;				// 移動方向
	m_fAddMoveVec = m_moveData.moveKey[m_CntKey].fAddMoveVec;		// 移動方向の加算値
	m_fWaveLength = m_moveData.moveKey[m_CntKey].fWaveLength;		// 波長
	m_fWaveSize = m_moveData.moveKey[m_CntKey].fWaveSize;			// 波の大きさ
	m_fSpeed = m_moveData.moveKey[m_CntKey].fSpeed;					// 移動速度
}
