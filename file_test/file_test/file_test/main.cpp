#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	FILE *fp;	/* (1)�t�@�C���|�C���^�̐錾 */
	char s[256];
	int outbf[100];
	int inbuf[100];
	/* (2)�t�@�C���̃I�[�v�� */
	/*  �����ŁA�t�@�C���|�C���^���擾���� */
	if ((fp = fopen("./smpl.txt", "w+")) == NULL) {
		printf("file open error!!\n");
		exit(EXIT_FAILURE);	/* (3)�G���[�̏ꍇ�͒ʏ�A�ُ�I������ */
	}

	for (int i = 0; i<100; i++){
		outbf[i] = i;
	}

	fwrite(outbf, sizeof(int), sizeof(outbf)/2, fp);
	fseek(fp, 0, SEEK_SET);
	fread(inbuf, sizeof(int), sizeof(outbf)/2, fp);

	/* �ǂݍ��݃f�[�^�̊m�F */
	for (int i = 0; i < 100; i++){
		printf("%3d\n", inbuf[i]);
	}
		
	

	/* �t�@�C���N���[�Y */
	//fclose(fp);



	return 0;
}