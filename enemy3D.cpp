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
#include "sound.h"
#include "score.h"
#include "bullet3D.h"
#include "follow_bullet3D.h"
#include "game.h"
#include "motion_player3D.h"
#include "particle.h"
#include "effect3D.h"

//*****************************************************************************
// 静的メンバ変数の定義
//*****************************************************************************
const float CEnemy3D::LINTERN_BULLET_SPAWN_RANGE = 30.0f;				// 返し弾の生成範囲
const float CEnemy3D::LINTERN_BULLET_MOVE_VEC = 0.15f;					// 返し弾の移動方向
int CEnemy3D::s_nCntSetEnemy = 0;										// 敵のカウント

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : 3Dエネミーを生成する
//=============================================================================
CEnemy3D * CEnemy3D::Create(const int nNumModel)
{
	// オブジェクトインスタンス
	CEnemy3D *pEnemy3D = nullptr;

	// メモリの解放
	pEnemy3D = new CEnemy3D;

	// メモリの確保ができなかった
	assert(pEnemy3D != nullptr);

	// 数値の初期化
	pEnemy3D->Init(nNumModel);

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

	m_pEffect3D = nullptr;								// エフェクトのインスタンス
	m_moveMode = MODE_NONE;								// 移動モード
	m_shotMode = SHOTMODE_NONE;							// 弾の種別
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 移動量
	m_moveData = {};									// 移動情報
	m_fSpeed = 10.0f;									// 移動速度
	m_fMoveVec = 0.0f;									// 移動方向
	m_fAddMoveVec = 0.0f;								// 移動方向の加算値
	m_fWave = 0.0f;										// 波
	m_fWaveLength = 0.0f;								// 波長
	m_fWaveSize = 0.0f;									// 波の大きさ
	m_fBulletRot = 0.0f;								// 弾の発射方向
	m_fDiffusionWidth = 0.0f;							// 弾の拡散範囲
	m_fBulletSpeed = 0.0f;								// 弾速
	m_fCoeffBullet = 0.0f;								// 弾の追従の減衰係数
	m_nMaxBullet = 0;									// 弾の発射数
	m_nCntKey = 0;										// キーカウント
	m_nCntFrame = 0;									// フレームカウント
	m_nLife = 0;										// 体力
	m_nScore = 0;										// スコア
	m_nCntShot = 0;										// 弾発射までのカウント
	m_nMaxShot = 0;										// 弾発射するカウント
	m_bMove = false;									// 移動を行っている
	m_bUse = false;										// 使用されているかどうか

	// 疑似乱数の初期化
	srand((unsigned int)time(NULL));

	// 設置した敵の数をインクリメント
	s_nCntSetEnemy++;
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CEnemy3D::~CEnemy3D()
{
	// 設置した敵の数をデクリメント
	s_nCntSetEnemy--;
}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CEnemy3D::Init(const int nNumModel)
{
	// モデルの初期化
	CModel3D::Init(nNumModel);

	// あたり判定の設定
	SetColisonPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetColisonSize(D3DXVECTOR3(50.0f, 50.0f, 50.0f));

	// エフェクトの設定
	m_pEffect3D = CEffect3D::Create();
	m_pEffect3D->SetPos(GetPos());
	m_pEffect3D->SetSize(GetColisonSize());
	m_pEffect3D->SetLife(-1);
	m_pEffect3D->SetRenderMode(CEffect3D::MODE_ADD);
	m_pEffect3D->LoadTex(27);

	// 体力
	m_nLife = 100;

	// スコア
	m_nScore = 100;

	// 弾発射するカウント
	m_nMaxShot = 180;

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
	}
	else
	{
		m_moveMode = MODE_NONE;
	}

	// 移動量の設定
	SetMove();

	// 弾の発射
	Shot();

	// 移動
	pos += m_move;

	// 位置の設定
	SetPos(pos);
	m_pEffect3D->SetPos(pos);
	m_pEffect3D->SetSize(GetColisonSize());

	if (GetColorType() == CObject::TYPE_WHITE)
	{
		m_pEffect3D->SetColor(D3DXCOLOR(0.5f, 0.7f, 1.0f, 0.5f));
	}
	else if (GetColorType() == CObject::TYPE_BLACK)
	{
		m_pEffect3D->SetColor(D3DXCOLOR(1.0f, 0.1f, 0.1f, 0.5f));
	}

	// スクリーン内の判定
	ScreenIn();

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
{
	// サウンド情報の取得
	CSound *pSound = CApplication::GetSound();

	if (m_bUse)
	{// 変数宣言
		COLOR_TYPE MyColorType = GetColorType();
		int nMyAttack = nAttack;

		pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT);

		if (MyColorType != colorType)
		{// 色のタイプが同一の場合
			nMyAttack *= 2;
		}

		// 体力の減算
		m_nLife -= nMyAttack;

		if (m_nLife <= 0)
		{// 体力が0の場合
			m_nLife = 0;

			if (CApplication::GetMode() == CApplication::MODE_GAME)
			{
				CScore *pScore = CGame::GetScore();
				pScore->AddScore(m_nScore);
			}

			pSound->PlaySound(CSound::SOUND_LABEL_SE_EXPLOSION);
			m_pEffect3D->Uninit();

			// データ格納用変数
			CBullet3D * pBullet3D;
			D3DXVECTOR3 pos = GetPos();
			D3DXVECTOR3 rot = GetRot();
			D3DXVECTOR3 randLinternBulletSpawnRange = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			float fRandMoveVec = 0.0f;

			// パーティクルの生成
			CParticle *pParticle = CParticle::Create();
			pParticle->SetPos(pos);
			pParticle->SetSize(D3DXVECTOR3(40.0f, 40.0f, 0.0f));
			pParticle->SetPopRange(D3DXVECTOR3(3.0f, 3.0f, 3.0f));
			pParticle->SetSpeed(5.0f);
			pParticle->SetEffectLife(30);
			pParticle->SetMoveVec(D3DXVECTOR3(D3DX_PI * 2.0f, D3DX_PI * 2.0f, 0.0f));
			pParticle->SetLife(10);
			pParticle->SetColor(D3DXCOLOR(1.0f, 0.4f, 0.1f, 1.0f));
			pParticle->SetMaxEffect(3);

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
	// サウンド情報の取得
	CSound *pSound = CApplication::GetSound();

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

	if (m_nCntShot % m_nMaxShot == 0)
	{
		pSound->PlaySound(CSound::SOUND_LABEL_SE_SHOT);

		// 計算用変数の設定
		CBullet3D * pBullet3D = nullptr;
		CFollowBullet3D* pFollowBullet3D = nullptr;
		CMotionPlayer3D* pPlayer = nullptr;
		D3DXVECTOR3 targetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 diffPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 diffRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		float fBulletRot = 0.0f;

		switch (m_shotMode)
		{
		case CEnemy3D::SHOTMODE_NORMAL:
			// 弾の生成
			pBullet3D = CBullet3D::Create();
			pBullet3D->SetPos(GetPos());
			pBullet3D->SetSize(D3DXVECTOR3(10.0f, 10.0f, 0.0f));
			pBullet3D->SetMoveVec(D3DXVECTOR3(rot.x + D3DX_PI * -0.5f, rot.y + m_fBulletRot, 0.0f));
			pBullet3D->SetSpeed(m_fBulletSpeed);
			pBullet3D->SetColor(bulletColor);
			pBullet3D->SetColorType(GetColorType());
			pBullet3D->SetParent(CObject::OBJTYPE_3DENEMY);
			break;

		case CEnemy3D::SHOTMODE_DIFFUSION:

			for (int nCntBullet = 0; nCntBullet < m_nMaxBullet; nCntBullet++)
			{// 弾の発射角度の設定
				fBulletRot = m_fBulletRot - (m_fDiffusionWidth / 2) + (m_fDiffusionWidth / m_nMaxBullet * nCntBullet);
				fBulletRot = CApplication::RotNormalization(fBulletRot);

				// 弾の生成
				pBullet3D = CBullet3D::Create();
				pBullet3D->SetPos(GetPos());
				pBullet3D->SetSize(D3DXVECTOR3(10.0f, 10.0f, 0.0f));
				pBullet3D->SetMoveVec(D3DXVECTOR3(rot.x + D3DX_PI * -0.5f, rot.y + fBulletRot, 0.0f));
				pBullet3D->SetSpeed(m_fBulletSpeed);
				pBullet3D->SetColor(bulletColor);
				pBullet3D->SetColorType(GetColorType());
				pBullet3D->SetParent(CObject::OBJTYPE_3DENEMY);
			}

			break;

		case CEnemy3D::SHOTMODE_SNIPE:
			if (CGame::GetUsePlayer())
			{// 目的の向きの算出
				pPlayer = CGame::GetMotionPlayer3D();
				targetPos = pPlayer->GetPos();
				diffPos = targetPos - GetPos();
				diffRot.y = atan2f(diffPos.x, diffPos.z);
				diffRot.x = atan2f(sqrtf((diffPos.x * diffPos.x) + (diffPos.z * diffPos.z)), diffPos.y);
				diffRot.z = 0.0f;
			}

			// 弾の生成
			pBullet3D = CBullet3D::Create();
			pBullet3D->SetPos(GetPos());
			pBullet3D->SetSize(D3DXVECTOR3(10.0f, 10.0f, 0.0f));
			pBullet3D->SetMoveVec(diffRot);
			pBullet3D->SetSpeed(m_fBulletSpeed);
			pBullet3D->SetColor(bulletColor);
			pBullet3D->SetColorType(GetColorType());
			pBullet3D->SetParent(CObject::OBJTYPE_3DENEMY);
			break;

		case CEnemy3D::SHOTMODE_FOLLOW:
			// 弾の生成
			pFollowBullet3D = CFollowBullet3D::Create();
			pFollowBullet3D->SetPos(GetPos());
			pFollowBullet3D->SetSize(D3DXVECTOR3(10.0f, 10.0f, 0.0f));
			pFollowBullet3D->SetMoveVec(D3DXVECTOR3(rot.x + D3DX_PI * -0.5f, rot.y, 0.0f));
			pFollowBullet3D->SetSpeed(m_fBulletSpeed);
			pFollowBullet3D->SetCoefficient(m_fCoeffBullet);
			pFollowBullet3D->SetColor(bulletColor);
			pFollowBullet3D->SetColorType(GetColorType());
			pFollowBullet3D->SetParent(CObject::OBJTYPE_3DENEMY);
			break;

		case CEnemy3D::SHOTMODE_NONE:
			break;

		default:
			assert(false);
			break;
		}
		
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

	if (m_nCntFrame >= m_moveData.moveKey[m_nCntKey].nFrame)
	{
		m_nCntFrame = 0;
		m_nCntKey++;

		if (m_nCntKey >= m_moveData.nMaxKey)
		{
			if (m_moveData.bLoop)
			{
				m_nCntKey = 0;
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
	m_moveMode = m_moveData.moveKey[m_nCntKey].moveMode;				// 移動モード
	m_fMoveVec = m_moveData.moveKey[m_nCntKey].fMoveVec;				// 移動方向
	m_fAddMoveVec = m_moveData.moveKey[m_nCntKey].fAddMoveVec;			// 移動方向の加算値
	m_fWaveLength = m_moveData.moveKey[m_nCntKey].fWaveLength;			// 波長
	m_fWaveSize = m_moveData.moveKey[m_nCntKey].fWaveSize;				// 波の大きさ
	m_fSpeed = m_moveData.moveKey[m_nCntKey].fSpeed;					// 移動速度
}

//=============================================================================
// スクリーンの判定
// Author : 唐﨑結斗
// 概要 : スクリーン内にいるとき、敵を使用可能にする
//=============================================================================
void CEnemy3D::ScreenIn()
{
	// 位置の取得
	D3DXVECTOR3 pos = GetPos();

	// 当たり判定の取得
	D3DXVECTOR3 collision = CObject::GetColisonSize();

	// スクリーン座標にキャスト
	D3DXVECTOR3 screenPos = CApplication::ScreenCastWorld(pos);

	if (screenPos.x >= 0.0f
		&& screenPos.x <= (float)CRenderer::SCREEN_WIDTH
		&& screenPos.y >= 0.0f
		&& screenPos.y <= (float)CRenderer::SCREEN_HEIGHT
		&& !m_bUse)
	{
		m_bUse = true;
	}
	else if ((screenPos.x + collision.x <= 0.0f
		|| screenPos.x - collision.x >= (float)CRenderer::SCREEN_WIDTH
		|| screenPos.y + collision.y <= 0.0f
		|| screenPos.y - collision.y >= (float)CRenderer::SCREEN_HEIGHT)
		&& m_bUse)
	{
		m_pEffect3D->Uninit();
		Uninit();
	}
}
