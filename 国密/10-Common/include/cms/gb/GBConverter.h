/*
 *去国标化方案的接口实现类。
  *本代码提供给用户一个名字空间和一个已经实现了的接口类，
  *可以直接使用类提供的接口来实现在xml文档中，
  *国标和科达自定义关键字，之间的相互转化。
  *依赖 tinyxml2 库。
  *2018-5-28 v1.0  author:lijianguo@kedacon.com
*/
#pragma once
namespace GBConverter {
#define out 
#define in
#define API
enum ErrorGBConverter {
	/*操作成功 无错误*/
	GBCONVERTER_SUCCESS = 0, 

	/*请检查xml文档是否规范*/
	GBCONVERTER_XMLPARSEFAIL, 

	/*传入的指针为空*/
	GBCONVERTER_XMLCONTENTEMPTER,

	/*内部将xml树转化成xml字符串的时候出错*/
	GBCONVERTER_XMLTOSTRINGERROR,

	/*传入的xml文档不规范*/
	GBCONVERTER_NOXMLELEMENT,

	/*xml树的深度太大，怕栈被顶破拒绝处理*/
	GBCONVERTER_RECURSIVEMAXDEPTH,

};

 class APIGBConverter
 {
 public:
	 /*
	 *使用默认xml树的递归深度 20
	 */
	 APIGBConverter();

	 /*
	 * 为了保护运行栈的安全可以自定义xml树的递归深度
	 */
	 APIGBConverter(int RecursiveDepth);

	 /*
	 * 将国标关键字转化成科达自定义关键字。
	 * pXmlContent      输入参数，必须是标准的xml格式字符串。
	 * pConvertResult   用于转化后保存结果的指针。
	 * nLen                 转化后的xml字符串长度。
	 * 返回值 请查看     enum ErrorGBConverter  
	 */
	 API  ErrorGBConverter  APIGB2KDConverter(in const char * pXmlContent, out const char ** pConvertResult, out int & nLen);
	
	 /*
	 * 将科达自定义关键字转化成科达国标关键字。
	 * pXmlContent      输入参数，必须是标准的xml格式字符串。
	 * pConvertResult   用于转化后保存结果的指针。
	 * nLen                 转化后的xml字符串长度。
	 * 返回值 请查看     enum ErrorGBConverter  
	 */
	 API  ErrorGBConverter APIKD2GBConverter(in const char * pXmlContent, out const char ** pConvertResult, out int & nLen);
	 
	 ~APIGBConverter();

 private:
	 void* m_pObjConverter;
 };
 

}

