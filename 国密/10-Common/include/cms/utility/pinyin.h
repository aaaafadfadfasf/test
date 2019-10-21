#pragma once

/**
*	将Utf8字符串转换为拼音的算法函数.
*	+by lzx@2017-01-11
*/

#include <string>

// 将一个Utf8字符串转换为拼音搜索字符串
// 格式是 原文#拼音首字母1#拼音首字母2 ...
// 其中 # 表示分隔符, 使用ASIC码 7
// 多个拼音首字母是因为可能存在多音字, 多音字的排列组合.
// 原文中非中文字符,转换后还是原字符
// +by lzx@2017-01-11
std::string Utf8ToPinyin( const std::string& utf8Str, char splitToken = 7 );
