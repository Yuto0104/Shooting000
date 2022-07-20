//=============================================================================
//
// �A�v���P�[�V�����N���X(application.h)
// Author : �������l
// �T�v : �e�N���X�̌Ăяo�����s���N���X
//
//=============================================================================
#ifndef _APPLICATION_H_		// ���̃}�N����`������ĂȂ�������
#define _APPLICATION_H_		// ��d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CRenderer;
class CKeyboard;
class CMouse;
class CTexture;
class CSound;
class CCamera;
class CPlayer2D;
class CMotionPlayer3D;
class CScore;

//=============================================================================
// �A�v���P�[�V�����N���X
// Author : �������l
// �T�v : �e�N���X�̌Ăяo�����s���N���X
//=============================================================================
class CApplication
{
public:
	//--------------------------------------------------------------------
	// �ÓI�����o�֐�
	//--------------------------------------------------------------------
	static CRenderer *GetRenderer() { return m_pRenderer; }								// �����_���[�̃Q�b�^�[
	static CKeyboard *GetKeyboard() { return m_pKeyboard; }								// �L�[�{�[�h�̃Q�b�^�[
	static CMouse *GetMouse() { return m_pMouse; }										// �}�E�X�̃Q�b�^�[
	static CTexture *GetTexture() { return m_pTexture; }								// �e�N�X�`���̃Q�b�^�[
	static CSound *GetSound() { return m_pSound; }										// �T�E���h�̃Q�b�^�[
	static CCamera *GetCamera() { return m_pCamera; }									// �J�����̃Q�b�^�[
	static CCamera *GetCameraBG() { return m_pCameraBG; }								// �w�i�J�����̃Q�b�^�[
	static CPlayer2D *GetPlayer2D() { return m_pPlayer2D; }								// �v���C���[�̃Q�b�^�[
	static CMotionPlayer3D *GetMotionPlayer3D() { return m_MotionPlayer3D; }			// ���[�V�����v���C���[�̃Q�b�^�[
	static CScore *GetScore() { return m_pScore; }										// �X�R�A�̎擾

	//--------------------------------------------------------------------
	// �R���X�g���N�^�ƃf�X�g���N�^
	//--------------------------------------------------------------------
	CApplication();
	~CApplication();

	//--------------------------------------------------------------------
	// �����o�֐�
	//--------------------------------------------------------------------
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);					// ������
	void Uninit();													// �I��
	void Update();													// �X�V
	void Draw();													// �`��

private:
	//--------------------------------------------------------------------
	// �ÓI�����o�ϐ�
	//--------------------------------------------------------------------
	static CRenderer *m_pRenderer;						// renderer�N���X
	static CKeyboard *m_pKeyboard;						// �L�[�{�[�h�N���X
	static CMouse *m_pMouse;							// �}�E�X�N���X
	static CTexture *m_pTexture;						// �e�N�X�`���N���X
	static CSound *m_pSound;							// �T�E���h�N���X
	static CCamera *m_pCamera;							// �Q�[���J�����N���X
	static CCamera *m_pCameraBG;						// �w�i�J�����N���X
	static CPlayer2D *m_pPlayer2D;						// �v���C���[�N���X
	static CMotionPlayer3D *m_MotionPlayer3D;			// ���[�V�����v���C���[�N���X
	static CScore *m_pScore;							// �X�R�A�N���X
};

#endif

