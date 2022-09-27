//=============================================================================
//
// モーションボスクラス(motion_boss.cpp)
// Author : 唐﨑結斗
// 概要 : モーションボス生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "motion_boss.h"
#include "motion.h"
#include "renderer.h"
#include "application.h"
#include "game.h"
#include "sound.h"
#include "particle.h"
#include "score.h"
#include "bullet3D.h"

//*****************************************************************************
// 定数定義
//*****************************************************************************

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : 3Dモーションプレイヤーを生成する
//=============================================================================
CMotionBoss * CMotionBoss::Create()
{
	// オブジェクトインスタンス
	CMotionBoss *pMotionBoss = nullptr;

	// メモリの解放
	pMotionBoss = new CMotionBoss;

	// メモリの確保ができなかった
	assert(pMotionBoss != nullptr);

	// 数値の初期化
	pMotionBoss->Init();

	// インスタンスを返す
	return pMotionBoss;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CMotionBoss::CMotionBoss()
{
	m_pMotion = nullptr;						// モーションインスタンス
	m_actionType = ACTION_NONE;					// アクションタイプ
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
	m_fMoveVec = 0.0f;							// 移動方向
	m_fWave = 0.0f;								// 波
	m_fSpeed = 0.0f;							// 移動速度
	m_fAddRot = 0.0f;							// 角度の加算
	m_nNumMotion = 0;							// 現在のモーション番号
	m_nNumMotionOld = 0;						// 過去のモーション番号
	m_nLife = 0;								// 寿命
	m_nScore = 0;								// スコア
	m_nCntShot = 0;								// 弾発射までのカウント
	m_nMaxShot = 0;								// 弾発射するカウント
	m_nCntFrame = 0;							// フレームカウント
	m_bMotion = false;							// モーションの使用状況
	m_bMotionBlend = false;						// モーションブレンドの使用状況
	m_bUse = false;								// 使用状況
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CMotionBoss::~CMotionBoss()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CMotionBoss::Init()
{
	// 初期化
	CModel3D::Init();

	// モーション情報
	m_pMotion = new CMotion("data/MOTION/boss.txt");
	assert(m_pMotion != nullptr);

	m_bUse = true;
	SetColisonPos(D3DXVECTOR3(0.0f, 0.0f, 100.0f));
	SetColisonSize(D3DXVECTOR3(100.0f, 100.0f, 100.0f));
	SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f));
	SetSize(D3DXVECTOR3(1.2f, 1.2f, 1.2f));
	SetColorType(CObject::TYPE_WHITE);
	m_nLife = 10000;
	m_actionType = ACTION_NONE;
	m_fMoveVec = D3DX_PI * 0.5f;	
	m_fSpeed = 2.0f;	

	SetObjType(CObject::OBJTYPE_3DBOSS);

	return E_NOTIMPL;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CMotionBoss::Uninit()
{
	if (m_pMotion != nullptr)
	{// 終了処理
		m_pMotion->Uninit();

		// メモリの解放
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	// 終了
	CModel3D::Uninit();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 更新を行う
//=============================================================================
void CMotionBoss::Update()
{
	// 情報の取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	// ニュートラルモーションの入力
	m_nNumMotion = TYPE_NEUTRAL;

	// 弾の発射
	Shot();

	// 移動
	Move();

	// モーションの設定
	MotionSet();

	// 位置の設定
	SetPos(pos + m_move);

	if (CollisionScreen())
	{// スクリーン当たり判定
		m_fMoveVec *= -1;
	}

	// 更新
	CModel3D::Update();
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 描画を行う
//=============================================================================
void CMotionBoss::Draw()
{
	// 描画
	CModel3D::Draw();

	// ワールドマトリックスの取得
	D3DXMATRIX mtxWorld = GetMtxWorld();

	// パーツの描画設定
	m_pMotion->SetParts(mtxWorld);
}

//=============================================================================
// 弾の発射
// Author : 唐﨑結斗
// 概要 : キー入力が行われた場合、弾を発射する
//=============================================================================
void CMotionBoss::Shot()
{
	if (m_actionType == ACTION_DIFFUSIONSHOT)
	{
		m_fAddRot += 0.01f;

		// サウンド情報の取得
		CSound *pSound = CApplication::GetSound();

		D3DXVECTOR3 pos = GetPos();
		D3DXVECTOR3 rot = GetRot();
		D3DXCOLOR bulletColor;					// 弾の色

		if (GetColorType() == CObject::TYPE_WHITE)
		{// 弾の色の設定
			bulletColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			m_nNumMotion = TYPE_ATTACK2;
		}
		else if (GetColorType() == CObject::TYPE_BLACK)
		{// 弾の色の設定
			bulletColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			m_nNumMotion = TYPE_ATTACK1;
		}

		m_nCntShot++;

		if (m_nCntShot % 10 == 0)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE_SHOT);

			// 計算用変数の設定
			CBullet3D * pBullet3D = nullptr;
			D3DXVECTOR3 targetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 diffPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 diffRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			float fBulletRot = 0.0f;

			for (int nCntBullet = 0; nCntBullet < 7; nCntBullet++)
			{// 弾の発射角度の設定
				fBulletRot = 0.0f - (D3DX_PI * 0.75f / 2) + (D3DX_PI * 0.75f / 7 * nCntBullet);
				fBulletRot = CApplication::RotNormalization(fBulletRot + sinf(m_fAddRot), -(D3DX_PI * 0.5f / 2), (D3DX_PI * 0.5f / 2));

				// 弾の生成
				pBullet3D = CBullet3D::Create();

				if (GetColorType() == CObject::TYPE_WHITE)
				{
					pBullet3D->SetPos(D3DXVECTOR3(pos.x + 55.0f, 0.0f, pos.z + 75.0f));
				}
				else if (GetColorType() == CObject::TYPE_BLACK)
				{
					pBullet3D->SetPos(D3DXVECTOR3(pos.x - 30.0f, 0.0f, pos.z + 60.0f));
				}

				pBullet3D->SetSize(D3DXVECTOR3(10.0f, 10.0f, 0.0f));
				pBullet3D->SetMoveVec(D3DXVECTOR3(D3DX_PI * -0.5f, fBulletRot, 0.0f));
				pBullet3D->SetSpeed(10.0f);
				pBullet3D->SetColor(bulletColor);
				pBullet3D->SetColorType(GetColorType());
				pBullet3D->SetParent(CObject::OBJTYPE_3DBOSS);
			}
		}
	}
}

//=============================================================================
// モーションの設定
// Author : 唐﨑結斗
// 概要 : モーションの変更やモーションブレンドの設定を行う
//=============================================================================
void CMotionBoss::MotionSet()
{
	// 現在のモーション番号の保管
	m_nNumMotionOld = m_nNumMotion;

	if (m_nNumMotionOld != m_nNumMotion)
	{// モーションタイプが変更された時
		m_pMotion->CntReset((int)(m_nNumMotionOld));
		m_bMotionBlend = true;
	}

	if (m_bMotionBlend)
	{// モーションブレンドを使用してる
		m_bMotionBlend = m_pMotion->MotionBlend((int)(m_nNumMotion));
	}
	else if (!m_bMotionBlend)
	{// モーションブレンドを使用してない場合
		m_bMotion = m_pMotion->PlayMotion((int)(m_nNumMotion));
	}
}

//=============================================================================
// ヒット
// Author : 唐﨑結斗
// 概要 : ダメージを与える
//=============================================================================
void CMotionBoss::Hit(COLOR_TYPE colorType, int nAttack)
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

			// データ格納用変数
			D3DXVECTOR3 pos = GetPos();
			D3DXVECTOR3 rot = GetRot();

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

			Uninit();
		}
	}
}

//=============================================================================
// 移動情報の設定
// Author : 唐﨑結斗
// 概要 : 移動情報の設定
//=============================================================================
void CMotionBoss::Move()
{
	if (m_actionType == ACTION_DIFFUSIONSHOT)
	{
		m_fWave += 0.1f;
		m_move.x = sinf(m_fMoveVec) * m_fSpeed;
		m_move.z = sinf(m_fWave) * 5.0f;
	}
}

//=============================================================================
// スクリーンのあたり判定
// Author : 唐﨑結斗
// 概要 : スクリーンとプレイヤーのあたり判定を行う
//=============================================================================
bool CMotionBoss::CollisionScreen()
{
	// 位置の取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 worldPos;
	bool bCollision = false;

	// 当たり判定の取得
	D3DXVECTOR3 collision = CObject::GetColisonSize();

	// スクリーン座標にキャスト
	D3DXVECTOR3 screenPos = CApplication::ScreenCastWorld(pos);
	D3DXVECTOR3 screenPosUp = CApplication::ScreenCastWorld(pos + collision);
	D3DXVECTOR3 screenPosUnder = CApplication::ScreenCastWorld(pos - collision);

	if (screenPosUnder.x < 0.0f)
	{
		worldPos = CApplication::WorldCastScreen(D3DXVECTOR3(collision.x, screenPos.y, screenPos.z));
		SetPos(D3DXVECTOR3(worldPos.x, pos.y, worldPos.z));
		screenPos = CApplication::ScreenCastWorld(GetPos());
		bCollision = true;
	}
	else if (screenPosUp.x >(float)CRenderer::SCREEN_WIDTH)
	{
		worldPos = CApplication::WorldCastScreen(D3DXVECTOR3((float)CRenderer::SCREEN_WIDTH - collision.x, screenPos.y, screenPos.z));
		SetPos(D3DXVECTOR3(worldPos.x, pos.y, worldPos.z));
		screenPos = CApplication::ScreenCastWorld(GetPos());
		bCollision = true;
	}

	if (screenPosUp.y < 0.0f)
	{
		worldPos = CApplication::WorldCastScreen(D3DXVECTOR3(screenPos.x, collision.y, screenPos.z));
		SetPos(D3DXVECTOR3(worldPos.x, pos.y, worldPos.z));
		screenPos = CApplication::ScreenCastWorld(GetPos());
		bCollision = true;
	}
	else if (screenPosUnder.y >(float)CRenderer::SCREEN_HEIGHT)
	{
		worldPos = CApplication::WorldCastScreen(D3DXVECTOR3(screenPos.x, (float)CRenderer::SCREEN_HEIGHT - collision.y, screenPos.z));
		SetPos(D3DXVECTOR3(worldPos.x, pos.y, worldPos.z));
		screenPos = CApplication::ScreenCastWorld(GetPos());
		bCollision = true;
	}

	return bCollision;
}

