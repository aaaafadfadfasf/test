/* base64.h -- Encode binary data using printable characters.
   Copyright (C) 2004, 2005, 2006 Free Software Foundation, Inc.
   Written by Simon Josefsson.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.  */

#ifndef BASE64_H
# define BASE64_H

/* Get size_t. */
# include <stddef.h>

/* Get bool. */
//# include <stdbool.h>

/* This uses that the expression (n+(k-1))/k means the smallest
   integer >= n/k, i.e., the ceiling of n/k.  */
# define BASE64_LENGTH(inlen) ((((inlen) + 2) / 3) * 4)

extern bool isbase64 (char ch);

extern void base64_encode (const char * in, size_t inlen,
			   char * out, size_t outlen);

extern size_t base64_encode_alloc (const char *in, size_t inlen, char **out);

extern bool base64_decode (const char * in, size_t inlen,
			   char * out, size_t *outlen);

extern bool base64_decode_alloc (const char *in, size_t inlen,
				 char **out, size_t *outlen);


/*****************************************************************************
   模块名      : cms2.0
   文件名      : base64codec.h
   相关文件    : 
   文件实现功能: base64编解码，接口封装，便于上层使用
   作者        : 
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人				修改内容
   2012/12/19   1.0        fanxg			      创建
******************************************************************************/

#include <string>
using std::string;

string Base64Encode(const string& strBuf);
string Base64Encode(const char* szBuf, size_t nBufLen);

string Base64Decode(const string& strBase64);
string Base64Decode(const char* szBase64, size_t nBase64);

#endif /* BASE64_H */
