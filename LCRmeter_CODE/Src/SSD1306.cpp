/*
 * SSD1306.cpp
 *
 *  Created on: 22.08.2019
 *      Author: Rafa³
 */

#include <SSD1306.hpp>


void SSD1306::Initialize (void)
{
#if 1
	Display_Off(); //display off//displayoff func
	Write_Command(0xD5); //--set display clock divide ratio/oscillator frequency
	Write_Command(0x80); //--set divide ratio  <default value> //*
	Write_Command(0xA8); //--set multiplex ratio(1 to 64) (display height)
	Write_Command(height-1); // //*
	Write_Command(0xD3); //-set display offset
	Write_Command(0x00); //-no offset
	Write_Command(0x40); //--set start line address
	Write_Command(0x8D); //--set DC-DC enable
	Write_Command(0x14); //	enable charge pump

	//Write_Command(0xA1); //--set segment re-map 0 to 127********************here put rotate screen
	Mirror_Screen(false);

	//Write_Command(0xC8); //Set COM Output Scan Direction // flipping screen C8-C0
	Flip_Screen(false);

	Write_Command(0xDA); //--set com pins hardware configuration
	Write_Command(hard_conf);

//	Write_Command(0x81); //--set contrast control register
//	Write_Command(0x8F);	//*
	Set_Brightness(150);

	Write_Command(0xD9); //--set pre-charge period
	Write_Command(0x22); //not f1?
	Write_Command(0xDB); //--set vcomh
	Write_Command(0x40); //0x20,0.77xVcc
	Write_Command(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	//Write_Command(0xA6); //--set normal display
	Set_Normal_Colors();

#else
	Write_Command(0xAE); //display off
	Write_Command(0xD5); //--set display clock divide ratio/oscillator frequency
	Write_Command(0xF0); //--set divide ratio
	Write_Command(0xA8); //--set multiplex ratio(1 to 64)
	Write_Command(0x3F); //
	Write_Command(0xD3); //-set display offset
	Write_Command(0x00); //-not offset
	Write_Command(0x40); //--set start line address
	Write_Command(0x8D); //--set DC-DC enable
	Write_Command(0x14); //
	Write_Command(0xA1); //--set segment re-map 0 to 127
	Write_Command(0xC8); //Set COM Output Scan Direction
	Write_Command(0xDA); //--set com pins hardware configuration
	Write_Command(0x12);
	Write_Command(0x81); //--set contrast control register
	Write_Command(0xFF);
	Write_Command(0xD9); //--set pre-charge period
	Write_Command(0x22); //not f1?
	Write_Command(0xDB); //--set vcomh
	Write_Command(0x40); //0x20,0.77xVcc
	Write_Command(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	Display_On; //--set normal display
#endif





	Write_Command(0x20); //Set Memory Addressing Mode
	Write_Command(0x00); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
//	Write_Command(0xB0); //Set Page Start Address for Page Addressing Mode,0-7
//
//	Write_Command(0x00); //---set low column address
//	Write_Command(0x10); //---set high column address
	Write_Command(0x21); //Column address
	Write_Command(0x00);
	Write_Command(127);

	Write_Command(0x22); //Page address
	Write_Command(0x00);
	Write_Command((height/8)-1);

//	Write_Command(0xAF); //--turn on SSD1306 panel


	Display_On();
	Clean();
	Update_Screen();

	if(last_error==0)
	  {
	    isinitialized=true;
	  }
	else
	  {
	    isinitialized=false;
	  }


}

void SSD1306::Clean(void)
{
  Fill(BLACK);
}

void SSD1306::Update_Screen(void)
{
	Write_Command(0x21); //Column address
	Write_Command(0x00);
	Write_Command(127);

	Write_Command(0x22); //Page address
	Write_Command(0x00);
	Write_Command((height/8)-1);

	Write_Data(buffer);
}

void SSD1306::Fill (SSD1306::Color c)
{
  if(c==Color::BLACK)
    {
      for (auto& b : buffer)
      {
          b=Color::BLACK;
      }
    }
  else
    {
      for (auto& b : buffer)
      {
          b=Color::WHITE;
      }
    }


}
//#define ver2
void SSD1306::Write_String (const std::string& str)
{
#ifndef ver2
	// Write until null-byte
  int i=0;
	while (str[i])
	{
	    Write_Char(str[i],Color::WHITE);
//		if (Write_Char(str[i]) != str[i])
//		{
//			// Char could not be written
//			//return str;
//		}
		i++;
	}
	//return 0;
#else
  // Write until null-byte
  int k = 0;
  uint32_t i, b, j;
  while (str[k])
    {
      // Use the font to write
      for (i = 0; i < font.FontHeight; i++)
	{
	  b = font.data[(str[k] - 32) * font.FontHeight + i];
	  for (j = 0; j < font.FontWidth; j++)
	    {
	      if ((b << j) & 0x8000)
		{
		  Draw_Pixel (Coordinates.X + j, (Coordinates.Y + i), WHITE);
		}
	      else
		{
		  Draw_Pixel (Coordinates.X + j, (Coordinates.Y + i), BLACK);
		}
	    }
	}

      Coordinates.X += font.FontWidth;
      k++;
    }

#endif
}

void SSD1306::Set_Font_size (Fonts::FontDef font)
{
  this->font=font;
}

void SSD1306::Write_Char (char str,SSD1306::Color color)
{
#ifndef ver2

#if 0
  uint32_t i, b, j;
  FontDef Font=Font_7x10;
  // Use the font to write
  for (i = 0; i < Font.FontHeight; i++)
    {
      b = Font.data[(str - 32) * Font.FontHeight + i];
      for (j = 0; j < Font.FontWidth; j++)
	{
		if ((b << j) & 0x8000)
		{
		    Draw_Pixel (Coordinates.X + j, (Coordinates.Y + i),WHITE);
		}
		else
		{
		    Draw_Pixel (Coordinates.X + j, (Coordinates.Y + i),BLACK);
		}
	}
    }
  // The current space is now taken
//  if(Coordinates.X+Font.FontWidth >=width)
//    {
//      Coordinates.X=0;
//      Coordinates.Y++;
//    }
//  else
//    {
//      Coordinates.X += Font.FontWidth;
//    }
  Coordinates.X += Font.FontWidth;

  // Return written char for validation
//  return ch;
#else
  if (color == Color::BLACK)
    {
      //str = ~str;
    }
  // Write until null-byte
  uint32_t i, b, j;

  // Use the font to write
  for (i = 0; i < font.FontHeight; i++)
    {
      b = font.data[(str - 32) * font.FontHeight + i];
      for (j = 0; j < font.FontWidth; j++)
	{
	  if (color == Color::BLACK)
	    {
	      if ((b << j) & 0x8000)
		{
		  Draw_Pixel (Coordinates.X + j, (Coordinates.Y + i), BLACK);
		}
	      else
		{
		  Draw_Pixel (Coordinates.X + j, (Coordinates.Y + i), WHITE);
		}
	    }
	  else
	    {
	      if ((b << j) & 0x8000)
		{
		  Draw_Pixel (Coordinates.X + j, (Coordinates.Y + i), WHITE);
		}
	      else
		{
		  Draw_Pixel (Coordinates.X + j, (Coordinates.Y + i), BLACK);
		}
	    }
	}
    }
  Coordinates.X += font.FontWidth;



#endif


#else


#endif
}

void SSD1306::Write_String_Inverted (const std::string& str)
{

	// Write until null-byte
int i=0;
	while (str[i])
	{
	    Write_Char(str[i],Color::BLACK);
//		if (Write_Char(str[i]) != str[i])
//		{
//			// Char could not be written
//			//return str;
//		}
		i++;
	}
	//return 0;
}

void SSD1306::Set_Brightness (uint8_t brightness)
{
	Write_Command(0x81);
	Write_Command(brightness);
}

void SSD1306::Draw_Pixel (uint8_t x, uint8_t y, SSD1306::Color c)
{
	if (x >= width || y >= height)
	{
		// Don't write outside the buffer
		return;
	}

	if (c == WHITE)
	{
	    buffer[x+width*(y/8)] |= (1 << (y % 8));
	}
	else
	{
	    buffer[x+width*(y/8)] &=~ (1 << (y % 8));
	}
}

void SSD1306::Display_Off (void)
{
  Write_Command (0xAE);
}

void SSD1306::Display_On (void)
{
  Write_Command(0xAF);
}

void SSD1306::Flip_Screen (bool flipped)
{
  if (flipped == 0)
    {
      Write_Command (0xC8); //Set COM Output Scan Direction
    }
  else
    {
      Write_Command (0xC0);
    }
}

void SSD1306::Set_Invert_Colors (void)
{
  Write_Command(0xA7);//normal colours
}

void SSD1306::Set_Normal_Colors (void)
{
  Write_Command(0xA6);//normal colours
}

void SSD1306::Draw_Image (uint8_t image)
{
}

void SSD1306::Set_Cursor(uint8_t x, uint8_t y)
{
  if(x>=width)
    {
      x=width;
    }
  if(y>=height)
    {
      y=height;
    }
Coordinates.X=x;
Coordinates.Y=y;
}

void SSD1306::Mirror_Screen (bool mirrored)
{
  if(mirrored == 0)
    {
  Write_Command(0xA1); //--set segment re-map 0 to 127********************here put rotate screen
    }
  else
    {
      Write_Command(0xA0); //--set segment re-map 0 to 127********************here put rotate screen
    }
}


bool SSD1306::IsInitialized (void)
{
  return isinitialized;
}
