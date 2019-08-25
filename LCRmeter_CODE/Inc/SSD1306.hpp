/*
 * SSD1306.hpp
 *
 *  Created on: 22.08.2019
 *      Author: Rafa³
 */

#ifndef SSD1306_HPP_
#define SSD1306_HPP_

//TODO send char, draw pixel, set contrast, initialize, sleep, fill screen, set cursor, rotate screen, invert colors,
//inverted string colors, support for 2 addresses, scroll if supported by chip itself, image
//read data from display??
//TODO diacritic signs

#include <string>
#include <array>


class SSD1306
{
public:
  template<typename T>
  SSD1306 (uint8_t screen_height,T c):height(screen_height), con(c)
  {
    //TODO sanity check

  }
  enum Color {WHITE=0, BLACK=0xff};

  void Initialize(void);
  void Fill(Color);
  void Write_String(const std::string &str);//TODO consider making another function for c-style strings
  void Write_String_Inverted(const std::string &str);
  //TODO read about string_view (C++17)
  void Set_Brightness(uint8_t brightness);
  void Draw_Pixel(uint8_t x,uint8_t y, Color c);
  void Display_Off(void);
  void Display_On(void);
  void Set_Invert_Colors (void);//TODO change this, so the rest of functions are similar
  void Set_Normal_Colors (void);
  void Flip_Screen (bool flipped);
  void Mirror_Screen(bool mirrored);
  void Update_Screen(void);
//  void Invert_Colors(void);
  void Draw_Image(uint8_t image);
  //void Rotate_screen()
  //invert screen or just rotate + degree?


private:
  void delay(uint32_t miliseconds);
  void Write_Command(uint8_t com);

  void Reset(void);
  void Write_Char(char c);
  const uint8_t height;
  const static uint32_t buffer_size=64/8*128;
  std::array<uint8_t, buffer_size> buffer;
  template<typename T>
  T con;

  void Write_Data(std::array<uint8_t, buffer_size>  &data);
  //uint8_t buffer[64/8*128];//TODO consider using dynamic allocation
};

#endif /* SSD1306_HPP_ */
