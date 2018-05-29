#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <opencv.hpp> 
/*
	����������
*/
//���ú�Initialization
#define COLOUR_INITIALIZATION_DATA 0
//���ͼ����
#define COLOUR_IMAGE_RGB 3	//��ͨ��
#define COLOUR_IMAGE_RGBA 4	//��͸���ȵ����ͼ
//�������ļ�����׺
#define COLOUR_FILE_SUFFIX "colour"	//�������ļ���׺��
//�����Ᵽ�淽ʽ
#define COLOUR_FILE_SEVE_ADD 0	//�Ը��ӵķ�ʽ���浽�ļ���,�ļ��������
#define COLOUR_FILE_SEVE_NEW 1	//����Ϊ���ļ�������ļ������򱣴�ʧ��
#define COLOUR_FILE_SEVE_NEW_FORCE 11	//����Ϊ���ļ�������ļ��洢�򸲸�
//ͼ��ѹ����ʽ
#define COLOUR_COMPRESS_MODE_JPEG 0	//JPEG��ʽѹ��
//�����
int Colour_Errror = 0;				//�������ͣ�GetERRor�������ݴ��������ж����еĴ���״̬
char Colour_Error_Func[256] = {0};	//������
#define COLOUR_ERROR_POINTER_NULL -1							//ָ��Ϊ��
#define COLOUR_ERROR_MALLOC_NULL -2								//malloc�����ڴ�ʧ��
#define COLOUR_ERROR_CREATE		-3								//�޷�������ɫ������
#define COLOUR_ERROR_TRANSBOUNDARY	-4							//����ֵ������ɫ�������Ĵ�С��Χ
#define COLOUR_ERROR_FILE			-5							//�޷���ָ���ļ�
#define COLOUR_ERROR_FILE_READ_0	-6							//��ȡ����Ϊ��
#define COLOUR_ERROR_SUBSCRIPT_0	-7							//�±��0��ʼ
#define COLOUR_ERROR_IMAGE_NULL		-8							//ͼ��ָ��Ϊ��
#define COLOUR_ERROR_IMAGE_RGB		-9							//��ͼ����һ�����ͼ
#define COLOUR_ERROR_FILE_NO_EXISTENT -10						//��ɫ�������ļ�������
#define COLOUR_ERROR_FILE_EXISTENT -11							//��ɫ�������ļ��Ѵ���
#define COLOUR_ERROR_FILE_NO_MODE	-12							//û��������淽ʽ
#define COLOUR_ERROR_STR_NULL			-13						//���ݽ������ַ�Ϊ��
#define COLOUR_ERROR_FILE_INCORRECT		-14						//����һ����ȷ����ɫ�������ļ�
#define COLOUR_ERROR_FILE_WRITE	-15								//д������Ϊ��
#define COLOUR_ERROR_CROSS		-16								//������ˮ����ͼ���п���С
#define COLOUR_ERROR_IMAGE_XY_POS -17							//Ҫ������ͼ�����곬��ͼ�����Χ
#define COLOUR_NORMAL			0								//����ִ������
//���ͺ�
#define COLOURIN(TYPE) inline TYPE								//������������
#define COLOURIN_SIGNED(TYPE) inline signed TYPE				//�����ŵ�������������
//���޺�
#define COLOUR_MAX_MIN_RGB_ARR_SIZE 3				//MAX_��MIN,RGB�����С,����ָ����Ϊ���������ޱ����������
/*
	�꺯��
*/
#define	COLOUR_FUNCTION_CONTRAST(a,b)	(a==b)?0:1;	//��Ŀ����ʽ�ж��Ƿ����
/*
	��ɫ�����ṹ��
*/
typedef struct Colour_Features{//�ýṹ��ӵ�м�¼ͷ�ڵ㣬β�ڵ�,�ϸ�Ԫ�����¸�Ԫ�صĶ������ݽṹ��֧���±�����
	Colour_Features(int _Val){//���캯��,����û���Ҫ��ʾ��ʼ��ʱ������øú�����
		memset(Colour_Name, _Val, sizeof(Colour_Name));
		for (int i = 0; i < 3; ++i){
			Colour_Max_RGB[i] = _Val;
			Colour_Min_RGB[i] = _Val;
		}
		Colour_Label = _Val;
	}
	char Colour_Name[256];		//��ɫ��
	double Colour_Max_RGB[COLOUR_MAX_MIN_RGB_ARR_SIZE];	//�����ɫ��Χ
	double Colour_Min_RGB[COLOUR_MAX_MIN_RGB_ARR_SIZE];	//��С��ɫ��Χ
	int Colour_Label = 0;	//���ڼ�¼���,��¼��ǰ���������ĸ�
	Colour_Features *Colour_Next = NULL;	//��һ��Ԫ��

}Colour;
/*
	��ɫ���������к�
*/
int Colour_Serial = 0;
/*
	��ɫ�������׽ڵ㣬���ｫ��ӽṹ����ָ��������ڼ��ٽṹ���ڴ��Լ����뿪��
*/
Colour_Features *Colour_Top = NULL;	//���һ��ѹ��Ԫ�ؽڵ�
/*
��ɫ��
*/
typedef struct Colour_Form{
	Colour_Form(int _Val){//���캯��,����û���Ҫ��ʾ��ʼ��ʱ������øú�����
		memset(Colour_Name, _Val, sizeof(Colour_Name));
	}
	char Colour_Name[256];		//��ɫ��
	int Colour_Label = 0;		//��ǰ��Ծ�ڵ�
	Colour_Form *Colour_Next = NULL;	//��һ��Ԫ��
	int	X = 0;	//ͼ�������
	int Y = 0;	//ͼ��������

}Form;
/*
��ɫ�����к�
*/
int Colour_Form_Serial = 0;
/*
���ڼ�¼�߽�Ԫ��
*/
Colour_Form *Colour_Form_Top = NULL;
/*
��С�ṹ��
*/
typedef struct Colour_Form_X_Y{
	Colour_Form_X_Y(int X,int Y){//���캯��,����û���Ҫ��ʾ��ʼ��ʱ������øú�����
		X = X;
		Y = Y;
	}
	int	X = 0;	//ͼ�������
	int Y = 0;	//ͼ��������

}Form_X_Y;
//������ز�������
//��ȡ���һ��ִ�д���������Ϣ����������ʾ����
COLOURIN(void)  Get_Colour_Error_State_Print(){
	char Error[256] = { 0 };	//������Ϣ
	switch (Colour_Errror)
	{
	case COLOUR_ERROR_POINTER_NULL:
		sprintf(Error, "ִ�д�����:%s,�������ͣ�%s!", Colour_Error_Func, "���ݽ�������ָ��Ϊ��");
		break;
	case COLOUR_ERROR_MALLOC_NULL:
		sprintf(Error, "ִ�д�����:%s,�������ͣ�%s!", Colour_Error_Func, "malloc���������ڴ�ʧ�ܣ���������޿����ڴ�");
		break;
	case COLOUR_ERROR_CREATE:
		sprintf(Error, "ִ�д�����:%s,�������ͣ�%s!", Colour_Error_Func,"�޷�������ɫ������");
		break;
	case COLOUR_ERROR_TRANSBOUNDARY:
		sprintf(Error, "ִ�д�����:%s,�������ͣ�%s!", Colour_Error_Func, "����ֵ������ɫ��������С��Χ");
		break;
	case COLOUR_ERROR_FILE:
		sprintf(Error, "ִ�д�����:%s,�������ͣ�%s!", Colour_Error_Func, "�޷���ָ���ļ�");
		break;
	case COLOUR_ERROR_FILE_READ_0:
		sprintf(Error, "ִ�д�����:%s,�������ͣ�%s!", Colour_Error_Func, "�ļ��ж�ȡ��������Ϊ��");
		break;
	case COLOUR_ERROR_SUBSCRIPT_0:
		sprintf(Error, "ִ�д�����:%s,�������ͣ�%s!", Colour_Error_Func, "�Ƿ���ָ���±�ʱ��0��ʼ��������1��");
		break;
	case COLOUR_ERROR_IMAGE_NULL:
		sprintf(Error, "ִ�д�����:%s,�������ͣ�%s!", Colour_Error_Func, "���ݽ�����ͼ��ָ��Ϊ��");
		break;
	case COLOUR_ERROR_IMAGE_RGB:
		sprintf(Error, "ִ�д�����:%s,�������ͣ�%s!", Colour_Error_Func, "��ͼ����һ�����ͼ");
		break;
	case COLOUR_ERROR_FILE_NO_EXISTENT:
		sprintf(Error, "ִ�д�����:%s,�������ͣ�%s!", Colour_Error_Func, "ͼ���������ļ�������");
		break;
	case COLOUR_ERROR_FILE_EXISTENT:
		sprintf(Error, "ִ�д�����:%s,�������ͣ�%s!", Colour_Error_Func, "ͼ���������ļ��Ѵ��ڣ������ظ�����");
		break;
	case COLOUR_ERROR_FILE_NO_MODE:
		sprintf(Error, "ִ�д�����:%s,�������ͣ�%s!", Colour_Error_Func, "�����ļ�ʱ����ʹ�õı��淽ʽ������");
		break;
	case COLOUR_ERROR_STR_NULL:
		sprintf(Error, "ִ�д�����:%s,�������ͣ�%s!", Colour_Error_Func, "���ݽ�����ָ��Ϊ��");
		break;
	case COLOUR_ERROR_FILE_INCORRECT:
		sprintf(Error, "ִ�д�����:%s,�������ͣ�%s!", Colour_Error_Func, "����һ����ȷ����ɫ�������ļ�");
		break; 
	case COLOUR_ERROR_FILE_WRITE:
		sprintf(Error, "ִ�д�����:%s,�������ͣ�%s!", Colour_Error_Func, "д�������");
		break; 
	case COLOUR_ERROR_CROSS:
		sprintf(Error, "ִ�д�����:%s,�������ͣ�%s!", Colour_Error_Func, "������ˮ����ͼ���д�С");
		break;
	case COLOUR_ERROR_IMAGE_XY_POS:
		sprintf(Error, "ִ�д�����:%s,�������ͣ�%s!", Colour_Error_Func, "Ҫ�������±곬��ͼ��Χ");
		break;
	default:
		printf("����ִ�в�û�з����κδ���");
		break;
	}
	//��ӡ������Ϣ
	printf(Error);
	
}
//��ȡ���һ��ִ�д����������Ϣ�������ַ�������ʽ����
COLOURIN(char*) Get_Colour_Error_State(){
	char Error[256] = { 0 };	//������Ϣ
	switch (Colour_Errror)
	{
	case COLOUR_ERROR_POINTER_NULL:
		sprintf(Error, "ִ�д�����:%s,�������ͣ�%s!", Colour_Error_Func, "���ݽ�������ָ��Ϊ��");
		break;
	case COLOUR_ERROR_MALLOC_NULL:
		sprintf(Error, "ִ�д�����:%s,�������ͣ�%s!", Colour_Error_Func, "malloc���������ڴ�ʧ�ܣ���������޿����ڴ�");
		break;
	case COLOUR_ERROR_CREATE:
		sprintf(Error, "ִ�д�����:%s,�������ͣ�%s!", Colour_Error_Func, "�޷�������ɫ������");
		break;
	case COLOUR_ERROR_TRANSBOUNDARY:
		sprintf(Error, "ִ�д�����:%s,�������ͣ�%s!", Colour_Error_Func, "����ֵ������ɫ��������С��Χ");
		break;
	case COLOUR_ERROR_FILE:
		sprintf(Error, "ִ�д�����:%s,�������ͣ�%s!", Colour_Error_Func, "�޷���ָ���ļ�");
		break;
	case COLOUR_ERROR_FILE_READ_0:
		sprintf(Error, "ִ�д�����:%s,�������ͣ�%s!", Colour_Error_Func, "�ļ��ж�ȡ��������Ϊ��");
		break;
	case COLOUR_ERROR_SUBSCRIPT_0:
		sprintf(Error, "ִ�д�����:%s,�������ͣ�%s!", Colour_Error_Func, "�Ƿ���ָ���±�ʱ��0��ʼ��������1��");
		break;
	case COLOUR_ERROR_IMAGE_NULL:
		sprintf(Error, "ִ�д�����:%s,�������ͣ�%s!", Colour_Error_Func, "���ݽ�����ͼ��ָ��Ϊ��");
		break;
	case COLOUR_ERROR_IMAGE_RGB:
		sprintf(Error, "ִ�д�����:%s,�������ͣ�%s!", Colour_Error_Func, "��ͼ����һ�����ͼ");
		break;
	case COLOUR_ERROR_FILE_NO_EXISTENT:
		sprintf(Error, "ִ�д�����:%s,�������ͣ�%s!", Colour_Error_Func, "ͼ���������ļ�������");
		break;
	case COLOUR_ERROR_FILE_EXISTENT:
		sprintf(Error, "ִ�д�����:%s,�������ͣ�%s!", Colour_Error_Func, "ͼ���������ļ��Ѵ��ڣ������ظ�����");
		break;
	case COLOUR_ERROR_FILE_NO_MODE:
		sprintf(Error, "ִ�д�����:%s,�������ͣ�%s!", Colour_Error_Func, "�����ļ�ʱ����ʹ�õı��淽ʽ������");
		break;
	case COLOUR_ERROR_STR_NULL:
		sprintf(Error, "ִ�д�����:%s,�������ͣ�%s!", Colour_Error_Func, "���ݽ�����ָ��Ϊ��");
		break;
	case COLOUR_ERROR_FILE_INCORRECT:
		sprintf(Error, "ִ�д�����:%s,�������ͣ�%s!", Colour_Error_Func, "����һ����ȷ����ɫ�������ļ�");
		break;
	case COLOUR_ERROR_FILE_WRITE:
		sprintf(Error, "ִ�д�����:%s,�������ͣ�%s!", Colour_Error_Func, "д�������");
		break;
	case COLOUR_ERROR_CROSS:
		sprintf(Error, "ִ�д�����:%s,�������ͣ�%s!", Colour_Error_Func, "������ˮ����ͼ���д�С");
		break;
	case COLOUR_ERROR_IMAGE_XY_POS:
		sprintf(Error, "ִ�д�����:%s,�������ͣ�%s!", Colour_Error_Func, "Ҫ�������±곬��ͼ��Χ");
		break;
	default:
		printf("����ִ�в�û�з����κδ���");
		break;
	}
	return Error;
}
/*
��ɫ��������ز�������
*/
//����һ���µ���ɫ����������ʼ��
COLOURIN_SIGNED(int) Colour_Features_Create_Initialization(struct Colour_Features** Colour/*Ҫ��ʼ������ɫ���ݽṹ*/){	
	//��colour��ʼ��ΪNULL
	(*Colour) = COLOUR_INITIALIZATION_DATA;
	//Ϊ�����һ���ڴ�
	(*Colour) = (struct Colour_Features*)malloc(sizeof(struct Colour_Features));
	if ((*Colour) == NULL){	//�ж��ڴ��Ƿ����ɹ�
		strcpy(Colour_Error_Func, "Colour_Features_Create_Initialization");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_MALLOC_NULL;	//��ǰ�����û�п����ڴ����
		return COLOUR_ERROR_MALLOC_NULL;	//��������ִ��
		
	}
	//�ÿյ�һ��Ԫ�ص����нڵ㣬�����ɿ�Ԫ��
	//�ÿ�Colour_Name
	memset((*Colour)->Colour_Name, COLOUR_INITIALIZATION_DATA, sizeof((*Colour)->Colour_Name));
	//ѭ���ÿ�MAX��MIN
	for (int i = 0; i < 3; ++i){
		(*Colour)->Colour_Max_RGB[i] = COLOUR_INITIALIZATION_DATA;
		(*Colour)->Colour_Min_RGB[i] = COLOUR_INITIALIZATION_DATA;
	}
	//�ڵ�ָ���ÿ�
	(*Colour)->Colour_Label = COLOUR_INITIALIZATION_DATA;
	(*Colour)->Colour_Next = COLOUR_INITIALIZATION_DATA;
	//���涥���ڵ�
	Colour_Top = (*Colour);
	//����ִ����ȷ
	return COLOUR_NORMAL;
	/*
	��ע��������һ������ʹ������ѹ�뺯��ѹ����ֵʱ���ᴴ��һ���µı��Ҳ����˵ʹ�ô˺��������ı����ױ���Ϊ�߽���nextΪ������ָ���ױ߽�ģ���Colour_TOP����ÿ���ڱ�ѹ���±���ʱֵ����ָ���±���ĵ�ַ������ָ��β�߽�
	˼·��Ϊ��ȷ�����ᷢ�������ڴ淶Χ���ж�BUG
	*/

}
//��ȡ�ṹ���С
COLOURIN_SIGNED(int) Colour_Get_Struct_Size(struct Colour_Features** Colour/*Ҫ��ȡ��С�Ľṹ��*/){
	if ((*Colour) == NULL){	//�ж��Ƿ�Ϊһ���սṹ��
		strcpy(Colour_Error_Func, "Colour_Get_Struct_Size");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	//ѭ�������ṹ�岢����
	int i = 0;
	//���浱ǰ����λ��
	struct Colour_Features *Colour_Current = (*Colour);
	//�õ���Ԫ�ر���
	(*Colour) = Colour_Top;
	//��ʼ��ͷ����
	for (; (*Colour)->Colour_Next != NULL; ++i){
		(*Colour) = (*Colour)->Colour_Next;	//�������NULL�������нڵ�Ԫ��
	}
	//�ڵ㸴ԭ
	(*Colour) = Colour_Current;
	return i;
}
//��һ������ѹ����ɫ�������ݽṹ��
COLOURIN_SIGNED(int) Colour_Features_Pressing_Basic_Type(struct Colour_Features** Colour/*Ҫѹ�����ɫ���ݽṹ*/, char *Colour_Name/*��ɫ��*/, double *Colour_Max_RGB/*��ɫ���ֵ*/, double *Colour_Min_RGB/*��ɫ��Сֵ*/){	//��ָ����ʽ���ʣ������ڴ濪��
	//�жϴ��ݽ�����ָ���Ƿ�Ϊ��
	if ((*Colour) == NULL){//�ж�ָ���Ƿ�Ϊ�գ����Ϊ�մ�����ǰ�ṹ��ָ��Ϊ��ָ��
		if (COLOUR_NORMAL == Colour_Features_Create_Initialization(Colour)){//����һ���µ���ɫ����������ʼ��
			struct Colour_Features *colour;
			if (COLOUR_NORMAL != Colour_Features_Create_Initialization(&colour)){	//�жϱ����Ƿ񴴽��ɹ�
				strcpy(Colour_Error_Func, "Colour_Features_Pressing_Basic_Type");	//���г����ĺ�����
				Colour_Errror = COLOUR_ERROR_MALLOC_NULL;
				return COLOUR_ERROR_MALLOC_NULL;
			}
			//Ϊ��ɫ�������ı��ֵ
			for (int i = 0; i < COLOUR_MAX_MIN_RGB_ARR_SIZE; ++i){	//ѹ��ֵ
				(*Colour)->Colour_Max_RGB[i] = Colour_Max_RGB[i];	//MAX
				(*Colour)->Colour_Min_RGB[i] = Colour_Min_RGB[i];	//MIN
			}
			//ѹ����ɫ��
			strcpy(colour->Colour_Name, Colour_Name);
			Colour_Serial = Colour_Serial + 1;
			colour->Colour_Label = Colour_Serial;	//���к�
			//���ýڵ����ӵ���������
			colour->Colour_Next = (*Colour);
			(*Colour) = colour;
		}
		else{	//�޷�������ɫ������
			strcpy(Colour_Error_Func, "Colour_Features_Pressing_Basic_Type");	//���г����ĺ�����
			Colour_Errror = COLOUR_ERROR_CREATE;
			return COLOUR_ERROR_CREATE;
		}

	}
	else{//������Ԫ��
		//���ڴ��д���һ����ɫ������
		struct Colour_Features *colour;
		if (COLOUR_NORMAL != Colour_Features_Create_Initialization(&colour)){	//�жϱ����Ƿ񴴽��ɹ�
			strcpy(Colour_Error_Func, "Colour_Features_Pressing_Basic_Type");	//���г����ĺ�����
			Colour_Errror = COLOUR_ERROR_MALLOC_NULL;
			return COLOUR_ERROR_MALLOC_NULL;
		}
		//Ϊ��ɫ�������ı��ֵ
		for (int i = 0; i < COLOUR_MAX_MIN_RGB_ARR_SIZE; ++i){	//ѹ��ֵ
			colour->Colour_Max_RGB[i] = Colour_Max_RGB[i];	//MAX
			colour->Colour_Min_RGB[i] = Colour_Min_RGB[i];	//MIN
		}
		//ѹ����ɫ��
		strcpy(colour->Colour_Name, Colour_Name);
		Colour_Serial = Colour_Serial + 1;
		colour->Colour_Label = Colour_Serial;	//���к�
		//���ýڵ����ӵ���������
		colour->Colour_Next = (*Colour);
		(*Colour) = colour;
	}
	return COLOUR_NORMAL;	//����ִ������
	
}
//�ͷ���ɫ������
COLOURIN(int) Colour_Delete(struct Colour_Features** Colour/*Ҫ�ͷŵ���ɫ������*/){
	if ((*Colour) == NULL){	//�ж��Ƿ�Ϊһ���սṹ��
		strcpy(Colour_Error_Func, "Colour_Delete");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	//��ȡ��ɫ�������Ĵ�С
	int Colour_size = Colour_Get_Struct_Size(Colour);
	struct Colour_Features* Colour_New;	//��������һ��ָ������ָ�������ӵĽڵ�Ԫ�أ����洢�κ�ֵ��������������ڴ�
	//ѭ��ɾ��
	for (int i = 0; i <= Colour_size; ++i){
		//������һ���ڵ��ַ������ѭ������
		Colour_New = (*Colour)->Colour_Next;
		//�ͷ��ڴ�
		delete(*Colour);
		(*Colour) = NULL;
		//ָ����һ���ڵ�,ָ��洢��һ���ڵ����ʱָ�룬�������γ���ѭ������
		(*Colour) = Colour_New;
	
	}
	Colour_Top = NULL;	//�߽縴ԭ
	return COLOUR_NORMAL;	//����ִ������
}
//�ͷ�ָ����ɫ������
COLOURIN(int) Colour_Delete_Appoint(struct Colour_Features** Colour/*Ҫ�ͷŵ���ɫ������*/, int Colour_Subscript/*��ɫ�����������±�*/){
	if ((*Colour) == NULL){	//�ж��Ƿ�Ϊһ���սṹ��
		strcpy(Colour_Error_Func, "Colour_Delete_Appoint");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	//�ж��±��ǲ��Ǵ�0��ʼ����ģ���ֹ���ʱ߽��׽ڵ�
	if (Colour_Subscript == 0){
		strcpy(Colour_Error_Func, "Colour_Delete_Appoint");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_SUBSCRIPT_0;
		return COLOUR_ERROR_SUBSCRIPT_0;
	}
	//�ж��±��������ѡ���Ƿ񳬳��ṹ���С��Χ
	if (Colour_Subscript > Colour_Get_Struct_Size(Colour)){
		strcpy(Colour_Error_Func, "Colour_Delete_Appoint");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_TRANSBOUNDARY;	//Խ��
		return COLOUR_ERROR_TRANSBOUNDARY;
	}
	int Reduction = 0;	//�����жϱ���λ���ǲ��Ǿ���Ҫɾ���ı���λ��
	//���浱ǰ����λ��
	struct Colour_Features *Colour_Current = (*Colour);
	//�õ�β������߽�Ԫ��λ��
	(*Colour) = Colour_Top;
	struct Colour_Features *Colour_Record = NULL;	//���ڼ�¼��һ��Ԫ��
	struct Colour_Features *Colour_Record1 = NULL;	//���ڼ�¼��һ��Ԫ��
	for (int i = Colour_Get_Struct_Size(Colour); i > Colour_Subscript /*����-1�������������±��1��ʼ���� >*/; --i){
		//��ȡ��һ��Ԫ��ָ��
		if (i == Colour_Subscript+1){
			Colour_Record = (*Colour);	//����
		}
		(*Colour) = (*Colour)->Colour_Next;
	}
	//�жϵ�ǰ����λ���ǲ���Ҫɾ���ı���λ��
	if (Colour_Current->Colour_Label == (*Colour)->Colour_Label){
		Reduction = 1;	//��ǰ����λ�þ���Ҫɾ���ı���λ��
	}
	//�ж�ָ������ڵ����һ���ڵ��Ƿ�Ϊ�ױ߽�ڵ�
	Colour_Record1 = (*Colour);
	Colour_Record1 = Colour_Record1->Colour_Next;
	if (Colour_Record1->Colour_Next == NULL){	//�߽��׽ڵ�
		//�ͷ�ָ���ڵ�Ԫ��
		free((*Colour));
		(*Colour) = NULL;
		//����Ǳ߽��׽ڵ㼴��Colour_Recordָ��ýڵ㼴��
		Colour_Record->Colour_Next = Colour_Record1;
		Colour_Record1 = Colour_Record;
		//��Colour����ָ����ȷ�Ľڵ�
		if (Reduction == 0){	//�ڵ㻹ԭ
			(*Colour) = Colour_Current;
		}
		else{	//ָ�����ԭλ�ú�Ľڵ�Ԫ��
			(*Colour) = Colour_Record;
		}
	}
	else{//���Ǳ߽��׽ڵ�
		//�ͷ�ָ���ڵ�Ԫ��
		free((*Colour));
		(*Colour) = NULL;
		//�ڵ�Ԫ��ƴ��
		Colour_Record1->Colour_Next = Colour_Record;
		//��Colour����ָ����ȷ�Ľڵ�
		if (Reduction == 0){	//�ڵ㻹ԭ
			(*Colour) = Colour_Current;
		}
		else{	//ָ�����ԭλ�ú�Ľڵ�Ԫ��
			(*Colour) = Colour_Record;
		}
	}
	return COLOUR_NORMAL;

}
//��ȡ��ɫ�������е�ǰ��Ծ��������к�
COLOURIN(int) Colour_Get_Struct_Label(struct Colour_Features** Colour/*Ҫ��ȡ����ɫ��������Label*/){
	if ((*Colour) == NULL){	//�ж��Ƿ�Ϊһ���սṹ��
		strcpy(Colour_Error_Func, "Colour_Get_Struct_Label");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	return (*Colour)->Colour_Label;	//����Label
	
}
//������-��ȡ��ɫ��������ָ������Ԫ��_Colour_Max_RGB
COLOURIN(double) Colour_Get_Struct_MAX_RGB(struct Colour_Features** Colour/*Ҫ��ȡ����ɫ��������Colour*/, int Colour_Subscript/*��ɫ�����������±�*/, int MAX_Subscript/*������ѡ���±�*/){
	if ((*Colour) == NULL){	//�ж��Ƿ�Ϊһ���սṹ��
		strcpy(Colour_Error_Func, "Colour_Get_Struct_MAX_RGB");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	//�ж��±��ǲ��Ǵ�0��ʼ����ģ���ֹ���ʱ߽��׽ڵ�
	if (Colour_Subscript == 0 || MAX_Subscript == 0){
		strcpy(Colour_Error_Func, "Colour_Get_Struct_MAX_RGB");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_SUBSCRIPT_0;
		return COLOUR_ERROR_SUBSCRIPT_0;
	}
	//�ж��±��������ѡ���Ƿ񳬳��ṹ���С��Χ
	if (Colour_Subscript > Colour_Get_Struct_Size(Colour)){
		strcpy(Colour_Error_Func, "Colour_Get_Struct_MAX_RGB");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_TRANSBOUNDARY;	//Խ��
		return COLOUR_ERROR_TRANSBOUNDARY;
	}
	//�жϷ�Χ�Ƿ񳬳������С
	if (MAX_Subscript > COLOUR_MAX_MIN_RGB_ARR_SIZE){
		strcpy(Colour_Error_Func, "Colour_Get_Struct_MAX_RGB");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_TRANSBOUNDARY;	//Խ��
		return COLOUR_ERROR_TRANSBOUNDARY;
	}
	//��ȡָ����ɫֵ
	//���浱ǰ����λ��
	struct Colour_Features *Colour_Current = (*Colour);
	//�õ�β���߽�Ԫ��λ��
	(*Colour) = Colour_Top;
	for (int i = Colour_Get_Struct_Size(Colour); i > Colour_Subscript /*����-1�������������±��1��ʼ����*/; --i){
		(*Colour) = (*Colour)->Colour_Next;
	}
	//�ڵ㸴ԭ
	//(*Colour) = Colour_Current;	//���ﲻ�ܽڵ㸴ԭ����Ϊ��ԭ�Ļ��޷���ȡ����ǰֵ
	//���ػ�ȡ����max
	return (*Colour)->Colour_Max_RGB[MAX_Subscript - 1];	//�����±��Ǵ�0��ʼ����ģ�������������Ҫ-1
}
//������-��ȡ��ɫ��������ָ������Ԫ��_Colour_Min_RGB
COLOURIN(double) Colour_Get_Struct_MIN_RGB(struct Colour_Features** Colour/*Ҫ��ȡ����ɫ��������Colour*/, int Colour_Subscript/*��ɫ�����������±�*/, int MIN_Subscript/*������ѡ���±�*/){
	if ((*Colour) == NULL){	//�ж��Ƿ�Ϊһ���սṹ��
		strcpy(Colour_Error_Func, "Colour_Get_Struct_MIN_RGB");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	//�ж��±��ǲ��Ǵ�0��ʼ����ģ���ֹ���ʱ߽��׽ڵ�
	if (Colour_Subscript == 0 || MIN_Subscript == 0){
		strcpy(Colour_Error_Func, "Colour_Get_Struct_MIN_RGB");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_SUBSCRIPT_0;
		return COLOUR_ERROR_SUBSCRIPT_0;
	}
	//�ж��±��������ѡ���Ƿ񳬳��ṹ���С��Χ
	if (Colour_Subscript > Colour_Get_Struct_Size(Colour)){
		strcpy(Colour_Error_Func, "Colour_Get_Struct_MIN_RGB");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_TRANSBOUNDARY;	//Խ��
		return COLOUR_ERROR_TRANSBOUNDARY;
	}
	//�жϷ�Χ�Ƿ񳬳������С
	if (MIN_Subscript > COLOUR_MAX_MIN_RGB_ARR_SIZE){
		strcpy(Colour_Error_Func, "Colour_Get_Struct_MIN_RGB");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_TRANSBOUNDARY;	//Խ��
		return COLOUR_ERROR_TRANSBOUNDARY;
	}
	//��ȡָ����ɫֵ
	//���浱ǰ����λ��
	struct Colour_Features *Colour_Current = (*Colour);
	//�õ�β������߽�Ԫ��λ��
	(*Colour) = Colour_Top;

	for (int i = Colour_Get_Struct_Size(Colour); i > Colour_Subscript /*����-1�������������±��1��ʼ����*/; --i){
		(*Colour) = (*Colour)->Colour_Next;
	}
	//�ڵ㸴ԭ
	//(*Colour) = Colour_Current;	//���ﲻ�ܽڵ㸴ԭ����Ϊ��ԭ�Ļ��޷���ȡ����ǰֵ
	//���ػ�ȡ����min
	return (*Colour)->Colour_Min_RGB[MIN_Subscript - 1];	//�����±��Ǵ�0��ʼ����ģ�������������Ҫ-1
}
//������-��ȡ��ɫ��������ָ������Ԫ��_Colour_Name
COLOURIN(char*) Colour_Get_Struct_Colour_Name(struct Colour_Features** Colour/*Ҫ��ȡ����ɫ��������Colour*/, int Colour_Subscript/*��ɫ�����������±�*/){
	if ((*Colour) == NULL){	//�ж��Ƿ�Ϊһ���սṹ��
		strcpy(Colour_Error_Func, "Colour_Get_Struct_Colour_Name");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return (char*)/*��������*/COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	//�ж��±��ǲ��Ǵ�0��ʼ����ģ���ֹ���ʱ߽��׽ڵ�
	if (Colour_Subscript == 0){
		strcpy(Colour_Error_Func, "Colour_Get_Struct_Colour_Name");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_SUBSCRIPT_0;
		return (char*)/*��������*/COLOUR_ERROR_SUBSCRIPT_0;
	}
	//�ж��±��������ѡ���Ƿ񳬳��ṹ���С��Χ
	if (Colour_Subscript > Colour_Get_Struct_Size(Colour)){
		strcpy(Colour_Error_Func, "Colour_Get_Struct_Colour_Name");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_TRANSBOUNDARY;	//Խ��
		return (char*)/*��������*/COLOUR_ERROR_TRANSBOUNDARY;
	}
	//��ȡָ����ɫֵ
	//���浱ǰ����λ��
	struct Colour_Features *Colour_Current = (*Colour);
	//�õ�β���߽����Ԫ��λ��
	(*Colour) = Colour_Top;
	for (int i = Colour_Get_Struct_Size(Colour); i > Colour_Subscript/*����-1�������������±��1��ʼ����*/; --i){
		(*Colour) = (*Colour)->Colour_Next;
	}
	//�ڵ㸴ԭ
	//(*Colour) = Colour_Current;	//���ﲻ�ܽڵ㸴ԭ����Ϊ��ԭ�Ļ��޷���ȡ����ǰֵ
	//���ػ�ȡ����name
	return (*Colour)->Colour_Name;	//�����±��Ǵ�0��ʼ����ģ�������������Ҫ-1
}
//������-��ȡ��ɫ��������ָ������Ԫ��
COLOURIN(Colour_Features*) Colour_Get_Struct_Data(struct Colour_Features** Colour/*Ҫ��ȡ����ɫ��������Colour*/, int Colour_Subscript/*��ɫ�����������±�*/){
	if ((*Colour) == NULL){	//�ж��Ƿ�Ϊһ���սṹ��
		strcpy(Colour_Error_Func, "Colour_Get_Struct_Data");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return (Colour_Features*)/*��������*/COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	//�ж��±��ǲ��Ǵ�0��ʼ����ģ���ֹ���ʱ߽��׽ڵ�
	if (Colour_Subscript == 0){
		strcpy(Colour_Error_Func, "Colour_Get_Struct_Data");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_SUBSCRIPT_0;
		return (Colour_Features*)/*��������*/COLOUR_ERROR_SUBSCRIPT_0;
	}
	//�ж��±��������ѡ���Ƿ񳬳��ṹ���С��Χ
	if (Colour_Subscript > Colour_Get_Struct_Size(Colour)){
		strcpy(Colour_Error_Func, "Colour_Get_Struct_Data");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_TRANSBOUNDARY;	//Խ��
		return (Colour_Features*)/*��������*/COLOUR_ERROR_TRANSBOUNDARY;
	}
	//��ȡָ����ɫֵ
	//���浱ǰ����λ��
	struct Colour_Features *Colour_Current = (*Colour);
	//�õ�β���߽����
	(*Colour) = Colour_Top;
	//�õ�ָ������
	for (int i = Colour_Get_Struct_Size(Colour); i > Colour_Subscript/*����-1�������������±��1��ʼ����*/; --i){
		(*Colour) = (*Colour)->Colour_Next;
	}
	//����COPY
	//��colour��ʼ��ΪNULL
	struct Colour_Features *colour = COLOUR_INITIALIZATION_DATA;
	//Ϊ�����һ���ڴ�
	colour = (struct Colour_Features*)malloc(sizeof(struct Colour_Features));
	strcpy(colour->Colour_Name, (*Colour)->Colour_Name);
	//Ϊ��ɫ�������ı��ֵ
	for (int i = 0; i < COLOUR_MAX_MIN_RGB_ARR_SIZE; ++i){	//ѹ��ֵ
		colour->Colour_Max_RGB[i] = (*Colour)->Colour_Max_RGB[i];	//MAX
		colour->Colour_Min_RGB[i] = (*Colour)->Colour_Min_RGB[i];	//MIN
	}
	//�ڵ㸴ԭ
	(*Colour) = Colour_Current;
	//����date
	return colour;
}
//��ɫ������COPY����_ǳ��������Copy����ֻ��Copyһ����Ԫ�صĵ�ַ
COLOURIN_SIGNED(int) Colour_Copy(struct Colour_Features** Colour/*ҪCopy����ɫ������*/,struct Colour_Features** Colour1/*ҪCopy������ɫ������*/){
	if ((*Colour) == NULL || Colour1 == NULL){	//�ж��Ƿ�Ϊһ���սṹ��
		strcpy(Colour_Error_Func, "Colour_Copy");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	(*Colour1) = (*Colour);	//Copy
	return COLOUR_NORMAL;	//����ִ����ȷ
}
//��ȡ�������С
COLOURIN_SIGNED(int) Colour_Get_State_Size(char *Colour_State_Lib/*Ҫ��ȡ����ɫ������*/){
	
	if (Colour_State_Lib == NULL){	//�ж��Ƿ�Ϊһ����ָ��
		strcpy(Colour_Error_Func, "Colour_Get_State_Size");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	int i = 0;	//ѭ������
	int size = 0;	//�������С
	char a = 0;	//��ʱֵ
	for (; Colour_State_Lib[i] != '#'; ++i){
		if (Colour_State_Lib[i] == '}'/*�߽����*/){
			++size;
		}

	}
	return size;
}
//��ɫ���������л�
COLOURIN_SIGNED(int) Colour_Serialize(struct Colour_Features** Colour/*���л��Ľṹ��*/, char *Colour_State_Lib/*Ҫ���л�����ɫ������*/){
	if ((*Colour) == NULL){	//�ж��Ƿ�Ϊһ���սṹ��
		strcpy(Colour_Error_Func, "Colour_Serialize");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	if (Colour_State_Lib == NULL){	//�ж��Ƿ�Ϊһ����ָ��
		strcpy(Colour_Error_Func, "Colour_Serialize");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	//��ʼ����ѭ��
	(*Colour) = Colour_Top;	//��ȡ�׽ڵ�
	char colour_name[256] = { 0 };	//�ڵ���ɫ����
	double max_rgb[3] = { 0 };	//�ڵ���ɫ���޴�С
	double min_rgb[3] = { 0 };	//�ڵ���ɫ���޴�С
	char max_str[COLOUR_MAX_MIN_RGB_ARR_SIZE][255] = { 0 };	//�ַ�������ӳ��
	char min_str[COLOUR_MAX_MIN_RGB_ARR_SIZE][255] = { 0 };	//�ַ�������ӳ��
	int Arr_state = 0;	//����״̬
	int j = 0;	//ѭ������
	int y = 0;	//���ұ���������
	int hei_max = 0;	//max��min�������±�
	int hei_min = 0;
	for (int i = 0; i < Colour_Get_State_Size(Colour_State_Lib); ++i){
		for (;; ++j){
			if (Colour_State_Lib[j] == ':'){//�ҵ���ʼ��ʶ��
				switch (Arr_state){
				case 0:{	//��ȡname
						   y = j+1;	//��ȡ��ǰѭ��ֵ��+1�����ָ���
						   for (int h = 0;h<255/*����һ��ֵ,��ֹ��Ϊ�Ҳ��������������ѭ��*/; ++h,y++){
							   if (Colour_State_Lib[y] == '&'){	//������
								   Arr_state = Arr_state + 1;
								   break;
							   }
							   else{	
								   colour_name[h] = Colour_State_Lib[y];	//��ȡ��ɫ��
							   }
						   }
				}
					break;
				case 1:{	//��ȡmax
						   y = j + 1;	//��ȡ��ǰѭ��ֵ��+1�����ָ���
						   int max_inp = 0;	//ά���±�
						   for (int k = 0;; ++k,++y){
							   if (Colour_State_Lib[y] == '&'){
								   Arr_state = Arr_state + 1;
								   break;	//����
							   }
							   if (Colour_State_Lib[y] == ','){	//�ָ���
								   hei_max += 1;	//�±����
								   max_inp = 0;		//��xά�±����ʱ���뽫�±���Ϊ0
								   y = y + 1;		//�����ָ���
								   
							   }
							   max_str[hei_max][max_inp] = Colour_State_Lib[y];	//��ȡ�ؼ�����
							   max_inp = max_inp + 1;
						   }
				}	

					break;
				case 2:{	//��ȡmin
						   y = j + 1;	//��ȡ��ǰѭ��ֵ��+1�����ָ���
						   int max_inp = 0; //ά���±�
						   for (int k = 0;; ++k,++y){
							   if (Colour_State_Lib[y] == '&'){
								   Arr_state = Arr_state + 1;
								   break;	//����
							   }
							   if (Colour_State_Lib[y] == ','){	//�ָ���
								   hei_min += 1;	//�±����
								   max_inp = 0;		//��xά�±����ʱ���뽫�±���Ϊ0
								  //  y = y + 1;		//�����ָ�����min����������ԭ��min�ָ�ֵ��max�Ѿ�������
							   }
							   else{
								   min_str[hei_min][max_inp] = Colour_State_Lib[y];	//��ȡ�ؼ�����
								   max_inp = max_inp + 1;
							   }

						   }  
				}
					break;
				default:
					break;
				}
			
			}
			if (Colour_State_Lib[j] == '}'){	//������
				//����ȡ���Ĺؼ��ַ�����ת��������
				for (int i = 0; i < COLOUR_MAX_MIN_RGB_ARR_SIZE; ++i){
					max_rgb[i] = atof(max_str[i]);	//max
					min_rgb[i] = atof(min_str[i]);	//min
				}
				Colour_Features_Pressing_Basic_Type(Colour, colour_name, max_rgb, min_rgb);	//ѹ�����
				for (int i = 0; i < COLOUR_MAX_MIN_RGB_ARR_SIZE; ++i){//����ַ����飬��ֹ�ַ������ص�
					memset(max_str[i], 0, sizeof(max_str[i]));	//max
					memset(min_str[i], 0, sizeof(min_str[i]));	//min
				}
				memset(colour_name, 0, sizeof(colour_name));	//��ɫ�������ÿգ���ֹ�ַ��ص�
				Arr_state = 0;	//״̬��ָ�
				hei_max = 0;	//�����±���0
				hei_min = 0;	//�����±���0
				j = j + 1;
				break;
			}
		}
	}
	(*Colour) = Colour_Top;	//���ص��׽ڵ�
	return COLOUR_NORMAL;
}
/*
��ɫ����ز�������
*/
//����һ����ɫ��
COLOURIN_SIGNED(int) Colour_Form_Features_Create_Initialization(struct Colour_Form** _Colour_Form/*Ҫ��ʼ������ɫ���ݽṹ*/){
	//��colour��ʼ��ΪNULL
	(*_Colour_Form) = COLOUR_INITIALIZATION_DATA;
	//Ϊ�����һ���ڴ�
	(*_Colour_Form) = (struct Colour_Form*)malloc(sizeof(struct Colour_Form));
	if ((*_Colour_Form) == NULL){	//�ж��ڴ��Ƿ����ɹ�
		strcpy(Colour_Error_Func, "Colour_Form_Features_Create_Initialization");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_MALLOC_NULL;	//��ǰ�����û�п����ڴ����
		return COLOUR_ERROR_MALLOC_NULL;	//��������ִ��

	}
	//�ÿյ�һ��Ԫ�ص����нڵ㣬�����ɿ�Ԫ��
	//�ÿ�Colour_Name
	memset((*_Colour_Form)->Colour_Name, COLOUR_INITIALIZATION_DATA, sizeof((*_Colour_Form)->Colour_Name));
	(*_Colour_Form)->Colour_Next = COLOUR_INITIALIZATION_DATA;
	//���涥���ڵ�
	Colour_Form_Top = (*_Colour_Form);
	//����ִ����ȷ
	return COLOUR_NORMAL;
	/*
	��ע��������һ������ʹ������ѹ�뺯��ѹ����ֵʱ���ᴴ��һ���µı��Ҳ����˵ʹ�ô˺��������ı����ױ���Ϊ�߽���nextΪ������ָ���ױ߽�ģ���Colour_TOP����ÿ���ڱ�ѹ���±���ʱֵ����ָ���±���ĵ�ַ������ָ��β�߽�
	˼·��Ϊ��ȷ�����ᷢ�������ڴ淶Χ���ж�BUG
	*/

}
//��ȡ��ɫ����С
COLOURIN_SIGNED(int) Colour_Form_Get_Size(Colour_Form** Colour_Form){
	if ((*Colour_Form) == NULL){	//�ж��Ƿ�Ϊһ���սṹ��
		strcpy(Colour_Error_Func, "Colour_Form_Get_Size");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	//ѭ�������ṹ�岢����
	int i = 0;
	//���浱ǰ����λ��
	struct Colour_Form *Colour_Current = (*Colour_Form);
	//�õ���Ԫ�ر���
	(*Colour_Form) = Colour_Form_Top;
	//��ʼ��ͷ����
	for (; (*Colour_Form)->Colour_Next != NULL; ++i){
		(*Colour_Form) = (*Colour_Form)->Colour_Next;	//�������NULL�������нڵ�Ԫ��
	}
	//�ڵ㸴ԭ
	(*Colour_Form) = Colour_Current;
	return i;
}
//��ȡ��ɫ������ɫ��
COLOURIN(char*) Colour_Form_Get_Name(Colour_Form** Colour_Form, int Colour_Subscript/*��ɫ��,�����±�*/){
	if ((*Colour_Form) == NULL){	//�ж��Ƿ�Ϊһ���սṹ��
		strcpy(Colour_Error_Func, "Colour_Form_Get_Name");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return (char*)/*��������*/COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	//�ж��±��ǲ��Ǵ�0��ʼ����ģ���ֹ���ʱ߽��׽ڵ�
	if (Colour_Subscript == 0){
		strcpy(Colour_Error_Func, "Colour_Form_Get_Name");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_SUBSCRIPT_0;
		return (char*)/*��������*/COLOUR_ERROR_SUBSCRIPT_0;
	}
	//�ж�С����û�г���ָ����С
	if (Colour_Subscript > Colour_Form_Get_Size(Colour_Form)){
		strcpy(Colour_Error_Func, "Colour_Form_Get_Name");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_TRANSBOUNDARY;
		return (char*)/*��������*/COLOUR_ERROR_TRANSBOUNDARY;
	}
	//���浱ǰ����λ��
	struct Colour_Form *Colour_Current = (*Colour_Form);
	//�õ���Ԫ�ر���
	(*Colour_Form) = Colour_Form_Top;
	//��ʼ��ͷ����
	for (int i = Colour_Form_Get_Size(Colour_Form); i > Colour_Subscript; --i){
		(*Colour_Form) = (*Colour_Form)->Colour_Next;	//�������NULL�������нڵ�Ԫ��
	}
	//�ڵ㸴ԭ
	//(*Colour_Form) = Colour_Current;//���ﲻ�ܽڵ㸴ԭ����Ϊ��ԭ�Ļ��޷���ȡ����ǰֵ
	return (*Colour_Form)->Colour_Name;	//����ָ���±�name
}
//��ȡ��ɫ����X-Yֵ
COLOURIN(Colour_Form_X_Y*) Colour_Form_Get_X_Y(struct Colour_Form** _Colour_Form/*��ɫ��*/, int Colour_Subscript/*��ɫ��,�����±�*/){
	if ((*_Colour_Form) == NULL){	//�ж��Ƿ�Ϊһ���սṹ��
		strcpy(Colour_Error_Func, "Colour_Form_Get_Name");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return (Colour_Form_X_Y*)/*��������*/COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	//�ж��±��ǲ��Ǵ�0��ʼ����ģ���ֹ���ʱ߽��׽ڵ�
	if (Colour_Subscript == 0){
		strcpy(Colour_Error_Func, "Colour_Form_Get_Name");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_SUBSCRIPT_0;
		return (Colour_Form_X_Y*)/*��������*/COLOUR_ERROR_SUBSCRIPT_0;
	}
	//�ж�С����û�г���ָ����С
	if (Colour_Subscript > Colour_Form_Get_Size(_Colour_Form)){
		strcpy(Colour_Error_Func, "Colour_Form_Get_Name");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_TRANSBOUNDARY;
		return (Colour_Form_X_Y*)/*��������*/COLOUR_ERROR_TRANSBOUNDARY;
	}
	//���浱ǰ����λ��
	struct Colour_Form *Colour_Current = (*_Colour_Form);
	//�õ���Ԫ�ر���
	(*_Colour_Form) = Colour_Form_Top;
	//��ʼ��ͷ����
	for (int i = Colour_Form_Get_Size(_Colour_Form); i > Colour_Subscript; --i){
		(*_Colour_Form) = (*_Colour_Form)->Colour_Next;	//�������NULL�������нڵ�Ԫ��
	}
	//�ڵ㸴ԭ
	//(*Colour_Form) = Colour_Current;//���ﲻ�ܽڵ㸴ԭ����Ϊ��ԭ�Ļ��޷���ȡ����ǰֵ
	struct Colour_Form_X_Y *_X_Y = (Colour_Form_X_Y*)malloc(sizeof(Colour_Form_X_Y));  //{ (*_Colour_Form)->X, (*_Colour_Form)->Y };//ջ�ķ�ʽ�޷���ȡ
	//X-Y��ֵ
	_X_Y->X = (*_Colour_Form)->X;
	_X_Y->Y = (*_Colour_Form)->Y;
	return _X_Y;	//����ִ������
}
//��ɫ���ͷ�
COLOURIN(int) Colour_Form_Delete(struct Colour_Form** Colour_Form/*Ҫ�ͷŵ���ɫ������*/){
	if ((*Colour_Form) == NULL){	//�ж��Ƿ�Ϊһ���սṹ��
		strcpy(Colour_Error_Func, "Colour_Form_Delete");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	//��ȡ��ɫ�������Ĵ�С
	int Colour_size = Colour_Form_Get_Size(Colour_Form);
	struct Colour_Form* Colour_New;	//��������һ��ָ������ָ�������ӵĽڵ�Ԫ�أ����洢�κ�ֵ��������������ڴ�
	//ѭ��ɾ��
	for (int i = 0; i <= Colour_size; ++i){
		//������һ���ڵ��ַ������ѭ������
		Colour_New = (*Colour_Form)->Colour_Next;
		//�ͷ��ڴ�
		delete(*Colour_Form);
		(*Colour_Form) = NULL;
		//ָ����һ���ڵ�,ָ��洢��һ���ڵ����ʱָ�룬�������γ���ѭ������
		(*Colour_Form) = Colour_New;

	}
	Colour_Form_Top = NULL;	//�߽縴ԭ
	return COLOUR_NORMAL;	//����ִ������
}
//ѹ������
COLOURIN_SIGNED(int) Colour_Form_Push(struct Colour_Form** _Colour_Form/*Ҫѹ�����ɫ��*/, char *Colour_Form_Name/*��ɫ����*/,int X/*��*/,int Y/*������*/){
	if ((*_Colour_Form) == NULL){	//�ж��Ƿ�Ϊһ���սṹ��
		strcpy(Colour_Error_Func, "Colour_Form_Push");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	if (Colour_Form_Name == NULL){	//�ж��Ƿ�Ϊһ���սṹ��
		strcpy(Colour_Error_Func, "Colour_Form_Push");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	//����һ���ڴ�
	Colour_Form* Colour_Form_New = NULL;
	Colour_Form_Features_Create_Initialization(&Colour_Form_New);
	//���
	memset(Colour_Form_New->Colour_Name, 0, sizeof(Colour_Form_New->Colour_Name));
	//��ֵ
	strcpy(Colour_Form_New->Colour_Name, Colour_Form_Name);
	//���кŸ�ֵ
	Colour_Form_Serial = Colour_Form_Serial + 1;
	Colour_Form_New->Colour_Label = Colour_Form_Serial;
	//x��y���긳ֵ
	Colour_Form_New->X = X;
	Colour_Form_New->Y = Y;
	/*	//���׽ڵ���ڿգ��Ա��ȡ�߽�
	(*_Colour_Form)->X = X;
	(*_Colour_Form)->Y = Y;
	*/
	//��βƴ��
	Colour_Form_New->Colour_Next = (*_Colour_Form);
	(*_Colour_Form) = Colour_Form_New;
	return COLOUR_NORMAL;	//����ִ������
}
//����ͼ����
//���ű�����������0.Ϊ��λ���Ϊ0.9 ��0��ʼ����
COLOURIN(IplImage*) Colour_Image_Zoom(IplImage* Image/*Ҫ���ŵ�ͼ��*/, double  Zoom_Multiple = 0.2/*Ҫ���ŵı���*/){
	if (Image == NULL){	//�ж��Ƿ�Ϊһ���սṹ��
		strcpy(Colour_Error_Func, "Colour_Distinguish_Simple");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return (IplImage*)/*��������*/COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	//����ͼ��
	CvSize Image_Size = cvSize(Image->width*Zoom_Multiple, Image->height*Zoom_Multiple);	//�������ű���
	IplImage *Image_Zoom = cvCreateImage(Image_Size, Image->depth, Image->nChannels);	//����һ������ָ�����ź��ͼ���ڴ�
	cvResize(Image, Image_Zoom, CV_INTER_NN);// ����ͼ��
	//             CV_INTER_NN - ����ڲ�ֵ,  
	//             CV_INTER_LINEAR - ˫���Բ�ֵ (ȱʡʹ��)  
	//             CV_INTER_AREA - ʹ�����ع�ϵ�ز�������ͼ����Сʱ�򣬸÷������Ա��Ⲩ�Ƴ��֡�  
	/*��ͼ��Ŵ�ʱ�������� CV_INTER_NN ����..*/
	//             CV_INTER_CUBIC - ������ֵ.  
	return Image_Zoom;	//�������ź��ͼ��
}
//��ɫ������ʶ����_��Ȼ��RGB�ȶ�_����Աȷ�,�ŵ㣺�����ٶ��еȣ���׼����ߣ�ȱ�㣺�����Ƚϱ�׾
COLOURIN(Colour_Form*) Colour_Distinguish_Simple(struct Colour_Features** Colour/*ʶ�����ɫ������*/, IplImage* Image/*Ҫʶ����ɫ��ͼ��*/){
	if ((*Colour) == NULL){	//�ж��Ƿ�Ϊһ���սṹ��
		strcpy(Colour_Error_Func, "Colour_Distinguish_Simple");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return (Colour_Form*)/*��������*/COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	if (Image == NULL){	//�ж��Ƿ�Ϊһ���սṹ��
		strcpy(Colour_Error_Func, "Colour_Distinguish_Simple");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_IMAGE_NULL;	//���ݽ�����ָ��Ϊ��
		return (Colour_Form*)/*��������*/COLOUR_ERROR_IMAGE_NULL;	//��������ִ��
	}
	//�ж��Ƿ���һ����ͨ��RGB���ͼ
	if (Image->nChannels != COLOUR_IMAGE_RGB){
		strcpy(Colour_Error_Func, "Colour_Distinguish_Simple");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_IMAGE_RGB;	//����һ�����ͼ
		return (Colour_Form*)/*��������*/COLOUR_ERROR_IMAGE_RGB;	//��������ִ��
	}
	//��ʼʶ��
	//������ɫ��
	Colour_Form* Colour_Form_New = NULL;
	int Repeat = 0;	//�����ж���ɫ���Ƿ��ظ�
	//��ʼ��
	Colour_Form_Features_Create_Initialization(&Colour_Form_New);
	int Colour_Size = Colour_Get_Struct_Size(Colour);	//��ȡ��ɫ���������ݽṹ���С
	CvScalar scalar;    //scalar 
	unsigned  long long int _X;	//����X������ֵ
	for (int i = 0; i <= Image->height - 1; ++i){	//-1����Ϊ��ջspָ��ָ���ַ�±��Ǵ�0��ʼ��
		for (int j = 0; j <= Image->width - 1; ++j){
			scalar = cvGet2D(Image, i, j);    //��ȡ���ص��RGB��ɫ����  
			(*Colour) = Colour_Top;	//�ָ��������߽�ڵ�
			for (int k = 0; k < Colour_Size; ++k){
				//��һ����Ȼ���ȶ��㷨
				if (scalar.val[2] <= (*Colour)->Colour_Max_RGB[0] && scalar.val[1] <= (*Colour)->Colour_Max_RGB[1] && scalar.val[0] <= (*Colour)->Colour_Max_RGB[2] &&//max
					scalar.val[2] >= (*Colour)->Colour_Min_RGB[0] && scalar.val[1] >= (*Colour)->Colour_Min_RGB[1] && scalar.val[0] >= (*Colour)->Colour_Min_RGB[2]){//min
					//ѹ��֮ǰ�ж�һ���Ƿ��ظ�
					if (Colour_Form_New != NULL){	//�ж���ɫ�����Ƿ����Ѿ���ȡ������ɫ
						Colour_Form_New = Colour_Form_Top;
						for (int i = 0; i < Colour_Form_Get_Size(&Colour_Form_New); ++i){
							//�ж��Ƿ�һ��
							if (strcmp((*Colour)->Colour_Name, Colour_Form_New->Colour_Name) == 0){
								Repeat = 1;	//�����ظ���־
								Colour_Form_New = Colour_Form_Top;	//�߽�ڵ�ָ�
								break;
							}
							//У����һ��
							Colour_Form_New = Colour_Form_New->Colour_Next;
						}
						if (Repeat == 0){
							//�ָ���β���߽�
							Colour_Form_New = Colour_Form_Top;
							//ѹ����ɫ����
							if (COLOUR_NORMAL != Colour_Form_Push(&Colour_Form_New, (*Colour)->Colour_Name,j,i)){	//����Ӧ����ɫѹ��
								return (Colour_Form*)-1;	//ѹ��ʧ��
							}
							else{
								break;	//��ȡ��һ������ֵ
							}
						}
						else{
							Repeat = 0;	//�ظ���־λ�ÿ�
							//break;
						}
					}

				}
				(*Colour) = (*Colour)->Colour_Next;
			}
		}

	}
	(*Colour) = Colour_Top;
	return Colour_Form_New;	//����ȡ������ɫ������
}
//��ɫ������ʶ����_��Ȼ��RGB�ȶ�_����Աȷ�_�߽��Եģ����vague_��Եģ��������һ����ɫ��MAX��MIN�����ٽ�ֵ֮��ȡֵ��ģ��ֵVague�����ų���MAX��MINֵ�ķ�Χ�������ֵ,��ֵ������
COLOURIN(Colour_Form*) Colour_Distinguish_Simple_Vague(struct Colour_Features** Colour/*ʶ�����ɫ������*/, IplImage* Image/*Ҫʶ����ɫ��ͼ��*/, int Vague = 1/*��Եģ��ֵ*/){
	if ((*Colour) == NULL){	//�ж��Ƿ�Ϊһ���սṹ��
		strcpy(Colour_Error_Func, "Colour_Distinguish_Simple");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return (Colour_Form*)/*��������*/COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	if (Image == NULL){	//�ж��Ƿ�Ϊһ���սṹ��
		strcpy(Colour_Error_Func, "Colour_Distinguish_Simple");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_IMAGE_NULL;	//���ݽ�����ָ��Ϊ��
		return (Colour_Form*)/*��������*/COLOUR_ERROR_IMAGE_NULL;	//��������ִ��
	}
	//�ж��Ƿ���һ����ͨ��RGB���ͼ
	if (Image->nChannels != COLOUR_IMAGE_RGB){
		strcpy(Colour_Error_Func, "Colour_Distinguish_Simple");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_IMAGE_RGB;	//����һ�����ͼ
		return (Colour_Form*)/*��������*/COLOUR_ERROR_IMAGE_RGB;	//��������ִ��
	}
	//��ʼʶ��
	//������ɫ��
	Colour_Form* Colour_Form_New = NULL;
	int Repeat = 0;	//�����ж���ɫ���Ƿ��ظ�
	//��ʼ��
	Colour_Form_Features_Create_Initialization(&Colour_Form_New);
	int Colour_Size = Colour_Get_Struct_Size(Colour);	//��ȡ��ɫ���������ݽṹ���С
	CvScalar scalar;    //scalar 
	unsigned  long long int _X;	//����X������ֵ
	for (int i = 0; i <= Image->height - 1; ++i){	//-1����Ϊ��ջspָ��ָ���ַ�±��Ǵ�0��ʼ��
		for (int j = 0; j <= Image->width - 1; ++j){
			scalar = cvGet2D(Image, i, j);    //��ȡ���ص��RGB��ɫ����  
			(*Colour) = Colour_Top;	//�ָ��������߽�ڵ�
			for (int k = 0; k < Colour_Size; ++k){
				//��һ����Ȼ���ȶ��㷨
				//����0����Ϊֵ�պ����
				if (scalar.val[2] <= (*Colour)->Colour_Max_RGB[0] && scalar.val[1] <= (*Colour)->Colour_Max_RGB[1] && scalar.val[0] <= (*Colour)->Colour_Max_RGB[2] &&//max
					scalar.val[2] >= (*Colour)->Colour_Min_RGB[0] && scalar.val[1] >= (*Colour)->Colour_Min_RGB[1] && scalar.val[0] >= (*Colour)->Colour_Min_RGB[2] ||	//min
					scalar.val[2] <= (*Colour)->Colour_Max_RGB[0] + Vague && scalar.val[1] <= (*Colour)->Colour_Max_RGB[1] + Vague && scalar.val[0] <= (*Colour)->Colour_Max_RGB[2] + Vague &&//max
					scalar.val[2] >= (*Colour)->Colour_Min_RGB[0] + Vague && scalar.val[1] >= (*Colour)->Colour_Min_RGB[1] + Vague && scalar.val[0] >= (*Colour)->Colour_Min_RGB[2] + Vague){//min
					//ѹ��֮ǰ�ж�һ���Ƿ��ظ�
					if (Colour_Form_New != NULL){	//�ж���ɫ�����Ƿ����Ѿ���ȡ������ɫ
						Colour_Form_New = Colour_Form_Top;
						for (int i = 0; i < Colour_Form_Get_Size(&Colour_Form_New); ++i){
							//�ж��Ƿ�һ��
							if (strcmp((*Colour)->Colour_Name, Colour_Form_New->Colour_Name) == 0){
								Repeat = 1;	//�����ظ���־
								Colour_Form_New = Colour_Form_Top;	//�߽�ڵ�ָ�
								break;
							}
							//У����һ��
							Colour_Form_New = Colour_Form_New->Colour_Next;
						}
						if (Repeat == 0){
							//�ָ���β���߽�
							Colour_Form_New = Colour_Form_Top;
							//ѹ����ɫ����
							if (COLOUR_NORMAL != Colour_Form_Push(&Colour_Form_New, (*Colour)->Colour_Name, j, i)){	//����Ӧ����ɫѹ��
								return (Colour_Form*)-1;	//ѹ��ʧ��
							}
							else{
								break;	//��ȡ��һ������ֵ
							}
						}
						else{
							Repeat = 0;	//�ظ���־λ�ÿ�
							//break;
						}
					}

				}
				(*Colour) = (*Colour)->Colour_Next;
			}
		}

	}
	(*Colour) = Colour_Top;
	return Colour_Form_New;	//����ȡ������ɫ������
}
//��ɫ������ʶ����_��Ȼ��RGB�ȶ�_��Сͼ��Աȷ� �ŵ㣺�����죬ȱ�㣺���ű���ԽСʱ���ܻ���ɲ�����ɫ��ʧ���ҵ��������ű����������ʻ��������������Խ���ʹ��Ĭ��ֵ
//�߽��Եģ����vague_��Եģ��������һ����ɫ��MAX��MIN�����ٽ�ֵ֮��ȡֵ��ģ��ֵVague�����ų���MAX��MINֵ�ķ�Χ�������ֵ,��ֵ������
//���ű�����������0.Ϊ��λ���Ϊ0.9 ��0��ʼ���㣬���ŵı���ԽС�����ٶ�Խ��,��ɫ���ſ��ܻᵼ�»�ȡ������ɫ˳���λ������һ��ͼ����ɫ�ֲ��ǣ��ڰ��̣�����ȡ�����Ŀ������̰׺ڣ���Ϊ���Ż���ͼ�������С������ɫ�ص���һ�𷽱����!
COLOURIN(Colour_Form*) Colour_Distinguish_Simple_Zoom(struct Colour_Features** Colour/*ʶ�����ɫ������*/, IplImage* Image/*Ҫʶ����ɫ��ͼ��*/, double  Zoom_Multiple = 0.2/*Ҫ���ŵı���������ʹ��Ĭ��ֵ*/) {
	if ((*Colour) == NULL){	//�ж��Ƿ�Ϊһ���սṹ��
		strcpy(Colour_Error_Func, "Colour_Distinguish_Simple");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return (Colour_Form*)/*��������*/COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	if (Image == NULL){	//�ж��Ƿ�Ϊһ���սṹ��
		strcpy(Colour_Error_Func, "Colour_Distinguish_Simple");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_IMAGE_NULL;	//���ݽ�����ָ��Ϊ��
		return (Colour_Form*)/*��������*/COLOUR_ERROR_IMAGE_NULL;	//��������ִ��
	}
	//�ж��Ƿ���һ����ͨ��RGB���ͼ
	if (Image->nChannels != COLOUR_IMAGE_RGB){
		strcpy(Colour_Error_Func, "Colour_Distinguish_Simple");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_IMAGE_RGB;	//����һ�����ͼ
		return (Colour_Form*)/*��������*/COLOUR_ERROR_IMAGE_RGB;	//��������ִ��
	}
	IplImage *Image_Zoom =  Colour_Image_Zoom(Image, Zoom_Multiple);	//����
	//cvSaveImage("c:\\1.jpg", Image_Zoom);
	//��ʼʶ��
	//������ɫ��,���ﲻ�����ظ����������ʶ��������Ϊ���������ظ����жϣ���Ҫ�漰����������֮��Ķ�ջ����������Ϊ�˽�ʡʱ��������Ѻ�������չ�����������Ӵ���ķ�ʽ�������ʱ��
	Colour_Form* Colour_Form_New = NULL;
	int Repeat = 0;	//�����ж���ɫ���Ƿ��ظ�
	//��ʼ��
	Colour_Form_Features_Create_Initialization(&Colour_Form_New);
	int Colour_Size = Colour_Get_Struct_Size(Colour);	//��ȡ��ɫ���������ݽṹ���С
	CvScalar scalar;    //scalar 
	for (int i = 0; i <= Image_Zoom->height - 1; ++i){	//-1����Ϊ��ջspָ��ָ���ַ�±��Ǵ�0��ʼ��
		for (int j = 0; j <= Image_Zoom->width - 1; ++j){
			scalar = cvGet2D(Image_Zoom, i, j);    //��ȡ���ص��RGB��ɫ����  
			(*Colour) = Colour_Top;	//�ָ��������߽�ڵ�
			for (int k = 0; k < Colour_Size; ++k){
				//��һ����Ȼ���ȶ��㷨
				if (scalar.val[2] <= (*Colour)->Colour_Max_RGB[0] && scalar.val[1] <= (*Colour)->Colour_Max_RGB[1] && scalar.val[0] <= (*Colour)->Colour_Max_RGB[2] &&//max
					scalar.val[2] >= (*Colour)->Colour_Min_RGB[0] && scalar.val[1] >= (*Colour)->Colour_Min_RGB[1] && scalar.val[0] >= (*Colour)->Colour_Min_RGB[2]){//min
					//ѹ��֮ǰ�ж�һ���Ƿ��ظ�
					if (Colour_Form_New != NULL){	//�ж���ɫ�����Ƿ����Ѿ���ȡ������ɫ
						Colour_Form_New = Colour_Form_Top;
						for (int i = 0; i < Colour_Form_Get_Size(&Colour_Form_New); ++i){
							//�ж��Ƿ�һ��
							if (strcmp((*Colour)->Colour_Name, Colour_Form_New->Colour_Name) == 0){
								Repeat = 1;	//�����ظ���־
								Colour_Form_New = Colour_Form_Top;	//�߽�ڵ�ָ�
								break;
							}
							//У����һ��
							Colour_Form_New = Colour_Form_New->Colour_Next;
						}
						if (Repeat == 0){
							//printf("%s,%lf,%lf,%lf\n%lf,%lf,%lf", (*Colour)->Colour_Name, (*Colour)->Colour_Min_RGB[0], (*Colour)->Colour_Min_RGB[1], (*Colour)->Colour_Min_RGB[2], scalar.val[2], scalar.val[1], scalar.val[0]);
							//getchar();
							//�ָ���β���߽�
							Colour_Form_New = Colour_Form_Top;
							//ѹ����ɫ����
							if (COLOUR_NORMAL != Colour_Form_Push(&Colour_Form_New, (*Colour)->Colour_Name,j,i)){	//����Ӧ����ɫѹ��
								return (Colour_Form*)-1;	//ѹ��ʧ��
							}
							else{
								break;	//��ȡ��һ������ֵ
							}
						}
						else{
							Repeat = 0;	//�ظ���־λ�ÿ�
							//break;
						}
					}

				}
				(*Colour) = (*Colour)->Colour_Next;
			}
		}

	}
	(*Colour) = Colour_Top;
	return Colour_Form_New;	//����ȡ������ɫ������
}
//��ɫ������ʶ����_��Ȼ��RGB�ȶ�_��Сͼ��Աȷ�_��Եģ����
COLOURIN(Colour_Form*) Colour_Distinguish_Simple_Zoom_Vague(struct Colour_Features** Colour/*ʶ�����ɫ������*/, IplImage* Image/*Ҫʶ����ɫ��ͼ��*/, double  Zoom_Multiple = 0.2/*Ҫ���ŵı���������ʹ��Ĭ��ֵ*/, int Vague = 1/*��Եģ��ֵ*/){
	if ((*Colour) == NULL){	//�ж��Ƿ�Ϊһ���սṹ��
		strcpy(Colour_Error_Func, "Colour_Distinguish_Simple");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return (Colour_Form*)/*��������*/COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	if (Image == NULL){	//�ж��Ƿ�Ϊһ���սṹ��
		strcpy(Colour_Error_Func, "Colour_Distinguish_Simple");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_IMAGE_NULL;	//���ݽ�����ָ��Ϊ��
		return (Colour_Form*)/*��������*/COLOUR_ERROR_IMAGE_NULL;	//��������ִ��
	}
	//�ж��Ƿ���һ����ͨ��RGB���ͼ
	if (Image->nChannels != COLOUR_IMAGE_RGB){
		strcpy(Colour_Error_Func, "Colour_Distinguish_Simple");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_IMAGE_RGB;	//����һ�����ͼ
		return (Colour_Form*)/*��������*/COLOUR_ERROR_IMAGE_RGB;	//��������ִ��
	}
	IplImage *Image_Zoom = Colour_Image_Zoom(Image, Zoom_Multiple);	//����
	//��ʼʶ��
	//������ɫ��,���ﲻ�����ظ����������ʶ��������Ϊ���������ظ����жϣ���Ҫ�漰����������֮��Ķ�ջ����������Ϊ�˽�ʡʱ��������Ѻ�������չ�����������Ӵ���ķ�ʽ�������ʱ��
	Colour_Form* Colour_Form_New = NULL;
	int Repeat = 0;	//�����ж���ɫ���Ƿ��ظ�
	//��ʼ��
	Colour_Form_Features_Create_Initialization(&Colour_Form_New);
	int Colour_Size = Colour_Get_Struct_Size(Colour);	//��ȡ��ɫ���������ݽṹ���С
	CvScalar scalar;    //scalar 
	for (int i = 0; i <= Image_Zoom->height - 1; ++i){	//-1����Ϊ��ջspָ��ָ���ַ�±��Ǵ�0��ʼ��
		for (int j = 0; j <= Image_Zoom->width - 1; ++j){
			scalar = cvGet2D(Image_Zoom, i, j);    //��ȡ���ص��RGB��ɫ����  
			(*Colour) = Colour_Top;	//�ָ��������߽�ڵ�
			for (int k = 0; k < Colour_Size; ++k){
				//��һ����Ȼ���ȶ��㷨,ע�⸡��������ֱ��ʹ����Ŀ���������Ҫʹ���ض��ĺ������м���~��Ϊ��������CPU�ڲ�ʹ�õ��Ǹ�����ALU���㵥Ԫ��ָ�,���������Լ������ֵ�ķ�ʽ���бȽ�
				//����0����Ϊֵ�պ����
				if (scalar.val[2] <= (*Colour)->Colour_Max_RGB[0] && scalar.val[1] <= (*Colour)->Colour_Max_RGB[1] && scalar.val[0] <= (*Colour)->Colour_Max_RGB[2] &&//max
					scalar.val[2] >= (*Colour)->Colour_Min_RGB[0] && scalar.val[1] >= (*Colour)->Colour_Min_RGB[1] && scalar.val[0] >= (*Colour)->Colour_Min_RGB[2] ||	//min
					scalar.val[2] <= (*Colour)->Colour_Max_RGB[0] + Vague && scalar.val[1] <= (*Colour)->Colour_Max_RGB[1] + Vague && scalar.val[0] <= (*Colour)->Colour_Max_RGB[2] + Vague &&//max
					scalar.val[2] >= (*Colour)->Colour_Min_RGB[0] + Vague && scalar.val[1] >= (*Colour)->Colour_Min_RGB[1] + Vague && scalar.val[0] >= (*Colour)->Colour_Min_RGB[2] + Vague){//min
					//ѹ��֮ǰ�ж�һ���Ƿ��ظ�
					if (Colour_Form_New != NULL){	//�ж���ɫ�����Ƿ����Ѿ���ȡ������ɫ
						Colour_Form_New = Colour_Form_Top;
						for (int i = 0; i < Colour_Form_Get_Size(&Colour_Form_New); ++i){
							//�ж��Ƿ�һ��
							if (strcmp((*Colour)->Colour_Name, Colour_Form_New->Colour_Name) == 0){
								Repeat = 1;	//�����ظ���־
								Colour_Form_New = Colour_Form_Top;	//�߽�ڵ�ָ�
								break;
							}
							//У����һ��
							Colour_Form_New = Colour_Form_New->Colour_Next;
						}
						if (Repeat == 0){
							//�ָ���β���߽�
							Colour_Form_New = Colour_Form_Top;
							//ѹ����ɫ����
							if (COLOUR_NORMAL != Colour_Form_Push(&Colour_Form_New, (*Colour)->Colour_Name, j, i)){	//����Ӧ����ɫѹ��
								return (Colour_Form*)-1;	//ѹ��ʧ��
							}
							else{
								break;	//��ȡ��һ������ֵ
							}
						}
						else{
							Repeat = 0;	//�ظ���־λ�ÿ�
							//break;
						}
					}

				}
				(*Colour) = (*Colour)->Colour_Next;
			}
		}

	}
	(*Colour) = Colour_Top;
	return Colour_Form_New;	//����ȡ������ɫ������
}
//��ɫ������ʶ����_��Ȼ��RGB�ȶ�_ʮ�ַ�ˮ���㷨
COLOURIN(Colour_Form*) Colour_Distinguish_Simple_Cross_Watershed(struct Colour_Features** Colour/*ʶ�����ɫ������*/, IplImage* Image/*Ҫʶ����ɫ��ͼ��*/,int Hnucleus = 10/*�˴�С*/){
	if ((*Colour) == NULL){	//�ж��Ƿ�Ϊһ���սṹ��
		strcpy(Colour_Error_Func, "Colour_Distinguish_Simple");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return (Colour_Form*)/*��������*/COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	if (Image == NULL){	//�ж��Ƿ�Ϊһ���սṹ��
		strcpy(Colour_Error_Func, "Colour_Distinguish_Simple");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_IMAGE_NULL;	//���ݽ�����ָ��Ϊ��
		return (Colour_Form*)/*��������*/COLOUR_ERROR_IMAGE_NULL;	//��������ִ��
	}
	//�ж��Ƿ���һ����ͨ��RGB���ͼ
	if (Image->nChannels != COLOUR_IMAGE_RGB){
		strcpy(Colour_Error_Func, "Colour_Distinguish_Simple");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_IMAGE_RGB;	//����һ�����ͼ
		return (Colour_Form*)/*��������*/COLOUR_ERROR_IMAGE_RGB;	//��������ִ��
	}
	//�жϺ˴�С�Ƿ񳬳���ˮ�����д�С
	if (Hnucleus > (Image->height) / 4){
		strcpy(Colour_Error_Func, "Colour_Distinguish_Simple");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_CROSS;	
		return (Colour_Form*)/*��������*/COLOUR_ERROR_CROSS;	//��������ִ��
	}
	//���򻮷�
	//��λʮ����������
	Colour_Form* Colour_Form_New = NULL;
	//��ʼ��
	Colour_Form_Features_Create_Initialization(&Colour_Form_New);
	int Colour_Size = Colour_Get_Struct_Size(Colour);	//��ȡ��ɫ���������ݽṹ���С
	int Repeat = 0;	//�����ж���ɫ���Ƿ��ظ�
	int Rest = (Image->height);	//��ȡ�д�С
	int Core = Rest / 4;	//����λ��
	CvScalar scalar;    //scalar 
	for (int i = Core; i < Core+Hnucleus/*�����м�����=������*/; ++i){	//��ˮ��
		for (int j = 0; j < Image->width; ++j){
			scalar = cvGet2D(Image, i, j);    //��ȡ���ص��RGB��ɫ����  
			(*Colour) = Colour_Top;	//�ָ��������߽�ڵ�
			for (int k = 0; k < Colour_Size; ++k){
				//��һ����Ȼ���ȶ��㷨,ע�⸡��������ֱ��ʹ����Ŀ���������Ҫʹ���ض��ĺ������м���~��Ϊ��������CPU�ڲ�ʹ�õ��Ǹ�����ALU���㵥Ԫ��ָ�,���������Լ������ֵ�ķ�ʽ���бȽ�
				if (scalar.val[2] <= (*Colour)->Colour_Max_RGB[0] && scalar.val[1] <= (*Colour)->Colour_Max_RGB[1] && scalar.val[0] <= (*Colour)->Colour_Max_RGB[2] &&//max
					scalar.val[2] >= (*Colour)->Colour_Min_RGB[0] && scalar.val[1] >= (*Colour)->Colour_Min_RGB[1] && scalar.val[0] >= (*Colour)->Colour_Min_RGB[2]){//min
					//ѹ��֮ǰ�ж�һ���Ƿ��ظ�
					if (Colour_Form_New != NULL){	//�ж���ɫ�����Ƿ����Ѿ���ȡ������ɫ
						Colour_Form_New = Colour_Form_Top;
						for (int i = 0; i < Colour_Form_Get_Size(&Colour_Form_New); ++i){
							//�ж��Ƿ�һ��
							if (strcmp((*Colour)->Colour_Name, Colour_Form_New->Colour_Name) == 0){
								Repeat = 1;	//�����ظ���־
								Colour_Form_New = Colour_Form_Top;	//�߽�ڵ�ָ�
								break;
							}
							//У����һ��
							Colour_Form_New = Colour_Form_New->Colour_Next;
						}
						if (Repeat == 0){
							//�ָ���β���߽�
							Colour_Form_New = Colour_Form_Top;
							//ѹ����ɫ����
							if (COLOUR_NORMAL != Colour_Form_Push(&Colour_Form_New, (*Colour)->Colour_Name, j, i)){	//����Ӧ����ɫѹ��
								return (Colour_Form*)-1;	//ѹ��ʧ��
							}
							else{
								break;	//��ȡ��һ������ֵ
							}
						}
						else{
							Repeat = 0;	//�ظ���־λ�ÿ�
							//break;
						}
					}

				}
				(*Colour) = (*Colour)->Colour_Next;
			}
		}
	}
	return Colour_Form_New;
	
}
//��ɫ������ʶ����_��Ȼ��RGB�ȶ�_ʮ�ַ�ˮ���㷨_��Եģ����
COLOURIN(Colour_Form*) Colour_Distinguish_Simple_Cross_Watershed_Vague(struct Colour_Features** Colour/*ʶ�����ɫ������*/, IplImage* Image/*Ҫʶ����ɫ��ͼ��*/, int Hnucleus = 10/*�˴�С*/, int Vague = 1/*��Եģ��ֵ*/){
	if ((*Colour) == NULL){	//�ж��Ƿ�Ϊһ���սṹ��
		strcpy(Colour_Error_Func, "Colour_Distinguish_Simple");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return (Colour_Form*)/*��������*/COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	if (Image == NULL){	//�ж��Ƿ�Ϊһ���սṹ��
		strcpy(Colour_Error_Func, "Colour_Distinguish_Simple");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_IMAGE_NULL;	//���ݽ�����ָ��Ϊ��
		return (Colour_Form*)/*��������*/COLOUR_ERROR_IMAGE_NULL;	//��������ִ��
	}
	//�ж��Ƿ���һ����ͨ��RGB���ͼ
	if (Image->nChannels != COLOUR_IMAGE_RGB){
		strcpy(Colour_Error_Func, "Colour_Distinguish_Simple");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_IMAGE_RGB;	//����һ�����ͼ
		return (Colour_Form*)/*��������*/COLOUR_ERROR_IMAGE_RGB;	//��������ִ��
	}
	//�жϺ˴�С�Ƿ񳬳���ˮ�����д�С
	if (Hnucleus > (Image->height) / 4){
		strcpy(Colour_Error_Func, "Colour_Distinguish_Simple");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_CROSS;	
		return (Colour_Form*)/*��������*/COLOUR_ERROR_CROSS;	//��������ִ��
	}
	//���򻮷�
	//��λʮ����������
	Colour_Form* Colour_Form_New = NULL;
	//��ʼ��
	Colour_Form_Features_Create_Initialization(&Colour_Form_New);
	int Colour_Size = Colour_Get_Struct_Size(Colour);	//��ȡ��ɫ���������ݽṹ���С
	int Repeat = 0;	//�����ж���ɫ���Ƿ��ظ�
	int Rest = (Image->height);	//��ȡ�д�С
	int Core = Rest / 4;	//����λ��
	CvScalar scalar;    //scalar 
	for (int i = Core; i < Core + Hnucleus/*�����м�����=������*/; ++i){	//��ˮ��
		for (int j = 0; j < Image->width; ++j){
			scalar = cvGet2D(Image, i, j);    //��ȡ���ص��RGB��ɫ����  
			(*Colour) = Colour_Top;	//�ָ��������߽�ڵ�
			for (int k = 0; k < Colour_Size; ++k){
				//��һ����Ȼ���ȶ��㷨,ע�⸡��������ֱ��ʹ����Ŀ���������Ҫʹ���ض��ĺ������м���~��Ϊ��������CPU�ڲ�ʹ�õ��Ǹ�����ALU���㵥Ԫ��ָ�,���������Լ������ֵ�ķ�ʽ���бȽ�
				if (scalar.val[2] <= (*Colour)->Colour_Max_RGB[0] && scalar.val[1] <= (*Colour)->Colour_Max_RGB[1] && scalar.val[0] <= (*Colour)->Colour_Max_RGB[2] &&//max
					scalar.val[2] >= (*Colour)->Colour_Min_RGB[0] && scalar.val[1] >= (*Colour)->Colour_Min_RGB[1] && scalar.val[0] >= (*Colour)->Colour_Min_RGB[2] ||	//min
					scalar.val[2] <= (*Colour)->Colour_Max_RGB[0] + Vague && scalar.val[1] <= (*Colour)->Colour_Max_RGB[1] + Vague && scalar.val[0] <= (*Colour)->Colour_Max_RGB[2] + Vague &&//max
					scalar.val[2] >= (*Colour)->Colour_Min_RGB[0] + Vague && scalar.val[1] >= (*Colour)->Colour_Min_RGB[1] + Vague && scalar.val[0] >= (*Colour)->Colour_Min_RGB[2] + Vague){//min
					//ѹ��֮ǰ�ж�һ���Ƿ��ظ�
					if (Colour_Form_New != NULL){	//�ж���ɫ�����Ƿ����Ѿ���ȡ������ɫ
						Colour_Form_New = Colour_Form_Top;
						for (int i = 0; i < Colour_Form_Get_Size(&Colour_Form_New); ++i){
							//�ж��Ƿ�һ��
							if (strcmp((*Colour)->Colour_Name, Colour_Form_New->Colour_Name) == 0){
								Repeat = 1;	//�����ظ���־
								Colour_Form_New = Colour_Form_Top;	//�߽�ڵ�ָ�
								break;
							}
							//У����һ��
							Colour_Form_New = Colour_Form_New->Colour_Next;
						}
						if (Repeat == 0){
							//�ָ���β���߽�
							Colour_Form_New = Colour_Form_Top;
							//ѹ����ɫ����
							if (COLOUR_NORMAL != Colour_Form_Push(&Colour_Form_New, (*Colour)->Colour_Name, j, i)){	//����Ӧ����ɫѹ��
								return (Colour_Form*)-1;	//ѹ��ʧ��
							}
							else{
								break;	//��ȡ��һ������ֵ
							}
						}
						else{
							Repeat = 0;	//�ظ���־λ�ÿ�
							//break;
						}
					}

				}
				(*Colour) = (*Colour)->Colour_Next;
			}
		}
	}
	return Colour_Form_New;

}
//����ͼ��ָ��λ�õ����ص���ʲô��ɫ
COLOURIN(Colour_Form*) Colour_Retrieval_Position(struct Colour_Features** Colour/*ʶ�����ɫ������*/, IplImage* Image/*Ҫʶ����ɫ��ͼ��*/, int Pos_X/*Ҫʶ���x����*/, int Pos_Y/*Ҫʶ���y����*/){
	if ((*Colour) == NULL){	//�ж��Ƿ�Ϊһ���սṹ��
		strcpy(Colour_Error_Func, "Colour_Retrieval_Position");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return (Colour_Form*)/*��������*/COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	if (Image == NULL){	//�ж��Ƿ�Ϊһ���սṹ��
		strcpy(Colour_Error_Func, "Colour_Retrieval_Position");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_IMAGE_NULL;	//���ݽ�����ָ��Ϊ��
		return (Colour_Form*)/*��������*/COLOUR_ERROR_IMAGE_NULL;	//��������ִ��
	}
	//�ж��Ƿ���һ����ͨ��RGB���ͼ
	if (Image->nChannels != COLOUR_IMAGE_RGB){
		strcpy(Colour_Error_Func, "Colour_Retrieval_Position");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_IMAGE_RGB;	//����һ�����ͼ
		return (Colour_Form*)/*��������*/COLOUR_ERROR_IMAGE_RGB;	//��������ִ��
	}
	//�ж�x-y�Ƿ����ͼ�����ط�Χ
	if (Pos_X > Image->width || Pos_Y > Image->height){
		strcpy(Colour_Error_Func, "Colour_Retrieval_Position");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_IMAGE_XY_POS;
		return (Colour_Form*)/*��������*/COLOUR_ERROR_IMAGE_XY_POS;	//��������ִ��
	}
	Colour_Form* Colour_Form_New = NULL;
	//��ʼ��
	Colour_Form_Features_Create_Initialization(&Colour_Form_New);
	int Colour_Size = Colour_Get_Struct_Size(Colour);	//��ȡ��ɫ���������ݽṹ���С
	int Repeat = 0;	//�����ж���ɫ���Ƿ��ظ�
	int Rest = (Image->height);	//��ȡ�д�С
	CvScalar scalar;    //scalar 
	(*Colour) = Colour_Top;	//�ָ��������߽�ڵ�
	scalar = cvGet2D(Image, Pos_Y, Pos_X);    //��ȡ���ص��RGB��ɫ����  
	for (int k = 0; k < Colour_Size; ++k){
		//��һ����Ȼ���ȶ��㷨
		if ((fabs(scalar.val[2] - (*Colour)->Colour_Max_RGB[0]) == 0) && (fabs(scalar.val[1] - (*Colour)->Colour_Max_RGB[1]) == 0) && (fabs(scalar.val[0] - (*Colour)->Colour_Max_RGB[2]) == 0) &&//max
			(fabs(scalar.val[2] - (*Colour)->Colour_Min_RGB[0]) >= 0) && (fabs(scalar.val[1] - (*Colour)->Colour_Min_RGB[1]) >= 0) && (fabs(scalar.val[0] - (*Colour)->Colour_Min_RGB[2]) >= 0)){//min
			//ѹ��֮ǰ�ж�һ���Ƿ��ظ�
			if (Colour_Form_New != NULL){	//�ж���ɫ�����Ƿ����Ѿ���ȡ������ɫ
				Colour_Form_New = Colour_Form_Top;
				for (int i = 0; i < Colour_Form_Get_Size(&Colour_Form_New); ++i){
					//�ж��Ƿ�һ��
					if (strcmp((*Colour)->Colour_Name, Colour_Form_New->Colour_Name) == 0){
						Repeat = 1;	//�����ظ���־
						Colour_Form_New = Colour_Form_Top;	//�߽�ڵ�ָ�
						break;
					}
					//У����һ��
					Colour_Form_New = Colour_Form_New->Colour_Next;
				}
				if (Repeat == 0){
					//�ָ���β���߽�
					Colour_Form_New = Colour_Form_Top;
					//ѹ����ɫ����
					if (COLOUR_NORMAL != Colour_Form_Push(&Colour_Form_New, (*Colour)->Colour_Name, Pos_X, Pos_Y)){	//����Ӧ����ɫѹ��
						return (Colour_Form*)-1;	//ѹ��ʧ��
					}
					else{
						break;	//��ȡ��һ������ֵ
					}
				}
				else{
					Repeat = 0;	//�ظ���־λ�ÿ�
					//break;
				}
			}

		}
		(*Colour) = (*Colour)->Colour_Next;

	}
	return Colour_Form_New;	//����ʶ�𵽵���ɫ
}
/*
��ɫ��������ز�������
*/
//У�����Ƿ���һ����������ɫ������
COLOURIN_SIGNED(int) Colour_State_File_Check(char *Colour_State_Name/*��ɫ�ļ���*/){
	//�ж��Ƿ�Ϊһ����ָ��
	if (Colour_State_Name == NULL){	//�ж��Ƿ�Ϊһ����ָ��
		strcpy(Colour_Error_Func, "Colour_State_File_Check");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	//�ж�·���Ƿ���Ч
	if (0 == strlen(Colour_State_Name)){
		strcpy(Colour_Error_Func, "Colour_State_File_Check");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_STR_NULL;	//���ַ�
		return COLOUR_ERROR_STR_NULL;	//��������ִ��
	}
	//У���ļ����Ƿ���ȷ
	char Colour_Name_Str[] = COLOUR_FILE_SUFFIX;	//��ȡ��׺
	int Colour_Name_Size = strlen(Colour_Name_Str);	//��ȡ��׺����С
	int Path_Size = strlen(Colour_State_Name);	//����·����С
	//�ȶ�
	for (int i = Colour_Name_Size; i > 0; --i, --Path_Size){
		if (Colour_Name_Str[i] != Colour_State_Name[Path_Size]){
			strcpy(Colour_Error_Func, "Colour_State_File_Check");	//���г����ĺ�����
			Colour_Errror = COLOUR_ERROR_FILE_INCORRECT;	//����һ����ȷ��ɫ�������ļ�
			return COLOUR_ERROR_FILE_INCORRECT;		//��������ִ��
		}
	}	
	return COLOUR_NORMAL;	//����ִ������

}
//�����е���ɫ�����Ᵽ�浽����
COLOURIN_SIGNED(int) Colour_State_File_Seve(char *Colour_State_Lib/*��ɫ������*/, char *Path/*Ҫ�����·��*/, int Mode = COLOUR_FILE_SEVE_NEW_FORCE/*Ҫ����ķ�ʽ*/){
	//�жϴ��ݽ�����ָ���Ƿ�Ϊ��
	if (Colour_State_Lib == NULL){	//�ж��Ƿ�Ϊһ����ָ��
		strcpy(Colour_Error_Func, "Colour_State_File_Seve");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	if (Path == NULL){	//�ж��Ƿ�Ϊһ����ָ��
		strcpy(Colour_Error_Func, "Colour_State_File_Seve");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	if (COLOUR_ERROR_FILE_INCORRECT == Colour_State_File_Check(Path)){	//�ж��ļ����Ƿ���ȷ
		strcpy(Colour_Error_Func, "Colour_State_File_Seve");	//���г����ĺ�����
		return -1;	//����һ����ȷ����ɫ�������ļ�
	}
	//�����ļ�ָ��
	FILE *fp = NULL;
	//У�鱣�淽ʽ
	switch (Mode){
	case COLOUR_FILE_SEVE_ADD:	//�Ը��ӵķ�ʽ�����ļ�
		fp = fopen(Path, "r");	//����ֻ���ķ�ʽ�򿪣�ȷ���ļ����ڣ�
		if (fp == NULL){
			strcpy(Colour_Error_Func, "Colour_State_File_Seve");	//���г����ĺ�����
			Colour_Errror = COLOUR_ERROR_FILE_NO_EXISTENT;	//�ļ�������
			return COLOUR_ERROR_FILE_NO_EXISTENT;	//��������ִ��
		}
		else{//�ļ����������´�һ��
			fclose(fp);
			fp = NULL;
			fp = fopen(Path, "a");
		}
		break;
	case COLOUR_FILE_SEVE_NEW:		//�����ļ��ķ�ʽ����
		fp = fopen(Path, "r");	//����ֻ���ķ�ʽ�򿪣�ȷ���ļ��Ƿ���ڣ�
		if (fp != NULL){
			strcpy(Colour_Error_Func, "Colour_State_File_Seve");	//���г����ĺ�����
			Colour_Errror = COLOUR_ERROR_FILE_EXISTENT;	//��ɫ�����ļ��Ѵ���
			return COLOUR_ERROR_FILE_EXISTENT;	//��������ִ��
		}
		else{//�����ڵĻ��򴴽����ļ�������
			fclose(fp);
			fp = NULL;
			fp = fopen(Path, "w");
		}
		break;
	case COLOUR_FILE_SEVE_NEW_FORCE:		//�������ļ�
		fp = fopen(Path, "w");
		break;
	default:
		strcpy(Colour_Error_Func, "Colour_State_File_Seve");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_FILE_NO_MODE;	//����ı��淽ʽ
		return COLOUR_ERROR_FILE_NO_MODE;	//��������ִ��
		break;
	}
	//д��
	fwrite(Colour_State_Lib, strlen(Colour_State_Lib), 1, fp);
	//�ͷ�
	fclose(fp);
	fp = NULL;
	return COLOUR_NORMAL;	//����ִ������
}
//�����е���ɫ�����Ᵽ�浽����_���л��ķ�ʽ_�������滻�Ը��ӵķ�ʽ���ӵ��ļ�β��
COLOURIN_SIGNED(int) Colour_Struct_File_Seve(struct Colour_Features** Colour/*Ҫ�������ɫ������*/, char *Path/*Ҫ�����·��*/){
	//�жϴ��ݽ�����ָ���Ƿ�Ϊ��
	if ((*Colour) == NULL){	//�ж��Ƿ�Ϊһ����ָ��
		strcpy(Colour_Error_Func, "Colour_Struct_File_Seve");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	if (Path == NULL){	//�ж��Ƿ�Ϊһ����ָ��
		strcpy(Colour_Error_Func, "Colour_Struct_File_Seve");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	if (COLOUR_ERROR_FILE_INCORRECT == Colour_State_File_Check(Path)){	//�ж��ļ����Ƿ���ȷ
		strcpy(Colour_Error_Func, "Colour_Struct_File_Seve");	//���г����ĺ�����
		return -1;	//����һ����ȷ����ɫ�������ļ�
	}
	//�����ļ�ָ��
	FILE *fp = NULL;
	//���淽ʽ
	fp = fopen(Path, "a");
	char File_[256] = { 0 };	//�ļ�����
	//д��
	struct Colour_Features *Colour_Current = (*Colour);	//���浱ǰ�ڵ�
	(*Colour) = Colour_Top;	//�õ��߽�Ԫ��
	for (int i = 1; i <= Colour_Get_Struct_Size(Colour); ++i){
		sprintf(File_, "{COLOUR:%s&MAX:%d,%d,%d&MIN:%d,%d,%d&}", Colour_Get_Struct_Colour_Name(Colour, i), Colour_Get_Struct_MAX_RGB(Colour, i, 1), \
			Colour_Get_Struct_MAX_RGB(Colour, i, 2), Colour_Get_Struct_MAX_RGB(Colour, i, 3), Colour_Get_Struct_MIN_RGB(Colour, i, 1), Colour_Get_Struct_MIN_RGB(Colour, i, 2), \
			Colour_Get_Struct_MIN_RGB(Colour, i, 3));//��ʽ���ַ�
		if (0 == fwrite(File_, strlen(File_), 1, fp)){//�Ը��ӵķ�ʽд��
			strcpy(Colour_Error_Func, "Colour_Struct_File_Seve");	//���г����ĺ�����
			Colour_Errror = COLOUR_ERROR_FILE_WRITE;	//д������Ϊ��
			return COLOUR_ERROR_FILE_WRITE;	//��������ִ��
		}	
		(*Colour) = (*Colour)->Colour_Next;	//��һ��
	}
	fwrite("#", strlen("#"), 1, fp);	//д�������
	//�ڵ㸴ԭ
	(*Colour) = Colour_Current;
	//�ͷ�
	fclose(fp);
	fp = NULL;
	return COLOUR_NORMAL;	//����ִ������
}
//�����е���ɫ�����Ᵽ�浽����_���л��ķ�ʽ_�������滻
COLOURIN_SIGNED(int) Colour_Struct_File_Seve_W(struct Colour_Features** Colour/*Ҫ�������ɫ������*/, char *Path/*Ҫ�����·��*/){
	//�жϴ��ݽ�����ָ���Ƿ�Ϊ��
	if ((*Colour) == NULL){	//�ж��Ƿ�Ϊһ����ָ��
		strcpy(Colour_Error_Func, "Colour_Struct_File_Seve");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	if (Path == NULL){	//�ж��Ƿ�Ϊһ����ָ��
		strcpy(Colour_Error_Func, "Colour_Struct_File_Seve");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	if (COLOUR_ERROR_FILE_INCORRECT == Colour_State_File_Check(Path)){	//�ж��ļ����Ƿ���ȷ
		strcpy(Colour_Error_Func, "Colour_Struct_File_Seve");	//���г����ĺ�����
		return -1;	//����һ����ȷ����ɫ�������ļ�
	}
	//�����ļ�ָ��
	FILE *fp = NULL;
	//���淽ʽ
	fp = fopen(Path, "w");
	char File_[256] = { 0 };	//�ļ�����
	//д��
	struct Colour_Features *Colour_Current = (*Colour);	//���浱ǰ�ڵ�
	(*Colour) = Colour_Top;	//�õ��߽�Ԫ��
	for (int i = 1; i <= Colour_Get_Struct_Size(Colour); ++i){
		sprintf(File_, "{COLOUR:%s&MAX:%d,%d,%d&MIN:%d,%d,%d&}", Colour_Get_Struct_Colour_Name(Colour, i), Colour_Get_Struct_MAX_RGB(Colour, i, 1), \
			Colour_Get_Struct_MAX_RGB(Colour, i, 2), Colour_Get_Struct_MAX_RGB(Colour, i, 3), Colour_Get_Struct_MIN_RGB(Colour, i, 1), Colour_Get_Struct_MIN_RGB(Colour, i, 2), \
			Colour_Get_Struct_MIN_RGB(Colour, i, 3));//��ʽ���ַ�
		if (0 == fwrite(File_, strlen(File_), 1, fp)){//�Ը��ӵķ�ʽд��
			strcpy(Colour_Error_Func, "Colour_Struct_File_Seve");	//���г����ĺ�����
			Colour_Errror = COLOUR_ERROR_FILE_WRITE;	//д������Ϊ��
			return COLOUR_ERROR_FILE_WRITE;	//��������ִ��
		}
		(*Colour) = (*Colour)->Colour_Next;	//��һ��
	}
	fwrite("#", strlen("#"), 1, fp);	//д�������
	//�ڵ㸴ԭ
	(*Colour) = Colour_Current;
	//�ͷ�
	fclose(fp);
	fp = NULL;
	return COLOUR_NORMAL;	//����ִ������
}
//��ɫ������ʵ����_�ӱ����ļ�����
COLOURIN_SIGNED(int) Colour_Serialize_Path(struct Colour_Features** Colour/*���л��Ľṹ��*/, char *Colour_State_Lib_Path/*Ҫ���л�����ɫ������·��*/){
	if ((*Colour) == NULL){	//�ж��Ƿ�Ϊһ���սṹ��
		strcpy(Colour_Error_Func, "Colour_Serialize_Path");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	if (Colour_State_Lib_Path == NULL){	//�ж��Ƿ�Ϊһ����ָ��
		strcpy(Colour_Error_Func, "Colour_Serialize_Path");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	if (COLOUR_ERROR_FILE_INCORRECT == Colour_State_File_Check(Colour_State_Lib_Path)){	//�ж��ļ����Ƿ���ȷ
		strcpy(Colour_Error_Func, "Colour_Serialize_Path");	//���г����ĺ�����
		return -1;	//����һ����ȷ����ɫ�������ļ�
	}
	FILE *fp = NULL;	//�ļ�ָ��
	fp = fopen(Colour_State_Lib_Path, "r");
	if (fp == NULL){
		strcpy(Colour_Error_Func, "Colour_Serialize_Path");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_FILE;
		return COLOUR_ERROR_FILE;
	}
	//��ȡ�ļ��������ڴ����洢�ռ�
	fseek(fp, 0, SEEK_END); //��λ���ļ�ĩ 
	int nFileLen = ftell(fp); //�ļ�����
	char *file_str = (char*)malloc(nFileLen);	//�����洢�ռ�
	if (0 == fread(file_str, nFileLen, 1, fp)){	//���ļ����ݶ�ȡ���洢�ռ���
		strcpy(Colour_Error_Func, "Colour_Serialize_Path");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_FILE_READ_0;
		return COLOUR_ERROR_FILE_READ_0;
	}
	fclose(fp);	//�ر��ļ�ָ��
	//������ɫ������ʵ��������ʵ����
	Colour_Serialize(Colour, file_str);
	return COLOUR_NORMAL;	//��������ִ��

}
//����һ���µ���ɫ�������ļ�
COLOURIN_SIGNED(int) Colour_State_File_New(char *Path/*��ɫ������·��*/){
	if (Path == NULL){	//�ж��Ƿ�Ϊһ����ָ��
		strcpy(Colour_Error_Func, "Colour_State_File_New");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	if (COLOUR_ERROR_FILE_INCORRECT == Colour_State_File_Check(Path)){	//�ж��ļ����Ƿ���ȷ
		strcpy(Colour_Error_Func, "Colour_State_File_New");	//���г����ĺ�����
		return -1;	//����һ����ȷ����ɫ�������ļ�
	}
	//�����ļ�
	FILE *fp = fopen(Path, "w");	//�������ļ�
	fclose(fp);
	fp = NULL;
	return COLOUR_NORMAL;	//����ִ������
}
//����ɫ��������ص��ڴ�
COLOURIN(char*) Colour_State_Memory(char *Path/*��ɫ������·��*/){
	if (Path == NULL){	//�ж��Ƿ�Ϊһ����ָ��
		strcpy(Colour_Error_Func, "Colour_State_Memory");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return (char*)COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	if (COLOUR_ERROR_FILE_INCORRECT == Colour_State_File_Check(Path)){	//�ж��ļ����Ƿ���ȷ
		strcpy(Colour_Error_Func, "Colour_State_Memory");	//���г����ĺ�����
		return (char*)-1;	//����һ����ȷ����ɫ�������ļ�
	}
	//�����ļ����ڴ�
	FILE *fp = NULL;	//�ļ�ָ��
	fp = fopen(Path, "r");
	if (fp == NULL){
		strcpy(Colour_Error_Func, "Colour_State_Memory");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_FILE;
		return (char*)COLOUR_ERROR_FILE;
	}
	//��ȡ�ļ��������ڴ����洢�ռ�
	fseek(fp, 0, SEEK_END); //��λ���ļ�ĩ 
	int nFileLen = ftell(fp); //�ļ�����
	fseek(fp, 0, SEEK_SET); //�ָ����ļ�ͷ
	char *file_str = (char*)malloc(nFileLen);	//�����洢�ռ�
	if (0 == fread(file_str, nFileLen, 1, fp)){	//���ļ����ݶ�ȡ���洢�ռ���
		strcpy(Colour_Error_Func, "Colour_State_Memory");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_FILE_READ_0;
		return (char*)COLOUR_ERROR_FILE_READ_0;
	}
	fclose(fp);	//�ر��ļ�ָ��
	fp = NULL;
	return file_str;

}
//����ɫ�������ͷ�_ָ�뷽ʽ
COLOURIN(int) Colour_State_Dlete(char *Colour_State/*��ɫ������*/){
	if (Colour_State == NULL){	//�ж��Ƿ�Ϊһ����ָ��
		strcpy(Colour_Error_Func, "Colour_State_Dlete");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	free(Colour_State);
	Colour_State = NULL;
	return COLOUR_NORMAL;	//����ִ������

}
//����ɫ�������ͷ�_���鷽ʽ
COLOURIN(int) Colour_State_Dlete_Arr(char *Colour_State/*��ɫ������*/){
	if (Colour_State == NULL){	//�ж��Ƿ�Ϊһ����ָ��
		strcpy(Colour_Error_Func, "Colour_State_Dlete");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	memset(Colour_State, 0, sizeof(strlen(Colour_State)));	//�ÿ�
	return COLOUR_NORMAL;	//����ִ������
}
//������ɫʶ���ٽ�ֵ

//����
//��ɫ������༭��
COLOURIN_SIGNED(int) Colour_State_File_Edit(char *Path/*��ɫ������·��*/){
	if (Path == NULL){	//�ж��Ƿ�Ϊһ����ָ��
		strcpy(Colour_Error_Func, "Colour_State_File_Edit");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_POINTER_NULL;	//���ݽ�����ָ��Ϊ��
		return COLOUR_ERROR_POINTER_NULL;	//��������ִ��
	}
	//�ж�·���Ƿ���Ч
	if (0 == strlen(Path)){
		strcpy(Colour_Error_Func, "Colour_State_File_Edit");	//���г����ĺ�����
		Colour_Errror = COLOUR_ERROR_STR_NULL;	//���ַ�
		return COLOUR_ERROR_STR_NULL;	//��������ִ��
	}
	if (COLOUR_ERROR_FILE_INCORRECT == Colour_State_File_Check(Path)){	//�ж��ļ����Ƿ���ȷ
		strcpy(Colour_Error_Func, "Colour_State_File_Edit");	//���г����ĺ�����
		return -1;	//����һ����ȷ����ɫ�������ļ�
	}
	//���л�
	Colour_Features* Colour;	//��ɫ������
	Colour_Features_Create_Initialization(&Colour);	//��ʼ��
	if (COLOUR_NORMAL != Colour_Serialize_Path(&Colour, Path)){
		strcpy(Colour_Error_Func, "Colour_State_File_Edit");	//���г����ĺ�����
		return -1;	//�޷����л���ɫ������
	}
	//����༭��
iuns:
	printf("****************************************\n");
	printf("��ɫ������༭��v2.0\n");
	printf("****************************************\n");
	printf("*��ɫ��������Ϣ��\n");
	printf("��ɫ�������С��%d\n", Colour_Get_Struct_Size(&Colour));
	printf("��ѡ��Ҫ���еĲ���:\n");
	printf("1.ɾ��ָ����ɫ��\n");
	printf("2.������ɫ��\n");
ins:
	printf("�����룺%d");
	int J_Dm = 0;	//ѡ��
	int Sequence = 0;	//����
	char Colour_File[256] = { 0 };	//�����ļ�·��
	int Frequency = 0;	//��ɫ������
	char Colour_Name[256] = { 0 };	//����ɫ��
	double Colour_Max[3] = { 0.0f };	//��MAX
	double Colour_Min[3] = { 0.0f };	//��MAX
	scanf("%d", &J_Dm);
	getchar();//�ո�
	switch (J_Dm){
	case 1:
		printf("������Ҫɾ�������У�");
		scanf("%d", &Sequence);
		getchar();//�ո�
		if (COLOUR_NORMAL != Colour_Delete_Appoint(&Colour, Sequence)){
			strcpy(Colour_Error_Func, "Colour_State_File_Edit");	//���г����ĺ�����
			return -1;	//�޷�ɾ��������ָ����ɫ����
		}
		//ɾ��
		Colour_Struct_File_Seve_W(&Colour, Path);
		printf("ɾ���ɹ�!\n");
		goto iuns;	//�ص��׽���
		break;
	case 2:
		printf("������Ҫ���Ӷ����ű���");
		scanf("%d", &Frequency);
		Colour = Colour_Top;	//�߽�ڵ�
		for (int i = 0; i <= Frequency; ++i){//��ʼ�����û�������ֵ
			printf("��ʼ���ӵ�%d�ű���\n", i + 1);
			printf("������%d�ű�����ɫ����:", i + 1);
			scanf("%s", Colour_Name);
			getchar();//�ո�
			printf("������%d�ű���MAXֵ,��\",\"�ָ�:", i + 1);
			scanf("%lf,%lf,%lf", Colour_Max[0], Colour_Max[1], Colour_Max[2]);
			getchar();//�ո�
			printf("������%d�ű���MINֵ,��\",\"�ָ�:", i + 1);
			scanf("%lf,%lf,%lf", Colour_Min[0], Colour_Min[1], Colour_Min[2]);
			getchar();//�ո�
			Colour_Features_Pressing_Basic_Type(&Colour, Colour_Name, Colour_Max, Colour_Min);//д��
			//���
			memset(Colour_Name, 0, sizeof(Colour_Name));
			//����Ļ��ظ���ֵ���������
		}
		//д��
		Colour_Struct_File_Seve_W(&Colour, Path);
		printf("�������!");
		goto iuns;	//�ص��׽���
		break;
	default:
		printf("������ȷ�ļ�����!");
		goto ins;
		break;
	}
	return COLOUR_NORMAL;	//����ִ������
}
//ͼ��ѹ����������ʹ�ô˺�����ѹ��ͼ�񣬱��ں��ڴ���
COLOURIN(IplImage*) Colour_Compress(IplImage *Image/*ͼ��*/, int Compress_Mode = COLOUR_COMPRESS_MODE_JPEG){
	IplImage *T_Image;
	switch (Compress_Mode){
	case COLOUR_COMPRESS_MODE_JPEG:
		//��ʽ��
		cvSaveImage("image.jpg",Image);
		//ʵ����
		T_Image = cvLoadImage("image.jpg");
		//ɾ��
		//system("del image.jpeg");
		//����
		return T_Image;
		break;
	}
}

//demo
//��ɫ������
char colour_state[] = {"{COLOUR:��ɫ&MAX:255,255,255&MIN:255,255,255&}{COLOUR:��ɫ&MAX:0,0,0&MIN:0,0,0&}{COLOUR:��ɫ&MAX:195,195,195&MIN:120,120,120&}{COLOUR:����ɫ&MAX:110,140,142&MIN:100,130,132&}{COLOUR:ǳ��ɫ&MAX:0,162,232&MIN:0,152,222&}{COLOUR:�ֺ�ɫ&MAX:154,76,115&MIN:144,70,105&}"
						"{COLOUR:��ɫ&MAX:184,123,95&MIN:174,113,85&}{COLOUR:���ɫ&MAX:136,20,21&MIN:126,0,11&}{COLOUR:��ɫ&MAX:255,28,36&MIN:150,0,0&}{COLOUR:��ɫ&MAX:0,255,0&MIN:0,255,0&}"
						"{COLOUR:��ɫ&MAX:255,255,0&MIN:240,240,0&}{COLOUR:��ɫ&MAX:255,127,39&MIN:245,117,29&}{COLOUR:��ɫ&MAX:34,177,76&MIN:24,167,66&}{COLOUR:ε��ɫ&MAX:0,162,255&MIN:0,152,245&}{COLOUR:��ɫ&MAX:0,0,255&MIN:0,0,1&}"
						"{COLOUR:����ɫ&MAX:63,72,204&MIN:53,62,194&}{COLOUR:��ɫ&MAX:163,73,164&MIN:153,63,154&}{COLOUR:�ۺ�ɫ&MAX:255,174,201&MIN:245,164,1191&}{COLOUR:�ۺ�ɫ&MAX:255,174,201&MIN:245,164,119&}{COLOUR:��ɫ&MAX:255,201,14&MIN:245,191,4&}{COLOUR:ǳ��ɫ&MAX:239,228,176&MIN:229,218,166&}"
						"{COLOUR:���ɫ&MAX:181,230,29&MIN:171,220,19&}{COLOUR:ǳ����ɫ&MAX:153,217,234&MIN:143,207,224&}{COLOUR:����ɫ&MAX:112,146,190&MIN:102,136,180&}{COLOUR:����ɫ&MAX:200,191,231&MIN:190,181,231&}#" }; 
//������©�������ɫ������
char colour_state1[] = { "{COLOUR:��ɫ&MAX:0,0,0&MIN:0,0,0&}#" };
//Ҫʶ���ͼ����
char colour_img[] = "d:\\features.jpg";
//�ȶ�_����ȶԷ�
void routine(){
	//����һ����ɫ�����ṹ��
	Colour *sd = NULL;
	//��ʼ����ɫ�����ṹ��
	Colour_Features_Create_Initialization(&sd);
	//���л���ɫ�����ṹ��
	Colour_Serialize(&sd, colour_state);
	//����һ����ɫ��
	Colour_Form *colo = NULL;
	//��ʼ����ɫ��
	Colour_Form_Features_Create_Initialization(&colo);
	//��Ҫʶ���ͼ���ļ����ص��ڴ�
	IplImage *img = cvLoadImage(colour_img);
	if (img == NULL){
		printf("�޷���ͼ���ļ�");
		getchar();
		return;
	}
	//ʶ����ɫ
	colo = Colour_Distinguish_Simple(&sd, img);
	//��ӡʶ�𵽵���ɫ
	printf("ʶ����ɣ���ȡ����%d����ɫ\n", Colour_Form_Get_Size(&colo));
	for (int i = 0; i < Colour_Form_Get_Size(&colo); ++i){
		printf("��%d����ɫ��%s\n", i + 1, Colour_Form_Get_Name(&colo, i+1/*�±��1��ʼ*/));
	}
	//��ȡ���һ��ִ�д���ĺ����Լ���������,����ӡ
	Get_Colour_Error_State_Print();
	Colour_Delete(&sd);	//�ͷ���ɫ������
	Colour_Form_Delete(&colo);	//�ͷ���ɫ��
	Colour_State_Dlete_Arr(colour_state);	//�ͷ���ɫ������_�ڴ�����

}

//�ȶ�_���űȶԷ�
void routine_zoom(){
	//����һ����ɫ�����ṹ��
	Colour *sd = NULL;
	//��ʼ����ɫ�����ṹ��
	Colour_Features_Create_Initialization(&sd);
	//���л���ɫ�����ṹ��
	Colour_Serialize(&sd, colour_state);
	//����һ����ɫ��
	Colour_Form *colo = NULL;
	//��ʼ����ɫ��
	Colour_Form_Features_Create_Initialization(&colo);
	//��Ҫʶ���ͼ���ļ����ص��ڴ�
	IplImage *img = cvLoadImage(colour_img);
	//ʶ����ɫ
	Colour_Distinguish_Simple_Zoom(&sd, img);
	//��ӡʶ�𵽵���ɫ
	printf("ʶ����ɣ���ȡ����%d����ɫ\n", Colour_Form_Get_Size(&colo));
	for (int i = 0; i < Colour_Form_Get_Size(&colo); ++i){
		printf("��%d����ɫ��%s\n", i + 1, Colour_Form_Get_Name(&colo, i + 1/*�±��1��ʼ*/));
	}
	//��ȡ���һ��ִ�д���ĺ����Լ���������,����ӡ
	Get_Colour_Error_State_Print();
	Colour_Delete(&sd);	//�ͷ���ɫ������
	Colour_Form_Delete(&colo);	//�ͷ���ɫ��
	Colour_State_Dlete_Arr(colour_state);	//�ͷ���ɫ������_�ڴ�����
}

//�ȶ�_��ȡ��ɫ�����X��Y
void routine_X_Y(){
	//����һ����ɫ�����ṹ��
	Colour *sd = NULL;
	//��ʼ����ɫ�����ṹ��
	Colour_Features_Create_Initialization(&sd);
	//���л���ɫ�����ṹ��
	Colour_Serialize(&sd, colour_state);
	//����һ����ɫ��
	Colour_Form *colo = NULL;
	//��ʼ����ɫ��
	Colour_Form_Features_Create_Initialization(&colo);
	//��Ҫʶ���ͼ���ļ����ص��ڴ�
	IplImage *img = cvLoadImage("d:\\test.jpg");
	if (img == NULL){
		printf("�޷���ͼ���ļ�");
		getchar();
		return;
	}
	//ʶ����ɫ
	colo = Colour_Distinguish_Simple(&sd, img);
	//����һ������ṹ��
	Colour_Form_X_Y *_X_Y = NULL;
	//��ӡʶ�𵽵���ɫ
	printf("ʶ����ɣ���ȡ����%d����ɫ\n", Colour_Form_Get_Size(&colo));
	_X_Y = Colour_Form_Get_X_Y(&colo,  2/*�±��1��ʼ*/);
	for (int i = 0; i < Colour_Form_Get_Size(&colo); ++i){
		_X_Y = Colour_Form_Get_X_Y(&colo, i + 1/*�±��1��ʼ*/);
		printf("��%d����ɫ��%s\nX����Ϊ:%d\nY����Ϊ:%d\n", i + 1, Colour_Form_Get_Name(&colo, i + 1/*�±��1��ʼ*/), _X_Y->X, _X_Y->Y);
	}
	//��ȡ���һ��ִ�д���ĺ����Լ���������,����ӡ
	Get_Colour_Error_State_Print();
	getchar();
	Colour_Delete(&sd);	//�ͷ���ɫ������
	Colour_Form_Delete(&colo);	//�ͷ���ɫ��
	Colour_State_Dlete_Arr(colour_state);	//�ͷ���ɫ������_�ڴ�����
}

//�ȶ�_����ȶԷ�_ģ����Ե
void routine_vague(){
	//����һ����ɫ�����ṹ��
	Colour *sd = NULL;
	//��ʼ����ɫ�����ṹ��
	Colour_Features_Create_Initialization(&sd);
	//���л���ɫ�����ṹ��
	Colour_Serialize(&sd, colour_state);
	//����һ����ɫ��
	Colour_Form *colo = NULL;
	//��ʼ����ɫ��
	Colour_Form_Features_Create_Initialization(&colo);
	//��Ҫʶ���ͼ���ļ����ص��ڴ�
	IplImage *img = cvLoadImage("d:\\tesy.jpg");
	if (img == NULL){
		printf("�޷���ͼ���ļ�");
		getchar();
		return;
	}
	//ʶ����ɫ
	colo = Colour_Distinguish_Simple_Vague(&sd, img);
	//��ӡʶ�𵽵���ɫ
	printf("ʶ����ɣ���ȡ����%d����ɫ\n", Colour_Form_Get_Size(&colo));
	for (int i = 0; i < Colour_Form_Get_Size(&colo); ++i){
		printf("��%d����ɫ��%s\n", i + 1, Colour_Form_Get_Name(&colo, i + 1/*�±��1��ʼ*/));
	}
	//��ȡ���һ��ִ�д���ĺ����Լ���������,����ӡ
	Get_Colour_Error_State_Print();
	getchar();
	Colour_Delete(&sd);	//�ͷ���ɫ������
	Colour_Form_Delete(&colo);	//�ͷ���ɫ��
	Colour_State_Dlete_Arr(colour_state);	//�ͷ���ɫ������_�ڴ�����

}

//�ȶ�_ʮ�ַ�ˮ���㷨
void corss(){
	//����һ����ɫ�����ṹ��
	Colour *sd = NULL;
	//��ʼ����ɫ�����ṹ��
	Colour_Features_Create_Initialization(&sd);
	//���л���ɫ�����ṹ��
	Colour_Serialize(&sd, colour_state);
	//����һ����ɫ��
	Colour_Form *colo = NULL;
	//��ʼ����ɫ��
	Colour_Form_Features_Create_Initialization(&colo);
	//��Ҫʶ���ͼ���ļ����ص��ڴ�
	IplImage *img = cvLoadImage("d:\\test.jpg");
	if (img == NULL){
		printf("�޷���ͼ���ļ�");
		getchar();
		return;
	}
	//ʶ����ɫ
	colo = Colour_Distinguish_Simple_Cross_Watershed(&sd, img,100);
	//��ӡʶ�𵽵���ɫ
	printf("ʶ����ɣ���ȡ����%d����ɫ\n", Colour_Form_Get_Size(&colo));
	for (int i = 0; i < Colour_Form_Get_Size(&colo); ++i){
		printf("��%d����ɫ��%s\n", i + 1, Colour_Form_Get_Name(&colo, i + 1/*�±��1��ʼ*/));
	}
	//��ȡ���һ��ִ�д���ĺ����Լ���������,����ӡ
	Get_Colour_Error_State_Print();

}

//��ȡָ�����ص����ɫ
void get_pos(){
	//����һ����ɫ�����ṹ��
	Colour *sd = NULL;
	//��ʼ����ɫ�����ṹ��
	Colour_Features_Create_Initialization(&sd);
	//���л���ɫ�����ṹ��
	Colour_Serialize(&sd, colour_state);
	//����һ����ɫ��
	Colour_Form *colo = NULL;
	//��ʼ����ɫ��
	Colour_Form_Features_Create_Initialization(&colo);
	//��Ҫʶ���ͼ���ļ����ص��ڴ�
	IplImage *img = cvLoadImage("d:\\test.jpg");
	if (img == NULL){
		printf("�޷���ͼ���ļ�");
		getchar();
		return;
	}
	//ʶ����ɫ
	colo = Colour_Retrieval_Position(&sd, img,0,0);
	//��ӡʶ�𵽵���ɫ
	printf("ʶ����ɣ���ȡ����%d����ɫ\n", Colour_Form_Get_Size(&colo));
	for (int i = 0; i < Colour_Form_Get_Size(&colo); ++i){
		printf("��%d����ɫ��%s\n", i + 1, Colour_Form_Get_Name(&colo, i + 1/*�±��1��ʼ*/));
	}
	//��ȡ���һ��ִ�д���ĺ����Լ���������,����ӡ
	Get_Colour_Error_State_Print();

}

//������ɫ�����⵽�ļ�_���ڴ����ݵķ�ʽ
void file_seve(){
	Colour_State_File_Seve(colour_state, "d:\\1.colour");
	//��ȡ���һ��ִ�д���ĺ����Լ���������,����ӡ
	Get_Colour_Error_State_Print();
	getchar();
	Colour_State_Dlete_Arr(colour_state);	//�ͷ���ɫ������_�ڴ�����
}

//������ɫ�����⵽�ļ��������л��ķ�ʽ_���ӵķ�ʽ
void file_seve_(){
	//����һ����ɫ�����ṹ��
	Colour *sd = NULL;
	//��ʼ����ɫ�����ṹ��
	Colour_Features_Create_Initialization(&sd);
	//���л���ɫ�����ṹ��
	Colour_Serialize(&sd, colour_state);
	//д��
	Colour_Struct_File_Seve(&sd, "d:\\1.colour");
	//��ȡ���һ��ִ�д���ĺ����Լ���������,����ӡ
	Get_Colour_Error_State_Print();
	getchar();
	Colour_Delete(&sd);	//�ͷ���ɫ������
	Colour_State_Dlete(colour_state);	//�ͷ���ɫ������_�ڴ�����
}

//������ɫ�����⵽�ļ��������л��ķ�ʽ_���ļ��ķ�ʽ
void file_seve_1(){
	//����һ����ɫ�����ṹ��
	Colour *sd = NULL;
	//��ʼ����ɫ�����ṹ��
	Colour_Features_Create_Initialization(&sd);
	//���л���ɫ�����ṹ��
	Colour_Serialize(&sd, colour_state);
	//д��
	Colour_Struct_File_Seve_W(&sd, "d:\\1.colour");
	//��ȡ���һ��ִ�д���ĺ����Լ���������,����ӡ
	Get_Colour_Error_State_Print();
	getchar();
	Colour_Delete(&sd);	//�ͷ���ɫ������
	Colour_State_Dlete(colour_state);	//�ͷ���ɫ������_�ڴ�����
}

//��ȡ��ɫ������ṹ��Ĵ�С
void colour_get(){
	//����һ����ɫ�����ṹ��
	Colour *sd = NULL;
	//��ʼ����ɫ�����ṹ��
	Colour_Features_Create_Initialization(&sd);
	//���л���ɫ�����ṹ��
	Colour_Serialize(&sd, colour_state);
	//��ȡ��С
	printf("��ɫ������Ĵ�СΪ��%d\n", Colour_Get_Struct_Size(&sd));
	//��ȡ���һ��ִ�д���ĺ����Լ���������,����ӡ
	Get_Colour_Error_State_Print();
	getchar();
	Colour_Delete(&sd);	//�ͷ���ɫ������
	Colour_State_Dlete(colour_state);	//�ͷ���ɫ������_�ڴ�����
}

//��ȡ��ɫ�������ļ���С
void colour_file_get(){
	//�Ƚ���ɫ��������ؽ���
	char *colour_state5 = Colour_State_Memory("d:\\1.colour");
	//��ȡ��С
	printf("��ɫ�������ļ���С(���ֽڣ���ɫ�ṹ����)��%d", Colour_Get_State_Size(colour_state5));
	//��ȡ���һ��ִ�д���ĺ����Լ���������,����ӡ
	Get_Colour_Error_State_Print();
	Colour_State_Dlete(colour_state5);	//�ͷ���ɫ������_�ڴ�����
}

//ͼ��©���⣬��׼ģʽ
void img_(){
	IplImage *img = cvLoadImage("d:\\test.jpg");
	if (img == NULL){
		printf("�޷���ͼ���ļ�");
		getchar();
		return;
	}
	//����һ����ɫ�����ṹ��
	Colour *sd = NULL;
	//��ʼ����ɫ�����ṹ��
	Colour_Features_Create_Initialization(&sd);
	//���л���ɫ�����ṹ��
	Colour_Serialize(&sd, colour_state1);
	//����һ����ɫ��
	Colour_Form *colo = NULL;
	//��ʼ����ɫ��
	Colour_Form_Features_Create_Initialization(&colo);
	//ʶ����ɫ
	for (int i = 0; i <= img->height - 1; ++i){
		for (int j = 0; j <= img->width - 1; ++j){
			colo = Colour_Retrieval_Position(&sd, img, j, i);
			if (Colour_Form_Get_Size(&colo) == 0){
				printf("ͼ���еĵ�%d�У���%d����ɫ©��\n", i, j);
			}
		}

	}
	printf("��ͼ����û��©���");
}

//ͼ��©����_����ģʽ
void img(){
	IplImage *img = cvLoadImage("d:\\test.jpg");
	if (img == NULL){
		printf("�޷���ͼ���ļ�");
		getchar();
		return;
	}
	//����һ����ɫ�����ṹ��
	Colour *sd = NULL;
	//��ʼ����ɫ�����ṹ��
	Colour_Features_Create_Initialization(&sd);
	//���л���ɫ�����ṹ��
	Colour_Serialize(&sd, colour_state1);
	//����һ����ɫ��
	Colour_Form *colo = NULL;
	//��ʼ����ɫ��
	Colour_Form_Features_Create_Initialization(&colo);
	IplImage *Image_Zoom = Colour_Image_Zoom(img);	//����
	//ʶ����ɫ
	for (int i = 0; i <= Image_Zoom->height - 1; ++i){
		for (int j = 0; j <= Image_Zoom->width - 1; ++j){
			colo = Colour_Retrieval_Position(&sd, Image_Zoom, j, i);
			if (Colour_Form_Get_Size(&colo) == 0){
				printf("ͼ���еĵ�%d�У���%d����ɫ©��\n", i, j);
			}
		}

	}
	printf("��ͼ����û��©���");
}
#include "stdlib.h"
#include "time.h"
int main(){
	printf("���»س���ʼʶ����ɫ");
	getchar();
	long i = 10000000L; clock_t start, finish; double Total_time;; start = clock(); while (i--);
	routine();
	finish = clock();
	Total_time = (double)(finish - start) / CLOCKS_PER_SEC;
	printf("\n��������ʱ�䣺%0.3f���� \n", Total_time); 
	getchar();
	return 0;
	/*
	char a, b;
	char c;
	a = -10;
	b = -10;
	c = a + b;
	printf("%d", c);
	getchar();
	*/
	//©����
	//img();

	//����ȶԷ�
	//routine();
	//BLOCK_2_GPIOB_CRL = 2;    //??GPIO_B???????  
	/*
	int num = 0;
	printf("%d", ++num + num++ + --num + num--);
	getchar();
	*/
	
	//���űȶԷ�
	//routine_zoom();
	/*
	float a = 22.0;
	float b = 23.0;
	if (a >= b){
		printf("%lf", fabs(a));
	}
	getchar();
	*/
	//����ȶ�_ģ����Ե��
	//routine_vague();


	//��ȡ��ɫ�������С
	//colour_get();
	
	//��ȡָ����ɫ���ص�
	//get_pos();

	//ѹ��ͼ��+��Ե�ȶԷ�
	//compress();
	
	//��ȡ��ɫ�������ļ���С(��ɫ�ṹ����)
	//colour_file_get()
	
	//ʮ�ַ�ˮ��ȶ��㷨
	//corss();

	
	//��ȡĳ����ɫλ��ͼ���еģ�XY����
	//routine_X_Y();
	

	
	//����һ����ɫ�����⵽�����ļ������ڴ����ݵķ�ʽ
	//file_seve();
	

	
	//������ɫ�����⵽�ļ��������л��ķ�ʽ_���ӵķ�ʽ
	//file_seve_();
	

	
	//������ɫ�����⵽�ļ��������л��ķ�ʽ_���ļ��ķ�ʽ
	//file_seve_1();
	

}