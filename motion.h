//**************************************************************************************************
//
// モーション処理(motion.h)
// Auther：唐﨑結斗
// 概要 : モーションクラスの設定
//
//**************************************************************************************************
#ifndef _MOTION_H_			// このマクロ定義がされてなかったら
#define _MOTION_H_			// 二重インクルード防止のマクロ定義

//***************************************************************************
// インクルード
//***************************************************************************
#include "main.h"
#include "model3D.h"
#include "renderer.h"

//=============================================================================
// モーションクラス
// Author : 唐﨑結斗
// 概要 : モーション設定を行うクラス
//=============================================================================
class CMotion
{
public:
	//***************************************************************************
	// 定数定義
	//***************************************************************************
	static const unsigned int MAX_MOTION = (128);					// モーション数の最大数
	static const unsigned int MAX_MODEL_PARTS = (128);				// モデル数の最大数
	static const unsigned int MAX_KEY = (64);						// キーの最大数
	static const unsigned int MAX_KEYSET = (64);					// キー設定の最大数
	static const unsigned int MOTION_BLEND_FRAM = (12);				// モーションブレンドのフレーム数	

	//***************************************************************
	// パーツファイル構造体を定義
	//***************************************************************
	typedef struct
	{
		char	aName[MAX_MODEL_PARTS];		// 名前
	}PartsFile;

	//***************************************************************
	// キー構造体を定義
	//***************************************************************
	typedef struct
	{
		D3DXVECTOR3		pos;	// 現在位置
		D3DXVECTOR3		rot;	// 現在の向き
	}MyKey;

	//***************************************************************
	// キー設定構造体を定義
	//***************************************************************
	typedef struct
	{
		int		nFrame;			// フレーム数
		MyKey	key[MAX_KEY];	// キー情報
	}MyKeySet;

	//***************************************************************
	// モーション設定構造体を定義
	//***************************************************************
	typedef struct
	{
		int				nNumKey;				// キー数
		int				nCntKeySet;				// キーセットカウント
		int				nCntFrame;				// フレームカウント
		bool			bLoop;					// ループ使用状況
		MyKeySet		keySet[MAX_KEYSET];		// キー設定情報
	}MyMotion;

	//***************************************************************
	// モデルパーツ構造体を定義
	//***************************************************************
	typedef struct
	{
		CModel3D::MODEL_MATERIAL		material;				// マテリアル情報
		D3DXMATRIX						mtxWorld;				// ワールドマトリックス
		D3DXVECTOR3						posOrigin;				// 元の位置
		D3DXVECTOR3						rotOrigin;				// 元の向き
		D3DXVECTOR3						pos;					// 現在の位置
		D3DXVECTOR3						rot;					// 現在の向き
		D3DXVECTOR3						posDest;				// 目的の位置
		D3DXVECTOR3						rotDest;				// 目的の向き
		int								nIdxModelParent;		// 親モデルのインデックス数
		int								nType;					// パーツのタイプ
	}Parts;

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CMotion(const char *pFileName);
	~CMotion();

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	// 初期化
	void Init(void);

	// モーションの初期設定
	void SetMotion(const int nCntMotionSet);

	// パーツの設定
	void SetParts(D3DXMATRIX mtxWorld);

	// パーツの色設定
	void SetParts(D3DXMATRIX mtxWorld, const D3DXCOLOR col);

	// モーションの再生
	bool PlayMotion(const int nCntMotionSet);

	// モーションブレンド
	bool MotionBlend(const int nCntMotionSet);	

	// モーション読み込み
	void LoodSetMotion(const char *pFileName);

	// 終了
	void Uninit(void);

	// カウントリセット
	void CntReset(const int nNumMotionOld);

	// モーションの再読み込み
	void ReloadMotion(const char *pFileName);

private:
	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	MyMotion	*m_motion;							// モーション
	Parts		*m_parts;							// パーツ
	PartsFile	m_partsFile[MAX_MODEL_PARTS];		// パーツのXファイル名
	int			m_nMaxParts;						// パーツ数
};

#endif

