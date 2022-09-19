//=============================================================================
//
// ゲームクラス(game.h)
// Author : 唐﨑結斗
// 概要 : ゲームクラスの管理を行う
//
//=============================================================================
#ifndef _GAME_H_		// このマクロ定義がされてなかったら
#define _GAME_H_		// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "scene_mode.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CMotionPlayer3D;
class CScore;
class CLifeManager;
class CEnergyGage;
class CEnemyManager;

//=============================================================================
// ゲームクラス
// Author : 唐﨑結斗
// 概要 : ゲーム生成を行うクラス
//=============================================================================
class CGame : public CSceneMode
{
public:
	//--------------------------------------------------------------------
	// 静的メンバ関数
	//--------------------------------------------------------------------
	static CMotionPlayer3D *GetMotionPlayer3D() { return m_MotionPlayer3D; }			// モーションプレイヤーのゲッター
	static CScore *GetScore() { return m_pScore; }										// スコアの取得
	static CLifeManager *GetLifeManager() { return m_pLifeManager; }					// ライフマネージャーの取得
	static CEnergyGage *GetEnergyGage() { return m_pEnergyGage; }						// エネルギーゲージの取得
	static CEnemyManager *GetEnemyManager() { return m_pEnemyManager; }					// エネミーマネージャーの取得
	static bool GetUsePlayer() { return m_bUsePlayer; }									// プレイヤーの使用状況の取得
	static void SetUsePlayer(const bool bUse) { m_bUsePlayer = bUse; }					// プレイヤーの使用状況の設定
	static void SetGame(const bool bGame) { m_bGame = bGame; }							// ゲームの状況の設定

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CGame();
	~CGame() override;

	//--------------------------------------------------------------------
	// 静的メンバ変数
	//--------------------------------------------------------------------
	static CMotionPlayer3D *m_MotionPlayer3D;										// モーションプレイヤークラス
	static CScore *m_pScore;														// スコアクラス
	static CLifeManager *m_pLifeManager;											// ライフマネージャークラス
	static CEnergyGage *m_pEnergyGage;												// エネルギーゲージ
	static CEnemyManager *m_pEnemyManager;											// エネミーマネージャークラス
	static bool m_bUsePlayer;														// プレイヤーを使用しているか
	static bool m_bGame;															// ゲームの状況

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update() override;		// 更新
	void Draw() override;		// 描画
};

#endif



