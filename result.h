//=============================================================================
//
// リザルトクラス(result.h)
// Author : 唐�ｱ結斗
// 概要 : リザルトクラスの管理を行う
//
//=============================================================================
#ifndef _RESULT_H_		// このマクロ定義がされてなかったら
#define _RESULT_H_		// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "scene_mode.h"

//=============================================================================
// リザルトクラス
// Author : 唐�ｱ結斗
// 概要 : リザルト生成を行うクラス
//=============================================================================
class CResult : public CSceneMode
{
public:
	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CResult();
	~CResult() override;

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update() override;		// 更新
	void Draw() override;		// 描画

};

#endif




