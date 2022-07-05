//**************************************************************************************************
//
// �J��������(camera.h)
// Auther�F�������l
// �T�v : �J�����̐ݒ菈��
//
//**************************************************************************************************

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <assert.h>

#include "camera.h"
#include "application.h"
#include "renderer.h"
#include "input.h"
#include "mouse.h"
#include "keyboard.h"
#include "move.h"
#include "calculation.h"

//*****************************************************************************
// �萔��`
//*****************************************************************************
const float CCamera::CAMERA_NEAR = (1.0f);				// �j�A
const float CCamera::CAMERA_FUR = (10000.0f);			// �t�@�[

//=============================================================================
// �R���X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�������ɍs������
//=============================================================================
CCamera::CCamera()
{
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ���_
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �����_
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ������x�N�g��
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ����
	m_rotMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړ�����
	m_mtxProj = {};										// �v���W�F�N�V�����}�g���b�N�X
	m_mtxView = {};										// �r���[�}�g���b�N�X
	m_viewType = TYPE_CLAIRVOYANCE;						// ���e���@�̎��
	m_fDistance = 0.0f;									// ���_���璍���_�܂ł̋���
	m_fRotMove = 0.0f;									// �ړ�����
}

//=============================================================================
// �f�X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�I�����ɍs������
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
// ������
// Author : �������l
// �T�v : ���_�ƒ����_�̊Ԃ̋������Z�o����
//=============================================================================
HRESULT CCamera::Init()
{
	m_posV = D3DXVECTOR3(0.0f, 0.0f, -150.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// �Œ�
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ���_�ƒ����_�̋���
	D3DXVECTOR3 posDiss = m_posR - m_posV;
	m_fDistance = sqrtf((posDiss.y * posDiss.y) + (posDiss.x * posDiss.x) + (posDiss.z * posDiss.z));

	// �p�x�̎Z�o
	m_rot.y = atan2f(posDiss.x, posDiss.z);
	m_rot.x = atan2f(sqrtf((posDiss.x * posDiss.x) + (posDiss.z * posDiss.z)), posDiss.y);
	m_rot.z = 0.0f;

	// �������̊m��
	m_pRoll = new CMove;
	assert(m_pRoll != nullptr);
	m_pRoll->SetMoving(2.0f, 10.0f, 0.0f, 0.2f);

	return S_OK;
}

//=============================================================================
// �I��
// Author : �������l
// �T�v : �I��
//=============================================================================
void CCamera::Uninit(void)
{
	if (m_pRoll != nullptr)
	{// �I������
	 // �������̉��
		delete m_pRoll;
		m_pRoll = nullptr;
	}
}

//=============================================================================
// �X�V
// Author : �������l
// �T�v : �X�V
//=============================================================================
void CCamera::Update(void)
{
	Rotate();
	Move();
}

//=============================================================================
// �J�����̐ݒ�
// Author : �������l
// �T�v : �r���[�}�g���b�N�X�̐ݒ�
//=============================================================================
void CCamera::Set()
{// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);			// �s�񏉊����֐�

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProj);			// �s�񏉊����֐�

	switch (m_viewType)
	{
	case TYPE_CLAIRVOYANCE:
		// �v���W�F�N�V�����}�g���b�N�X�̍쐬(�������e)
		D3DXMatrixPerspectiveFovLH(&m_mtxProj,					// �v���W�F�N�V�����}�g���b�N�X
			D3DXToRadian(45.0f),								// ����p
			(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,			// �A�X�y�N�g��
			CAMERA_NEAR,										// �j�A
			CAMERA_FUR);										// �t�@�[
		break;

	case TYPE_PARALLEL:
		// �v���W�F�N�V�����}�g���b�N�X�̍쐬(���s���e)
		D3DXMatrixOrthoLH(&m_mtxProj,							// �v���W�F�N�V�����}�g���b�N�X
			(float)SCREEN_WIDTH,								// ��
			(float)SCREEN_HEIGHT,								// ����
			CAMERA_NEAR,										// �j�A
			CAMERA_FUR);										// �t�@�[
		break;

	default:
		assert(false);
		break;
	}

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProj);
}

//=============================================================================
// �J�����̉�]
// Author : �������l
// �T�v : 
//=============================================================================
void CCamera::Rotate(void)
{
	// ���͏��̎擾
	const float MIN_MOUSE_MOVED = 5.0f;
	const float ROTATE_MOUSE_MOVED = 0.45f;

	CMouse *pMouse = CApplication::GetMouse();
	D3DXVECTOR3 rollDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	int nRotateType = -1;

	if (pMouse->GetPress(CMouse::MOUSE_KEY_LEFT)
		|| pMouse->GetPress(CMouse::MOUSE_KEY_RIGHT))
	{// �}�E�X�̈ړ��ʂ̎擾
		D3DXVECTOR3 mouseMove = pMouse->GetMouseMove();

		if (mouseMove.x > MIN_MOUSE_MOVED || mouseMove.x < -MIN_MOUSE_MOVED)
		{// �}�E�X����苗��(X)�ړ������Ƃ�
			if (mouseMove.x > MIN_MOUSE_MOVED)
			{
				rollDir.y += (D3DX_PI / 180.0f) * ROTATE_MOUSE_MOVED;
			}
			if (mouseMove.x < -MIN_MOUSE_MOVED)
			{
				rollDir.y += (D3DX_PI / 180.0f) * -ROTATE_MOUSE_MOVED;
			}
		}

		if (mouseMove.y > MIN_MOUSE_MOVED || mouseMove.y < -MIN_MOUSE_MOVED)
		{// �}�E�X����苗��(Y)�ړ������Ƃ�
			if (mouseMove.y > MIN_MOUSE_MOVED)
			{
				rollDir.x += (D3DX_PI / 180.0f) * ROTATE_MOUSE_MOVED;
			}
			if (mouseMove.y < -MIN_MOUSE_MOVED)
			{
				rollDir.x += (D3DX_PI / 180.0f) * -ROTATE_MOUSE_MOVED;
			}
		}

		if (pMouse->GetPress(CMouse::MOUSE_KEY_LEFT))
		{// ��]�^�C�v�̍X�V
			nRotateType = 1;
		}
		else
		{// ��]�^�C�v�̍X�V
			nRotateType = 0;
		}
	}

	// ���C�W���̌v�Z
	m_pRoll->Moving(rollDir);

	// �J�����̌����̐��K��
	RotNormalization(rollDir.x);
	RotNormalization(rollDir.y);
	RotNormalization(rollDir.z);

	// ��]
	m_rot += m_pRoll->GetMove();

	if (m_rot.y < -D3DX_PI)
	{// ������-D3DX_PI�����̎�
		m_rot.y += D3DX_PI * 2;
	}
	if (m_rot.y > D3DX_PI)
	{// ������D3DX_PI�ȏ�̎�
		m_rot.y -= D3DX_PI * 2;
	}
	if (m_rot.x < 0.0f + 0.1f)
	{// ������0�ȉ��̎�
		m_rot.x = 0.0f + 0.1f;
	}
	if (m_rot.x > D3DX_PI - 0.1f)
	{// ������D3DX_PI�ȏ�̎�
		m_rot.x = D3DX_PI - 0.1f;
	}

	if (nRotateType == 0)
	{// �����_�̎Z�o
		m_posR.z = m_posV.z + sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;
		m_posR.x = m_posV.x + sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
		m_posR.y = m_posV.y + cosf(m_rot.x) * m_fDistance;
	}
	else
	{// ���_�̎Z�o
		m_posV.z = m_posR.z - sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;
		m_posV.x = m_posR.x - sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
		m_posV.y = m_posR.y - cosf(m_rot.x) * m_fDistance;
	}
}

void CCamera::Move(void)
{
	const float CAMERA_MOVE_SPEED = 0.5f;
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	if (pKeyboard->GetPress(DIK_W) == true
		|| pKeyboard->GetPress(DIK_A) == true
		|| pKeyboard->GetPress(DIK_D) == true
		|| pKeyboard->GetPress(DIK_S) == true)
	{// �ړ��L�[�������ꂽ
	 // �O�㍶�E�ړ�
		if (pKeyboard->GetPress(DIK_W) == true)
		{// [W]�L�[�������ꂽ��
			if (pKeyboard->GetPress(DIK_A) == true)
			{// [A]�L�[�������ꂽ��
				// �ړ������̍X�V
				m_rotMove.y = D3DX_PI * -0.25f;
				m_rotMove.x = D3DX_PI * -0.25f;
			}
			else if (pKeyboard->GetPress(DIK_D) == true)
			{// [D]�L�[�������ꂽ��
				// �ړ������̍X�V
				m_rotMove.y = D3DX_PI * 0.25f;
				m_rotMove.x = D3DX_PI * 0.25f;
			}
			else
			{// �ړ������̍X�V
				m_rotMove.y = D3DX_PI * 0.0f;
				m_rotMove.x = D3DX_PI * 0.0f;
			}
		}
		else if (pKeyboard->GetPress(DIK_S) == true)
		{// [S]�L�[�������ꂽ��
			if (pKeyboard->GetPress(DIK_A) == true)
			{// [A]�L�[�������ꂽ��
				// �ړ������̍X�V
				m_rotMove.y = D3DX_PI * -0.75f;
				m_rotMove.x = D3DX_PI * -0.75f;
			}
			else if (pKeyboard->GetPress(DIK_D) == true)
			{// [D]�L�[�������ꂽ��
				// �ړ������̍X�V
				m_rotMove.y = D3DX_PI * 0.75f;
				m_rotMove.x = D3DX_PI * 0.75f;
			}
			else
			{// �ړ������̍X�V
				m_rotMove.y = 0.0f;
				m_rotMove.x = D3DX_PI;
			}
		}
		else if (pKeyboard->GetPress(DIK_A) == true)
		{// [A]�L�[�������ꂽ��
			// �ړ������̍X�V
			m_rotMove.y = D3DX_PI * -0.5f;
			m_rotMove.x = D3DX_PI * 0.0f;
		}
		else if (pKeyboard->GetPress(DIK_D) == true)
		{// [D]�L�[�������ꂽ��
			// �ړ������̍X�V
			m_rotMove.y = D3DX_PI * 0.5f;
			m_rotMove.x = D3DX_PI * 0.0f;
		}

		// �ړ������̎Z�o
		m_rotMove.x += m_rot.x;

		if (m_rotMove.x > D3DX_PI)
		{// �ړ������̐��K��
			m_rotMove.x -= D3DX_PI * 2;
		}
		else if (m_rotMove.x < -D3DX_PI)
		{// �ړ������̐��K��
			m_rotMove.x += D3DX_PI * 2;
		}

		m_rotMove.y += m_rot.y;

		if (m_rotMove.y > D3DX_PI)
		{// �ړ������̐��K��
			m_rotMove.y -= D3DX_PI * 2;
		}
		else if (m_rotMove.y < -D3DX_PI)
		{// �ړ������̐��K��
			m_rotMove.y += D3DX_PI * 2;
		}

		// ���_�ʒu�̍X�V
		m_posV.z += sinf(m_rotMove.x) * cosf(m_rotMove.y) * CAMERA_MOVE_SPEED;
		m_posV.x += sinf(m_rotMove.x) * sinf(m_rotMove.y) * CAMERA_MOVE_SPEED;
		m_posV.y += cosf(m_rotMove.x) * CAMERA_MOVE_SPEED;

		// �����_�̎Z�o
		m_posR.z = m_posV.z + sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;
		m_posR.x = m_posV.x + sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
		m_posR.y = m_posV.y + cosf(m_rot.x) * m_fDistance;
	}
}


