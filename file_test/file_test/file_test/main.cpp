#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	FILE *fp;	/* (1)ファイルポインタの宣言 */
	char s[256];
	int outbf[100];
	int inbuf[100];
	/* (2)ファイルのオープン */
	/*  ここで、ファイルポインタを取得する */
	if ((fp = fopen("./smpl.txt", "w+")) == NULL) {
		printf("file open error!!\n");
		exit(EXIT_FAILURE);	/* (3)エラーの場合は通常、異常終了する */
	}

	for (int i = 0; i<100; i++){
		outbf[i] = i;
	}

	fwrite(outbf, sizeof(int), sizeof(outbf)/2, fp);
	fseek(fp, 0, SEEK_SET);
	fread(inbuf, sizeof(int), sizeof(outbf)/2, fp);

	/* 読み込みデータの確認 */
	for (int i = 0; i < 100; i++){
		printf("%3d\n", inbuf[i]);
	}
		
	

	/* ファイルクローズ */
	//fclose(fp);



	return 0;
}