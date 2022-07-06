//**************************************************************************************************
//
// �v�Z����(calculation.cpp)
// Auther�F�������l
//
//**************************************************************************************************

//***************************************************************************
// �C���N���[�h
//***************************************************************************
#include "main.h"
#include "calculation.h"

//=============================================================================
// ���[���h���W�ւ̃L���X�g����
// Author : �������l
// �T�v : pos,rot�̍s��v�Z���s���A���[�J�����W�����[���h���W�ɕϊ�����
//=============================================================================
D3DXVECTOR3	WorldCastVtx(D3DXVECTOR3 vtx, D3DXVECTOR3 FormerPos, D3DXVECTOR3 FormerRot)
{
	// �ϐ��錾
	D3DXMATRIX		mtxWorldVtx;				// ���[���h�}�g���b�N�X
	D3DXMATRIX		mtxRot, mtxTrans;			// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	// �s�񏉊����֐�(��������[�s��]��[�P�ʍs��]�ɏ�����)
	D3DXMatrixIdentity(&mtxWorldVtx);

	// �ʒu�𔽉f
	// �s��ړ��֐� (��������X,Y,Z�����̈ړ��s����쐬)
	D3DXMatrixTranslation(&mtxTrans, vtx.x, vtx.y, vtx.z);
	D3DXMatrixMultiply(&mtxWorldVtx, &mtxWorldVtx, &mtxTrans);		// �s��|���Z�֐�

	// �����̔��f
	// �s���]�֐� (��������[���[(y)�s�b�`(x)���[��(z)]�����̉�]�s����쐬)
	D3DXMatrixRotationYawPitchRoll(&mtxRot, FormerRot.y, FormerRot.x, FormerRot.z);

	// �s��|���Z�֐� (������ * ��O������������Ɋi�[)
	D3DXMatrixMultiply(&mtxWorldVtx, &mtxWorldVtx, &mtxRot);

	// �ʒu�𔽉f
	// �s��ړ��֐� (��������X,Y,Z�����̈ړ��s����쐬)
	D3DXMatrixTranslation(&mtxTrans, FormerPos.x, FormerPos.y, FormerPos.z);
	D3DXMatrixMultiply(&mtxWorldVtx, &mtxWorldVtx, &mtxTrans);		// �s��|���Z�֐�

	return D3DXVECTOR3(mtxWorldVtx._41, mtxWorldVtx._42, mtxWorldVtx._43);
}

D3DXVECTOR3 WorldCastVtx(D3DXVECTOR3 vtx, D3DXVECTOR3 FormerPos, D3DXQUATERNION FormeQuaternion)
{
	// �ϐ��錾
	D3DXMATRIX		mtxWorldVtx;				// ���[���h�}�g���b�N�X
	D3DXMATRIX		mtxRot, mtxTrans;			// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	// �s�񏉊����֐�(��������[�s��]��[�P�ʍs��]�ɏ�����)
	D3DXMatrixIdentity(&mtxWorldVtx);

	// �ʒu�𔽉f
	// �s��ړ��֐� (��������X,Y,Z�����̈ړ��s����쐬)
	D3DXMatrixTranslation(&mtxTrans, vtx.x, vtx.y, vtx.z);
	D3DXMatrixMultiply(&mtxWorldVtx, &mtxWorldVtx, &mtxTrans);		// �s��|���Z�֐�

	 // �N�H�[�^�j�I���̎g�p�����p���̐ݒ�
	D3DXMatrixRotationQuaternion(&mtxRot, &FormeQuaternion);            // �N�I�[�^�j�I���ɂ��s���]

	// �s��|���Z�֐� (������ * ��O������������Ɋi�[)
	D3DXMatrixMultiply(&mtxWorldVtx, &mtxWorldVtx, &mtxRot);

	// �ʒu�𔽉f
	// �s��ړ��֐� (��������X,Y,Z�����̈ړ��s����쐬)
	D3DXMatrixTranslation(&mtxTrans, FormerPos.x, FormerPos.y, FormerPos.z);
	D3DXMatrixMultiply(&mtxWorldVtx, &mtxWorldVtx, &mtxTrans);		// �s��|���Z�֐�

	return D3DXVECTOR3(mtxWorldVtx._41, mtxWorldVtx._42, mtxWorldVtx._43);
}

//=============================================================================
// �p�x�̐��K������
// Author : �������l
// �T�v : �p�x���~������2�{�𒴂����Ƃ��ɔ͈͓��ɖ߂�
//=============================================================================
float RotNormalization(float fRot)
{
	if (fRot >= D3DX_PI)
	{// �ړ������̐��K��
		fRot -= D3DX_PI * 2;
	}
	else if (fRot <= -D3DX_PI)
	{// �ړ������̐��K��
		fRot += D3DX_PI * 2;
	}

	return fRot;
}
