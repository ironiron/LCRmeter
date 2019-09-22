/*
 * image.hpp
 *
 *  Created on: 18.09.2019
 *      Author: Rafa�
 */

#ifndef IMAGE_HPP_
#define IMAGE_HPP_

namespace Tables
{

  const static uint8_t sandals[1023] =
    { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xfb, 0xfe, 0xff, 0xff, 0xff, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xe3, 0xfe, 0xff, 0xfe, 0x7e, 0x10, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xc3,
	0xfe, 0xff, 0xff, 0x3c, 0xdf, 0x9f, 0xff, 0xff, 0xff, 0xff, 0x17, 0xfe,
	0xff, 0xff, 0xbd, 0xdf, 0xcf, 0xff, 0xff, 0xff, 0xfe, 0x67, 0xfe, 0xff,
	0xff, 0xdb, 0xdf, 0xef, 0xff, 0xff, 0xff, 0xf9, 0xef, 0xfe, 0xff, 0xff,
	0xd3, 0xdf, 0xe7, 0xff, 0xff, 0xff, 0xf3, 0xcf, 0xfe, 0xff, 0xff, 0xef,
	0xdf, 0xf7, 0xff, 0xff, 0xff, 0xc7, 0xdf, 0xfe, 0xff, 0xff, 0xc7, 0xdf,
	0xf7, 0xff, 0xff, 0xfe, 0x27, 0x9f, 0xfe, 0xff, 0xff, 0xb7, 0xdf, 0xf7,
	0xff, 0xfe, 0xf8, 0xcf, 0xbf, 0xfe, 0xff, 0xfe, 0x7b, 0xdf, 0x87, 0xff,
	0xfe, 0x67, 0xdf, 0x7f, 0xfe, 0xff, 0xfc, 0xf9, 0xde, 0x3f, 0xff, 0xfe,
	0x4f, 0xbe, 0x7f, 0xfe, 0xff, 0xf3, 0xfc, 0x80, 0xff, 0xff, 0xfe, 0x3f,
	0x3e, 0xff, 0xfe, 0xff, 0xe7, 0xfe, 0xff, 0xff, 0xff, 0xfd, 0x7f, 0x7e,
	0xff, 0xfe, 0xff, 0xcf, 0xff, 0x7f, 0xff, 0xff, 0xf9, 0xff, 0x7e, 0xff,
	0xfe, 0xff, 0xff, 0xff, 0xbf, 0xff, 0xff, 0xfb, 0xfe, 0x7d, 0xff, 0xfe,
	0xff, 0xff, 0xff, 0xcf, 0xff, 0xff, 0xf7, 0xfe, 0xf9, 0xff, 0xfe, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xfe, 0xfb, 0xff, 0xfe, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xef, 0xfe, 0xfb, 0xff, 0xfe, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xcf, 0xfe, 0xf3, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0x9f, 0xfe, 0xf7, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0x3f, 0xfe, 0xe7, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe,
	0x7f, 0xfd, 0xef, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff,
	0xf9, 0xcf, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xff, 0xf3,
	0xdf, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0x87, 0xff, 0xf8, 0x1f,
	0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 0xcf, 0xff,
	0xfe, 0xff, 0xff, 0xff, 0xff, 0xc7, 0xff, 0xff, 0xff, 0xbf, 0xff, 0xfe,
	0xff, 0xff, 0xff, 0xfe, 0x3d, 0x3f, 0xff, 0xff, 0xbb, 0xff, 0xfe, 0xff,
	0xff, 0xff, 0xfc, 0xfe, 0x7f, 0xff, 0xff, 0xb3, 0xff, 0xfe, 0xff, 0xff,
	0xff, 0xf3, 0xc0, 0xff, 0xff, 0xff, 0x03, 0xff, 0xfe, 0xff, 0xff, 0xff,
	0x8f, 0xfc, 0xff, 0xff, 0xfe, 0x3f, 0xff, 0xfe, 0xff, 0xff, 0xfc, 0x3f,
	0xff, 0xff, 0xff, 0xff, 0x7f, 0xff, 0xfe, 0xff, 0xff, 0xf1, 0xff, 0xff,
	0xff, 0x80, 0x00, 0x7f, 0xff, 0xfe, 0xfe, 0xfe, 0x07, 0xff, 0xe0, 0x00,
	0x3f, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xcf, 0xff, 0xfc, 0x1f, 0xc0,
	0x1f, 0xff, 0xff, 0xfe, 0xff, 0xff, 0x80, 0x00, 0x03, 0xff, 0xff, 0xc0,
	0x7f, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x87,
	0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfb, 0xff,
	0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xfe,
	0xff, 0xff, 0xff, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff,
	0xfe, 0x0e, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, };

}
#endif /* IMAGE_HPP_ */