//=============================================================================
//
// パーティクルクラス(particle.h)
// Author : 唐﨑結斗
// 概要 : パーティクル生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>
#include <time.h>

#include "particle.h"
#include "effect3D.h"
#include "renderer.h"
#include "application.h"

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : スコアを生成する
//=============================================================================
CParticle *CParticle::Create()
{
	// オブジェクトインスタンス
	CParticle *pParticle = nullptr;

	pParticle = new CParticle;

	// メモリの確保ができなかった
	assert(pParticle != nullptr);

	// 数値の初期化
	pParticle->Init();

	// 疑似乱数の初期化
	srand((unsigned int)time(NULL));

	// インスタンスを返す
	return pParticle;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CParticle::CParticle(int nPriority /*= CObject::PRIORITY_LEVEL3*/) : CObject(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f,0.0f,0.0f);				// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 過去の位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 向き
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 大きさ
	m_popRange = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 生成範囲
	m_moveVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動方向
	m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 色
	m_fSpeed = 0.0f;									// 速度
	m_nLife = 0;										// 寿命
	m_nMaxEffect = 0;									// 位置フレーム出すエフェクト数
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CParticle::~CParticle()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CParticle::Init()
{
	// 位置の設定
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 向きの設定
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 大きさの設定
	m_size = D3DXVECTOR3(30.0f, 30.0f, 0.0f);

	// 位置フレーム出すエフェクト数
	m_nMaxEffect = 1;									

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CParticle::Uninit()
{
	// スコアの解放
	Release();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 更新を行う
//=============================================================================
void CParticle::Update()
{
	m_nLife--;

	SetEffect();

	if (m_nLife <= 0)
	{
		Uninit();
	}
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 描画を行う
//=============================================================================
void CParticle::Draw()
{

}

//=============================================================================
// 位置のセッター
// Author : 唐﨑結斗
// 概要 : 位置のメンバ変数に引数を代入
//=============================================================================
void CParticle::SetPos(const D3DXVECTOR3 &pos)
{
	m_pos = pos;
}

//=============================================================================
// 向きのセッター
// Author : 唐﨑結斗
// 概要 : 向きのメンバ変数に引数を代入
//=============================================================================
void CParticle::SetRot(const D3DXVECTOR3 &rot)
{
	m_rot = rot;
}

//=============================================================================
// 大きさのセッター
// Author : 唐﨑結斗
// 概要 : 大きさのメンバ変数に引数を代入
//=============================================================================
void CParticle::SetSize(const D3DXVECTOR3 & size)
{
	m_size = size;
}

//=============================================================================
// エフェクトの生成
// Author : 唐﨑結斗
// 概要 : エフェクトの生成
//=============================================================================
void CParticle::SetEffect()
{
	// 変数宣言
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f,0.0f,0.0f);					// 位置
	D3DXVECTOR3 moveVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動方向
	float fSize = 0.0f;												// 大きさ
	float fSpeed = 0.0f;											// 速度
	int nEffectLife = 0;													// 寿命

	for (int nCntEffect = 0; nCntEffect < m_nMaxEffect; nCntEffect++)
	{// 数値の算出
		pos.x = m_pos.x + (float)(-(int)(m_popRange.x / 2.0f) + rand() % ((int)(m_popRange.x) + 1));
		pos.y = m_pos.y + (float)(-(int)(m_popRange.y / 2.0f) + rand() % ((int)(m_popRange.y) + 1));
		pos.z = m_pos.z + (float)(-(int)(m_popRange.z / 2.0f) + rand() % ((int)(m_popRange.z) + 1));
		fSize = (float)(rand() % ((int)(sqrtf((m_size.x * m_size.x) + (m_size.y * m_size.y)) * 100.0f) + 1)) / 100.0f;
		moveVec.x = (((int)(m_moveVec.x * 100.0f) / 2) - (rand() % ((int)(m_moveVec.x * 100.0f) + 1))) / 100.0f;
		moveVec.y = (((int)(m_moveVec.y * 100.0f) / 2) - (rand() % ((int)(m_moveVec.y * 100.0f) + 1))) / 100.0f;
		moveVec.x = CApplication::RotNormalization(moveVec.x);
		moveVec.y = CApplication::RotNormalization(moveVec.y);
		nEffectLife = rand() % (m_nEffectLife + 1) + 1;
		fSpeed = (float)(rand() % ((int)(m_fSpeed * 100.0f) + 1)) / 100.0f;

		CEffect3D *pEffect = CEffect3D::Create();
		pEffect->SetPos(pos);
		pEffect->SetSize(D3DXVECTOR3(fSize, fSize, 0.0f));
		pEffect->SetMoveVec(moveVec);
		pEffect->SetLife(nEffectLife);
		pEffect->SetSpeed(fSpeed);
		pEffect->SetRenderMode(CEffect3D::MODE_ADD);
		pEffect->SetColor(m_color);
	}	
}
