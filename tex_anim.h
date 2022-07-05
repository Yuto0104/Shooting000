//=============================================================================
//
// テクスチャアニメーション処理(tex_anim.h)
// Author : 唐﨑結斗
// 概要 : テクスチャーアニメーションを計算する
//
//=============================================================================
#ifndef _TEX_ANIM_H_		// このマクロ定義がされてなかったら
#define _TEX_ANIM_H_		// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main.h"

//=============================================================================
// テクスチャアニメーションクラス
// Author : 唐﨑結斗
// 概要 : テクスチャーアニメーションを計算する
//=============================================================================
class CTexAnim
{
public:
	//--------------------------------------------------------------------
	// TEX_PATTERN変数の追加
	// Author : 唐﨑結斗
	// 概要 : テクスチャアニメーションのパターン数設定に使用
	//--------------------------------------------------------------------
	struct TEX_PATTERN
	{
		// 変数宣言
		int   x, y;

		constexpr TEX_PATTERN() : x(0), y(0) { }
		constexpr TEX_PATTERN(int _x, int _y) : x(_x), y(_y) { }

		// 加算
		TEX_PATTERN operator +(const TEX_PATTERN &texPattern) {
			TEX_PATTERN pattern;
			pattern.x = x + texPattern.x;
			pattern.y = y + texPattern.y;
			return pattern;
		}

		// 加算(+=)
		TEX_PATTERN &operator +=(const TEX_PATTERN &texPattern) {
			x += texPattern.x;
			y += texPattern.y;
			return *this;
		}

		// 減算
		TEX_PATTERN operator -(const TEX_PATTERN &texPattern) {
			TEX_PATTERN pattern;
			pattern.x = x - texPattern.x;
			pattern.y = y - texPattern.y;
			return pattern;
		}

		// 減算(-=)
		TEX_PATTERN &operator -=(const TEX_PATTERN &texPattern) {
			x -= texPattern.x;
			y -= texPattern.y;
			return *this;
		}

		// 乗算
		TEX_PATTERN operator *(const TEX_PATTERN &texPattern) {
			TEX_PATTERN pattern;
			pattern.x = x * texPattern.x;
			pattern.y = y * texPattern.y;
			return pattern;
		}

		// 乗算(*=)
		TEX_PATTERN &operator *=(const TEX_PATTERN &texPattern) {
			x *= texPattern.x;
			y *= texPattern.y;
			return *this;
		}

		// 代入
		TEX_PATTERN &operator =(const TEX_PATTERN &texPattern) {
			x = texPattern.x;
			y = texPattern.y;
			return *this;
		}
	};

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CTexAnim();
	virtual ~CTexAnim();

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	void SetAnim(TEX_PATTERN patternAnim, int nLife, bool bLoop);		// アニメーションの初期設定
	void PlayAnim();													// アニメーションの再生
	TEX_PATTERN GetNumAnim() { return m_numAnim; }						// アニメーションのパターンNo.のゲッター
	D3DXVECTOR2 GetAddTex() { return m_addTex; }						// テクスチャ座標の増加量のゲッター

private:
	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	TEX_PATTERN		m_patternAnim;			// アニメーションのパターン
	TEX_PATTERN		m_numAnim;				// アニメーションのパターンNo.
	D3DXVECTOR2		m_addTex;				// テクスチャ座標の増加量
	int				m_nMaxCntAnim;			// アニメーションの切り替え
	int				m_nCntAnim;				// アニメーションカウント
	bool			m_bLoop;				// ループの使用状況
};

#endif
