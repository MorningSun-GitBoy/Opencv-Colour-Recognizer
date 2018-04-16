/*��ɫʶ����
*�汾��v1.0
*���ߣ���־��
*3.28-4.16 15��59
*/
//�ÿ���ҪOpencv2.4.9��֧�֣�
#include <stdio.h>
#include <opencv.hpp> 
#include <stdlib.h>

//file
#define FILE_NAME_MIX 256

//colour file
#define COLOURFILE ".colour"

//colour typedef define
#define COLOURIN(TYPE) inline TYPE

//free
#define PASS	NULL;

//colour typedef bool
typedef unsigned char	COLOUR_BOOL;	//-125~125
#define COLOUR_BOOL_FALSE 1
#define COLOUR_BOOL_TRUE 0
#define COLOUR_BOOL_NOT 3

//machine_learning_colour_train
#define KEY_FILE "key" COLOURFILE
//colour error
typedef struct colour_error{
	char error_type[256];
	COLOUR_BOOL colour_sign = COLOUR_BOOL_FALSE;

}colour_error;

colour_error colour_error_type;
#define COLOUR_ERROR_NULL "���ݽ�����ָ��û��Ϊ������ڴ�"
#define COLOUR_ERROR_FILE_NULL "�޷���ָ���ļ�"
#define COLOUR_ERROR_FILE_IMAGE_NULL "�޷���ָ��ͼ���ļ�"
#define COLOUR_ERROR_FILE_NAME "�ļ���׺���ǳ���ָ�����������׺���ļ�������"
#define COLOUR_ERROR_FILE_NOT_EXISTENCE	"���ļ�������"
#define COLOUR_ERROR__NOT_NO_METHOD	"û���������"
#define COLOUR_ERROR_FILE_ERROR	"�ⲻ��һ����ȷ���������ļ�"

//ģ������
#define COLOUR_FUZZY_METHOD_GAUSSIAN 0	//��˹ģ��
#define COLOUR_FUZZY_METHOD_SIMPLE 1	//��ģ��
#define COLOUR_FUZZY_METHOD_MEDIAN 2	//��ֵģ��
//colour sign
char colour_sign[] = { "<colour:��ɫ,MIN:R=0&G=0&B=0&,MIX:R=255&G=0&B=0&><colour:��ɫ,MIN:R=0&G=0&B=0&,MIX:R=0&G=255&B=0&><colour:��ɫ,MIN:R=0&G=0&B=0&,MIX:R=0&G=0&B=255&>" };	//�Դ�����ɫ������
char *p_colour_sign = colour_sign;
//�Դ�����ɫ�����⣬��ֹй©�Ѿ����÷��ʽ�ķ���Ƕ�뵽�⵱�У��ǿ�Դ�����⣬���������dllʱ��������ͻᱻһ�������dll�У�������ɶ����ƣ�����ȱ�����޷����ȸ��µķ�ʽ�����������⣬�����µ�������ʱ�������dll���ŵ㣺�����Ը�ǿ��


//-
//��ɫ��Χ������
typedef struct colour{
	colour(int _Val){//���캯�������ڳ�ʼ��ʱ����,����û���Ҫ��ʾ��ʼ��ʱ�����øú�����
		memset(colour_name, _Val, sizeof(colour_name));
		for (int i = 0; i < 3; ++i){
			colour_min_rgb[i] = _Val;
			colour_mix_rgb[i] = _Val;
		}
		colour_size = _Val;
		*colour_next = _Val;
		*colour_top = _Val;
	}
	char colour_name[256];
	double colour_mix_rgb[3];
	double colour_min_rgb[3];
	int colour_size = 0;
	colour *colour_next = NULL;
	colour *colour_top = NULL;
}colour;
//��ɫ��Χ��������ز�������

//��ɫ��Χ�����������ڴ�
COLOURIN(void) colour_malloc(colour** colour_malloc){
	//����˼ά��ʹ�ö���ָ�뷽ʽ���Ϊcolour_malloc�����ڴ棬����ʹ��EAX��EDX�Ĵ�������������ٶȡ�
	*colour_malloc = (colour*)malloc(sizeof(colour));	//����ָ�������
	(*colour_malloc)->colour_next = NULL;
}

//��ȡ��ɫ��Χ��������С-�ѱ�colour_size�������棬�˺����ѷ���
COLOURIN(void) colour_get_size(colour** colour_, int *size){
	for (int i = 0;; ++i){
		*(colour_) = (*colour_)->colour_next;
		if (*(colour_) = NULL){
			*size = i;
			break;
		}
	}
}

//��ɫ������ṹ����ע��ú��������ڵ�һ�е��ã�����û����ɫ�����⽫�޷����в���
COLOURIN(void) colour_range_control(colour** colour_ = NULL, char **rgb_str = NULL, COLOUR_BOOL Self_Identification_Library = COLOUR_BOOL_TRUE/*�Ƿ�ʹ���Դ������⣬�����ѡ��ΪTRUE����rgb_str�ļ���Ч*/){
	//�ж�ָ���Ƿ�Ϊ��
	if (*colour_ == NULL){
		colour_error_type.colour_sign = COLOUR_BOOL_TRUE;
		strcpy_s(colour_error_type.error_type, COLOUR_ERROR_NULL);
		return;
	}
	//�ж�rgb_str�Ƿ�Ϊ��
	if (*rgb_str == NULL){
		colour_error_type.colour_sign = COLOUR_BOOL_TRUE;
		strcpy_s(colour_error_type.error_type, COLOUR_ERROR_NULL);
		return;
	}
	//�ж��Ƿ�ʹ���Դ�����ɫ������
	if (Self_Identification_Library == COLOUR_BOOL_TRUE){
		//ʹ���Դ�����ɫ������
		//ֱ��ʹ�õݹ鷽ʽ
		colour_range_control(colour_, &p_colour_sign, COLOUR_BOOL_FALSE);
		return;	//�����˴κ���

	}
	//��ȡ��ʶ����С
	char *sing_str = *rgb_str;
	char sing_char = 0;
	int sign_size = 0;
	int colour_sing_str_size = strlen(*rgb_str);
	//��ȡ�ܹ��ж��ٸ���ɫ����
	for (int i = 0; i <= colour_sing_str_size; ++i){
		sing_char = sing_str[i];
		if (sing_char == 0x3E){
			sign_size += 1;
		}
	}
	//(*colour_)->colour_size = colour_sing_str_size;
	//������ɫ������
	int j = 0/*���ڼ�¼��ȡ�����뵽�Ķ���*/, n = 0/*������name�±�*/, s = 0/*���ڱ�ʾ�����Ķ���*/, t = 0/*���ڶ�ȡminxRGBֵ*/, min = 0/*���ڱ�ʾmin�±�*/, of = 0/*���ڵ���ѭ�����±�j�±��ڻ�ȡ���ϵ��ַ�ʱ�ͻ�ֹͣ��¼of�������¼*/, mix = 0/*��¼mix*/, c = 0/*���ڶ�ȡmixRGBֵ*/;
	for (int i = 0; i < sign_size; ++i){
		colour *colour_1 = NULL;
		colour_malloc(&colour_1);
		colour_1->colour_top = colour_1;
		colour_1->colour_size = sign_size;	//����洢���ݽṹ�Ĵ�С
		memset(colour_1->colour_name, 0, sizeof(colour_1->colour_name));
		/*�Ż�˼·��ע���ڵײ㷽�棬c���Ա������ڱ������Ҫ���ַ�ת����ʮ��������ת����asccii�룬Ȼ����ת�ɶ����ƣ���������ֱ�ӽ���ת����ʮ������������߱����ٶ�
		: = 0x3A
		, = 0x2C
		= = 0x3D
		& = 0x26
		> = 0x3E
		*/
		//ѭ����ȡ
		for (;; ++j){
			if (sing_str[j] == 0x3A){
				switch (s){
				case 0:{
						   for (int k = j + 1;; ++k, ++n){
							   if (sing_str[k] == 0x2C){
								   s += 1;//����������ֻ�ȡ��ϸû�ȡ��ɫȡֵ��Χ��
								   n = 0;
								   break;
							   }
							   colour_1->colour_name[n] = sing_str[k];

						   }
				}
				case 1:{//��ȡMIN��RGB
						   of = j;
						   min = 0;
						   char w[4] = { 0 };
						   for (int u = 0; u < 3; ++u){
							   for (int y = of + 1;; ++y){
								   if (sing_str[y] == 0x3D){
									   for (int f = y;; ++f){
										   if (sing_str[f] == 0x26){
											   colour_1->colour_min_rgb[min] = atof(w);
											   min += 1;
											   t = 0;
											   of = f;
											   break;
										   }
										   w[t] = sing_str[f + 1];
										   if (t < 3){//У���Ƿ񵽴�rgb���ֵ
											   t = t + 1;
										   }

									   }
									   break;
								   }

							   }
						   }
				}
				}

			}
			if (sing_str[j] == 0x2C){//��ȡ����,��ʱ�����Ѿ�����β�ˣ����Կ���ֱ�ӻ�ȡmix��RGBֵ��
				char w[4] = { 0 };
				mix = 0;
				for (int i = of; sing_str[i] != 0x3E; ++i){
					if (sing_str[i] == 0x3D){
						for (int d = i;; ++d){
							if (sing_str[d] == 0x26){
								colour_1->colour_mix_rgb[mix] = atof(w);
								mix += 1;
								c = 0;
								of = d;
								break;
							}
							w[c] = sing_str[d + 1];
							if (c < 3){
								c += 1;
							}

						}
					}
				}
				j = of;//����j�±�
				s = 0;//����ʶλ����

				colour_1->colour_next = (*colour_);
				(*colour_) = colour_1;

				break;
				

			}
		}

	}

}

//��ɫ������ṹ��_�ļ���ʽ��ע��ú��������ڵ�һ�е��ã�����û����ɫ������������Ĳ������޷�����
COLOURIN(void) colour_range_control_file(colour** colour_ = NULL, char *colour_file = NULL, COLOUR_BOOL Self_Identification_Library = COLOUR_BOOL_TRUE/*�Ƿ�ʹ���Դ������⣬�����ѡ��ΪTRUE����rgb_str�ļ���Ч*/){
	//�ж�ָ���Ƿ�Ϊ��
	if (*colour_ == NULL){
		colour_error_type.colour_sign = COLOUR_BOOL_TRUE;
		strcpy_s(colour_error_type.error_type, COLOUR_ERROR_NULL);
		return;
	}
	//�ж���ɫ�������ļ�·���Ƿ�Ϊ��
	if (colour_file == NULL){
		colour_error_type.colour_sign = COLOUR_BOOL_TRUE;
		strcpy_s(colour_error_type.error_type, COLOUR_ERROR_NULL);
		return;
	}
	//�ж��������Ƿ���ȷ
	int str_len = strlen(colour_file);
	int colour_size = strlen(COLOURFILE);	//Ϊ��ͳһ����������һ��������ļ���
	char a[] = COLOURFILE;	//�ȶ���ʱ����
	int a_size = colour_size;
	a_size = a_size - 1;
	for (int i = str_len; i > str_len - colour_size; --i, --a_size){	//�ж��ļ���׺�Ƿ���ȷ��ע��������ϸ�����ļ���׺������������ȡ�ģ������ļ���׺����Ҫ
		if (a[a_size] != colour_file[i]){
			colour_error_type.colour_sign = COLOUR_BOOL_TRUE;
			strcpy_s(colour_error_type.error_type, COLOUR_ERROR_FILE_ERROR);
			return;
		}


	}
	//�ж��Ƿ�ʹ���Դ�����ɫ������
	if (Self_Identification_Library == COLOUR_BOOL_TRUE){
		//ʹ���Դ�����ɫ������
		//���ʹ���Դ��������⣬�Ͳ���Ҫȥ���������ݽṹ���������ļ���ֱ�ӵ���colour_range_control�������ɣ���Ϊ�Դ����������ļ��Ǵ����ڿ��еĶ����ļ����ڿ��е�����������str�ַ�����ʽ�洢�ģ�����ֱ�Ӵ��ε�colour_range_control�м��ɣ�
		colour_range_control(colour_, &p_colour_sign, COLOUR_BOOL_FALSE);
		return;	//�����˴κ���

	}
	//��ȡ��ʶ����С
	FILE *fp = fopen(colour_file, "r");
	if (fp == NULL){	//�޷�������������ļ�
		colour_error_type.colour_sign = COLOUR_BOOL_TRUE;
		strcpy_s(colour_error_type.error_type, COLOUR_ERROR_FILE_NULL);
		return;
	}
	//���������ļ������ɫ������ȡ����
	fseek(fp, 0, SEEK_END); //��λ���ļ�ĩ 
	int nFileLen = ftell(fp); //�ļ�����
	fseek(fp, 0, SEEK_SET);	//�ָ����ļ�ͷ����ֹ���ļ�β��ȡ����
	char *sing_str = (char*)malloc(nFileLen);	//malloc�����С���ļ���󣬾ͷ�����Ķ��ڴ�
	memset(sing_str, 0, nFileLen);	//������ڴ�ֵ�޷�ȷ�����������Ϊ0��ȷ�����ݿɿ���
	fread(sing_str, nFileLen, 1, fp);
	char sing_char = 0;
	int sign_size = 0;
	int colour_sing_str_size = strlen(colour_file);
	//��ȡ�ܹ��ж��ٸ���ɫ����
	for (int i = 0; i <= colour_sing_str_size; ++i){
		sing_char = sing_str[i];
		if (sing_char == 0x3E){
			sign_size += 1;
		}
	}
	//(*colour_)->colour_size = colour_sing_str_size;
	//������ɫ������
	int j = 0/*���ڼ�¼��ȡ�����뵽�Ķ���*/, n = 0/*������name�±�*/, s = 0/*���ڱ�ʾ�����Ķ���*/, t = 0/*���ڶ�ȡminxRGBֵ*/, min = 0/*���ڱ�ʾmin�±�*/, of = 0/*���ڵ���ѭ�����±�j�±��ڻ�ȡ���ϵ��ַ�ʱ�ͻ�ֹͣ��¼of�������¼*/, mix = 0/*��¼mix*/, c = 0/*���ڶ�ȡmixRGBֵ*/;
	for (int i = 0; i < sign_size; ++i){
		colour *colour_1 = NULL;
		colour_malloc(&colour_1);
		colour_1->colour_top = colour_1;
		colour_1->colour_size = sign_size;	//����洢���ݽṹ�Ĵ�С
		memset(colour_1->colour_name, 0, sizeof(colour_1->colour_name));
		/*�Ż�˼·��ע���ڵײ㷽�棬c���Ա������ڱ������Ҫ���ַ�ת����ʮ��������ת����asccii�룬Ȼ����ת�ɶ����ƣ���������ֱ�ӽ���ת����ʮ������������߱����ٶ�
		: = 0x3A
		, = 0x2C
		= = 0x3D
		& = 0x26
		> = 0x3E
		*/
		//ѭ����ȡ
		for (;; ++j){
			if (sing_str[j] == 0x3A){
				switch (s){
				case 0:{
						   for (int k = j + 1;; ++k, ++n){
							   if (sing_str[k] == 0x2C){
								   s += 1;//����������ֻ�ȡ��ϸû�ȡ��ɫȡֵ��Χ��
								   n = 0;
								   break;
							   }
							   colour_1->colour_name[n] = sing_str[k];

						   }
				}
				case 1:{//��ȡMIN��RGB
						   of = j;
						   min = 0;
						   char w[4] = { 0 };
						   for (int u = 0; u < 3; ++u){
							   for (int y = of + 1;; ++y){
								   if (sing_str[y] == 0x3D){
									   for (int f = y;; ++f){
										   if (sing_str[f] == 0x26){
											   colour_1->colour_min_rgb[min] = atof(w);
											   min += 1;
											   t = 0;
											   of = f;
											   break;
										   }
										   w[t] = sing_str[f + 1];
										   if (t < 3){//У���Ƿ񵽴�rgb���ֵ
											   t = t + 1;
										   }

									   }
									   break;
								   }

							   }
						   }
				}
				}

			}
			if (sing_str[j] == 0x2C){//��ȡ����,��ʱ�����Ѿ�����β�ˣ����Կ���ֱ�ӻ�ȡmix��RGBֵ��
				char w[4] = { 0 };
				mix = 0;
				for (int i = of; sing_str[i] != 0x3E; ++i){
					if (sing_str[i] == 0x3D){
						for (int d = i;; ++d){
							if (sing_str[d] == 0x26){
								colour_1->colour_mix_rgb[mix] = atof(w);
								mix += 1;
								c = 0;
								of = d;
								break;
							}
							w[c] = sing_str[d + 1];
							if (c < 3){
								c += 1;
							}

						}
					}
				}
				j = of;//����j�±�
				s = 0;//����ʶλ����

				colour_1->colour_next = (*colour_);
				(*colour_) = colour_1;

				break;


			}
		}

	}

}
//��ɫ�������ͷ�
COLOURIN(void) colour_dele(colour** colour_){
	colour* colour_next = NULL;
	if ((*colour_)->colour_size == 0){
		PASS	//ʲô������
			return;
	}
	colour_next = (*colour_)->colour_next;
	free((*colour_));
	*colour_ = NULL;
	for (int i = 0; i < (*colour_)->colour_size; ++i){
		if (i == 0){//�ж��Ƿ��ǵ�һ�Σ��ͷ��׽ڵ㣬ע�����һ��ʼ�ͷ�colour�Ļ����޷���ȡ�����next�ڵ��ַ��������ڴ�й©
			colour_next = (*colour_)->colour_next;
			free((*colour_));
		}
		else{//colour_�����Ѿ����ͷ��ˣ����������Ѿ�ָ����next�ڵ㣬������һ�ͷ��׽ڵ�֮��Ľڵ㼴�ɣ�
			colour* lour = colour_next;
			colour_next = colour_next->colour_next;	
			free(lour);
		}
	}
}

//��ɫ��Χ���������ֶ�ѵ���������������û����ֶ�����������,�汾��1.0  �ó���Ϊ��ɫʶ�����ӿ��Դ���һ�����򣬿���ֱ�ӵ���
COLOURIN(void) colour_manual_generate(char colour_file_path[]/*colour** colour_ = NULL ,�պ���ܻ������������ܻ��õ�������һ���ӿ�*/){
	//����ֵ
	int colour_bit = 1;	//��ͨ��
	COLOUR_BOOL open_file = COLOUR_BOOL_FALSE;
	COLOUR_BOOL file_name = COLOUR_BOOL_TRUE;
	/*
	if (*colour_ != NULL){
		colour_error_type.colour_sign = COLOUR_BOOL_TRUE;
		strcpy_s(colour_error_type.error_type, COLOUR_ERROR_NULL);
		return;
	}
	*/
	char l_w = 0;
	printf("*****************************\n");
	printf("ͼ����ɫʶ����-�ֶ�ѵ����\n");
	printf("*****************************\n");
	printf("�����뵱ǰ����ϵͳ�ͺţ��Ա��ں���Ļ�����ʼ����L(linux)/W(windows)����");
	scanf("%c", &l_w);
	getchar();	//�ѿո�Ե�
	int kpt = -1;
	//0x6C = l 0x77 = w
	//0x4C = L 0x57 = W
	if (l_w == 0x6C || l_w == 0x4C){	//linux
		kpt = 0;
		printf("\n������Linux������ʼ�����ֶ���ɫѵ���⻷��..\n\n");
	}
	else if (l_w == 0x77 || l_w == 0x57){  //windows
		kpt = 1;
		printf("\n������Windows������ʼ�����ֶ���ɫѵ���⻷��..\n\n");
	}
	else{
		printf("\n����Ĳ���ϵͳ���ͣ������޷�ʶ��ʹ��Ĭ��ϵͳ�ͺŽ��л������죺Windows\n\n");
		kpt = 1;
	}
	ins:
	//----��ʼ�����û����ֶ�����������
	printf("���ڳ�ʼ������...\n\n");
	/*
	colour_ = {0};	//��ʾ��ʼ���ṹ��
	*/
	printf("��ʾ��ʼ�����ݽṹ�������!\n\n");
	//----��������ʶ���ֶ�ѵ����������
	printf("*****************************\n\n");
	printf("ͼ����ɫʶ����-�ֶ�����ѵ����1.0\n\n");
	printf("*****************************\n\n");
	printf("�Ƿ�����ѵ���⣿(Y/N)��");
	char y_n = 0;
	scanf("%c", &y_n);
	FILE *fp = NULL;
	//0x79 = y 0x6E = n
	//0x59 = Y 0x4E = N
	if (y_n == 0x79 || y_n == 0x59){	//y
		in_y:
		printf("\n������ѵ����λ��:");
		char path[256] = {0};
		scanf("%s", path);
		getchar();	//�Ե��س�
		int str_len = strlen(path);
		int colour_size = strlen(COLOURFILE);	//Ϊ��ͳһ����������һ��������ļ���
		char a[] = COLOURFILE;	//�ȶ���ʱ����
		int a_size = colour_size;
		a_size = a_size - 1;	//�����±��0��ʼ������Ҫ-1
		for (int i = str_len - 1; i > str_len - colour_size; --i, --a_size){	//�ж��ļ���׺�Ƿ���ȷ��ע��������ϸ�����ļ���׺������������ȡ�ģ������ļ���׺����Ҫ
			if (a[a_size] != path[i]){
				printf("\n�ⲻ��һ����ȷ���������ļ���ʽ,����������\n\n");
				goto in_y;
			}
		}
		fopen_s(&fp, path, "r");	//���Զ��ķ�ʽ�򿪣�ȷ���ļ�����
		if (fp == NULL){
			printf("�޷��򿪸��ļ����ļ������ڣ�\n\n����������\n\n");
			goto in_y;
		}
		else{
			if (kpt == 0){
				system("clear");
			}
			if (kpt == 1){
				system("cls");
			}
			printf("*************************************\n");
			printf("���ڼ���%s��������....\n", path);
			fclose(fp);
			fopen_s(&fp, path, "a");	//�Ը��ӵķ�ʽ��
			printf("����������:%s�ɹ�\n", path);
			printf("*************************************\n\n");
		}
	}
	else if (y_n == 0x6E || y_n == 0x4E){ //n
		if (kpt == 0){
			system("clear");
		}
		if (kpt == 1){
			system("cls");
		}
		//PASS;	//ʲô������
	}
	else{
		//��ƽ̨���ԣ����ݲ���ϵͳ��ͬ���ò�ͬ���ն˴������windows������ն˻������ĳ����ǣ�cls��LINUX:clear
		if (kpt == 0){
			system("clear");
		}
		if (kpt == 1){
			system("cls");
		}
		printf("�����޷�ȷ��������ļ������Ƿ���ȷ����������һ�Σ�\n\n");
		goto ins;
	}
	//��ʼ�����ֶ���ɫѵ��
	ins_1:
	printf("\n-��ѡ����Ҫ���е��ֶ�ѵ������\n\n");
	printf("����ݱ��ѡ���ӦҪѵ���ķ�����\n\n");
	printf("1.����һ��ͼƬ���ɳ�����ȡ��ͼƬ�е���ɫRGB����������RGB������֪�û��������û�ѡ���Ӧ��RGB��������RGB������Ӧ����ɫ�����������,�ù��̻��Թ��ظ����ص㣡\n\n");
	printf("2.������֪��RGB��������ɫ�����뵽�����У�����ʽ�����浽�������У�����������򸽼Ӳ��������½���\n\n");
	printf("3.�ȶԲ�������\n\n");
	printf("4.�˳��ֶ�ѵ��\n\n");
	printf("<����������ѡ��>:");
	int option = 0;
	scanf("%d", &option);
	getchar();	//�Ե��س�
	int d = 0;
	colour *s = {0};
	colour_malloc(&s);
	FILE *RGB = NULL;
	char sd[1024] = {0};
	switch (option){
	case 1:{	//����ͼ����ȡ
			   COLOUR_BOOL image_ = COLOUR_BOOL_NOT;//�����ж�
		   img_in:
			   if (kpt == 0){
				   system("clear");
			   }
			   if (kpt == 1){
				   system("cls");
			   }
			   if (image_ == COLOUR_BOOL_FALSE){
				   printf("ͼ���ļ��޷���.����������\n");
			   }
			   printf("������Ҫ������ͼƬ�ļ�·����");
			   char img_path[256] = {0};
			   scanf("%s", &img_path);
			   getchar();	//�Ե��س�fflus
			   if (kpt == 0){
				   system("clear");
			   }
			   if (kpt == 1){
				   system("cls");
			   }
			   printf("******************************\n");
			   printf("���ڼ���ͼ���ļ�...\n");
			   IplImage *image = NULL;	//img
			   if (colour_bit == 1){
				   image = cvLoadImage(img_path);    //��ͨ��
			   }
			   else{
				   image = cvLoadImage(img_path,0);	//��ͨ��
			   }
			   if (image == NULL){
				   image_ = COLOUR_BOOL_FALSE;
				   goto img_in;
			   }
			   else{
				   image_ = COLOUR_BOOL_TRUE;
			   }
			   printf("����ͼ���ļ��ɹ�\n");
			   printf("******************************\n");
			   //��ʼ����RGB��ɫ����
			   if (fp != NULL){	//������������
				   //����һ����ɫѡ����
				   colour* c = NULL;
				   //�����ڴ�
				   colour_malloc(&c);
				   //����ɫ�������е�������ȡ����
				   fseek(fp, 0, SEEK_END); //��λ���ļ�ĩ 
				   int nFileLen = ftell(fp); //�ļ�����
				   char *colour_str = (char*)malloc(nFileLen);
				   fread(colour_str, nFileLen, 1, fp);	//��ɫ������ȡ
				   colour_range_control(&c, &colour_str, COLOUR_BOOL_FALSE);	//��ɫ�����ṹ��
				   if (colour_error_type.colour_sign == COLOUR_BOOL_TRUE){	//�ж���û�д���
					   printf("colour_type_error:%s,���»س���������", colour_error_type.error_type);
					   getchar();
					   goto img_in;
				   }
				   //��ʼʶ��
				   //��ʼʶ����ɫ
				   printf("������Ҫ��������ȡ������ݱ����·����");
				   char path[256];
				   scanf("%c", path);
				   getchar();	//�س�
				   if (kpt == 0){
					   system("clear");
				   }
				   if (kpt == 1){
					   system("cls");
				   }
				   printf("********************************\n");
				   printf("���ڴ򿪱����ļ���...\n");
				   CvScalar scalar;    //scalar  
				   int num = 0;
				   int form_size = 0;
				   FILE* colour_fp = NULL;
				   unsigned long int colour_num = 0;
				   //�����Զ���ʽ�򿪣��鿴�ļ��Ƿ����
				   fopen_s(&colour_fp, path, "r");
				   if (colour_fp == NULL){
					   printf("�޷��������ļ����ļ�������.\n");
					   printf("���ڴ����ļ���...");
					   fopen_s(&colour_fp, path, "w+");
					   if (colour_fp == NULL){
						   printf("�޷��������ļ�!\n���»س�������");
						   getchar();
						   return;
					   }
					   else{
						   printf("�ļ������ɹ�!\n");
					   }

				   }
				   //���ӷ�ʽ��
				   fclose(colour_fp);
				   fopen_s(&colour_fp, path, "a");
				   printf("********************************\n");
				   printf("��ʼ��ȡ,��Լ��Ҫ��%d��\n", image->height);
				   for (int i = 0; i <= image->height - 1; ++i){
					   printf("���ڽ��е�%d�μ���\n", i);
					   for (int j = 0; j <= image->width - 1; ++j){
						   scalar = cvGet2D(image, i, j);    //��ȡ���ص��RGB��ɫ����  
						   //�ȶ��㷨_��ͨ��
						   if (colour_bit == 1){
							   if (scalar.val[2] >= c->colour_min_rgb[0] && scalar.val[1] >= c->colour_min_rgb[1] && scalar.val[0] >= c->colour_min_rgb[2] &&
								   scalar.val[2] <= c->colour_mix_rgb[0] && scalar.val[1] <= c->colour_mix_rgb[1] && scalar.val[0] <= c->colour_mix_rgb[2]){	//�����ɫ����������ɫһ�£���ֱ��pass
								   printf("�����ظ���RGB��ɫ������Ϊ��MIN:%d,%d,%d ~ MIX:%d,%d,%d���������ص�λ����:��%d�е�%d��", c->colour_min_rgb[0], c->colour_min_rgb[1], c->colour_min_rgb[2], c->colour_mix_rgb[0], c->colour_mix_rgb[1], c->colour_mix_rgb[2], i, j);


							   }
							   else{
								   //���򱣴浽�����ļ�
								   char as[256];
								   sprintf(as, "R:%d,G:%d,B:%d\n", scalar.val[2], scalar.val[1], scalar.val[0]);
								   fwrite(as, strlen(as) + 1/*����\n*/, 1, colour_fp);
								   colour_num += 1;
							   }
						   }//�ȶ��㷨_��ͨ��
						   else if (colour_bit == 0){
							   if (scalar.val[2] >= c->colour_min_rgb[0] && scalar.val[0] <= c->colour_mix_rgb[2]){	//�����ɫ����������ɫһ�£���ֱ��pass
								   printf("�����ظ�����ɫ������Ϊ��%d ~ %d���������ص�λ����:��%d�е�%d��", c->colour_min_rgb[0], c->colour_mix_rgb[0], i, j);
							   }
							   else{
								   //���򱣴浽�����ļ�
								   char as[256];
								   sprintf(as, "colour:%d\n", scalar.val[2], scalar.val[1], scalar.val[0]);
								   fwrite(as, strlen(as) + 1/*����\n*/, 1, colour_fp);
								   colour_num += 1;
							   }
						   }
					   }
					   printf("������ɣ����ι���ȡ��%d��δ֪RGB��ɫ���������»س�����", colour_num);
					   if (open_file == COLOUR_BOOL_TRUE){
						   if (kpt == 0){	//��ƽ̨
							   printf("�Ѿ�Ϊ���Զ����������ļ�");
							   char system_[256] = { 0 };
							   sprintf(system_, "cat %s", colour_file_path);
							   system(system_);
						   }
						   if (kpt == 1){
							   printf("�Ѿ�Ϊ���Զ����������ļ�");
							   char system_[256] = { 0 };
							   sprintf(system_, "cd %s", colour_file_path);
							   system(system_);
						   }

					   }

				   }
			   }
			   else{
				   printf("����û��Ϊ�����ƶ�һ������ƥ��������⣬�����޷����У�\n\n");
				   printf("��������������������·������ѡ�񱾹���!\n\n");
				   goto ins;

			   }
			   printf("�������.��������rgb��ɫ�����Ѿ����浽��%s�ļ�Ŀ¼�£����»س�����", colour_file_path);
			   getchar();
			   return;
	}
	break;
	case 2:{//������֪RGB��������ɫ�����뵽�����У������浽�����⵱�У����ӣ��������򴴽�
			   fopen_s(&RGB, colour_file_path, "r");
			   if (RGB == NULL){//�ļ�������,�����ļ�
				   fopen_s(&RGB, colour_file_path, "w");
			   }
			   //վ��fp����ֹ��ֹ����������ռ��
			   if (kpt == 0){
				   system("clear");
			   }
			   if (kpt == 1){
				   system("cls");
			   }
			   int s1 = 0;
		   ins_2:
			   printf("**************************\n");
			   printf("-�ֶ�RGB��ɫ����ѵ��\n\n");
			   printf("**************************\n");
			   printf("������Ҫ��ȡ�ķ�������:");
			   scanf("%d", &d);
			   printf("\n");
			   if (d > 99){
				   printf("������С���ƣ�һ��ֻ������99��\n\n");
				   goto ins_2;
			   }
			   char asd[1024];
			   getchar();	//�س�
			   for (int i = 0; i < d; ++i){
				   printf("\n���ڽ��еڡ�%d������ɫ������ȡ\n\n",i+1);
				   printf("\n��ɫ����");
				   scanf("%s", s->colour_name);
				   getchar();	//�س�
				   printf("MIN-RGB���ԡ��ո񡱷ָ:");
				   scanf("%d %d %d", &s->colour_min_rgb[0], &s->colour_min_rgb[1], &s->colour_min_rgb[2]);
				   getchar();	//�س�
				   printf("MIX-RGB���ԡ��ո񡱷ָ:");
				   scanf("%d %d %d", &s->colour_mix_rgb[0], &s->colour_mix_rgb[1], &s->colour_mix_rgb[2]);
				   getchar();//�س�
				   sprintf(sd, "%s<colour:%s,MIN:R=%d&G=%d&B=%d&,MIX:R=%d&G=%d&B=%d&>", sd,s->colour_name, s->colour_min_rgb[0], s->colour_min_rgb[1], s->colour_min_rgb[2], s->colour_mix_rgb[0], s->colour_mix_rgb[1], s->colour_mix_rgb[2]);
			   }
			   //������ɱ���
			   printf("�ֶ�ѵ����ɫ����������ɣ����ڽ���ɫ�������浽%s��������..\n", colour_file_path);
			   //�ж�һ��Ҫ������ļ��ǲ���һ����ȷ��colour�������ļ�
			   int str_len = strlen(colour_file_path);
			   int colour_size = strlen(COLOURFILE);	//Ϊ��ͳһ����������һ��������ļ���
			   char a[] = COLOURFILE;	//�ȶ���ʱ����
			   int a_size = colour_size;
			   a_size = a_size - 1;	//�����±��0��ʼ������Ҫ-1
			   for (int i = str_len-1; i > str_len - colour_size; --i, --a_size){	//�ж��ļ���׺�Ƿ���ȷ��ע��������ϸ�����ļ���׺������������ȡ�ģ������ļ���׺����Ҫ
				   if (a[a_size] != colour_file_path[i]){
					   printf("Ҫ������ļ�������ȷ���������ļ�.\n");
					   if (file_name == COLOUR_BOOL_TRUE){
						   printf("�������¹滮�������ļ���..\n");
						   if (str_len >= FILE_NAME_MIX - a_size){	//arr size - colour_file_size = 247(-1),���1�ǳ���Խ��İ�ȫ�Ŀ��ǣ��ļ�����С��������
							   printf("�޷����¹滮�������ļ�!\n");
							   colour_error_type.colour_sign = COLOUR_BOOL_TRUE;
							   strcpy_s(colour_error_type.error_type, COLOUR_ERROR_FILE_NAME);
							   return;
						   }
						   else{
							   //Ϊ�ļ��Զ����Ϻ�׺
							   char b[] = COLOURFILE;
							   int k = 0;
							   for (int j = str_len; j < str_len + colour_size; ++j, ++k){
								   colour_file_path[j] = b[k];
								   if (k == colour_size){	//����Խ��
									   break;
								   }
							   }
							   break;
						   }
					   }
				   }


			   }
			   FILE* fp = NULL;
			   fclose(RGB);
			   fopen_s(&fp, colour_file_path, "a");
			   if (fp == NULL){
				   colour_error_type.colour_sign = COLOUR_BOOL_TRUE;
				   strcpy_s(colour_error_type.error_type, COLOUR_ERROR_FILE_NULL);
				   return;
			   }
			   fwrite(sd, strlen(sd), 1, fp);
			   //�ֶ�ѵ�����������
			   printf("ѵ������������������ѱ��浽:%sĿ¼�ļ���\n", colour_file_path);
			   if (open_file == COLOUR_BOOL_TRUE){
				   printf("�Ѿ�Ϊ���Զ����������ļ�");
				   if (kpt == 0){	//��ƽ̨
					   char system_[256] = { 0 };
					   sprintf(system_, "cat %s", colour_file_path);
					   system(system_);
				   }
				   if (kpt == 1){
					   char system_[256] = { 0 };
					   sprintf(system_, "cd %s", colour_file_path);
					   system(system_);
				   }

			   }
			   printf("���»س�������������������д�뵽�ļ�����!");
			   getchar();
			   
	}
	break;
	case 3:{//�ȶԲ�������
			   if (kpt == 0){
				   system("clear");
			   }
			   if (kpt == 1){
				   system("cls");
			   }
			   printf("\n��ѡ��Ҫ���õĲ���:\n\n");
			   printf("1.����ͼ��ͨ��\n\n");
			   printf("2.���ü��������ر�\n\n");
			   printf("3.���ô洢�ļ���ʽ����ȷ�Ƿ��Զ�����\n\n");
			   printf("4.���öԱ��㷨\n\n");
			   printf("5.������һ��ѡ��\n\n");
			   printf(":");
			   char a;
			   scanf("%d", &a);
			   switch (a){
			   case 1:
				   printf("�������ͨ�����ǵ�ͨ��0(��ͨ��)/1(��ͨ��)��");
				   int in;
				   scanf("%d", &in);
				   if (in == 0){
					   if (kpt == 0){
						   system("clear");
					   }
					   if (kpt == 1){
						   system("cls");
					   }
					   printf("������ɣ���ǰ�Ե�ͨ��ģʽ����ͼ��\n");
					   goto ins_1;
				   }
				   else if (in == 1){
					   if (kpt == 0){
						   system("clear");
					   }
					   if (kpt == 1){
						   system("cls");
					   }
					   printf("������ɣ���ǰ�Զ�ͨ��ģʽ����ͼ��\n");
					   goto ins_1;
				   }
				   else{
					   if (kpt == 0){
						   system("clear");
					   }
					   if (kpt == 1){
						   system("cls");
					   }
					   printf("��������..�޷���ɵ�ǰ������������");
					   goto ins_1;
				   }
				   break;
			   case 2:
				   ins_3:
				   printf("�ֶ�ѵ����ɺ��Ƿ��Զ����������ļ�?(y/n)��");
				   char a;
				   scanf("%c", &a);
				   if (a == 0x79 || a == 0x59){	//y
					   open_file = COLOUR_BOOL_TRUE;
					   if (kpt == 0){
						   system("clear");
					   }
					   if (kpt == 1){
						   system("cls");
					   }
					   printf("������ɣ���ѵ�������ɫ�����󣬻��Զ����洢��ɫ�������������ļ��򿪣�\n");
					  
				   }
				   else if (a == 0x6E || a == 0x4E){ //n
					   open_file = COLOUR_BOOL_FALSE;
					   if (kpt == 0){
						   system("clear");
					   }
					   if (kpt == 1){
						   system("cls");
					   }
					   printf("������ɣ���ѵ�������ɫ�����󣬲����Զ����洢��ɫ�������������ļ��򿪣�\n");
				   }
				   else{
					   if (kpt == 0){
						   system("clear");
					   }
					   if (kpt == 1){
						   system("cls");
					   }
					   printf("��������..�޷���ɵ�ǰ������������\n");
					   goto ins_3;
				   }
				   break;
			   case 3:
				   printf("�Ƿ����ļ��洢��ʽ����ȷʱ�Զ�������0(yes)/1(no)��");
				   int in1;
				   scanf("%d", &in1);
				   if (in1 == 0){
					   file_name = COLOUR_BOOL_TRUE;
					   if (kpt == 0){
						   system("clear");
					   }
					   if (kpt == 1){
						   system("cls");
					   }
					   printf("������ɣ����ļ���ʽ����ȷʱ���Զ�����\n");
					   goto ins_1;
				   }
				   else if (in1 == 1){
					   file_name = COLOUR_BOOL_FALSE;
					   if (kpt == 0){
						   system("clear");
					   }
					   if (kpt == 1){
						   system("cls");
					   }
					   printf("������ɣ����ļ���ʽ����ȷʱ�����Զ���������������ʾ�û�\n");
					   goto ins_1;
				   }
				   else{
					   if (kpt == 0){
						   system("clear");
					   }
					   if (kpt == 1){
						   system("cls");
					   }
					   printf("��������..�޷���ɵ�ǰ������������");
					   goto ins_1;
				   }
				   break;
			   case 4:
				   if (kpt == 0){
					   system("clear");
				   }
				   if (kpt == 1){
					   system("cls");
				   }
				   printf("��ʱ��ֻ�д�ͳ������ɫֵ�Ա�һ���㷨��\n\n");
				   goto ins_1;
				   break;
			   case 5:
				   if (kpt == 0){
					   system("clear");
				   }
				   if (kpt == 1){
					   system("cls");
				   }
				   goto ins_1;
				   break;
			   default:
				   if (kpt == 0){
					   system("clear");
				   }
				   if (kpt == 1){
					   system("cls");
				   }
				   printf("�����ѡ��\n");
				   goto ins_1;

			   }
	}
	break;
	case 4:{	//�˳�
			   printf("\nͼ��ʶ����-�ֶ�ѵ��1.0 �����Ѿ�����\n");
			   return;
	}
	break;
	default:
		printf("�����ѡ��\n");
		goto ins;

	}

	
}


//�Զ�ѵ����1.0
COLOURIN(void) machine_learning_colour_train(char path[]/*ͼƬ�ļ�·��*/, char txt[]/*ѵ���ļ���*/, char Catalog[]/*ѵ�����Ŀ¼*/,char colour_path[]/*������·��*/){
	char path_txt[1024] = {0};
	sprintf(path_txt, "%s\\%s", path, txt);
	FILE *fp = fopen(path_txt, "r");
	if (fp == NULL){
		printf("�޷���ʼѵ����ԭ���޷���ѵ���ļ���");
		getchar();
		return;
	}
	//��ȡѵ������������·��
	char path_Catalog[1024] = { 0 };
	sprintf(path_Catalog, "%s\\%s", Catalog, KEY_FILE);
	FILE *fp1 = fopen(path_Catalog, "r");	//����ֻ���ķ�ʽȷ���ļ��Ƿ����
	if (fp1 == NULL){	//�����ڴ���
		fp1 = fopen(path_Catalog, "w");
	}
	else{	//�����Ը��ӵķ�ʽ��
		fclose(fp1);	//�ر����´�
		fp1 = fopen(path_Catalog, "a");
	}
	char colour_str[256] = {0};	//ѵ��������ʱ���վ��
	int error_num = 0;	//��¼ѵ��ʧ�ܵ�ͼ���ļ�	
	int ok_num = 0;	//��¼ѵ���ɹ���ͼ���ļ�
	//�������ļ����ݽṹ��
	colour* colour_ = NULL;
	colour_malloc(&colour_);
	colour_range_control(&colour_);
	if (colour_error_type.colour_sign == COLOUR_BOOL_TRUE){
		printf("colour_type_error:%s", colour_error_type.error_type);
		getchar();
		return;
	}
	if (fp == NULL){
		printf("�޷�����Զ�ѵ��,�����ļ�������!\n");
	}
	printf("*********************��ʼѵ��*********************\n");
	char strLine[256] = { 0 };
	int i = 0;	//ѵ����¼��
	int num = 0;	//�Ѿ������������ص�
	int file_size = 0;	//�ж����ļ�
	while (!feof(fp))                                   //ѭ����ȡÿһ�У�ֱ���ļ�β  
	{
		fgets(strLine, sizeof(strLine), fp);
		file_size += 1;	//++file_num
	}
	//����Ų��ָ��,��λ���ļ�ͷ
	fseek(fp, 0L, SEEK_SET);
	while (!feof(fp))                                   //ѭ����ȡÿһ�У�ֱ���ļ�β  
	{
		fgets(strLine, sizeof(strLine), fp);                     //��fp��ָ����ļ�һ�����ݶ���strLine������  
		if (strLine[strlen(strLine) - 1/*arr_0*/] == '\n'){	//ɾ������
			strncpy(strLine, strLine, (strlen(strLine) - 1));	//copyԭ����-1(\n)

		}
		//�Ѿ��õ���һ���ļ�����ʼѭ��ѵ��
		i += 1;
		printf("����ѵ���ڡ�%d/%d����ͼƬ", i+1,file_size+1);
		//��ʼѵ����x��ͼƬ
		IplImage *image = cvLoadImage(strLine,1);	//ԭͼ�����һ��ͼ���ļ�
		if (image == NULL){
			printf("%s����ͼ���ļ��޷���..\n", strLine);
			error_num += 1;
		}
		else{
			CvScalar scalar;    //scalar  
			for (int i = 0; i <= image->height - 1; ++i){
				for (int j = 0; j <= image->width - 1; ++j){
					scalar = cvGet2D(image, i, j);    //��ȡ���ص��RGB��ɫ����  
					for (int z = 0;; ++z){
						//�ȶ��㷨_1
						if (scalar.val[2] >= colour_->colour_min_rgb[0] && scalar.val[1] >= colour_->colour_min_rgb[1] && scalar.val[0] >= colour_->colour_min_rgb[2] &&
							scalar.val[2] <= colour_->colour_mix_rgb[0] && scalar.val[1] <= colour_->colour_mix_rgb[1] && scalar.val[0] <= colour_->colour_mix_rgb[2]){

							//���˵��ظ���ɫ����һ����ɫ
							if (num < colour_->colour_size){	//�Թ�����ѭ��������ɫ����������ʱ�����޷��жϳ��������ɫ��ֱ�ӷ����ѻ�ȡ������ɫ
								colour_ = colour_->colour_next;
								num += 1;
							}
							else{
								for (int y = 0; y < num; ++y){
									colour_ = colour_->colour_top;
								}
								num = 0; 
								break;	//����ѭ��
							}

						}
						else{	//����������û��ʶ���������ɫ��������
							memset(colour_str, 0, sizeof(colour_str));
							sprintf(colour_str, "<colour:,MIN:R=%d&G=%d&B=%d&,MIX:R=%d&G=%d&B=%d&>", 0, 0, 0, scalar.val[2], scalar.val[1], scalar.val[0]);
							fwrite(path_Catalog, strlen(path_Catalog), 1, fp1);

						}
					}
				}
			}
		}
		printf("��ʼѵ����һ��\n");
		
	}
}
//��һ��RGBͼ���������ɫ��Ϣ���浽����
COLOURIN(void) machine_learning_colour_train_debug(IplImage *To_Identify_Colour_The_Image/*ͼƬ�ļ�·��*/, char path[]/*��Ϣ���·��*/){
	IplImage *imag = cvLoadImage(path);
	CvScalar scalar;    //scalar  

	if (imag == NULL){
		colour_error_type.colour_sign = COLOUR_BOOL_TRUE;
		strcpy_s(colour_error_type.error_type, COLOUR_ERROR_FILE_IMAGE_NULL);
		return;
	}
	FILE *fp = fopen(path, "w");
	if (fp == NULL){ 
		colour_error_type.colour_sign = COLOUR_BOOL_TRUE;
		strcpy_s(colour_error_type.error_type, COLOUR_ERROR_FILE_NULL);
		return;
	}
	char str[256] = { 0 };
	for (int i = 0; i <= imag->height - 1; ++i){
		for (int j = 0; j <= imag->width - 1; ++j){
			//������д��
			memset(str, 0, sizeof(str));
			sprintf(str, "height_inx:%d,width_inx:%d,R=%d,G=%d,B=%d\n", i, j, scalar.val[2], scalar.val[1], scalar.val[0]);
			fwrite(str, strlen(str), 1, fp);
		}
	}
	
}
//�����е���ɫ�������浽�������ļ���
COLOURIN(void) colour_features_seve(char str[],char path[256]){
	//�ж�һ��Ҫ������ļ��ǲ���һ����ȷ��colour�������ļ�
	int str_len = strlen(path);
	int colour_size = strlen(COLOURFILE);	//Ϊ��ͳһ����������һ��������ļ���
	char a[] = COLOURFILE;	//�ȶ���ʱ����
	int a_size = colour_size;
	a_size = a_size - 1;
	for (int i = str_len; i > str_len - colour_size; --i, --a_size){	//�ж��ļ���׺�Ƿ���ȷ��ע��������ϸ�����ļ���׺������������ȡ�ģ������ļ���׺����Ҫ
		if (a[a_size] != path[i]){
			if (str_len >= FILE_NAME_MIX - a_size){	//arr size - colour_file_size = 249(-1),���1�ǳ���Խ��İ�ȫ�Ŀ���
				colour_error_type.colour_sign = COLOUR_BOOL_TRUE;
				strcpy_s(colour_error_type.error_type, COLOUR_ERROR_FILE_NAME);
				return;
			}
			else{
				//Ϊ�ļ��Զ����Ϻ�׺
				char b[] = COLOURFILE;
				int k = 0;
				for (int j = str_len - 1;j < str_len+colour_size - 1; ++j,++k){
					path[j] = b[k];
					if (k == colour_size - 1){
						break;
					}
				}
				break;
			}
		}
	
	
	}
	//д���ļ�
	FILE* fp = NULL;
	fopen_s(&fp,path, "w");
	if (fp == NULL){
		colour_error_type.colour_sign = COLOUR_BOOL_TRUE;
		strcpy_s(colour_error_type.error_type, COLOUR_ERROR_FILE_NULL);
		return;
	}
	fwrite(str, strlen(str), 1, fp);

}

//-
//ͼ����ɫ������
typedef struct image_colour_form{
	char colour_name[256];
	int colour_num = 0;
	image_colour_form* next_colour_name;
	image_colour_form* top_colour_name;

}image_colour_form;
//ͼ����ɫ��������ز�������

//��ɫ�����������ڴ�
COLOURIN(void) image_form_malloc(image_colour_form** image_colour_form_malloc){
	//����˼ά����ָ�뷽ʽ���Ϊimage_colour_form_malloc�����ڴ棬����ʹ��EAX��EDX�Ĵ�������������ٶȡ�
	*image_colour_form_malloc = (image_colour_form*)malloc(sizeof(image_colour_form));	//����ָ�������
}

//��ɫ���������
COLOURIN(char *)colour_sing_load(char path[]){
	//�ж�һ��Ҫ���ص��ļ��ǲ���һ����ȷ��colour�������ļ�
	int str_len = strlen(path);
	int colour_size = strlen(COLOURFILE);	//Ϊ��ͳһ����������һ��������ļ���
	char a[] = COLOURFILE;	//�ȶ���ʱ����
	int a_size = colour_size;
	a_size = a_size - 1;
	for (int i = str_len; i > str_len - colour_size; --i, --a_size){	//�ж��ļ���׺�Ƿ���ȷ��ע��������ϸ�����ļ���׺������������ȡ�ģ������ļ���׺����Ҫ
		if (a[a_size] != path[i]){
			if (str_len >= FILE_NAME_MIX - a_size){	//arr size - colour_file_size = 249(-1),���1�ǳ���Խ��İ�ȫ�Ŀ���
				colour_error_type.colour_sign = COLOUR_BOOL_TRUE;
				strcpy_s(colour_error_type.error_type, COLOUR_ERROR_FILE_NAME);
				return "-1";
			}
			else{
				//Ϊ�ļ��Զ����Ϻ�׺
				char b[] = COLOURFILE;
				int k = 0;
				for (int j = str_len - 1; j < str_len + colour_size - 1; ++j, ++k){
					path[j] = b[k];
					if (k == colour_size - 1){
						break;
					}
				}
				break;
			}
		}


	}
	FILE* fp = NULL;
	fopen_s(&fp, path, "w");
	if (fp == NULL){
		colour_error_type.colour_sign = COLOUR_BOOL_TRUE;
		strcpy_s(colour_error_type.error_type, COLOUR_ERROR_FILE_NULL);
		return "-1";
	}
	fseek(fp, 0, SEEK_END); //��λ���ļ�ĩ 
	int nFileLen = ftell(fp); //�ļ�����
	fseek(fp, 0, SEEK_SET);	//�ָ����ļ�ͷ����ֹ���ļ�β��ȡ����
	char *colour_str = (char*)malloc(nFileLen);
	memset(colour_str, 0, nFileLen);	//������ڴ�ֵ�޷�ȷ�����������Ϊ0��ȷ�����ݿɿ���
	fread(colour_str, nFileLen, 1, fp);
	return colour_str;
}

//��ɫʶ����
image_colour_form *Colour_Distinguish(IplImage *To_Identify_Colour_The_Image, colour** colour_)        {
	image_colour_form *image_colour_form_return1;
	image_form_malloc(&image_colour_form_return1);
	//��ʼʶ����ɫ
	CvScalar scalar;    //scalar  
	int num = 0;
	int form_size = 0;
	int colour_num = 0;
	for (int i = 0; i <= To_Identify_Colour_The_Image->height - 1; ++i){	//-1����Ϊ��ջspָ��ָ���ַ�±��Ǵ�0��ʼ��
		for (int j = 0; j <= To_Identify_Colour_The_Image->width - 1; ++j){
			scalar = cvGet2D(To_Identify_Colour_The_Image, i, j);    //��ȡ���ص��RGB��ɫ����  
			for (int k = 0;k<(*colour_)->colour_size; ++k){
				//�ȶ��㷨_1
				//<= <,>=,>  �ڶԱ�ʱ����ȷ����Χ������min��mix֮�䣬����Ҫ��֤���п����Ա�����ڵ��ڻ���ڣ�С�ڵ��ڻ�С��
				if ((scalar.val[2] >= (*colour_)->colour_min_rgb[0] && scalar.val[1] >= (*colour_)->colour_min_rgb[1] && scalar.val[0] >= (*colour_)->colour_min_rgb[2])||
					(scalar.val[2] > (*colour_)->colour_min_rgb[0] && scalar.val[1] > (*colour_)->colour_min_rgb[1] && scalar.val[0] > (*colour_)->colour_min_rgb[2])
					&&(scalar.val[2] <= (*colour_)->colour_mix_rgb[0] && scalar.val[1] <= (*colour_)->colour_mix_rgb[1] && scalar.val[0] <= (*colour_)->colour_mix_rgb[2])||
					(scalar.val[2] < (*colour_)->colour_mix_rgb[0] && scalar.val[1] < (*colour_)->colour_mix_rgb[1] && scalar.val[0] < (*colour_)->colour_mix_rgb[2])){
					image_colour_form *image_colour_form_return;
					image_form_malloc(&image_colour_form_return);
					colour_num += 1;	//��¼��ȡ���˶�����ɫ
					strcpy_s(image_colour_form_return->colour_name, (*colour_)->colour_name); //����ɫ���ƶ�ȡ����
					image_colour_form_return->top_colour_name = image_colour_form_return;
					image_colour_form_return->next_colour_name = image_colour_form_return1;
					image_colour_form_return1 = image_colour_form_return;
					//���˵��ظ���ɫ����һ����ɫ
					if (num < (*colour_)->colour_size){	//�Թ�����ѭ��������ɫ����������ʱ�����޷��жϳ��������ɫ����ʼ��ȡ��һ����ɫֵ
						(*colour_) = (*colour_)->colour_next;
						num += 1;
					}
					else{
						for (int y = 0; y < num; ++y){//��ɫ�ֻ�
							(*colour_) = (*colour_)->colour_top;
						}
						num = 0;
						break;
						//return image_colour_form_return;
					}
				}
				
			}
		}
	}
	for (int i = 0; i <= (*colour_)->colour_size; ++i){	//��㸴ԭ
		(*colour_) = (*colour_)->colour_top;
	}
	for (int i = 0; i <= image_colour_form_return1->colour_num; ++i){	//��㸴ԭ
		image_colour_form_return1->colour_num = colour_num;	//��ÿ���ڵ㶼��¼��ɫ����
		image_colour_form_return1 = image_colour_form_return1->top_colour_name;
	}
	if (image_colour_form_return1->colour_num == 0){
		sprintf(image_colour_form_return1->colour_name, "NULL");
	}
	return image_colour_form_return1;

}

//��ɫʶ����-���԰�
image_colour_form * Colour_Distinguish_Debug(IplImage *To_Identify_Colour_The_Image, colour** colour_,char colour_error_path[]){
	
	image_colour_form *image_colour_form_return;
	image_form_malloc(&image_colour_form_return);
	image_colour_form *image_colour_form_return1;
	image_form_malloc(&image_colour_form_return1);
	//��ʼʶ����ɫ
	CvScalar scalar;    //scalar  
	int num = 0;
	int form_size = 0;
	int colour_num = 0;
	FILE *fp = NULL;
	char str[256] = { 0 };
	fopen_s(&fp, colour_error_path, "w");
	if (fp == NULL){
		colour_error_type.colour_sign = COLOUR_BOOL_TRUE;
		strcpy_s(colour_error_type.error_type, COLOUR_ERROR_FILE_NULL);
		return (image_colour_form*)-1;
	}
	for (int i = 0; i <= To_Identify_Colour_The_Image->height - 1; ++i){
		for (int j = 0; j <= To_Identify_Colour_The_Image->width - 1; ++j){
			scalar = cvGet2D(To_Identify_Colour_The_Image, i, j);    //��ȡ���ص��RGB��ɫ����  
			for (int k = 0;; ++k){
				//�ȶ��㷨_1
				if ((scalar.val[2] >= (*colour_)->colour_min_rgb[0] && scalar.val[1] >= (*colour_)->colour_min_rgb[1] && scalar.val[0] >= (*colour_)->colour_min_rgb[2]) ||
					(scalar.val[2] > (*colour_)->colour_min_rgb[0] && scalar.val[1] > (*colour_)->colour_min_rgb[1] && scalar.val[0] > (*colour_)->colour_min_rgb[2])
					&& (scalar.val[2] <= (*colour_)->colour_mix_rgb[0] && scalar.val[1] <= (*colour_)->colour_mix_rgb[1] && scalar.val[0] <= (*colour_)->colour_mix_rgb[2]) ||
					(scalar.val[2] < (*colour_)->colour_mix_rgb[0] && scalar.val[1] < (*colour_)->colour_mix_rgb[1] && scalar.val[0] < (*colour_)->colour_mix_rgb[2])){
					image_colour_form *image_colour_form_return;
					image_form_malloc(&image_colour_form_return);
					strcpy_s(image_colour_form_return->colour_name, (*colour_)->colour_name); //����ɫ���ƶ�ȡ����
					colour_num  += 1;
					image_colour_form_return->top_colour_name = image_colour_form_return;
					image_colour_form_return->next_colour_name = image_colour_form_return1;
					image_colour_form_return1 = image_colour_form_return;
					//���˵��ظ���ɫ����һ����ɫ
					if (num < (*colour_)->colour_size){	//�Թ�����ѭ��������ɫ����������ʱ�����޷��жϳ��������ɫ��ֱ�ӷ����ѻ�ȡ������ɫ
						(*colour_) = (*colour_)->colour_next;
						num += 1;
					}
					else{
						for (int y = 0; y < num; ++y){//��ɫ�ֻ�
							(*colour_) = (*colour_)->colour_top;
						}
						num = 0;
						break;
						//return image_colour_form_return;
					}
				}
				else{	//�޷�ʶ�����ɫ,д�뵽�ļ�����
					sprintf(str, "error_colour:R = %d,G = %d,B = %d\n", scalar.val[2], scalar.val[1], scalar.val[0]);
					fwrite(str, strlen(str), 1, fp);

				}

			}
		}
	}
	for (int i = 0; i <= (*colour_)->colour_size; ++i){	//��㸴ԭ
		(*colour_) = (*colour_)->colour_top;
	}
	for (int i = 0; i <= image_colour_form_return1->colour_num; ++i){	//��㸴ԭ
		image_colour_form_return1->colour_num = colour_num;	//��ÿ���ڵ㶼��¼��ɫ����
		image_colour_form_return1 = image_colour_form_return1->top_colour_name;
	}
	if (image_colour_form_return1->colour_num == 0){
		sprintf(image_colour_form_return1->colour_name, "NULL");
	}
	return image_colour_form_return1;
}


//-
//colour�ļ��༭�� 1.0
COLOURIN(void) colour_vim(char colour_path[]){
	printf("***********************\n");
	printf("COLOUR������༭��-1.0\n");
	printf("***********************\n");
	printf("���ڼ���������...");
	int str_len = strlen(colour_path);
	int colour_size = strlen(COLOURFILE);	//Ϊ��ͳһ����������һ��������ļ���
	char a[] = COLOURFILE;	//�ȶ���ʱ����
	int a_size = colour_size;
	a_size = a_size - 1;	//�����±��0��ʼ������Ҫ-1
	for (int i = str_len - 1; i > str_len - colour_size; --i, --a_size){	//�ж��ļ���׺�Ƿ���ȷ��ע��������ϸ�����ļ���׺������������ȡ�ģ������ļ���׺����Ҫ
		if (a[a_size] != colour_path[i]){
			printf("�ⲻ��һ����ȷ���������ļ�.�����ѻ���...");
			getchar();
			return;
		}
	}
	printf("�ļ���ʽУ��ɹ�\n");
	//���ڴ�ӳ��ķ�ʽ�����ļ�
	//�÷������ʺϿ�ƽ̨������
	/*
	HANDLE hFile = CreateFileForMapping(pathName,GENERIC_READ,NULL,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == NULL)
	{
		DWORD it = GetLastError();//ERROR_ALREADY_EXISTS
		TRACE(_T("�����ļ�ӳ�����ʧ��,�������:%d/r/n"), GetLastError());
		return;
	}
	HANDLE hFileMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	if (hFile == NULL)
	{
	DWORD it = GetLastError();//ERROR_ALREADY_EXISTS
	TRACE(_T("�����ļ�ӳ�����ʧ��,�������:%d/r/n"), GetLastError());
	return;
	}
	*/
	//��ͳ��׼cģ���ڴ�ӳ�䷽��

	FILE* fp = fopen(colour_path, "r");	
	if (fp == NULL){
		printf("�޷���ָ���������ļ��������ѻ���...");
		getchar();
		return;
	}
	printf("������򿪳ɹ�\n");
	//�ļ��򿪳ɹ�ģ������ڴ�ӳ��(�ǹ����ڴ�)
	fseek(fp,0, SEEK_END); //��λ���ļ�ĩ 
	int nFileLen = ftell(fp); //�ļ�����
	//��ȡ��������
	char ad;
	char file_name[256] = { 0 };
	int min = 0, mix = 0;
	for (int i = strlen(colour_path);i>=0; --i){
			ad = colour_path[i];
			if (a == "."){//��ȡ�ļ������ֵ
			//�ָ��߷ָ�
				mix = i;

			}
			if(a == "\\"){//��ȡ�ļ�����Сֵ
				min = i;
				for (int j = 0; j <= mix; ++j,++min){
					file_name[j] = colour_path[min];
				}
				break;
			
			}

	}
	printf("������������Ϣ��ȡ�ɹ�...\n");
	/*
	//��ֹ�����ݵĿ�ӳ��
	if (nFileLen < 10){
		printf("�������ⲻ��ȷ�������Ѿ�����...");
		getchar();
		return;
	}
	*/
	char *colour_str = (char*)malloc(nFileLen);
	fread(colour_str, nFileLen, 1,fp);	//�ļ����ݵ��ڴ�
	//�����ļ��ں�I/Oռ��
	fclose(fp);
	printf("������ӳ�����\n");
	printf("���ڽ������������ݽṹ��..\n");
	//���������ݽṹ��
	colour* colour_;
	colour_malloc(&colour_);
	colour_range_control(&colour_, &colour_str,COLOUR_BOOL_FALSE);
	if (colour_error_type.colour_sign == COLOUR_BOOL_TRUE){
		printf("�޷�������ݽṹ������,�ڲ�����,�����ѻ���...");
		getchar();
		return;
	}
	//�ͷ���ʱӳ���ڴ�
	free(colour_str);
	colour_str = NULL;
	//���ݽṹ����ɼ��ɶ��������ɾ�Ĳ���
	//��ӡ��������Ϣ
	printf("���������ݽṹ�����\n");
	printf("*********************************\n");
	printf("������������Ϣ:\n");
	printf("�������ļ���:%s\n", file_name);
	printf("�������ֽڴ�С:%d\n", nFileLen);
	printf("������������������:%d\n", colour_->colour_size);
	printf("*********************************\n");
	ins:
	printf("������Ҫ�������ļ�������Щ������\n");
	printf("1.�г�����������������Ϣ\n");
	printf("2.�鿴ָ��������������Ϣ\n");
	printf("3.�޸��ƶ�������������Ϣ\n");
	printf("4.ɾ���ƶ�����������\n");
	printf("5.ɾ����������������\n");
	printf("6.�������\n");
	printf("7.������µ��������ļ�\n");
	int lef = 0;
	int num = 0;
	int i = 0;
	char path_[256] = {0};
	switch (lef){
	case 1:{	//�г�����������������Ϣ
			   for (i = 0; i <= colour_->colour_size; ++i){
				   printf("%d.��ɫ���ƣ�%s\nRGB��С����ֵ(MIN):R=%d,G=%d,B=%d\nRGB�������ֵ(MIX):R=%d,G=%d,B=%d\n", i + 1, colour_->colour_name, colour_->colour_min_rgb[0], colour_->colour_min_rgb[1], colour_->colour_min_rgb[2], colour_->colour_mix_rgb[0], colour_->colour_mix_rgb[1], colour_->colour_mix_rgb[2]);
				   colour_ = colour_->colour_next;
			   }
			   printf("�Ѿ�Ϊ���г���%d����ɫ����\n", colour_->colour_size);
			   for (int j = 0; j <= i; ++j){
				   colour_ = colour_->colour_top;
			   }
			   goto ins;
	}
	case 2:{//�鿴ָ��������������Ϣ
			   ins_1:
			   printf("������Ҫ�鿴���������:");
			   scanf("%d", &num);
			   if (num > colour_->colour_size || num < colour_->colour_size){
				   printf("��Ŵ�����ɫ������������С����Ч�±�.����������\n");
				   goto ins_1;
			   }
			   for (int i = 0; i <= colour_->colour_size; ++i){
				   if (i == num - 1){
					   printf("%d.��ɫ���ƣ�%s\nRGB��С����ֵ(MIN):R=%d,G=%d,B=%d\nRGB�������ֵ(MIX):R=%d,G=%d,B=%d\n", i + 1, colour_->colour_name, colour_->colour_min_rgb[0], colour_->colour_min_rgb[1], colour_->colour_min_rgb[2], colour_->colour_mix_rgb[0], colour_->colour_mix_rgb[1], colour_->colour_mix_rgb[2]);
					   for (int j = 0; j < i;++j){
						   colour_ = colour_->colour_top;
					   }
					   goto ins;
				   }
				   colour_ = colour_->colour_next;
				   
			   }
			   goto ins;

		}
	case 3:{	//�޸�ָ��������������Ϣ
			   ins_3:
			   printf("������ָ������������ţ�");
			   scanf("%d", &num);
			   if (num > colour_->colour_size || num < colour_->colour_size){
				   printf("��Ŵ�����ɫ������������С����Ч�±�.����������\n");
				   goto ins_3;
			   }
			   for (int i = 0; i <= colour_->colour_size; ++i){
				   if (i == num - 1){
					   printf("%d.��ɫ���ƣ�%s\nRGB��С����ֵ(MIN):R=%d,G=%d,B=%d\nRGB�������ֵ(MIX):R=%d,G=%d,B=%d\n", i + 1, colour_->colour_name, colour_->colour_min_rgb[0], colour_->colour_min_rgb[1], colour_->colour_min_rgb[2], colour_->colour_mix_rgb[0], colour_->colour_mix_rgb[1], colour_->colour_mix_rgb[2]);
					   printf("�µ���ɫ���ƣ�");
					   scanf("%s", colour_->colour_name);
					   getchar();	//�س�
					   printf("�µ�RGBֵ��Сֵ(MIN)�ԡ��ո񡱷ָ���");
					   scanf("%d %d %d", colour_->colour_min_rgb[0], colour_->colour_min_rgb[1], colour_->colour_min_rgb[2]);
					   getchar();
					   printf("�µ�RGBֵ���ֵ(MIX)�ԡ��ո񡱷ָ���");
					   scanf("%d %d %d", colour_->colour_mix_rgb[0], colour_->colour_mix_rgb[1], colour_->colour_mix_rgb[2]);
					   getchar();	//�س�
					   printf("�޸����\n");
					   for (int j = 0; j <= i; ++j){
						   colour_ = colour_->colour_top;
					   }
					   goto ins;
				   }
				   colour_ = colour_->colour_next;
			}

	}
	case 4:{	//ɾ��ָ��������������Ϣ
		   ins_4:
			   printf("������ָ������������ţ�");
			   scanf("%d", &num);
			   if (num > colour_->colour_size || num < colour_->colour_size){
				   printf("��Ŵ�����ɫ������������С����Ч�±�.����������\n");
				   goto ins_4;
			   }
			   for (int i = 0; i <= colour_->colour_size; ++i){
				   if (i == num - 1){
					   *colour_->colour_name = NULL;
					   for (int j = 0; j < 3; ++j){
						   colour_->colour_min_rgb[j] = 0;
						   colour_->colour_mix_rgb[j] = 0;
					   }
					   printf("ɾ�����\n");
					   for (int j = 0; j < i; ++j){
						   colour_ = colour_->colour_top;
					   }
					   goto ins;
				   }
				   colour_ = colour_->colour_next;
			   }
	}
	case 5:{	//ɾ����������������
			   printf("����ɾ��...\n");
			   colour_dele(&colour_);
			   printf("ɾ�����\n");
			   goto ins;
	
	}
	case 6:{	//�������
			   printf("����ӳ�䵽�ļ���...");
			   fp = fopen(colour_path, "w");
			   if (fp == NULL){
				   printf("�޷����浽ԭ�ļ���..\nԭ��ԭ�ļ��Ѿ�ʧЧ..");
				   goto ins;
			   }
			   colour_str = (char*)malloc(nFileLen);
			   memset(colour_str, 0, nFileLen);
			   for (int i = 0; i <= colour_->colour_size; ++i){
				   colour_ = colour_->colour_next;
				   sprintf(colour_str, "%s<colour:%s,MIN:R=%d&G=%d&B=%d&,MIX:R=%d&G=%d&B=%d&>", colour_str,colour_->colour_name, colour_->colour_min_rgb[0], colour_->colour_min_rgb[1], colour_->colour_min_rgb[2], colour_->colour_mix_rgb[0], colour_->colour_mix_rgb[1], colour_->colour_mix_rgb[2]);
			   }
			   for (int j = 0; j < colour_->colour_size; ++j){	//�ָ����
				   colour_ = colour_->colour_top;
			   }
			   fwrite(colour_str, strlen(colour_str), 1, fp);	//д���ļ�
			   printf("ӳ���ļ��ɹ�");
			   fclose(fp);
			   free(colour_str);
			   colour_str = NULL;
			   goto ins;
		}
	case 7:{	//������µ������ļ�
			   printf("������Ҫ������ļ�·����");
			   scanf("%s", path_);
			   //����ǰ���һ�´����Ƿ������������ļ�����ļ�
			   fp = fopen(path_, "w");
			   if (fp == NULL){
				   printf("�޷���浽���ļ�,�������Ŀ¼Ȩ������..\n");
				   goto ins;
			   }
			   fclose(fp);	//������
			   colour_str = (char*)malloc(nFileLen);
			   memset(colour_str, 0, nFileLen);
			   for (int i = 0; i <= colour_->colour_size; ++i){
				   colour_ = colour_->colour_next;
				   sprintf(colour_str, "%s<colour:%s,MIN:R=%d&G=%d&B=%d&,MIX:R=%d&G=%d&B=%d&>", colour_str, colour_->colour_name, colour_->colour_min_rgb[0], colour_->colour_min_rgb[1], colour_->colour_min_rgb[2], colour_->colour_mix_rgb[0], colour_->colour_mix_rgb[1], colour_->colour_mix_rgb[2]);
			   }
			   for (int j = 0; j < colour_->colour_size; ++j){	//�ָ����
				   colour_ = colour_->colour_top;
			   }
			   colour_features_seve(colour_str, path_);
			   printf("����浽�ļ���%s,���Զ�����ļ��Ƿ��к�׺�����û���Ѿ��Զ�Ϊ����Ϻ�׺����\n", path_);
			   goto ins;

	
	}

	}




}

//��Ƶʶ�������⹦��_ͼ�����
//ȥ��ͼ����-ģ��ƽ����ݱ�Ե
COLOURIN(void) image_Remove_sawtooth(IplImage **image/*ģ����ͼƬ*/, int Fuzzy_method = COLOUR_FUZZY_METHOD_GAUSSIAN/*ģ������*/,int pox1 = 3/*�����*/,int pox2 = 3/*�����*/){	//�����Խ��ƽ��Ч��Խǿ���ٶ�Խ����ģ���̶�ȡ���ص��3x3��ֵ
	switch (Fuzzy_method){
	case COLOUR_FUZZY_METHOD_GAUSSIAN:	//��˹ƽ��
		cvSmooth(*image, *image, CV_GAUSSIAN, pox1, pox2);
		break;
	case COLOUR_FUZZY_METHOD_SIMPLE:	//��ƽ��,��ʹ�ô�ƽ����ʽ��pox2�������Ч��pox1��������ֵ��Ҳ�����������ص��ƽ���̶�/ģ����ƽ��ֵԽ�ߣ�ƽ���̶�Խ��
		cvSmooth(*image, *image, CV_BLUR, pox1);
		break;
	case COLOUR_FUZZY_METHOD_MEDIAN:		//��ֵ���ˣ�ͼ������һ�ֱ�����ԵROI���򲻱�ƽ���Ĵ���ʽ�����Է�ֹ�������򲻱�ģ��������ֹ�������٣���ʹ�ô�ƽ����ʽ��pox2�������Ч��pox1������Ϊƽ���̶�
		cvSmooth(*image, *image, CV_MEDIAN, pox1);
		break;
	default:	//û���ҵ���Ӧ�ķ�������ӡ���
		colour_error_type.colour_sign = COLOUR_BOOL_TRUE;
		strcpy_s(colour_error_type.error_type, COLOUR_ERROR__NOT_NO_METHOD);
		return;
	break;
	}
}


int main()
{
	//�������Ᵽ��һ��
	/*
	char str[] = { "<colour:��ɫ,MIN:R=0&G=0&B=0&,MIX:R=255&G=0&B=0&><colour:��ɫ,MIN:R=0&G=0&B=0&,MIX:R=0&G=255&B=0&><colour:��ɫ,MIN:R=0&G=0&B=0&,MIX:R=0&G=0&B=255&>" };
	char ss[256] = "d:\\1.x";
	//colour_features_seve(str, ss);
	*/
	/*
	//����������Ա���ɫ
	//����һ����ɫѡ����
	colour* c = NULL;
	//�����ڴ�
	colour_malloc(&c);
	//������ɫ����
	char str[] = { "<colour:��ɫ,MIN:R=0&G=0&B=0&,MIX:R=255&G=0&B=0&><colour:��ɫ,MIN:R=0&G=0&B=0&,MIX:R=0&G=255&B=0&><colour:��ɫ,MIN:R=0&G=0&B=0&,MIX:R=0&G=0&B=255&>" };
	char * p_str = str;
	colour_range_control(&c, &p_str, COLOUR_BOOL_FALSE);
	if (colour_error_type.colour_sign == COLOUR_BOOL_TRUE){
	printf("colour_type_error:%s", colour_error_type.error_type);
	getchar();
	return -1;
	}
	*/
	/*
	//�Ա���ɫ
	image_colour_form *image_colour_form_return;
	image_form_malloc(&image_colour_form_return);
	IplImage* img = cvLoadImage("e:\\1.png");
	if (img == NULL){
	printf("�޷���ͼ���ļ�!");
	getchar();
	return -1;
	};
	char str[] = { "<colour:��ɫ,MIN:R=0&G=0&B=0&,MIX:R=255&G=0&B=0&><colour:��ɫ,MIN:R=0&G=0&B=0&,MIX:R=255&G=0&B=0&><colour:��ɫ,MIN:R=0&G=0&B=0&,MIX:R=255&G=0&B=0&>" };
	char * p_str = str;
	colour* c = NULL;
	//�����ڴ�
	colour_malloc(&c);
	colour_range_control(&c, &p_str, COLOUR_BOOL_FALSE);
	image_colour_form_return = Colour_Distinguish(img, &c);
	printf("%s", image_colour_form_return->colour_name);
	getchar();
	*/
	/*
	//�ֶ���ɫѵ��
	char str[256] = "d:\\1.colour";
	colour_manual_generate(str);
	if (colour_error_type.colour_sign == COLOUR_BOOL_TRUE){
	printf("colour_type_error:%s", colour_error_type.error_type);
	getchar();
	}
	printf("�ֶ���ѵ�����...");
	getchar();
	*/
	//��ɫ������༭

	getchar();
	return 0;
}










