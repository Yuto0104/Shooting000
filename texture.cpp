//=============================================================================
//
// テクスチャ設定処理(texture.cpp)
// Author : 唐﨑結斗
// 概要 : テクスチャ設定を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <string.h>
#include <assert.h>

#include "texture.h"
#include "renderer.h" 
#include "application.h"

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CTexture::CTexture()
{
	
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CTexture::~CTexture()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 貼り付けるテクスチャ情報を格納する
//=============================================================================
void CTexture::Init(void)
{// レンダラーのゲット
	CRenderer *pRenderer = CApplication::GetRenderer();

	// ファイル名の設定
	const char *pFileName[] =
	{
		"data/TEXTURE/bullet000.png",				// 2Dポリゴン
		"data/TEXTURE/effect000.jpg" ,				// 2Dプレイヤー
		"data/TEXTURE/695783_VvJuOyc1.png",			// 2D弾
		"data/TEXTURE/explosion000.png",			// 2D爆発
		"data/TEXTURE/ボス.png",					// 2Dエネミー
		"data/TEXTURE/sky002.jpg",					// 空(0)
		"data/TEXTURE/number003.png",				// ナンバー(0)
		"data/TEXTURE/ハート.png",					// ハート(0)
	};

	for (int nCnt = 0; nCnt < MAX_TYPE; nCnt++)
	{//ポリゴンに貼り付けるテクスチャの読み込み
		D3DXCreateTextureFromFile(pRenderer->GetDevice(),
			pFileName[nCnt],
			&m_pTexture[nCnt]);
	}
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャの解放
//=============================================================================
void CTexture::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_TYPE; nCnt++)
	{//テクスチャの破棄	  
		if (m_pTexture[nCnt] != nullptr)
		{
			m_pTexture[nCnt]->Release();

			m_pTexture[nCnt] = nullptr;
		}
	}
}

//=============================================================================
// テクスチャポインタのゲッター	
// Author : 唐﨑結斗
// 概要 : テクスチャのゲッター
//=============================================================================
LPDIRECT3DTEXTURE9 CTexture::GetTexture(TEXTURE_TYPE type)
{
	LPDIRECT3DTEXTURE9 pTexture = nullptr;

	if (type != TYPE_NULL)
	{// タイプが設定されてる
		pTexture = m_pTexture[type];
	}

	return pTexture;
}
