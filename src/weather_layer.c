#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include "util.h"
#include "weather_layer.h"


	
static uint8_t WEATHER_ICONS[] = {
	RESOURCE_ID_ICON_CLEAR_DAY,
	RESOURCE_ID_ICON_CLEAR_NIGHT,
	RESOURCE_ID_ICON_RAIN,
	RESOURCE_ID_ICON_SNOW,
	RESOURCE_ID_ICON_SLEET,
	RESOURCE_ID_ICON_WIND,
	RESOURCE_ID_ICON_FOG,
	RESOURCE_ID_ICON_CLOUDY,
	RESOURCE_ID_ICON_PARTLY_CLOUDY_DAY,
	RESOURCE_ID_ICON_PARTLY_CLOUDY_NIGHT,
	RESOURCE_ID_ICON_ERROR,
};
//LEFT TOP WIDE HIGH
void weather_layer_init(WeatherLayer* weather_layer, GPoint pos) {
	layer_init(&weather_layer->layer, GRect(0,0,144,168));
	
	// Add background layer
	text_layer_init(&weather_layer->temp_layer_background, GRect(0, 0, 144, 95));
	text_layer_set_background_color(&weather_layer->temp_layer_background, GColorClear);
	layer_add_child(&weather_layer->layer, &weather_layer->temp_layer_background.layer);
	
    // Add temperature layer
	text_layer_init(&weather_layer->temp_layer, GRect(1, -5, 72, 95));
	text_layer_set_background_color(&weather_layer->temp_layer, GColorClear);
	text_layer_set_text_color(&weather_layer->lowvalue_layer, GColorWhite);
	text_layer_set_text_alignment(&weather_layer->temp_layer, GTextAlignmentCenter);
	text_layer_set_font(&weather_layer->temp_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_TEMP_42)));
	layer_add_child(&weather_layer->layer, &weather_layer->temp_layer.layer);

	//ADD HIGH LABEL VALUE LAYER
	text_layer_init(&weather_layer->highvalue_layer, GRect(118, 2, 32, 18));
	text_layer_set_background_color(&weather_layer->highvalue_layer, GColorClear);
	text_layer_set_text_color(&weather_layer->highvalue_layer, GColorWhite);
	text_layer_set_text_alignment(&weather_layer->highvalue_layer, GTextAlignmentLeft);
	text_layer_set_font(&weather_layer->highvalue_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_HIGH_LOW_16)));
	layer_add_child(&weather_layer->layer, &weather_layer->highvalue_layer.layer);
	
	//ADD LOW LABEL VALUE LAYER
	text_layer_init(&weather_layer->lowvalue_layer, GRect(118, 20, 32, 18));
	text_layer_set_background_color(&weather_layer->lowvalue_layer, GColorClear);
	text_layer_set_text_color(&weather_layer->lowvalue_layer, GColorWhite);
	text_layer_set_text_alignment(&weather_layer->lowvalue_layer, GTextAlignmentLeft);
	text_layer_set_font(&weather_layer->lowvalue_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_HIGH_LOW_16)));
	layer_add_child(&weather_layer->layer, &weather_layer->lowvalue_layer.layer);	

	
//SUNSET
	text_layer_init(&weather_layer->sslabel_layer, GRect(5, 142, 78, 18));
	text_layer_set_background_color(&weather_layer->sslabel_layer, GColorClear);
	text_layer_set_text_color(&weather_layer->sslabel_layer, GColorBlack);
	text_layer_set_text_alignment(&weather_layer->sslabel_layer, GTextAlignmentRight);
	text_layer_set_font(&weather_layer->sslabel_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_SUNSET_18)));
	layer_add_child(&weather_layer->layer, &weather_layer->sslabel_layer.layer);
	
//shvalue_layer	
	text_layer_init(&weather_layer->shvalue_layer, GRect(83, 142, 40, 18));
	text_layer_set_background_color(&weather_layer->shvalue_layer, GColorClear);
	text_layer_set_text_color(&weather_layer->shvalue_layer, GColorBlack);
	text_layer_set_text_alignment(&weather_layer->shvalue_layer, GTextAlignmentLeft);
	text_layer_set_font(&weather_layer->shvalue_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_SUNSET_18)));
	layer_add_child(&weather_layer->layer, &weather_layer->shvalue_layer.layer);
	
//smvalue_layer	
	text_layer_init(&weather_layer->smvalue_layer, GRect(108, 142, 60, 18));
	text_layer_set_background_color(&weather_layer->smvalue_layer, GColorClear);
	text_layer_set_text_color(&weather_layer->smvalue_layer, GColorBlack);
	text_layer_set_text_alignment(&weather_layer->smvalue_layer, GTextAlignmentLeft);
	text_layer_set_font(&weather_layer->smvalue_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_SUNSET_18)));
	layer_add_child(&weather_layer->layer, &weather_layer->smvalue_layer.layer);
	
	
	// Note absence of icon layer
	weather_layer->has_weather_icon = false;
}

void weather_layer_set_icon(WeatherLayer* weather_layer, WeatherIcon icon) {
	
	// Remove any possible existing weather icon
	if(weather_layer->has_weather_icon) {
		layer_remove_from_parent(&weather_layer->icon_layer.layer.layer);
		bmp_deinit_container(&weather_layer->icon_layer);
		weather_layer->has_weather_icon = false;
	}
	
	// Add weather icon
	bmp_init_container(WEATHER_ICONS[icon], &weather_layer->icon_layer);
	layer_add_child(&weather_layer->layer, &weather_layer->icon_layer.layer.layer);
	layer_set_frame(&weather_layer->icon_layer.layer.layer, GRect(75,2, 40, 40));
	weather_layer->has_weather_icon = true;
}

//SUNSET
void weather_layer_set_sh(WeatherLayer* weather_layer, int16_t ss) {
	memcpy(weather_layer->sh_str, itoa(ss), 4);
	int sh_pos = strlen(weather_layer->sh_str);
	memcpy(&weather_layer->sh_str[sh_pos], ":", 3);
	text_layer_set_text(&weather_layer->shvalue_layer, weather_layer->sh_str);
	
	memcpy(&weather_layer->sslbl_str[0], "Sunset:  ", 8);
	text_layer_set_text(&weather_layer->sslabel_layer, weather_layer->sslbl_str);
}
void weather_layer_set_sm(WeatherLayer* weather_layer, int16_t sm) {
	memcpy(weather_layer->sm_str, itoa(sm), 4);
	int sm_pos = strlen(weather_layer->sm_str);
	memcpy(&weather_layer->sm_str[sm_pos], " ", 3);
	text_layer_set_text(&weather_layer->smvalue_layer, weather_layer->sm_str);
	
}


void weather_layer_set_high(WeatherLayer* weather_layer, int16_t h) {
	memcpy(weather_layer->high_str, itoa(h), 4);
	int degree_pos = strlen(weather_layer->high_str);
	memcpy(&weather_layer->high_str[degree_pos], "°", 3);
	text_layer_set_text(&weather_layer->highvalue_layer, weather_layer->high_str);
	/*
	memcpy(&weather_layer->highlbl_str[0], "HIGH :", 6);
	text_layer_set_text(&weather_layer->highlabel_layer, weather_layer->highlbl_str);*/
}

void weather_layer_set_low(WeatherLayer* weather_layer, int16_t l) {
	memcpy(weather_layer->low_str, itoa(l), 4);
	int degree_poss = strlen(weather_layer->low_str);
	memcpy(&weather_layer->low_str[degree_poss], "°", 3);
	text_layer_set_text(&weather_layer->lowvalue_layer, weather_layer->low_str);
	/*
	memcpy(&weather_layer->lowlbl_str[0], "LOW :", 5);
	text_layer_set_text(&weather_layer->lowlabel_layer, weather_layer->lowlbl_str);*/
}


void weather_layer_set_temperature(WeatherLayer* weather_layer, int16_t t) {
	memcpy(weather_layer->temp_str, itoa(t), 4);
	int degree_pos = strlen(weather_layer->temp_str);
	
	if (strlen(weather_layer->temp_str) == 1 || 
		(strlen(weather_layer->temp_str) == 2 && weather_layer->temp_str[0] != '1')) {
	  // Don't move temperature if between 0-9° or 20°-99°
	  text_layer_set_font(&weather_layer->temp_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_TEMP_42)));
	  text_layer_set_text_alignment(&weather_layer->temp_layer, GTextAlignmentCenter);
	  memcpy(&weather_layer->temp_str[degree_pos], "°", 3);
	} else if (strlen(weather_layer->temp_str) == 2 && weather_layer->temp_str[0] == '1') {
	  // Move temperature slightly to the left if between 10°-19°
	  text_layer_set_font(&weather_layer->temp_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_TEMP_42)));
	  text_layer_set_text_alignment(&weather_layer->temp_layer, GTextAlignmentLeft);
	  memcpy(&weather_layer->temp_str[degree_pos], "°", 3); 
	} else if (strlen(weather_layer->temp_str) > 2) { 
	  // Shrink font size if above 99° or below -9°
	  text_layer_set_font(&weather_layer->temp_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_TEMP_42)));
	  text_layer_set_text_alignment(&weather_layer->temp_layer, GTextAlignmentCenter);
	  memcpy(&weather_layer->temp_str[degree_pos], "°", 3);
	}
	
	text_layer_set_text(&weather_layer->temp_layer, weather_layer->temp_str);
}



void weather_layer_deinit(WeatherLayer* weather_layer) {
	if(weather_layer->has_weather_icon)
		bmp_deinit_container(&weather_layer->icon_layer);
}