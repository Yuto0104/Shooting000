//=============================================================================
//
// �e�N�X�`���A�j���[�V��������(tex_anim.h)
// Author : �������l
// �T�v : �e�N�X�`���[�A�j���[�V�������v�Z����
//
//=============================================================================
#ifndef _TEX_ANIM_H_		// ���̃}�N����`������ĂȂ�������
#define _TEX_ANIM_H_		// ��d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "main.h"

//=============================================================================
// �e�N�X�`���A�j���[�V�����N���X
// Author : �������l
// �T�v : �e�N�X�`���[�A�j���[�V�������v�Z����
//=============================================================================
class CTexAnim
{
public:
	//--------------------------------------------------------------------
	// TEX_PATTERN�ϐ��̒ǉ�
	// Author : �������l
	// �T�v : �e�N�X�`���A�j���[�V�����̃p�^�[�����ݒ�Ɏg�p
	//--------------------------------------------------------------------
	struct TEX_PATTERN
	{
		// �ϐ��錾
		int   x, y;

		constexpr TEX_PATTERN() : x(0), y(0) { }
		constexpr TEX_PATTERN(int _x, int _y) : x(_x), y(_y) { }

		// ���Z
		TEX_PATTERN operator +(const TEX_PATTERN &texPattern) {
			TEX_PATTERN pattern;
			pattern.x = x + texPattern.x;
			pattern.y = y + texPattern.y;
			return pattern;
		}

		// ���Z(+=)
		TEX_PATTERN &operator +=(const TEX_PATTERN &texPattern) {
			x += texPattern.x;
			y += texPattern.y;
			return *this;
		}

		// ���Z
		TEX_PATTERN operator -(const TEX_PATTERN &texPattern) {
			TEX_PATTERN pattern;
			pattern.x = x - texPattern.x;
			pattern.y = y - texPattern.y;
			return pattern;
		}

		// ���Z(-=)
		TEX_PATTERN &operator -=(const TEX_PATTERN &texPattern) {
			x -= texPattern.x;
			y -= texPattern.y;
			return *this;
		}

		// ��Z
		TEX_PATTERN operator *(const TEX_PATTERN &texPattern) {
			TEX_PATTERN pattern;
			pattern.x = x * texPattern.x;
			pattern.y = y * texPattern.y;
			return pattern;
		}

		// ��Z(*=)
		TEX_PATTERN &operator *=(const TEX_PATTERN &texPattern) {
			x *= texPattern.x;
			y *= texPattern.y;
			return *this;
		}

		// ���
		TEX_PATTERN &operator =(const TEX_PATTERN &texPattern) {
			x = texPattern.x;
			y = texPattern.y;
			return *this;
		}
	};

	//--------------------------------------------------------------------
	// �R���X�g���N�^�ƃf�X�g���N�^
	//--------------------------------------------------------------------
	CTexAnim();
	virtual ~CTexAnim();

	//--------------------------------------------------------------------
	// �����o�֐�
	//--------------------------------------------------------------------
	void SetAnim(TEX_PATTERN patternAnim, int nLife, bool bLoop);		// �A�j���[�V�����̏����ݒ�
	void PlayAnim();													// �A�j���[�V�����̍Đ�
	TEX_PATTERN GetNumAnim() { return m_numAnim; }						// �A�j���[�V�����̃p�^�[��No.�̃Q�b�^�[
	D3DXVECTOR2 GetAddTex() { return m_addTex; }						// �e�N�X�`�����W�̑����ʂ̃Q�b�^�[

private:
	//--------------------------------------------------------------------
	// �����o�ϐ�
	//--------------------------------------------------------------------
	TEX_PATTERN		m_patternAnim;			// �A�j���[�V�����̃p�^�[��
	TEX_PATTERN		m_numAnim;				// �A�j���[�V�����̃p�^�[��No.
	D3DXVECTOR2		m_addTex;				// �e�N�X�`�����W�̑�����
	int				m_nMaxCntAnim;			// �A�j���[�V�����̐؂�ւ�
	int				m_nCntAnim;				// �A�j���[�V�����J�E���g
	bool			m_bLoop;				// ���[�v�̎g�p��
};

#endif
