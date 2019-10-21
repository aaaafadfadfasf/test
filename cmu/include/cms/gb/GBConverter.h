/*
 *ȥ���껯�����Ľӿ�ʵ���ࡣ
  *�������ṩ���û�һ�����ֿռ��һ���Ѿ�ʵ���˵Ľӿ��࣬
  *����ֱ��ʹ�����ṩ�Ľӿ���ʵ����xml�ĵ��У�
  *����Ϳƴ��Զ���ؼ��֣�֮����໥ת����
  *���� tinyxml2 �⡣
  *2018-5-28 v1.0  author:lijianguo@kedacon.com
*/
#pragma once
namespace GBConverter {
#define out 
#define in
#define API
enum ErrorGBConverter {
	/*�����ɹ� �޴���*/
	GBCONVERTER_SUCCESS = 0, 

	/*����xml�ĵ��Ƿ�淶*/
	GBCONVERTER_XMLPARSEFAIL, 

	/*�����ָ��Ϊ��*/
	GBCONVERTER_XMLCONTENTEMPTER,

	/*�ڲ���xml��ת����xml�ַ�����ʱ�����*/
	GBCONVERTER_XMLTOSTRINGERROR,

	/*�����xml�ĵ����淶*/
	GBCONVERTER_NOXMLELEMENT,

	/*xml�������̫����ջ�����ƾܾ�����*/
	GBCONVERTER_RECURSIVEMAXDEPTH,

};

 class APIGBConverter
 {
 public:
	 /*
	 *ʹ��Ĭ��xml���ĵݹ���� 20
	 */
	 APIGBConverter();

	 /*
	 * Ϊ�˱�������ջ�İ�ȫ�����Զ���xml���ĵݹ����
	 */
	 APIGBConverter(int RecursiveDepth);

	 /*
	 * ������ؼ���ת���ɿƴ��Զ���ؼ��֡�
	 * pXmlContent      ��������������Ǳ�׼��xml��ʽ�ַ�����
	 * pConvertResult   ����ת���󱣴�����ָ�롣
	 * nLen                 ת�����xml�ַ������ȡ�
	 * ����ֵ ��鿴     enum ErrorGBConverter  
	 */
	 API  ErrorGBConverter  APIGB2KDConverter(in const char * pXmlContent, out const char ** pConvertResult, out int & nLen);
	
	 /*
	 * ���ƴ��Զ���ؼ���ת���ɿƴ����ؼ��֡�
	 * pXmlContent      ��������������Ǳ�׼��xml��ʽ�ַ�����
	 * pConvertResult   ����ת���󱣴�����ָ�롣
	 * nLen                 ת�����xml�ַ������ȡ�
	 * ����ֵ ��鿴     enum ErrorGBConverter  
	 */
	 API  ErrorGBConverter APIKD2GBConverter(in const char * pXmlContent, out const char ** pConvertResult, out int & nLen);
	 
	 ~APIGBConverter();

 private:
	 void* m_pObjConverter;
 };
 

}

