/*
 * LLutils.h
 *
 *  Created on: Jun 21, 2025
 *      Author: xDDD
 */

#ifndef LLUTILS_H_
#define LLUTILS_H_

#include <map>
#include <unordered_map>
#include <stdint.h>
#include "sine.hpp"

//todo maybe create those out of template?

enum class sine_samples_t : unsigned int
{
 SINE_400_SAMPELS=0,
 SINE_200_SAMPELS,
 SINE_80_SAMPELS,
 SINE_40_SAMPELS,
};

static  inline const std::unordered_map<sine_samples_t,const uint16_t*> sine_option = {
        { sine_samples_t::SINE_400_SAMPELS, sine_table_400s_12bit.data() },
        { sine_samples_t::SINE_200_SAMPELS, sine_table_200samples_12bit.data()  },
        { sine_samples_t::SINE_80_SAMPELS, sine_table_80samples_12bit.data()  },
        { sine_samples_t::SINE_40_SAMPELS, sine_table_400s_12bit.data()  }
};

enum class Rseries_t : unsigned int
{
 R_100=0,// in ohms
 R_820,
 R_6,
};

static inline const std::unordered_map<Rseries_t, float> r_series_option = {
        {Rseries_t::R_100, 100 },
        {Rseries_t::R_820, 820 },
        {Rseries_t::R_6, 6 }
};

void Set_DAC_Frequency(sine_samples_t freq);
void Set_Rseries(Rseries_t R);

#endif /* LLUTILS_H_ */
