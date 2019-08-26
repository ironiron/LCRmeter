/*
 * SSD1306.hpp
 *
 *  Created on: 22.08.2019
 *      Author: Rafa³
 */

#ifndef SSD1306_HPP_
#define SSD1306_HPP_

//TODO send char,  sleep, rotate screen, invert colors,
//inverted string colors, support for 2 addresses, scroll if supported by chip itself, image

//TODO diacritic signs

#include <string>
#include <array>
#include "I2C.hpp"


class SSD1306
{
public:
  SSD1306 (const uint8_t screen_height,I2C &connection_port,uint8_t device_address=0x78):
    height(screen_height),  conn(connection_port),address(device_address)
  {
    //TODO sanity check

  }
  enum Color {BLACK=0, WHITE=0xff};

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
  void Set_Cursor(uint8_t x, uint8_t y);
//  void Invert_Colors(void);
  void Draw_Image(uint8_t image);
  //void Rotate_screen()
  //invert screen or just rotate + degree?


private:
  void delay(uint32_t miliseconds);
  void Write_Command(uint8_t com);

  void Reset(void);
  void Write_Char(char str);
  const uint8_t height;
  const uint8_t width=127;
  const static uint32_t buffer_size=32/8*128;
  std::array<uint8_t, buffer_size> buffer;
  I2C &conn;
  const uint8_t address;
  const uint8_t control_b_command=0x00;
  const uint8_t control_b_data=0x40;

  struct
  {
    uint8_t X;
    uint8_t Y;
  }Coordinates;

  void Write_Data(std::array<uint8_t, buffer_size>  &data);
  //uint8_t buffer[64/8*128];//TODO consider using dynamic allocation
};

#endif /* SSD1306_HPP_ */
